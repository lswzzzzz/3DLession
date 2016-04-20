#include "UVSkeleAnim.h"
#include "cocostudio/CocoStudio.h"
#include "DrawNode3D.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* UVSkeleAnim::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = UVSkeleAnim::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool UVSkeleAnim::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	InitUI();
	_angle = 0.0f;
	////得到屏幕尺寸
	auto s = Director::getInstance()->getWinSize();
	//创建3D层
	auto layer3D = Layer::create();
	addChild(layer3D, 0);

	//创建3D摄像机
	_camera = cocos2d::Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);
	//设置摄像机眼睛位置
	_camera->setPosition3D(Vec3(0, 100, 100));
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
	listener->onTouchesBegan = CC_CALLBACK_2(UVSkeleAnim::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(UVSkeleAnim::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(UVSkeleAnim::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	////创建动画模型
	auto spriteAniMesh = Sprite3D::create("UVSkeleAnim/rat.c3b");
	_spriteAni3D = spriteAniMesh;
	spriteAniMesh->setCameraMask(2);
	spriteAniMesh->setScale(3);
	spriteAniMesh->setPosition3D(Vec3(0, 0, 0));
	spriteAniMesh->setRotation3D(Vec3(0, 0, 0));

	//创建3D模型动画
	auto animation = Animation3D::create("UVSkeleAnim/rat.c3b");
	if (animation)
	{
		auto animate = Animate3D::create(animation);
		spriteAniMesh->runAction(RepeatForever::create(animate));
	}
	layer3D->addChild(spriteAniMesh);

    return true;
}
void UVSkeleAnim::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}

void UVSkeleAnim::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event)
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

void UVSkeleAnim::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}
void  UVSkeleAnim::InitUI()
{
	//加载UI界面
	auto rootNode = CSLoader::createNode("UVSkeleAnim/MainScene.csb");
	addChild(rootNode);
	//进行界面适配
	auto visibleSize = Director::getInstance()->getVisibleSize();
	rootNode->setContentSize(visibleSize);
	rootNode->setScale(0.4);
	rootNode->setPosition(visibleSize.width / 3, visibleSize.height / 2);
	ui::Helper::doLayout(rootNode);
	//获取面板容器
	cocos2d::ui::Layout* rootPane = static_cast<cocos2d::ui::Layout*>(rootNode->getChildByName("Panel_1"));
	//获取控件
	auto Button_1 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_1"));
	//给设置按钮标记，已区分点击的是那个按钮
	Button_1->setTag(1);
	//添加按钮点击事件
	Button_1->addClickEventListener(CC_CALLBACK_1(UVSkeleAnim::onButtonClick, this));

	auto Button_2 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_2"));
	Button_2->setTag(2);
	Button_2->addClickEventListener(CC_CALLBACK_1(UVSkeleAnim::onButtonClick, this));

	auto Button_3 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_3"));
	Button_3->setTag(3);
	Button_3->addClickEventListener(CC_CALLBACK_1(UVSkeleAnim::onButtonClick, this));

	auto Button_4 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_4"));
	Button_4->setTag(4);
	Button_4->addClickEventListener(CC_CALLBACK_1(UVSkeleAnim::onButtonClick, this));

	auto Button_5 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_5"));
	Button_5->setTag(5);
	Button_5->addClickEventListener(CC_CALLBACK_1(UVSkeleAnim::onButtonClick, this));

	auto Button_6 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_6"));
	Button_6->setTag(6);
	Button_6->addClickEventListener(CC_CALLBACK_1(UVSkeleAnim::onButtonClick, this));
}


void UVSkeleAnim::onButtonClick(cocos2d::Ref* sender)
{
	cocos2d::ui::Button* btn = (cocos2d::ui::Button*)sender;
	if (btn)
	{
		_spriteAni3D->stopAllActions();
		if (btn->getTag() == 1)
		{
			auto animation = Animation3D::create("UVSkeleAnim/rat.c3b");
			if (animation)
			{
				//创建空闲动画
				auto animate = Animate3D::createWithFrames(animation, 0, 23);
				_spriteAni3D->runAction(RepeatForever::create(animate));
			}
		}
		else if (btn->getTag() == 2)
		{
			auto animation = Animation3D::create("UVSkeleAnim/rat.c3b");
			if (animation)
			{
				//创建敲打动画
				auto animate = Animate3D::createWithFrames(animation, 30, 37);
				_spriteAni3D->runAction(RepeatForever::create(animate));
			}
		}
		else if (btn->getTag() == 3)
		{
			auto animation = Animation3D::create("UVSkeleAnim/rat.c3b");
			if (animation)
			{
				//创建死亡动画
				auto animate = Animate3D::createWithFrames(animation, 41, 76);
				_spriteAni3D->runAction(RepeatForever::create(animate));
			}
		}
		else if (btn->getTag() == 4)
		{
			auto animation = Animation3D::create("UVSkeleAnim/rat.c3b");
			if (animation)
			{
				//创建攻击1动画
				auto animate = Animate3D::createWithFrames(animation, 81, 99);
				_spriteAni3D->runAction(RepeatForever::create(animate));
			}
		}
		else if (btn->getTag() == 5)
		{
			auto animation = Animation3D::create("UVSkeleAnim/rat.c3b");
			if (animation)
			{
				//创建攻击2动画
				auto animate = Animate3D::createWithFrames(animation, 99, 117);
				_spriteAni3D->runAction(RepeatForever::create(animate));
			}
		}
		else
		{
			auto animation = Animation3D::create("UVSkeleAnim/rat.c3b");
			if (animation)
			{
				//创建行走动画
				auto animate = Animate3D::createWithFrames(animation, 122, 142);
				_spriteAni3D->runAction(RepeatForever::create(animate));
			}
		}
	}

}