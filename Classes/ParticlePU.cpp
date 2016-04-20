#include "ParticlePU.h"
#include "cocostudio/CocoStudio.h"
#include "DrawNode3D.h"
#include "ui/CocosGUI.h"
#include "Particle3D/CCParticleSystem3D.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* ParticlePU::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ParticlePU::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ParticlePU::init()
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
	_camera->setPosition3D(Vec3(0, 20, 20));
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
	layer3D->addChild(line);

	_particleSystem3D[0] = PUParticleSystem3D::create("ParticlePU/Particle3D/scripts/testParticle.pu");
	_particleSystem3D[0]->setCameraMask(2);
	_particleSystem3D[0]->startParticleSystem();
	layer3D->addChild(_particleSystem3D[0]);
	
	_particleSystem3D[1] = PUParticleSystem3D::create("ParticlePU/Particle3D/scripts/lineStreak.pu");
	_particleSystem3D[1]->setCameraMask(2);
	layer3D->addChild(_particleSystem3D[1]);

	_particleSystem3D[2] = PUParticleSystem3D::create("ParticlePU/Particle3D/scripts/advancedLodSystem.pu");
	_particleSystem3D[2]->setCameraMask(2);
	layer3D->addChild(_particleSystem3D[2]);

	_particleSystem3D[3] = PUParticleSystem3D::create("ParticlePU/Particle3D/scripts/electricBeamSystem.pu");
	_particleSystem3D[3]->setCameraMask(2);
	layer3D->addChild(_particleSystem3D[3]);

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(ParticlePU::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(ParticlePU::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(ParticlePU::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void ParticlePU::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}

void ParticlePU::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
	if (touches.size())
	{
		auto touch = touches[0];
		auto delta = touch->getDelta();
		static  float angle = 0;
		angle -= CC_DEGREES_TO_RADIANS(delta.x);
		if (_camera)
		{
			_camera->setPosition3D(Vec3(20 * sinf(angle), 20.0f, 20 * cosf(angle)));
			_camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
		}
	}
}

void ParticlePU::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}

void ParticlePU::InitUI()
{
	//加载UI界面
	auto rootNode = CSLoader::createNode("ParticlePU/MainScene.csb");
	addChild(rootNode);
	//进行界面适配
	auto visibleSize = Director::getInstance()->getVisibleSize();
	rootNode->setContentSize(visibleSize);
	rootNode->setScale(0.5);
	rootNode->setPosition(visibleSize.width / 3, visibleSize.height/2);
	ui::Helper::doLayout(rootNode);
	cocos2d::ui::Layout* rootPane = static_cast<cocos2d::ui::Layout*>(rootNode->getChildByName("Panel_1"));
	//获取控件
	auto Button_1 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_1"));
	Button_1->setTag(1);
	//添加按钮点击事件
	Button_1->addClickEventListener(CC_CALLBACK_1(ParticlePU::onButtonClick, this));


	//获取控件
	auto Button_2 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_2"));
	Button_2->setTag(2);
	//添加按钮点击事件
	Button_2->addClickEventListener(CC_CALLBACK_1(ParticlePU::onButtonClick, this));

	//获取控件
	auto Button_3 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_3"));
	Button_3->setTag(3);
	//添加按钮点击事件
	Button_3->addClickEventListener(CC_CALLBACK_1(ParticlePU::onButtonClick, this));

	//获取控件
	auto Button_4 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_4"));
	Button_4->setTag(4);
	//添加按钮点击事件
	Button_4->addClickEventListener(CC_CALLBACK_1(ParticlePU::onButtonClick, this));
}

void ParticlePU::onButtonClick(cocos2d::Ref* sender)
{
	cocos2d::ui::Button* btn = (cocos2d::ui::Button*)sender;
	if (btn)
	{
		for (int i = 0; i < 4; i++)
		{
			if (_particleSystem3D[i])
			{
				_particleSystem3D[i]->stopParticleSystem();
			}
		}
		if (1 == btn->getTag())
		{
			_particleSystem3D[0]->startParticleSystem();
		}
		else if (2 == btn->getTag())
		{
			_particleSystem3D[1]->startParticleSystem();
		}
		else if (3 == btn->getTag())
		{
			_particleSystem3D[2]->startParticleSystem();
		}
		else
		{
			_particleSystem3D[3]->startParticleSystem();
		}
	}
}