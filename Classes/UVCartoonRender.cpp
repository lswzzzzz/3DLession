#include "UVCartoonRender.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "DrawNode3D.h"

using namespace cocostudio::timeline;

Scene* UVCartoonRender::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = UVCartoonRender::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}


// on "init" you need to initialize your instance
bool UVCartoonRender::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	InitUI();
	_angle = 0.0f;
	auto s = Director::getInstance()->getWinSize();
	auto layer3D = Layer::create();
	addChild(layer3D, 0);

	_camera = cocos2d::Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);
	_camera->setPosition3D(Vec3(0, 60, 50));
	_camera->lookAt(Vec3(0, 20, 0));
	_camera->setCameraFlag(CameraFlag::USER1);
	layer3D->addChild(_camera);
	layer3D->setCameraMask(2);

	DrawNode3D* line = DrawNode3D::create();
	for (int j = -20; j <= 20; j++)
	{
		line->drawLine(Vec3(-100, 0, 5 * j), Vec3(100, 0, 5 * j), Color4F(1, 0, 0, 1));
	}
	//draw z
	for (int j = -20; j <= 20; j++)
	{
		line->drawLine(Vec3(5 * j, 0, -100), Vec3(5 * j, 0, 100), Color4F(0, 0, 1, 1));
	}
	line->setCameraMask(2);
	layer3D->addChild(line);

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(UVCartoonRender::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(UVCartoonRender::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(UVCartoonRender::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	auto spriteStaticMesh = Sprite3D::create("UVCartoonRender/mod_niutou.c3b");
	_sprite3D = spriteStaticMesh;
	auto shader = GLProgram::createWithFilenames("UVCartoonRender/Mesh.vert", "UVCartoonRender/Mesh.frag");
	_state = GLProgramState::create(shader);
	spriteStaticMesh->setGLProgramState(_state);
	spriteStaticMesh->setCameraMask(2);
	spriteStaticMesh->setScale(0.2);
	spriteStaticMesh->setPosition3D(Vec3(0, 0, 0));
	spriteStaticMesh->setRotation3D(Vec3(-90, 0, 0));
	layer3D->addChild(spriteStaticMesh);

	return true;
}

void UVCartoonRender::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}

void UVCartoonRender::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
	if (touches.size() > 0){
		auto touch = touches[0];
		auto delta = touch->getDelta();
		_angle -= CC_DEGREES_TO_RADIANS(delta.x);
		if (_camera){
			_camera->setPosition3D(Vec3(50 * sinf(_angle), 60.0f, 50 * cosf(_angle)));
			_camera->lookAt(Vec3(0.0f, 20.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
		}
	}
}

void UVCartoonRender::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}

void UVCartoonRender::InitUI(){
	auto rootNode = CSLoader::createNode("UVCartoonRender/MainScene.csb");
	addChild(rootNode);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	rootNode->setContentSize(visibleSize);
	rootNode->setScale(0.3);
	rootNode->setPosition(visibleSize.width / 3, visibleSize.height / 2);
	ui::Helper::doLayout(rootNode);
	cocos2d::ui::Layout* rootPane = static_cast<cocos2d::ui::Layout*>(rootNode->getChildByName("Panel_1"));
	//获取控件
	auto Button_1 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_1"));
	//给设置按钮标记，已区分点击的是那个按钮
	Button_1->setTag(1);
	//添加按钮点击事件
	Button_1->addClickEventListener(CC_CALLBACK_1(UVCartoonRender::onButtonClick, this));
	//获取控件
	auto Button_2 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_2"));
	//给设置按钮标记，已区分点击的是那个按钮
	Button_2->setTag(2);
	//添加按钮点击事件
	Button_2->addClickEventListener(CC_CALLBACK_1(UVCartoonRender::onButtonClick, this));
}

void UVCartoonRender::onButtonClick(cocos2d::Ref* sender)
{
	cocos2d::ui::Button* btn = (cocos2d::ui::Button*)sender;
	if (btn)
	{
		//卸载掉所有定时器
		unscheduleAllCallbacks();

		if (btn->getTag() == 2)
		{
			//创建模型自定义卡通效果
			auto shader = GLProgram::createWithFilenames("UVCartoonRender/toon.vert", "UVCartoonRender/toon.frag");
			_state = GLProgramState::create(shader);
			//全部子模型应用默认效果
			_sprite3D->setGLProgramState(_state);


			//创建一张混合纹理
			auto ToonTexture = Director::getInstance()->getTextureCache()->addImage("UVCartoonRender/toon2.jpg");
			Texture2D::TexParams tRepeatParams;
			//纹理滤波方式
			tRepeatParams.magFilter = GL_NEAREST;
			tRepeatParams.minFilter = GL_NEAREST;
			//使用重复贴图
			tRepeatParams.wrapS = GL_REPEAT;
			tRepeatParams.wrapT = GL_REPEAT;
			ToonTexture->setTexParameters(tRepeatParams);
			//将混合纹理贴图做为Shader参数设置到Shader中
			_state->setUniformTexture("toonTexture", ToonTexture);


		}
		else
		{
			//创建混合纹理shader
			auto shader = GLProgram::createWithFilenames("UVCartoonRender/Mesh.vert", "UVCartoonRender/Mesh.frag");
			_state = GLProgramState::create(shader);
			_sprite3D->setGLProgramState(_state);
		}
	}
	
}