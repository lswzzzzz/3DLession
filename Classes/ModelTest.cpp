#include "ModelTest.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "DrawNode3D.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* ModelTest::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = ModelTest::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ModelTest::init()
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	InitUI();
	//得到屏幕尺寸
	auto s = Director::getInstance()->getWinSize();
	//创建3D层
	_layer3D = Layer::create();
	addChild(_layer3D, 0);
	//创建一个3D摄像机，设置视锥的相关参数:视锥张开角度,宽高比，近截面距离，远截面距离
	auto pCamera = Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);
	_camera = pCamera;
	//设置摄像机眼睛位置
	pCamera->setPosition3D(Vec3(100, 30, 100));
	//设置摄像机观察点位置
	pCamera->lookAt(Vec3(50, 0, 50));
	/*在Cocos中，通过给一个摄像机设置一个标记数，可以指导定物体使用不同的摄像机进行观察，这里对新创建的摄像机设置一个标记。
	//在Cocos中定义了8个摄像机的标识，当然，8个绝对够用了。指定物体通过自身设置的摄像机模值（Mask)来与摄像机标记进行与(&)操作,如果大于0，即说明物体受这个摄像机的观察投影矩阵影响。
	enum class CameraFlag
	{
	DEFAULT = 1,
	USER1 = 1 << 1,
	USER2 = 1 << 2,
	USER3 = 1 << 3,
	USER4 = 1 << 4,
	USER5 = 1 << 5,
	USER6 = 1 << 6,
	USER7 = 1 << 7,
	USER8 = 1 << 8,
	};
	*/
	pCamera->setCameraFlag(CameraFlag::USER1);
	_layer3D->addChild(pCamera);
	//因为CameraFlag::USER1 为 2,则2 & 2 的值大于0，所以layer3D会出现在摄像机的观察物列表中。
	_layer3D->setCameraMask(2);
	//创建3D网格(20x20个格子)
	DrawNode3D* pPline = DrawNode3D::create();
	//横向绘制21条线，红色的。
	for (int j = 0; j < 21; j++)
	{
		pPline->drawLine(Vec3(0, 0, 5 * j), Vec3(100, 0, 5 * j), Color4F(1, 0, 0, 1));
	}
	//纵向绘制21条线，蓝色的
	for (int j = 0; j < 21; j++)
	{
		pPline->drawLine(Vec3(5 * j, 0, 0), Vec3(5 * j, 0, 100), Color4F(0, 0, 1, 1));
	}
	//因为CameraFlag::USER1 为 2,则2 & 2 的值大于0，所以pPline会出现在摄像机的观察物列表中。
	pPline->setCameraMask(2);
	pPline->setGlobalZOrder(-1);
	_layer3D->addChild(pPline);
	
	//创建静态模型
	_sprite3D = Sprite3D::create("ModelTest/qiu.c3b");
	//因为CameraFlag::USER1 为 2,则2 & 2 的值大于0，所以spriteStatic3D会出现在摄像机的观察物列表中。
	_sprite3D->setCameraMask(2);
	//设置缩放值
	_sprite3D->setScale(1);
	//设置位置
	_sprite3D->setPosition3D(Vec3(0, -20, 0));
	//设置旋转
	_sprite3D->setRotation3D(Vec3(0, 0, 0));

	//给模型添加自转动画
	_sprite3D->runAction(RepeatForever::create(Sequence::create(RotateBy::create(3.0f, Vec3(0, 360, 0)), nullptr)));
	//加入layer3D
	_layer3D->addChild(_sprite3D);
	
    return true;
}

void  ModelTest::InitUI()
{
	//加载UI界面
	auto rootNode = CSLoader::createNode("ModelTest/MainScene.csb");
	addChild(rootNode);
	//进行界面适配
	auto visibleSize = Director::getInstance()->getVisibleSize();

	rootNode->setContentSize(visibleSize);
	ui::Helper::doLayout(rootNode);
	cocos2d::ui::Layout* rootPane = static_cast<cocos2d::ui::Layout*>(rootNode->getChildByName("Panel_1"));
	//获取控件
	auto Button_1 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_1"));
	//给设置按钮标记，已区分点击的是那个按钮
	Button_1->setTag(1);
	//添加按钮点击事件
	Button_1->addClickEventListener(CC_CALLBACK_1(ModelTest::onButtonClick, this));


	auto Button_2 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_2"));
	Button_2->setTag(2);
	Button_2->addClickEventListener(CC_CALLBACK_1(ModelTest::onButtonClick, this));

	auto Button_3 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_3"));
	Button_3->setTag(3);
	Button_3->addClickEventListener(CC_CALLBACK_1(ModelTest::onButtonClick, this));

	auto Button_4 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_4"));
	Button_4->setTag(4);
	Button_4->addClickEventListener(CC_CALLBACK_1(ModelTest::onButtonClick, this));
}
void ModelTest::onButtonClick(cocos2d::Ref* sender)
{
	cocos2d::ui::Button* btn = (cocos2d::ui::Button*)sender;
	if (btn)
	{ 
		//移除当前模型
		if (_sprite3D)
		{
			_sprite3D->removeFromParent();
			_sprite3D = nullptr;
		}
		if (btn->getTag() == 1)
		{
			//创建静态模型
			_sprite3D = Sprite3D::create("ModelTest/qiu.c3b");
			//因为CameraFlag::USER1 为 2,则2 & 2 的值大于0，所以spriteStatic3D会出现在摄像机的观察物列表中。
			_sprite3D->setCameraMask(2);
			//设置缩放值
			_sprite3D->setScale(1);
			//设置位置
			_sprite3D->setPosition3D(Vec3(0, -20, 0));
			//设置旋转
			_sprite3D->setRotation3D(Vec3(0, 0, 0));
			//给模型添加自转动画
			_sprite3D->runAction(RepeatForever::create(Sequence::create(RotateBy::create(3.0f, Vec3(0, 360, 0)), nullptr)));
			//加入layer3D
			_layer3D->addChild(_sprite3D);
		}
		else if (btn->getTag() == 2)
		{
			//创建静态模型
			_sprite3D = Sprite3D::create("ModelTest/gun.c3b");
			//因为CameraFlag::USER1 为 2,则2 & 2 的值大于0，所以spriteStatic3D会出现在摄像机的观察物列表中。
			_sprite3D->setCameraMask(2);
			//设置缩放值
			_sprite3D->setScale(1);
			//设置位置
			_sprite3D->setPosition3D(Vec3(0, -20, 0));
			//设置旋转
			_sprite3D->setRotation3D(Vec3(0, 0, 0));
			//给模型添加自转动画
			_sprite3D->runAction(RepeatForever::create(Sequence::create(RotateBy::create(3.0f, Vec3(0, 360, 0)), nullptr)));
			//加入layer3D
			_layer3D->addChild(_sprite3D);
		}
		else if (btn->getTag() == 3)
		{
			//创建静态模型
			_sprite3D = Sprite3D::create("ModelTest/snake.c3b");
			//因为CameraFlag::USER1 为 2,则2 & 2 的值大于0，所以spriteStatic3D会出现在摄像机的观察物列表中。
			_sprite3D->setCameraMask(2);
			//设置缩放值
			_sprite3D->setScale(1);
			//设置位置
			_sprite3D->setPosition3D(Vec3(0, -20, 0));
			//设置旋转
			_sprite3D->setRotation3D(Vec3(0, 0, 0));
			//给模型添加自转动画
			_sprite3D->runAction(RepeatForever::create(Sequence::create(RotateBy::create(3.0f, Vec3(0, 360, 0)), nullptr)));
			//加入layer3D
			_layer3D->addChild(_sprite3D);
		}
		else
		{
			//创建静态模型
			_sprite3D = Sprite3D::create("ModelTest/mod_niutou.c3b");
			//因为CameraFlag::USER1 为 2,则2 & 2 的值大于0，所以spriteStatic3D会出现在摄像机的观察物列表中。
			_sprite3D->setCameraMask(2);
			//设置缩放值
			_sprite3D->setScale(0.2);
			//设置位置
			_sprite3D->setPosition3D(Vec3(0, -20, 0));
			//设置旋转
			_sprite3D->setRotation3D(Vec3(0, 0, 0));
			//给模型添加自转动画
			_sprite3D->runAction(RepeatForever::create(Sequence::create(RotateBy::create(3.0f, Vec3(0, 360, 0)), nullptr)));
			//加入layer3D
			_layer3D->addChild(_sprite3D);
		}

	}

}