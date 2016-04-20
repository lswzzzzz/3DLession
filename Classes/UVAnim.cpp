#include "UVAnim.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "DrawNode3D.h"

using namespace cocostudio::timeline;

Scene* UVAnim::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = UVAnim::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}


// on "init" you need to initialize your instance
bool UVAnim::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	InitUI();
	_angle = 0.0f;
	_meshUVRunTime = 0.0f;
	auto s = Director::getInstance()->getWinSize();
	auto layer3D = Layer::create();
	addChild(layer3D, 0);

	_camera = cocos2d::Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);
	_camera->setPosition3D(Vec3(0, 50, 50));
	_camera->lookAt(Vec3(0, 0, 0));
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

	auto sprite3D = Sprite3D::create("UVAnim/tortoise.c3b");
	_sprite3D = sprite3D;
	auto shader = GLProgram::createWithFilenames("UVAnim/qiu.vert", "UVAnim/qiu.frag");
	_state = GLProgramState::create(shader);
	sprite3D->setGLProgramState(_state);
	sprite3D->setCameraMask(2);
	sprite3D->setScale(0.05);
	sprite3D->setPosition3D(Vec3(0, 0, 0));
	sprite3D->setRotation3D(Vec3(-90, 0, 0));
	layer3D->addChild(sprite3D);


	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(UVAnim::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(UVAnim::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(UVAnim::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void UVAnim::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}

void UVAnim::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
	if (touches.size() > 0){
		auto touch = touches[0];
		auto delta = touch->getDelta();
		_angle -= CC_DEGREES_TO_RADIANS(delta.x);
		if (_camera){
			_camera->setPosition3D(Vec3(50 * sinf(_angle), 50.0f, 50 * cosf(_angle)));
			_camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
		}
	}
}

void UVAnim::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}

void UVAnim::meshUVTimeUpdate(float dt){
	if (_state){
		_meshUVRunTime += dt;
		_state->setUniformFloat("UVRunTime", _meshUVRunTime);
	}
}

void UVAnim::InitUI(){
	auto rootNode = CSLoader::createNode("UVAnim/MainScene.csb");
	addChild(rootNode);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	rootNode->setContentSize(visibleSize);
	rootNode->setScale(0.5);
	rootNode->setPosition(visibleSize.width / 3, visibleSize.height / 2);
	ui::Helper::doLayout(rootNode);
	cocos2d::ui::Layout* rootPane = static_cast<cocos2d::ui::Layout*>(rootNode->getChildByName("Panel_1"));
	//��ȡ�ؼ�
	auto Button_1 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_1"));
	//�����ð�ť��ǣ������ֵ�������Ǹ���ť
	Button_1->setTag(1);
	//��Ӱ�ť����¼�
	Button_1->addClickEventListener(CC_CALLBACK_1(UVAnim::onButtonClick, this));
	//��ȡ�ؼ�
	auto Button_2 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_2"));
	//�����ð�ť��ǣ������ֵ�������Ǹ���ť
	Button_2->setTag(2);
	//��Ӱ�ť����¼�
	Button_2->addClickEventListener(CC_CALLBACK_1(UVAnim::onButtonClick, this));
	//��ȡ�ؼ�
	auto Button_3 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_3"));
	//�����ð�ť��ǣ������ֵ�������Ǹ���ť
	Button_3->setTag(3);
	//��Ӱ�ť����¼�
	Button_3->addClickEventListener(CC_CALLBACK_1(UVAnim::onButtonClick, this));
}

void UVAnim::onButtonClick(cocos2d::Ref* sender)
{
	cocos2d::ui::Button* btn = (cocos2d::ui::Button*)sender;
	if (btn)
	{
		//ж�ص����ж�ʱ��
		unscheduleAllCallbacks();
		if (btn->getTag() == 1)
		{
			//����ģ���Զ���shader
			auto shader = GLProgram::createWithFilenames("UVAnim/qiu.vert", "UVAnim/qiu.frag");
			_state = GLProgramState::create(shader);
			_sprite3D->setGLProgramState(_state);
		}
		else if (btn->getTag() == 2)
		{
			//�����������shader
			auto shader = GLProgram::createWithFilenames("UVAnim/qiu.vert", "UVAnim/qiublendTexture.frag");
			_state = GLProgramState::create(shader);
			_sprite3D->setGLProgramState(_state);

			//����һ�Ż������
			auto shining_texture = Director::getInstance()->getTextureCache()->addImage("UVAnim/caustics.png");
			Texture2D::TexParams tRepeatParams;
			//�����˲���ʽ
			tRepeatParams.magFilter = GL_NEAREST;
			tRepeatParams.minFilter = GL_NEAREST;
			//ʹ���ظ���ͼ
			tRepeatParams.wrapS = GL_REPEAT;
			tRepeatParams.wrapT = GL_REPEAT;
			shining_texture->setTexParameters(tRepeatParams);
			//�����������ͼ��ΪShader�������õ�Shader��
			_state->setUniformTexture("caustics", shining_texture);
			//���û���������ɫ
			Vec4  blendTextureColor(1.0, 1.0, 1.0, 1.0);
			_state->setUniformVec4("blendTextureColor", blendTextureColor);
		}
		else
		{
			//�����������UV����shader
			auto shader = GLProgram::createWithFilenames("UVAnim/qiu.vert", "UVAnim/qiublendTextureUVAni.frag");
			_state = GLProgramState::create(shader);
			_sprite3D->setGLProgramState(_state);

			//����һ�Ż������
			auto shining_texture = Director::getInstance()->getTextureCache()->addImage("UVAnim/caustics.png");
			Texture2D::TexParams tRepeatParams;
			//�����˲���ʽ
			tRepeatParams.magFilter = GL_NEAREST;
			tRepeatParams.minFilter = GL_NEAREST;
			//ʹ���ظ���ͼ
			tRepeatParams.wrapS = GL_REPEAT;
			tRepeatParams.wrapT = GL_REPEAT;
			shining_texture->setTexParameters(tRepeatParams);
			//�����������ͼ��ΪShader�������õ�Shader��
			_state->setUniformTexture("caustics", shining_texture);
			//���û���������ɫ
			Vec4  blendTextureColor(1.0, 1.0, 1.0, 1.0);
			_state->setUniformVec4("blendTextureColor", blendTextureColor);
			//����UV�����ٶ�
			_state->setUniformVec2("UVSpeed", Vec2(0.5, 0.5));
			//����UV��������ʱ��
			_state->setUniformFloat("UVRunTime", _meshUVRunTime);
			//������ʱ���������������ʱ��
			schedule(schedule_selector(UVAnim::meshUVTimeUpdate));
		}
	}

}