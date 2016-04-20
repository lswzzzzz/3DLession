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
	////得到屏幕尺寸
	auto s = Director::getInstance()->getWinSize();
	//创建3D层
	auto layer3D = Layer::create();
	addChild(layer3D, 0);

	//创建3D摄像机
	_camera = cocos2d::Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);
	//设置摄像机眼睛位置
	_camera->setPosition3D(Vec3(0, 100, 100));
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

	//添加触摸事件
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(UVAnimStroke::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(UVAnimStroke::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(UVAnimStroke::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//创建3D静态模型
	auto spriteStaticMesh = Sprite3D::create("UVAnimStroke/qiu.c3b");
	_spriteStatic3D = spriteStaticMesh;
	spriteStaticMesh->setCameraMask(2);
	spriteStaticMesh->setScale(0.5);
	spriteStaticMesh->setPosition3D(Vec3(-30, 0, 0));
	spriteStaticMesh->setRotation3D(Vec3(0, 180, 0));

	//创建材质
	auto staticMeshMat = Material::createWithFilename("UVAnimStroke/3d_StaticMesh.material");
	staticMeshMat->setTechnique("normal");
	spriteStaticMesh->setMaterial(staticMeshMat);
	layer3D->addChild(spriteStaticMesh);


	////创建动画模型
	auto spriteAniMesh = Sprite3D::create("UVAnimStroke/orc.c3b");
	_spriteAni3D = spriteAniMesh;
	spriteAniMesh->setCameraMask(2);
	spriteAniMesh->setScale(2);
	spriteAniMesh->setPosition3D(Vec3(30, 0, 0));
	spriteAniMesh->setRotation3D(Vec3(0, 180, 0));

	//创建3D模型动画
	auto animation = Animation3D::create("UVAnimStroke/orc.c3b");
	if (animation)
	{
		auto animate = Animate3D::create(animation);
		spriteAniMesh->runAction(RepeatForever::create(animate));
	}

	//创建材质
	auto aniMeshmMat = Material::createWithFilename("UVAnimStroke/3d_AinMesh.material");
	aniMeshmMat->setTechnique("normal");
	//设置材质
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
	//加载UI界面
	auto rootNode = CSLoader::createNode("UVAnimStroke/MainScene.csb");
	addChild(rootNode);
	//进行界面适配
	auto visibleSize = Director::getInstance()->getVisibleSize();
	rootNode->setContentSize(visibleSize);
	rootNode->setScale(0.4);
	rootNode->setPosition(visibleSize.width / 3, visibleSize.height / 2);
	ui::Helper::doLayout(rootNode);
	cocos2d::ui::Layout* rootPane = static_cast<cocos2d::ui::Layout*>(rootNode->getChildByName("Panel_1"));
	//获取控件
	auto Button_1 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_1"));
	//给设置按钮标记，已区分点击的是那个按钮
	Button_1->setTag(1);
	//添加按钮点击事件
	Button_1->addClickEventListener(CC_CALLBACK_1(UVAnimStroke::onButtonClick, this));
	//获取控件
	auto Button_2 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_2"));
	//给设置按钮标记，已区分点击的是那个按钮
	Button_2->setTag(2);
	//添加按钮点击事件
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