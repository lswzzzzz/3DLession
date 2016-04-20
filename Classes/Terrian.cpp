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

	////�õ���Ļ�ߴ�
	auto s = Director::getInstance()->getWinSize();
	//����3D��
	auto layer3D = Layer::create();
	addChild(layer3D, 0);

	//����3D�����
	_camera = cocos2d::Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);
	//����������۾�λ��
	_camera->setPosition3D(Vec3(0, 10, 10));
	//����������۲��λ��
	_camera->lookAt(Vec3(0, 0, 0));
	_camera->setCameraFlag(CameraFlag::USER1);
	layer3D->addChild(_camera);
	layer3D->setCameraMask(2);

	//����3D����
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

	//�������� 
	//���ȣ�������Ҫ������������Ҫ������ϸ��ͼ  ��һ����ϸ��ͼ��·�����ڶ�����ϸ��ͼ���ڵ����ϵ�ƽ�̳ߴ� �ڶ���������Ĭ��ֵ���ǿ�ѡ��
	Terrain::DetailMap r("Terrian/TerrainTest/dirt.jpg"), g("Terrian/TerrainTest/Grass2.jpg"), 
		b("Terrian/TerrainTest/road.jpg"), a("Terrian/TerrainTest/GreenSkin.jpg");
	//Ȼ������ʹ��ϸ��ͼ�����Terrain::TerrainData,Terrain::TerrainData �Ĺ��캯������������ʽ�������û���ͬ�Ĺ�����ε���������������ֻ������õ�һ�֣����캯���ĵ�һ������Ϊ�߶�ͼ��·�����ڶ�������ΪAlpha��ͼ��·�������������ĸ�����Ϊ֮ǰ��������ĸ�DetailMap�ṹ�塣
	Terrain::TerrainData data("Terrian/TerrainTest/heightmap16.jpg", "Terrian/TerrainTest/alphamap.png", r, g, b, a);
	//��󣬽����������TerrainData����Terrain::create���ɴ�����������
	//Terrain::create�ĵڶ�������Ϊ����LOD�ѷ�ķ첹������LOD����������ͨ����Զ���ĵ��ο��Mesh���Լ򻯣��Ӷ��ڲ�Ӱ���Ӿ�Ч��������£�������ܵķ�������������ٽ����������ο��LOD��ͬʱ�����Ե���ܻ�����ѷ죬Ϊ����ϷЧ�������ۣ�������Ҫ���ѷ�������Terrain�ṩ���ַ�ʽ���ѷ��������ֱ���ȹ���Լ����ߵķ���������������ʹ��ȹ�߷������ѷ�����
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
	//����UI����
	auto rootNode = CSLoader::createNode("Terrian/MainScene.csb");
	addChild(rootNode);
	//���н�������
	auto visibleSize = Director::getInstance()->getVisibleSize();
	rootNode->setContentSize(visibleSize);
	ui::Helper::doLayout(rootNode);
	cocos2d::ui::Layout* rootPane = static_cast<cocos2d::ui::Layout*>(rootNode->getChildByName("Panel_1"));
	//��ȡ�ؼ�
	auto Button_1 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_1"));
	Button_1->setTag(1);
	//��Ӱ�ť����¼�
	Button_1->addClickEventListener(CC_CALLBACK_1(Terrian::onButtonClick, this));


	//��ȡ�ؼ�
	auto Button_2 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_2"));
	Button_2->setTag(2);
	//��Ӱ�ť����¼�
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