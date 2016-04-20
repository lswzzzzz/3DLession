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
	//�õ���Ļ�ߴ�
	auto s = Director::getInstance()->getWinSize();
	//����3D��
	_layer3D = Layer::create();
	addChild(_layer3D, 0);
	//����һ��3D�������������׶����ز���:��׶�ſ��Ƕ�,��߱ȣ���������룬Զ�������
	auto pCamera = Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);
	_camera = pCamera;
	//����������۾�λ��
	pCamera->setPosition3D(Vec3(100, 30, 100));
	//����������۲��λ��
	pCamera->lookAt(Vec3(50, 0, 50));
	/*��Cocos�У�ͨ����һ�����������һ�������������ָ��������ʹ�ò�ͬ����������й۲죬������´��������������һ����ǡ�
	//��Cocos�ж�����8��������ı�ʶ����Ȼ��8�����Թ����ˡ�ָ������ͨ���������õ������ģֵ��Mask)�����������ǽ�����(&)����,�������0����˵�����������������Ĺ۲�ͶӰ����Ӱ�졣
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
	//��ΪCameraFlag::USER1 Ϊ 2,��2 & 2 ��ֵ����0������layer3D�������������Ĺ۲����б��С�
	_layer3D->setCameraMask(2);
	//����3D����(20x20������)
	DrawNode3D* pPline = DrawNode3D::create();
	//�������21���ߣ���ɫ�ġ�
	for (int j = 0; j < 21; j++)
	{
		pPline->drawLine(Vec3(0, 0, 5 * j), Vec3(100, 0, 5 * j), Color4F(1, 0, 0, 1));
	}
	//�������21���ߣ���ɫ��
	for (int j = 0; j < 21; j++)
	{
		pPline->drawLine(Vec3(5 * j, 0, 0), Vec3(5 * j, 0, 100), Color4F(0, 0, 1, 1));
	}
	//��ΪCameraFlag::USER1 Ϊ 2,��2 & 2 ��ֵ����0������pPline�������������Ĺ۲����б��С�
	pPline->setCameraMask(2);
	pPline->setGlobalZOrder(-1);
	_layer3D->addChild(pPline);
	
	//������̬ģ��
	_sprite3D = Sprite3D::create("ModelTest/qiu.c3b");
	//��ΪCameraFlag::USER1 Ϊ 2,��2 & 2 ��ֵ����0������spriteStatic3D�������������Ĺ۲����б��С�
	_sprite3D->setCameraMask(2);
	//��������ֵ
	_sprite3D->setScale(1);
	//����λ��
	_sprite3D->setPosition3D(Vec3(0, -20, 0));
	//������ת
	_sprite3D->setRotation3D(Vec3(0, 0, 0));

	//��ģ�������ת����
	_sprite3D->runAction(RepeatForever::create(Sequence::create(RotateBy::create(3.0f, Vec3(0, 360, 0)), nullptr)));
	//����layer3D
	_layer3D->addChild(_sprite3D);
	
    return true;
}

void  ModelTest::InitUI()
{
	//����UI����
	auto rootNode = CSLoader::createNode("ModelTest/MainScene.csb");
	addChild(rootNode);
	//���н�������
	auto visibleSize = Director::getInstance()->getVisibleSize();

	rootNode->setContentSize(visibleSize);
	ui::Helper::doLayout(rootNode);
	cocos2d::ui::Layout* rootPane = static_cast<cocos2d::ui::Layout*>(rootNode->getChildByName("Panel_1"));
	//��ȡ�ؼ�
	auto Button_1 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_1"));
	//�����ð�ť��ǣ������ֵ�������Ǹ���ť
	Button_1->setTag(1);
	//��Ӱ�ť����¼�
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
		//�Ƴ���ǰģ��
		if (_sprite3D)
		{
			_sprite3D->removeFromParent();
			_sprite3D = nullptr;
		}
		if (btn->getTag() == 1)
		{
			//������̬ģ��
			_sprite3D = Sprite3D::create("ModelTest/qiu.c3b");
			//��ΪCameraFlag::USER1 Ϊ 2,��2 & 2 ��ֵ����0������spriteStatic3D�������������Ĺ۲����б��С�
			_sprite3D->setCameraMask(2);
			//��������ֵ
			_sprite3D->setScale(1);
			//����λ��
			_sprite3D->setPosition3D(Vec3(0, -20, 0));
			//������ת
			_sprite3D->setRotation3D(Vec3(0, 0, 0));
			//��ģ�������ת����
			_sprite3D->runAction(RepeatForever::create(Sequence::create(RotateBy::create(3.0f, Vec3(0, 360, 0)), nullptr)));
			//����layer3D
			_layer3D->addChild(_sprite3D);
		}
		else if (btn->getTag() == 2)
		{
			//������̬ģ��
			_sprite3D = Sprite3D::create("ModelTest/gun.c3b");
			//��ΪCameraFlag::USER1 Ϊ 2,��2 & 2 ��ֵ����0������spriteStatic3D�������������Ĺ۲����б��С�
			_sprite3D->setCameraMask(2);
			//��������ֵ
			_sprite3D->setScale(1);
			//����λ��
			_sprite3D->setPosition3D(Vec3(0, -20, 0));
			//������ת
			_sprite3D->setRotation3D(Vec3(0, 0, 0));
			//��ģ�������ת����
			_sprite3D->runAction(RepeatForever::create(Sequence::create(RotateBy::create(3.0f, Vec3(0, 360, 0)), nullptr)));
			//����layer3D
			_layer3D->addChild(_sprite3D);
		}
		else if (btn->getTag() == 3)
		{
			//������̬ģ��
			_sprite3D = Sprite3D::create("ModelTest/snake.c3b");
			//��ΪCameraFlag::USER1 Ϊ 2,��2 & 2 ��ֵ����0������spriteStatic3D�������������Ĺ۲����б��С�
			_sprite3D->setCameraMask(2);
			//��������ֵ
			_sprite3D->setScale(1);
			//����λ��
			_sprite3D->setPosition3D(Vec3(0, -20, 0));
			//������ת
			_sprite3D->setRotation3D(Vec3(0, 0, 0));
			//��ģ�������ת����
			_sprite3D->runAction(RepeatForever::create(Sequence::create(RotateBy::create(3.0f, Vec3(0, 360, 0)), nullptr)));
			//����layer3D
			_layer3D->addChild(_sprite3D);
		}
		else
		{
			//������̬ģ��
			_sprite3D = Sprite3D::create("ModelTest/mod_niutou.c3b");
			//��ΪCameraFlag::USER1 Ϊ 2,��2 & 2 ��ֵ����0������spriteStatic3D�������������Ĺ۲����б��С�
			_sprite3D->setCameraMask(2);
			//��������ֵ
			_sprite3D->setScale(0.2);
			//����λ��
			_sprite3D->setPosition3D(Vec3(0, -20, 0));
			//������ת
			_sprite3D->setRotation3D(Vec3(0, 0, 0));
			//��ģ�������ת����
			_sprite3D->runAction(RepeatForever::create(Sequence::create(RotateBy::create(3.0f, Vec3(0, 360, 0)), nullptr)));
			//����layer3D
			_layer3D->addChild(_sprite3D);
		}

	}

}