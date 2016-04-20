#include "UVAnimStroke.h"
#include "cocostudio/CocoStudio.h"
#include "DrawNode3D.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* UVAnimStroke::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = UVAnimStroke::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool UVAnimStroke::init()
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
	_camera->setPosition3D(Vec3(0, 100, 100));
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

	//��Ӵ����¼�
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(UVAnimStroke::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(UVAnimStroke::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(UVAnimStroke::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//����3D��̬ģ��
	auto spriteStaticMesh = Sprite3D::create("UVAnimStroke/qiu.c3b");
	_spriteStatic3D = spriteStaticMesh;
	spriteStaticMesh->setCameraMask(2);
	spriteStaticMesh->setScale(0.5);
	spriteStaticMesh->setPosition3D(Vec3(-30, 0, 0));
	spriteStaticMesh->setRotation3D(Vec3(0, 180, 0));

	//��������
	auto staticMeshMat = Material::createWithFilename("UVAnimStroke/3d_StaticMesh.material");
	staticMeshMat->setTechnique("normal");
	spriteStaticMesh->setMaterial(staticMeshMat);
	layer3D->addChild(spriteStaticMesh);


	////��������ģ��
	auto spriteAniMesh = Sprite3D::create("UVAnimStroke/orc.c3b");
	_spriteAni3D = spriteAniMesh;
	spriteAniMesh->setCameraMask(2);
	spriteAniMesh->setScale(2);
	spriteAniMesh->setPosition3D(Vec3(30, 0, 0));
	spriteAniMesh->setRotation3D(Vec3(0, 180, 0));

	//����3Dģ�Ͷ���
	auto animation = Animation3D::create("UVAnimStroke/orc.c3b");
	if (animation)
	{
		auto animate = Animate3D::create(animation);
		spriteAniMesh->runAction(RepeatForever::create(animate));
	}

	//��������
	auto aniMeshmMat = Material::createWithFilename("UVAnimStroke/3d_AinMesh.material");
	aniMeshmMat->setTechnique("normal");
	//���ò���
	spriteAniMesh->setMaterial(aniMeshmMat);
	layer3D->addChild(spriteAniMesh);

    return true;
}
void UVAnimStroke::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}

void UVAnimStroke::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
	if (touches.size())
	{
		auto touch = touches[0];
		auto delta = touch->getDelta();

		_angle -= CC_DEGREES_TO_RADIANS(delta.x);
		if (_camera)
		{
			_camera->setPosition3D(Vec3(100 * sinf(_angle), 100.0f, 100 * cosf(_angle)));
			_camera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
		}	
	}
}

void UVAnimStroke::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}
void  UVAnimStroke::InitUI()
{
	//����UI����
	auto rootNode = CSLoader::createNode("UVAnimStroke/MainScene.csb");
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
	Button_1->addClickEventListener(CC_CALLBACK_1(UVAnimStroke::onButtonClick, this));
	//��ȡ�ؼ�
	auto Button_2 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_2"));
	//�����ð�ť��ǣ������ֵ�������Ǹ���ť
	Button_2->setTag(2);
	//��Ӱ�ť����¼�
	Button_2->addClickEventListener(CC_CALLBACK_1(UVAnimStroke::onButtonClick, this));
}


void UVAnimStroke::onButtonClick(cocos2d::Ref* sender)
{
	cocos2d::ui::Button* btn = (cocos2d::ui::Button*)sender;
	if (btn)
	{
		if (btn->getTag() == 2){
			if (_spriteStatic3D){
				if (_spriteStatic3D->getMaterial(0)){
					_spriteStatic3D->getMaterial(0)->setTechnique("outline");
				}
			}
			if (_spriteAni3D){
				if (_spriteAni3D->getMaterial(0)){
					_spriteAni3D->getMaterial(0)->setTechnique("outline");
				}
			}
		}
		else{
			if (_spriteStatic3D){
				if (_spriteStatic3D->getMaterial(0)){
					_spriteStatic3D->getMaterial(0)->setTechnique("normal");


				}
			}
			if (_spriteAni3D){
				if (_spriteAni3D->getMaterial(0)){
					_spriteAni3D->getMaterial(0)->setTechnique("normal");
				}
			}
		}
	}

}