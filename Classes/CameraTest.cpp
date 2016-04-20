#include "CameraTest.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "DrawNode3D.h"

USING_NS_CC;
using namespace cocostudio::timeline;

Scene* CameraTest::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = CameraTest::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool CameraTest::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	//得到屏幕尺寸
	auto s = Director::getInstance()->getWinSize();
	//创建3D层
	_layer3D = Layer::create();
	addChild(_layer3D, 0);

	//创建一个3D摄像机，设置视锥的相关参数:视锥张开角度,宽高比，近截面距离，远截面距离
	auto pCamera = Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);
	_camera = pCamera;
	//设置摄像机眼睛位置
	pCamera->setPosition3D(Vec3(50, 80, -80));
	//设置摄像机观察目标点位置
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

	//创建3D网格
	DrawNode3D* pPline = DrawNode3D::create();
	//横向绘制20条线，红色的。
	for (int j = 0; j <= 20; j++)
	{
		pPline->drawLine(Vec3(0, 0, 5 * j), Vec3(100, 0, 5 * j), Color4F(1, 0, 0, 1));
	}
	//纵向绘制20条线，蓝色的
	for (int j = 0; j <= 20; j++)
	{
		pPline->drawLine(Vec3(5 * j, 0, 0), Vec3(5 * j, 0, 100), Color4F(0, 0, 1, 1));
	}

	//因为CameraFlag::USER1 为 2,则2 & 2 的值大于0，所以pPline会出现在摄像机的观察物列表中。
	pPline->setCameraMask(2);
	_layer3D->addChild(pPline);

	//创建一个观察点节点(一个坦克）
	DrawNode3D*	pBox_1 = DrawNode3D::create();

	Vec3	tBoxVertex[8];
	tBoxVertex[0] = Vec3(-1, 1, 1);
	tBoxVertex[1] = Vec3(-1, -1, 1);
	tBoxVertex[2] = Vec3(1, -1, 1);
	tBoxVertex[3] = Vec3(1, 1, 1);

	tBoxVertex[4] = Vec3(1, 1, -1);
	tBoxVertex[5] = Vec3(1, -1, -1);
	tBoxVertex[6] = Vec3(-1, -1, -1);
	tBoxVertex[7] = Vec3(-1, 1, -1);

	pBox_1->setPosition3D(Vec3(0, 0, 0));
	pBox_1->drawCube(tBoxVertex, Color4F(1, 0, 1, 1));
	pBox_1->setCameraMask(2);
	_Tank_Base = pBox_1;
	_layer3D->addChild(_Tank_Base);

	//创建一个观察点节点(坦克带一个炮管）
	DrawNode3D*	pBox_2 = DrawNode3D::create();
	tBoxVertex[0] = Vec3(-0.2, 0.8, 2);
	tBoxVertex[1] = Vec3(-0.2, 0.6, 2);
	tBoxVertex[2] = Vec3(0.2, 0.6, 2);
	tBoxVertex[3] = Vec3(0.2, 0.8, 2);

	tBoxVertex[4] = Vec3(0.2, 0.8, 0);
	tBoxVertex[5] = Vec3(0.2, 0.6, 0);
	tBoxVertex[6] = Vec3(-0.2, 0.6, 0);
	tBoxVertex[7] = Vec3(-0.2, 0.6, 0);

	pBox_2->setPosition3D(Vec3(0, 0, 0.5));
	pBox_2->drawCube(tBoxVertex, Color4F(1, 0, 1, 1));
	pBox_2->setCameraMask(2);
	_Tank_Gun = pBox_2;
	_Tank_Base->addChild(_Tank_Gun);
	//观察点进行位移动画
	_Tank_Base->runAction(RepeatForever::create(Sequence::create(
		MoveTo::create(5.0f, Vec3(0, 0, 100)), RotateTo::create(1.0f, Vec3(0, 90, 0)),
		MoveTo::create(5.0f, Vec3(100, 0, 100)), RotateTo::create(1.0f, Vec3(0, 180, 0)),
		MoveTo::create(5.0f, Vec3(100, 0, 0)), RotateTo::create(1.0f, Vec3(0, 270, 0)),
		MoveTo::create(5.0f, Vec3(0, 0, 0)), RotateTo::create(1.0f, Vec3(0, 360, 0)),
		nullptr)));
	//加载界面。
	InitUI();
    return true;
}

void  CameraTest::InitUI()
{
	//加载UI界面
	auto rootNode = CSLoader::createNode("CameraTest/MainScene.csb");
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
	Button_1->addClickEventListener(CC_CALLBACK_1(CameraTest::onButtonClick, this));

	auto Button_2 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_2"));
	Button_2->setTag(2);
	Button_2->addClickEventListener(CC_CALLBACK_1(CameraTest::onButtonClick, this));

	auto Button_3 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_3"));
	Button_3->setTag(3);
	Button_3->addClickEventListener(CC_CALLBACK_1(CameraTest::onButtonClick, this));

	auto Button_4 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_4"));
	Button_4->setTag(4);
	Button_4->addClickEventListener(CC_CALLBACK_1(CameraTest::onButtonClick, this));
}
void CameraTest::onButtonClick(cocos2d::Ref* sender)
{

	cocos2d::ui::Button* btn = (cocos2d::ui::Button*)sender;
	if (btn)
	{
		//重置
		unscheduleAllCallbacks();
		_Tank_Base->stopAllActions();
		_Tank_Base->setPosition3D(Vec3(0, 0, 0));
		_Tank_Base->setRotation3D(Vec3(0, 0, 0));

		//观察点进行位移动画
		_Tank_Base->runAction(RepeatForever::create(Sequence::create(
			MoveTo::create(5.0f, Vec3(0, 0, 100)), RotateTo::create(1.0f, Vec3(0, 90, 0)),
			MoveTo::create(5.0f, Vec3(100, 0, 100)), RotateTo::create(1.0f, Vec3(0, 180, 0)),
			MoveTo::create(5.0f, Vec3(100, 0, 0)), RotateTo::create(1.0f, Vec3(0, 270, 0)),
			MoveTo::create(5.0f, Vec3(0, 0, 0)), RotateTo::create(1.0f, Vec3(0, 360, 0)),
			nullptr)));


		if (btn->getTag() == 2)
		{
			//删除之前的摄像机
			_layer3D->removeChild(_camera);
			_Tank_Base->removeChild(_camera);
			//创建第一人称视角摄像机
			auto s = Director::getInstance()->getWinSize();
			auto pCamera = Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 500);
			_camera = pCamera;
			pCamera->setPosition3D(Vec3(0, 1, 0));
			pCamera->lookAt(Vec3(0, 0, 1));
			pCamera->setCameraFlag(CameraFlag::USER1);
			_Tank_Base->addChild(_camera);

			//创建一个定时器,实现摄像机第一人称
			schedule(schedule_selector(CameraTest::FirstView));

		}
		else if (btn->getTag() == 3)
		{
			//删除之前的摄像机
			_layer3D->removeChild(_camera);
			_Tank_Base->removeChild(_camera);
			//创建第三人称视角摄像机
			auto s = Director::getInstance()->getWinSize();
			auto pCamera = Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 500);
			_camera = pCamera;
			pCamera->setPosition3D(Vec3(0, 20, -20));
			pCamera->lookAt(_Tank_Gun->getPosition3D());
			pCamera->setCameraFlag(CameraFlag::USER1);
			_Tank_Base->addChild(_camera);

			//创建一个定时器,实现摄像机第三人称
			schedule(schedule_selector(CameraTest::ThirdView));
		}
		else if (btn->getTag() == 4)
		{
			//将摄像机转换为正交附视观察
			_layer3D->removeChild(_camera);
			_Tank_Base->removeChild(_camera);
			//创建正交投影摄像机
			auto s = Director::getInstance()->getWinSize();
			auto pCamera = Camera::createOrthographic(200, 200, 1, 500);
			_camera = pCamera;
			pCamera->setPosition3D(Vec3(-50, 200, -50));
			pCamera->lookAt(Vec3(-50, 0, -50), Vec3(1, 0, 0));
			pCamera->setCameraFlag(CameraFlag::USER1);
			_layer3D->addChild(_camera);

		}
		else
		{
			//将摄像机转换为正交附视观察
			_layer3D->removeChild(_camera);
			_Tank_Base->removeChild(_camera);

			//创建默认3D摄像机
			auto s = Director::getInstance()->getWinSize();
			auto pCamera = Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);
			_camera = pCamera;
			//设置摄像机眼睛位置
			pCamera->setPosition3D(Vec3(50, 80, -80));
			//设置摄像机观察目标点位置
			pCamera->lookAt(Vec3(50, 0, 50));

			pCamera->setCameraFlag(CameraFlag::USER1);
			_layer3D->addChild(pCamera);
		}

	}

}
void  CameraTest::FirstView(float dt)
{

	if (_camera)
	{
		_camera->setPosition3D(Vec3(0, 2, 0));
		_camera->lookAt(_Tank_Gun->getPosition3D() + Vec3(0, 2, 0), Vec3(0.0f, 1.0f, 0.0f));
	}
}

//第三人称视角
void  CameraTest::ThirdView(float dt)
{
	if (_camera)
	{
		//通过获取观察点位置，让摄像机跟随观察点做平移动画
		_camera->setPosition3D(Vec3(0, 20, -20));
		_camera->lookAt(_Tank_Gun->getPosition3D(), Vec3(0.0f, 1.0f, 0.0f));
	}
}