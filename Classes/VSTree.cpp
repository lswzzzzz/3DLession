#include "VSTree.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "DrawNode3D.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* VSTree::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = VSTree::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool VSTree::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	InitUI();
	_meshUVRunTime = 0.0f;
	////�õ���Ļ�ߴ�
	auto s = Director::getInstance()->getWinSize();
	//����3D��
	auto layer3D = Layer::create();
	addChild(layer3D, 0);

	//����3D�����
	_camera = cocos2d::Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);
	//����������۾�λ��
	_camera->setPosition3D(Vec3(0, 40, 40));
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
	line->setGlobalZOrder(-1);
	layer3D->addChild(line);

	
	//������̬ģ��
	auto sprite3D = Sprite3D::create("VSTree/tree.c3t");
	_sprite3D = sprite3D;
	//ʹ���Զ���Ĭ��shader
	auto shader = GLProgram::createWithFilenames("VSTree/tree.vert", "VSTree/tree.frag");
	//����һ��GL��̵�״̬ͨ��shader
	_state = GLProgramState::create(shader);
	sprite3D->setGLProgramState(_state);
	sprite3D->setCameraMask(2);
	sprite3D->setScale(0.02);
	sprite3D->setPosition3D(Vec3(0, 0, 0));
	sprite3D->setRotation3D(Vec3(-90, 0, 0));
	layer3D->addChild(sprite3D);

    return true;
}
void VSTree::meshUVTimeUpdate(float dt)
{
	if (_state)
	{
		_meshUVRunTime += dt;
		_state->setUniformFloat("UVRunTime", _meshUVRunTime);
	}
}
void  VSTree::InitUI()
{
	//����UI����
	auto rootNode = CSLoader::createNode("VSTree/MainScene.csb");
	addChild(rootNode);
	//���н�������
	auto visibleSize = Director::getInstance()->getVisibleSize();
	rootNode->setContentSize(visibleSize);
	rootNode->setScale(0.4);
	rootNode->setPosition(visibleSize.width / 3, visibleSize.height / 2);
	ui::Helper::doLayout(rootNode);
	cocos2d::ui::Layout* rootPane = static_cast<cocos2d::ui::Layout*>(rootNode->getChildByName("Panel_1"));
	//��ȡ�ؼ�
	auto Button_1 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_1"));
	//�����ð�ť��ǣ������ֵ�������Ǹ���ť
	Button_1->setTag(1);
	//��Ӱ�ť����¼�
	Button_1->addClickEventListener(CC_CALLBACK_1(VSTree::onButtonClick, this));
	//��ȡ�ؼ�
	auto Button_2 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_2"));
	//�����ð�ť��ǣ������ֵ�������Ǹ���ť
	Button_2->setTag(2);
	//��Ӱ�ť����¼�
	Button_2->addClickEventListener(CC_CALLBACK_1(VSTree::onButtonClick, this));
}
void VSTree::onButtonClick(cocos2d::Ref* sender)
{
	cocos2d::ui::Button* btn = (cocos2d::ui::Button*)sender;
	if (btn)
	{
		//ж�ص����ж�ʱ��
		unscheduleAllCallbacks();
		if (btn->getTag() == 1)
		{
			//����ģ���Զ���shader
			auto shader = GLProgram::createWithFilenames("VSTree/tree.vert", "VSTree/tree.frag");
			_state = GLProgramState::create(shader);
			//ȫ����ģ��Ӧ��Ĭ��Ч��
			_sprite3D->setGLProgramState(_state);
		}
		else 
		{
			
			
			//ʹ���Զ���Ĭ��shader
			auto shader = GLProgram::createWithFilenames("VSTree/treeSwing.vert", "VSTree/tree.frag");
			//����һ��GL��̵�״̬ͨ��shader
			_state = GLProgramState::create(shader);
			//��ȡģ����Ҷ��ģ��
			if (_sprite3D->getMeshByIndex(0))
			{
				//��Ҷ��ģ��Ӧ��ҡ��Ч��
				_sprite3D->getMeshByIndex(0)->setGLProgramState(_state);
			}
			//������ʱ����������Ҷҡ��ʱ��
			schedule(schedule_selector(VSTree::meshUVTimeUpdate));
		}
	}

}