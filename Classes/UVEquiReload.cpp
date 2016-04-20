#include "UVEquiReload.h"
#include "cocostudio/CocoStudio.h"
#include "DrawNode3D.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* UVEquiReload::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = UVEquiReload::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool UVEquiReload::init()
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
	listener->onTouchesBegan = CC_CALLBACK_2(UVEquiReload::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(UVEquiReload::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(UVEquiReload::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//创建3D换装动画模型
	_spriteMeshReskin = nullptr;
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 size = Director::getInstance()->getVisibleSize();
	std::string fileName = "UVEquiReload/ReskinGirl.c3b";
	auto sprite = Sprite3D::create(fileName);
	sprite->setScale(0.8);
	sprite->setRotation3D(Vec3(0, 0, 0));
	sprite->setPosition3D(Vec3(0, 0, 0));
	sprite->setCameraMask(2);
	layer3D->addChild(sprite);

	auto animation = Animation3D::create(fileName);
	if (animation){
		auto animate = Animate3D::create(animation);
		sprite->runAction(RepeatForever::create(animate));
	}
	_spriteMeshReskin = sprite;

	auto& body = _skins[(int)SkinType::UPPER_BODY];
	body.push_back("Girl_UpperBody01");
	body.push_back("Girl_UpperBody02");

	auto& pants = _skins[(int)SkinType::PANTS];
	pants.push_back("Girl_LowerBody01");
	pants.push_back("Girl_LowerBody02");

	auto& shoes = _skins[(int)SkinType::SHOES];
	shoes.push_back("Girl_Shoes01");
	shoes.push_back("Girl_Shoes02");

	auto& hair = _skins[(int)SkinType::HAIR];
	hair.push_back("Girl_Hair01");
	hair.push_back("Girl_Hair02");

	auto& face = _skins[(int)SkinType::FACE];
	face.push_back("Girl_Face01");
	face.push_back("Girl_Face02");

	auto& hand = _skins[(int)SkinType::HAND];
	hand.push_back("Girl_Hand01");
	hand.push_back("Girl_Hand02");

	auto& glasses = _skins[(int)SkinType::GLASSES];
	glasses.push_back("");
	glasses.push_back("Girl_Glasses01");

	memset(_curSkin, 0, (int)SkinType::MAX_TYPE * sizeof(int));

	applyCurSkin();

    return true;
}
void UVEquiReload::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}

void UVEquiReload::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event)
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

void UVEquiReload::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}

void UVEquiReload::applyCurSkin()
{
	if (!_spriteMeshReskin){
		return;
	}
	for (ssize_t i = 0; i < _spriteMeshReskin->getMeshCount(); i++){
		auto mesh = _spriteMeshReskin->getMeshByIndex(i);
		bool isVisible = false;
		for (int j = 0; j < (int)SkinType::MAX_TYPE; j++){
			if (mesh->getName() == _skins[j].at(_curSkin[j])){
				isVisible = true;
				break;
			}
		}
		_spriteMeshReskin->getMeshByIndex(static_cast<int>(i))->setVisible(isVisible);
	}
}

void  UVEquiReload::InitUI()
{
	//加载UI界面
	auto rootNode = CSLoader::createNode("UVEquiReload/MainScene.csb");
	addChild(rootNode);
	//进行界面适配
	auto visibleSize = Director::getInstance()->getVisibleSize();
	rootNode->setContentSize(visibleSize);
	rootNode->setScale(0.5);
	rootNode->setPosition(0, visibleSize.height / 4);
	ui::Helper::doLayout(rootNode);
	cocos2d::ui::Layout* rootPane = static_cast<cocos2d::ui::Layout*>(rootNode->getChildByName("Panel_1"));
	//获取控件
	auto Button_1 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_1"));
	//设置按钮数据
	Button_1->setUserData((void*)SkinType::HAIR);
	//添加按钮点击事件
	Button_1->addClickEventListener(CC_CALLBACK_1(UVEquiReload::onButtonClick, this));
	//获取控件
	auto Button_2 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_2"));
	//设置按钮用户数据
	Button_2->setUserData((void*)SkinType::GLASSES);
	//添加按钮点击事件
	Button_2->addClickEventListener(CC_CALLBACK_1(UVEquiReload::onButtonClick, this));

	//获取控件
	auto Button_3 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_3"));
	//设置按钮用户数据
	Button_3->setUserData((void*)SkinType::UPPER_BODY);
	//添加按钮点击事件
	Button_3->addClickEventListener(CC_CALLBACK_1(UVEquiReload::onButtonClick, this));

	//获取控件
	auto Button_4 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_4"));
	//设置按钮用户数据
	Button_4->setUserData((void*)SkinType::PANTS);
	//添加按钮点击事件
	Button_4->addClickEventListener(CC_CALLBACK_1(UVEquiReload::onButtonClick, this));

	//获取控件
	auto Button_5 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_5"));
	//设置按钮用户数据
	Button_5->setUserData((void*)SkinType::SHOES);
	//添加按钮点击事件
	Button_5->addClickEventListener(CC_CALLBACK_1(UVEquiReload::onButtonClick, this));
}


void UVEquiReload::onButtonClick(cocos2d::Ref* sender)
{
	cocos2d::ui::Button* btn = (cocos2d::ui::Button*)sender;
	if (btn)
	{
		long index = (long)(((cocos2d::ui::Button*)btn)->getUserData());
		if (index < (int)SkinType::MAX_TYPE){
			_curSkin[index] = (_curSkin[index] + 1) % _skins[index].size();
			applyCurSkin();
		}
	}

}