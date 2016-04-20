#include "Terrian.h"
#include "cocostudio/CocoStudio.h"
#include "DrawNode3D.h"
#include "ui/CocosGUI.h"
#include "Particle3D/CCParticleSystem3D.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* Terrian::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Terrian::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Terrian::init()
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
	_camera->setPosition3D(Vec3(0, 10, 10));
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

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(Terrian::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(Terrian::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(Terrian::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//创建地形 
	//首先，我们需要构建出地形需要的所有细节图  第一个是细节图的路径，第二个是细节图的在地形上的平铺尺寸 第二个参数有默认值，是可选的
	Terrain::DetailMap r("Terrian/TerrainTest/dirt.jpg"), g("Terrian/TerrainTest/Grass2.jpg"), 
		b("Terrian/TerrainTest/road.jpg"), a("Terrian/TerrainTest/GreenSkin.jpg");
	//然后，我们使用细节图构造出Terrain::TerrainData,Terrain::TerrainData 的构造函数有若干种形式以满足用户不同的构造地形的需求，在这里我们只给出最常用的一种，构造函数的第一个参数为高度图的路径，第二个参数为Alpha贴图的路径，接下来的四个参数为之前所构造的四个DetailMap结构体。
	Terrain::TerrainData data("Terrian/TerrainTest/heightmap16.jpg", "Terrian/TerrainTest/alphamap.png", r, g, b, a);
	//最后，将构造出来的TerrainData传给Terrain::create即可创建出地形了
	//Terrain::create的第二个参数为地形LOD裂缝的缝补方法，LOD简单来讲就是通过将远处的地形块的Mesh加以简化，从而在不影响视觉效果的情况下，提高性能的方法。但是如果临近的两个地形块的LOD不同时，其边缘可能会出现裂缝，为了游戏效果的美观，我们需要对裂缝进行填补。Terrain提供两种方式对裂缝进行填补，分别是裙边以及补边的方法，在这里我们使用裙边法来对裂缝进行填补
	auto terrain = Terrain::create(data, Terrain::CrackFixedType::SKIRT);
	terrain->setLODDistance(3.2f, 6.4f, 9.6f);
	terrain->setMaxDetailMapAmount(4);
	terrain->setCameraMask(2);
	terrain->setDrawWire(true);
	layer3D->addChild(terrain);
	_terrain = terrain;

	return true;
}

void Terrian::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}

void Terrian::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
	if (touches.size())
	{
		auto touch = touches[0];
		auto delta = touch->getDelta();
		static  float angle = 0;
		angle -= CC_DEGREES_TO_RADIANS(delta.x);
		if (_camera)
		{
			_camera->setPosition3D(Vec3(10 * sinf(angle), 10.0f, 10 * cosf(angle)));
			_camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
		}
	}
}

void Terrian::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}

void Terrian::InitUI()
{
	//加载UI界面
	auto rootNode = CSLoader::createNode("Terrian/MainScene.csb");
	addChild(rootNode);
	//进行界面适配
	auto visibleSize = Director::getInstance()->getVisibleSize();
	rootNode->setContentSize(visibleSize);
	ui::Helper::doLayout(rootNode);
	cocos2d::ui::Layout* rootPane = static_cast<cocos2d::ui::Layout*>(rootNode->getChildByName("Panel_1"));
	//获取控件
	auto Button_1 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_1"));
	Button_1->setTag(1);
	//添加按钮点击事件
	Button_1->addClickEventListener(CC_CALLBACK_1(Terrian::onButtonClick, this));


	//获取控件
	auto Button_2 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_2"));
	Button_2->setTag(2);
	//添加按钮点击事件
	Button_2->addClickEventListener(CC_CALLBACK_1(Terrian::onButtonClick, this));
}

void Terrian::onButtonClick(cocos2d::Ref* sender)
{
	cocos2d::ui::Button* btn = (cocos2d::ui::Button*)sender;
	if (btn)
	{
		if (1 == btn->getTag())
		{
			_terrain->setDrawWire(true);
		}
		else if (2 == btn->getTag())
		{
			_terrain->setDrawWire(false);
		}
	}
}