#include "EnvirMapper.h"
#include "cocostudio/CocoStudio.h"
#include "DrawNode3D.h"
#include "ui/CocosGUI.h"
#include "Particle3D/CCParticleSystem3D.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* EnvirMapper::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = EnvirMapper::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool EnvirMapper::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	InitUI();

	////得到屏幕尺寸
	auto s = Director::getInstance()->getWinSize();
	//创建3D层
	auto layer3D = Layer::create();
	addChild(layer3D, 0);

	//创建3D摄像机
	_camera = cocos2d::Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);
	//设置摄像机眼睛位置
	_camera->setPosition3D(Vec3(0, 0, 50));
	//设置摄像机观察点位置
	_camera->lookAt(Vec3(0, 0, 0));
	_camera->setCameraFlag(CameraFlag::USER1);
	layer3D->addChild(_camera);
	layer3D->setCameraMask(2);

	//创建3D网格
	DrawNode3D* line = DrawNode3D::create();
	//draw x
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
	//layer3D->addChild(line);

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(EnvirMapper::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(EnvirMapper::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(EnvirMapper::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//创建天空盒纹理
	auto textureCube = TextureCube::create("EnvirMapper/skybox/left.jpg", "EnvirMapper/skybox/right.jpg",
		"EnvirMapper/skybox/top.jpg", "EnvirMapper/skybox/bottom.jpg",
		"EnvirMapper/skybox/front.jpg", "EnvirMapper/skybox/back.jpg");

	_textureCube = textureCube;

	//设置纹理参数
	Texture2D::TexParams tRepeatParams;
	tRepeatParams.magFilter = GL_LINEAR;
	tRepeatParams.minFilter = GL_LINEAR;
	tRepeatParams.wrapS = GL_CLAMP_TO_EDGE;
	tRepeatParams.wrapT = GL_CLAMP_TO_EDGE;
	textureCube->setTexParameters(tRepeatParams);
	//创建天空盒对象

	auto skyBox = Skybox::create();
	skyBox->setTexture(textureCube);
	skyBox->setCameraMask(2);
	layer3D->addChild(skyBox);

	auto sprite3D = Sprite3D::create("EnvirMapper/qiu.c3b");
	_sprite3D = sprite3D;
	sprite3D->setScale(0.3);
	sprite3D->setCameraMask(2);
	layer3D->addChild(sprite3D);
	//创建环境映射shader
	auto shader = GLProgram::createWithFilenames("EnvirMapper/qiu.vert", "EnvirMapper/qiu.frag");
	auto state = GLProgramState::create(shader);
	_state = state;
	sprite3D->setGLProgramState(_state);
	return true;
}

void EnvirMapper::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}

void EnvirMapper::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
	if (touches.size())
	{
		auto touch = touches[0];
		auto delta = touch->getDelta();
		static  float angle = 0;
		angle -= CC_DEGREES_TO_RADIANS(delta.x);
		if (_camera)
		{
			_camera->setPosition3D(Vec3(50 * sinf(angle), 0.0f, 50 * cosf(angle)));
			_camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
		}
	}
}

void EnvirMapper::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}

void EnvirMapper::InitUI()
{
	//加载UI界面
	auto rootNode = CSLoader::createNode("EnvirMapper/MainScene.csb");
	addChild(rootNode);
	//进行界面适配
	auto visibleSize = Director::getInstance()->getVisibleSize();
	rootNode->setContentSize(visibleSize);
	rootNode->setScale(0.5);
	rootNode->setPosition(visibleSize.width / 3, visibleSize.height / 2);
	ui::Helper::doLayout(rootNode);
	cocos2d::ui::Layout* rootPane = static_cast<cocos2d::ui::Layout*>(rootNode->getChildByName("Panel_1"));
	if (rootPane)
	{
		//获取控件
		auto Button_1 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_1"));
		//给设置按钮标记，已区分点击的是那个按钮
		Button_1->setTag(1);
		//添加按钮点击事件
		Button_1->addClickEventListener(CC_CALLBACK_1(EnvirMapper::onButtonClick, this));
		//获取控件
		auto Button_2 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_2"));
		//给设置按钮标记，已区分点击的是那个按钮
		Button_2->setTag(2);
		//添加按钮点击事件
		Button_2->addClickEventListener(CC_CALLBACK_1(EnvirMapper::onButtonClick, this));
	}
}

void EnvirMapper::onButtonClick(cocos2d::Ref* sender)
{
	cocos2d::ui::Button* btn = (cocos2d::ui::Button*)sender;
	if (btn)
	{
		if (btn->getTag() == 1){
			auto shader = GLProgram::createWithFilenames("EnvirMapper/qiu.vert", "EnvirMapper/qiu.frag");
			_state = GLProgramState::create(shader);
			_sprite3D->setGLProgramState(_state);
		}
		else{
			auto shader = GLProgram::createWithFilenames("EnvirMapper/mapper.vert", "EnvirMapper/mapper.frag");
			_state = GLProgramState::create(shader);
			//设置环境贴图
			if (_textureCube){
				_state->setUniformTexture("u_cubeTex", _textureCube);
			}
			_sprite3D->setGLProgramState(_state);
		}
	}
}