#include "UVSkeleBind.h"
#include "cocostudio/CocoStudio.h"
#include "DrawNode3D.h"
#include "ui/CocosGUI.h"
#include "Particle3D/CCParticleSystem3D.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* UVSkeleBind::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = UVSkeleBind::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool UVSkeleBind::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	
	_angle = 0.0f;
	////得到屏幕尺寸
	auto s = Director::getInstance()->getWinSize();
	//创建3D层
	auto layer3D = Layer::create();
	addChild(layer3D, 0);

	//创建3D摄像机
	_camera = cocos2d::Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);
	//设置摄像机眼睛位置
	_camera->setPosition3D(Vec3(0, 50, 50));
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

	//添加触摸事件
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(UVSkeleBind::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(UVSkeleBind::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(UVSkeleBind::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	auto sprite = Sprite3D::create("UVSkeleBind/orc.c3b");
	sprite->setScale(2);
	sprite->setRotation3D(Vec3(0, 180, 0));
	sprite->setPosition3D(Vec3(0, 0, 0));
	sprite->setCameraMask(2);
	layer3D->addChild(sprite);

	auto sp = Sprite3D::create("UVSkeleBind/axe.c3b");
	sp->setCameraMask(2);
	sprite->getAttachNode("Bip001 R Hand")->addChild(sp);

	auto rootps = PUParticleSystem3D::create("UVSkeleBind/Particle3D/scripts/lineStreak.pu");
	rootps->setCameraMask(2);
	rootps->startParticleSystem();
	sprite->getAttachNode("Bip001 L Hand")->addChild(rootps);
	auto animation = Animation3D::create("UVSkeleBind/orc.c3b");
	if (animation){
		auto animate = Animate3D::create(animation);
		sprite->runAction(RepeatForever::create(animate));
	}
	
    return true;
}
void UVSkeleBind::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}

void UVSkeleBind::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
	if (touches.size())
	{
		auto touch = touches[0];
		auto delta = touch->getDelta();

		_angle -= CC_DEGREES_TO_RADIANS(delta.x);
		if (_camera)
		{
			_camera->setPosition3D(Vec3(50 * sinf(_angle), 50.0f, 50 * cosf(_angle)));
			_camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
		}
	}
}

void UVSkeleBind::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}