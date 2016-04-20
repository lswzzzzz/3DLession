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

	//���������,���ù����������ͼ ���ù����ģʽ ĿǰCocos����֧���������͵Ĺ���壺һ�ֳ����������X0Yƽ�棻��һ�����������ԭ��
	_Logo = BillBoard::create("BillBoardTest/Logo.png");
	//��������
	_Logo->setScale(0.2);
	//����λ��
	_Logo->setPosition3D(Vec3(50.0f, 0.0f, 50.0f));
	//��ΪCameraFlag::USER1 Ϊ 2,��2 & 2 ��ֵ����0������pPline�������������Ĺ۲����б��С�
	_Logo->setCameraMask(2);
	_layer3D->addChild(_Logo);

	//��ľ
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
		
		//��������
		_Tree[i]->setScale(rand_0_1() * 0.05 + 0.05);
		//����λ��
		_Tree[i]->setPosition3D(Vec3(rand_0_1() * 100, 0.0f, rand_0_1() * 100));
		//��ΪCameraFlag::USER1 Ϊ 2,��2 & 2 ��ֵ����0������pPline�������������Ĺ۲����б��С�
		_Tree[i]->setCameraMask(2);
		//����Ϊ����ʾ
		_Tree[i]->setVisible(false);
		_layer3D->addChild(_Tree[i]);
	}
	//ѩ��
	for (int i = 0; i < 50; i++)
	{
		_Snow[i] = BillBoard::create("BillBoardTest/snow.png");
		//��������
		_Snow[i]->setScale(rand_0_1() * 0.01 + 0.01);
		//����λ��
		_Snow[i]->setPosition3D(Vec3(rand_0_1() * 100, rand_0_1() * 30.0f, rand_0_1() * 100));
		//��ΪCameraFlag::USER1 Ϊ 2,��2 & 2 ��ֵ����0������pPline�������������Ĺ۲����б��С�
		_Snow[i]->setCameraMask(2);
		//����Ϊ����ʾ
		_Snow[i]->setVisible(false);
		_layer3D->addChild(_Snow[i]);
	}

	
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


	//Ѫ��_Blood
	_Blood = BillBoard::create("BillBoardTest/blood1.png");
	_Blood->setScale(0.05);
	_Blood->setPosition3D(Vec3(0, 2, 0));
	_Blood->setCameraMask(2);
	_Blood->setVisible(false);
	_Tank_Base->addChild(_Blood);

	//����һ����ʱ��,ʵ��������ƹ۲��360��ת
	schedule(schedule_selector(BillBoardTest::rotCamera));
    return true;
}

void  BillBoardTest::rotCamera(float dt)
{
	static float angle = 0.0f;
	angle += dt * 0.1;
	if (_camera)
	{
		//ͨ���Ƕȼ������ת���������۾���λ�ã��۲���λ��
		_camera->setPosition3D(Vec3(50 + 100 * sinf(angle), 30.0f, 50 + 100 * cosf(angle)));
		_camera->lookAt(Vec3(50,0,50));
	}

	//ѩ��
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
	//����UI����
	auto rootNode = CSLoader::createNode("BillBoardTest/MainScene.csb");
	addChild(rootNode);
	//���н�������
	auto visibleSize = Director::getInstance()->getVisibleSize();
	rootNode->setContentSize(visibleSize);
	ui::Helper::doLayout(rootNode);
	//��ȡ�������
	cocos2d::ui::Layout* rootPane = static_cast<cocos2d::ui::Layout*>(rootNode->getChildByName("Panel_1"));
	//��ȡ�ؼ�
	auto Button_1 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_1"));
	//�����ð�ť��ǣ������ֵ�������Ǹ���ť
	Button_1->setTag(1);
	//��Ӱ�ť����¼�
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
		//�Ƚ����еĹ���嶼��Ϊ����ʾ
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

		//��Ӧ���µİ�ť����Ӧ�ĸ��������ʾ��
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