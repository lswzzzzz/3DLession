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
    
	//�õ���Ļ�ߴ�
	auto s = Director::getInstance()->getWinSize();
	//����3D��
	_layer3D = Layer::create();
	addChild(_layer3D, 0);

	//����һ��3D�������������׶����ز���:��׶�ſ��Ƕ�,��߱ȣ���������룬Զ�������
	auto pCamera = Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);
	_camera = pCamera;
	//����������۾�λ��
	pCamera->setPosition3D(Vec3(50, 80, -80));
	//����������۲�Ŀ���λ��
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

	//����3D����
	DrawNode3D* pPline = DrawNode3D::create();
	//�������20���ߣ���ɫ�ġ�
	for (int j = 0; j <= 20; j++)
	{
		pPline->drawLine(Vec3(0, 0, 5 * j), Vec3(100, 0, 5 * j), Color4F(1, 0, 0, 1));
	}
	//�������20���ߣ���ɫ��
	for (int j = 0; j <= 20; j++)
	{
		pPline->drawLine(Vec3(5 * j, 0, 0), Vec3(5 * j, 0, 100), Color4F(0, 0, 1, 1));
	}

	//��ΪCameraFlag::USER1 Ϊ 2,��2 & 2 ��ֵ����0������pPline�������������Ĺ۲����б��С�
	pPline->setCameraMask(2);
	_layer3D->addChild(pPline);

	//����һ���۲��ڵ�(һ��̹�ˣ�
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

	//����һ���۲��ڵ�(̹�˴�һ���ڹܣ�
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
	//�۲�����λ�ƶ���
	_Tank_Base->runAction(RepeatForever::create(Sequence::create(
		MoveTo::create(5.0f, Vec3(0, 0, 100)), RotateTo::create(1.0f, Vec3(0, 90, 0)),
		MoveTo::create(5.0f, Vec3(100, 0, 100)), RotateTo::create(1.0f, Vec3(0, 180, 0)),
		MoveTo::create(5.0f, Vec3(100, 0, 0)), RotateTo::create(1.0f, Vec3(0, 270, 0)),
		MoveTo::create(5.0f, Vec3(0, 0, 0)), RotateTo::create(1.0f, Vec3(0, 360, 0)),
		nullptr)));
	//���ؽ��档
	InitUI();
    return true;
}

void  CameraTest::InitUI()
{
	//����UI����
	auto rootNode = CSLoader::createNode("CameraTest/MainScene.csb");
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
		//����
		unscheduleAllCallbacks();
		_Tank_Base->stopAllActions();
		_Tank_Base->setPosition3D(Vec3(0, 0, 0));
		_Tank_Base->setRotation3D(Vec3(0, 0, 0));

		//�۲�����λ�ƶ���
		_Tank_Base->runAction(RepeatForever::create(Sequence::create(
			MoveTo::create(5.0f, Vec3(0, 0, 100)), RotateTo::create(1.0f, Vec3(0, 90, 0)),
			MoveTo::create(5.0f, Vec3(100, 0, 100)), RotateTo::create(1.0f, Vec3(0, 180, 0)),
			MoveTo::create(5.0f, Vec3(100, 0, 0)), RotateTo::create(1.0f, Vec3(0, 270, 0)),
			MoveTo::create(5.0f, Vec3(0, 0, 0)), RotateTo::create(1.0f, Vec3(0, 360, 0)),
			nullptr)));


		if (btn->getTag() == 2)
		{
			//ɾ��֮ǰ�������
			_layer3D->removeChild(_camera);
			_Tank_Base->removeChild(_camera);
			//������һ�˳��ӽ������
			auto s = Director::getInstance()->getWinSize();
			auto pCamera = Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 500);
			_camera = pCamera;
			pCamera->setPosition3D(Vec3(0, 1, 0));
			pCamera->lookAt(Vec3(0, 0, 1));
			pCamera->setCameraFlag(CameraFlag::USER1);
			_Tank_Base->addChild(_camera);

			//����һ����ʱ��,ʵ���������һ�˳�
			schedule(schedule_selector(CameraTest::FirstView));

		}
		else if (btn->getTag() == 3)
		{
			//ɾ��֮ǰ�������
			_layer3D->removeChild(_camera);
			_Tank_Base->removeChild(_camera);
			//���������˳��ӽ������
			auto s = Director::getInstance()->getWinSize();
			auto pCamera = Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 500);
			_camera = pCamera;
			pCamera->setPosition3D(Vec3(0, 20, -20));
			pCamera->lookAt(_Tank_Gun->getPosition3D());
			pCamera->setCameraFlag(CameraFlag::USER1);
			_Tank_Base->addChild(_camera);

			//����һ����ʱ��,ʵ������������˳�
			schedule(schedule_selector(CameraTest::ThirdView));
		}
		else if (btn->getTag() == 4)
		{
			//�������ת��Ϊ�������ӹ۲�
			_layer3D->removeChild(_camera);
			_Tank_Base->removeChild(_camera);
			//��������ͶӰ�����
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
			//�������ת��Ϊ�������ӹ۲�
			_layer3D->removeChild(_camera);
			_Tank_Base->removeChild(_camera);

			//����Ĭ��3D�����
			auto s = Director::getInstance()->getWinSize();
			auto pCamera = Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);
			_camera = pCamera;
			//����������۾�λ��
			pCamera->setPosition3D(Vec3(50, 80, -80));
			//����������۲�Ŀ���λ��
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

//�����˳��ӽ�
void  CameraTest::ThirdView(float dt)
{
	if (_camera)
	{
		//ͨ����ȡ�۲��λ�ã������������۲����ƽ�ƶ���
		_camera->setPosition3D(Vec3(0, 20, -20));
		_camera->lookAt(_Tank_Gun->getPosition3D(), Vec3(0.0f, 1.0f, 0.0f));
	}
}