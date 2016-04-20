#include "BillBoardTest.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "DrawNode3D.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* BillBoardTest::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = BillBoardTest::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool BillBoardTest::init()
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

	//创建公告板,设置公告板纹理贴图 设置公告板模式 目前Cocos引擎支持两种类型的公告板：一种朝向摄像机的X0Y平面；另一种则朝向摄像机原点
	_Logo = BillBoard::create("BillBoardTest/Logo.png");
	//设置缩放
	_Logo->setScale(0.2);
	//设置位置
	_Logo->setPosition3D(Vec3(50.0f, 0.0f, 50.0f));
	//因为CameraFlag::USER1 为 2,则2 & 2 的值大于0，所以pPline会出现在摄像机的观察物列表中。
	_Logo->setCameraMask(2);
	_layer3D->addChild(_Logo);

	//树木
	for (int i = 0; i < 30; i++)
	{
		if (i % 2)
		{
			_Tree[i] = BillBoard::create("BillBoardTest/tree2.png");
		}
		else
		{
			_Tree[i] = BillBoard::create("BillBoardTest/tree1.png");
		}
		
		//设置缩放
		_Tree[i]->setScale(rand_0_1() * 0.05 + 0.05);
		//设置位置
		_Tree[i]->setPosition3D(Vec3(rand_0_1() * 100, 0.0f, rand_0_1() * 100));
		//因为CameraFlag::USER1 为 2,则2 & 2 的值大于0，所以pPline会出现在摄像机的观察物列表中。
		_Tree[i]->setCameraMask(2);
		//先设为不显示
		_Tree[i]->setVisible(false);
		_layer3D->addChild(_Tree[i]);
	}
	//雪花
	for (int i = 0; i < 50; i++)
	{
		_Snow[i] = BillBoard::create("BillBoardTest/snow.png");
		//设置缩放
		_Snow[i]->setScale(rand_0_1() * 0.01 + 0.01);
		//设置位置
		_Snow[i]->setPosition3D(Vec3(rand_0_1() * 100, rand_0_1() * 30.0f, rand_0_1() * 100));
		//因为CameraFlag::USER1 为 2,则2 & 2 的值大于0，所以pPline会出现在摄像机的观察物列表中。
		_Snow[i]->setCameraMask(2);
		//先设为不显示
		_Snow[i]->setVisible(false);
		_layer3D->addChild(_Snow[i]);
	}

	
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


	//血条_Blood
	_Blood = BillBoard::create("BillBoardTest/blood1.png");
	_Blood->setScale(0.05);
	_Blood->setPosition3D(Vec3(0, 2, 0));
	_Blood->setCameraMask(2);
	_Blood->setVisible(false);
	_Tank_Base->addChild(_Blood);

	//创建一个定时器,实现摄像机绕观察点360旋转
	schedule(schedule_selector(BillBoardTest::rotCamera));
    return true;
}

void  BillBoardTest::rotCamera(float dt)
{
	static float angle = 0.0f;
	angle += dt * 0.1;
	if (_camera)
	{
		//通过角度计算出旋转后的摄像机眼睛的位置，观察点的位置
		_camera->setPosition3D(Vec3(50 + 100 * sinf(angle), 30.0f, 50 + 100 * cosf(angle)));
		_camera->lookAt(Vec3(50,0,50));
	}

	//雪花
	for (int i = 0; i < 50; i++)
	{
		Vec3 tNewPos = _Snow[i]->getPosition3D() - Vec3(0, dt*4.0, 0);
		if (tNewPos.y < 0)
		{
			tNewPos.y = 30;
		}
		_Snow[i]->setPosition3D(tNewPos);

	}
}
void  BillBoardTest::InitUI()
{
	//加载UI界面
	auto rootNode = CSLoader::createNode("BillBoardTest/MainScene.csb");
	addChild(rootNode);
	//进行界面适配
	auto visibleSize = Director::getInstance()->getVisibleSize();
	rootNode->setContentSize(visibleSize);
	ui::Helper::doLayout(rootNode);
	//获取面板容器
	cocos2d::ui::Layout* rootPane = static_cast<cocos2d::ui::Layout*>(rootNode->getChildByName("Panel_1"));
	//获取控件
	auto Button_1 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_1"));
	//给设置按钮标记，已区分点击的是那个按钮
	Button_1->setTag(1);
	//添加按钮点击事件
	Button_1->addClickEventListener(CC_CALLBACK_1(BillBoardTest::onButtonClick, this));

	auto Button_2 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_2"));
	Button_2->setTag(2);
	Button_2->addClickEventListener(CC_CALLBACK_1(BillBoardTest::onButtonClick, this));

	auto Button_3 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_3"));
	Button_3->setTag(3);
	Button_3->addClickEventListener(CC_CALLBACK_1(BillBoardTest::onButtonClick, this));

	auto Button_4 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_4"));
	Button_4->setTag(4);
	Button_4->addClickEventListener(CC_CALLBACK_1(BillBoardTest::onButtonClick, this));
}

void BillBoardTest::onButtonClick(cocos2d::Ref* sender)
{
	cocos2d::ui::Button* btn = (cocos2d::ui::Button*)sender;
	if (btn)
	{ 
		//先将所有的公告板都设为不显示
		_Logo->setVisible(false);
		for (int i = 0; i < 30; i++)
		{
			_Tree[i]->setVisible(false);
		}
		for (int i = 0; i < 50; i++)
		{
			_Snow[i]->setVisible(false);
		}
		_Blood->setVisible(false);

		//响应按下的按钮，对应哪个公告板显示。
		if (btn->getTag() == 2)
		{
			for (int i = 0; i < 30; i++)
			{
				_Tree[i]->setVisible(true);
			}
		}
		else if (btn->getTag() == 3)
		{
			for (int i = 0; i < 50; i++)
			{
				_Snow[i]->setVisible(true);
			}
		}
		else if (btn->getTag() == 4)
		{
			_Blood->setVisible(true);
		}
		else
		{
			_Logo->setVisible(true);
		}
	}

}