#include "UVAnimLight.h"
#include "cocostudio/CocoStudio.h"
#include "DrawNode3D.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* UVAnimLight::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = UVAnimLight::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool UVAnimLight::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	InitUI();
	_angle = 0.0f;
	////�õ���Ļ�ߴ�
	auto s = Director::getInstance()->getWinSize();
	//����3D��
	auto layer3D = Layer::create();
	addChild(layer3D, 0);

	//����3D�����
	_camera = cocos2d::Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);
	//����������۾�λ��
	_camera->setPosition3D(Vec3(0, 50, 50));
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

	//������̬ģ��
	auto spriteStatic3D = Sprite3D::create("UVAnimLight/qiu.c3t");
	_sprite3D = spriteStatic3D;
	spriteStatic3D->setCameraMask(2);
	spriteStatic3D->setScale(0.5);
	spriteStatic3D->setPosition3D(Vec3(0, 0, 0));
	spriteStatic3D->setRotation3D(Vec3(0, 0, 0));
	//���ò�ʹ�õƹ���
	spriteStatic3D->setLightMask(0);
	layer3D->addChild(spriteStatic3D);

	//����������
	_ambientLight = AmbientLight::create(Color3B(0, 255, 0));
	_ambientLight->setEnabled(true);
	_ambientLight->setCameraMask(2);
	layer3D->addChild(_ambientLight);

	//���������
	_directionalLight = DirectionLight::create(Vec3(-1.0f, -1.0f, 0.0f), Color3B(0, 255, 0));
	_directionalLight->setCameraMask(2);
	_directionalLight->setEnabled(true);
	layer3D->addChild(_directionalLight);

	//�������
	_pointLight = PointLight::create(Vec3(0.0f, 50.0f, 0.0f), Color3B(0, 255, 0), 500.0f);
	_pointLight->setEnabled(true);
	_pointLight->setCameraMask(2);
	layer3D->addChild(_pointLight);

	//�����۹��
	_spotLight = SpotLight::create(Vec3(0, -1.0f, 0.0f), Vec3(0.0f, 50.0f, 0.0f), Color3B(0, 255, 0), 0.0, 0.5, 10000.0f);
	_spotLight->setCameraMask(2);
	_spotLight->setEnabled(true);
	layer3D->addChild(_spotLight);

	//��Ӵ����¼�
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(UVAnimLight::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(UVAnimLight::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(UVAnimLight::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	
    return true;
}
void UVAnimLight::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}

void UVAnimLight::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event)
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

void UVAnimLight::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}
void  UVAnimLight::InitUI()
{
	//����UI����
	auto rootNode = CSLoader::createNode("UVAnimLight/MainScene.csb");
	addChild(rootNode, 10);
	//���н�������
	auto visibleSize = Director::getInstance()->getVisibleSize();
	rootNode->setContentSize(visibleSize);
	rootNode->setScale(0.4);
	rootNode->setPosition(visibleSize.width / 6, visibleSize.height / 4);
	ui::Helper::doLayout(rootNode);
	//��ȡ�ؼ�
	auto Button_1 = static_cast<cocos2d::ui::Button*>(rootNode->getChildByName("Button_1"));
	//�����ð�ť��ǣ������ֵ�������Ǹ���ť
	Button_1->setTag(1);
	//��Ӱ�ť����¼�
	Button_1->addClickEventListener(CC_CALLBACK_1(UVAnimLight::onButtonClick, this));

	auto Button_2 = static_cast<cocos2d::ui::Button*>(rootNode->getChildByName("Button_2"));
	Button_2->setTag(2);
	Button_2->addClickEventListener(CC_CALLBACK_1(UVAnimLight::onButtonClick, this));

	auto Button_3 = static_cast<cocos2d::ui::Button*>(rootNode->getChildByName("Button_3"));
	Button_3->setTag(3);
	Button_3->addClickEventListener(CC_CALLBACK_1(UVAnimLight::onButtonClick, this));

	auto Button_4 = static_cast<cocos2d::ui::Button*>(rootNode->getChildByName("Button_4"));
	Button_4->setTag(4);
	Button_4->addClickEventListener(CC_CALLBACK_1(UVAnimLight::onButtonClick, this));

	auto Button_5 = static_cast<cocos2d::ui::Button*>(rootNode->getChildByName("Button_5"));
	Button_5->setTag(5);
	Button_5->addClickEventListener(CC_CALLBACK_1(UVAnimLight::onButtonClick, this));
}


void UVAnimLight::onButtonClick(cocos2d::Ref* sender)
{
	cocos2d::ui::Button* btn = (cocos2d::ui::Button*)sender;
	if (btn)
	{
		_ambientLight->setEnabled(false);
		_directionalLight->setEnabled(false);
		_pointLight->setEnabled(false);
		_spotLight->setEnabled(false);
		if (btn->getTag() == 1)
		{
			_ambientLight->setEnabled(true);
			if (_sprite3D)
			{
				_sprite3D->setLightMask(-1);
			}
			
		}
		else if (btn->getTag() == 2)
		{
			_directionalLight->setEnabled(true);
			if (_sprite3D)
			{
				_sprite3D->setLightMask(-1);
			}

		}
		else if (btn->getTag() == 3)
		{
			_pointLight->setEnabled(true);
			if (_sprite3D)
			{
				_sprite3D->setLightMask(-1);
			}

		}
		else if (btn->getTag() == 4)
		{
			_spotLight->setEnabled(true);
			if (_sprite3D)
			{
				_sprite3D->setLightMask(-1);
			}

		}
		else
		{
			if (_sprite3D)
			{
				_sprite3D->setLightMask(0);
			}
		}
	}

}