#include "AABBOBB.h"
#include "cocostudio/CocoStudio.h"
#include "DrawNode3D.h"
#include "ui/CocosGUI.h"
#include "Particle3D/CCParticleSystem3D.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* AABBOBB::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = AABBOBB::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool AABBOBB::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	InitUI();
	_meshCollType = 0;
	////得到屏幕尺寸
	auto s = Director::getInstance()->getWinSize();
	//创建3D层
	auto layer3D = Layer::create();
	addChild(layer3D, 0);

	//创建3D摄像机
	auto _camera = cocos2d::Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);
	//设置摄像机眼睛位置
	_camera->setPosition3D(Vec3(0, 50, 50));
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

	_spriteStatic3D[0] = Sprite3D::create("AABBOBB/qiu.c3b");
	_spriteStatic3D[0]->setCameraMask(2);
	_spriteStatic3D[0]->setScale(0.2);
	_spriteStatic3D[0]->setPosition3D(Vec3(-50, 0, 0));
	_spriteStatic3D[0]->setRotation3D(Vec3(0, 0, 0));
	layer3D->addChild(_spriteStatic3D[0]);

	if (_spriteStatic3D[0]){
		auto moveAction = MoveTo::create(4.f, Vec3(60, 0, 0));
		auto seq = Sequence::create(moveAction, CallFuncN::create(CC_CALLBACK_1(AABBOBB::aniEndCallBack, this)), nullptr);
		_spriteStatic3D[0]->runAction(seq);
	}
	
	_spriteStatic3D[1] = Sprite3D::create("AABBOBB/qiu.c3b");
	_spriteStatic3D[1]->setCameraMask(2);
	_spriteStatic3D[1]->setScale(0.2);
	_spriteStatic3D[1]->setPosition3D(Vec3(40, 0, 0));
	_spriteStatic3D[1]->setRotation3D(Vec3(0, 0, 0));
	layer3D->addChild(_spriteStatic3D[1]);

	for (int i = 0; i < 2; i++){
		_spriteStatic3DBox[i] = DrawNode3D::create();
		_spriteStatic3DBox[i]->setCameraMask(2);
		layer3D->addChild(_spriteStatic3DBox[i]);
	}
	_meshRotateValue = 0;
	if (_meshCollType == 0){
		this->schedule(schedule_selector(AABBOBB::updateAABBColl));
	}
	else{
		this->schedule(schedule_selector(AABBOBB::updateOBBColl));
	}
	return true;
}

void AABBOBB::aniEndCallBack(Node* seedNode){
	if (seedNode)
	{
		seedNode->stopAllActions();
		Vec3 pos = seedNode->getPosition3D();
		auto moveAction = MoveTo::create(4.f, Vec3(-pos.x, pos.y, pos.z));
		auto seq = Sequence::create(moveAction, CallFuncN::create(CC_CALLBACK_1(AABBOBB::aniEndCallBack, this)), nullptr);
		seedNode->runAction(seq);
	}
}

void AABBOBB::updateAABBColl(float dt){
	_meshRotateValue += dt * 100;
	cocos2d::AABB spriteStatic3DAABB[2];
	for (int i = 0; i < 2; i++){
		_spriteStatic3D[i]->setRotation3D(Vec3(0, _meshRotateValue, 0));
		spriteStatic3DAABB[i] = _spriteStatic3D[i]->getAABB();
	}

	bool bColl = spriteStatic3DAABB[0].intersects(spriteStatic3DAABB[1]);
	for (int i = 0; i < 2; i++){
		_spriteStatic3DBox[i]->clear();
		Vec3 corners[8] = {};
		spriteStatic3DAABB[i].getCorners(corners);
		if (bColl){
			_spriteStatic3DBox[i]->drawCube(corners, Color4F(1, 0, 0, 1));
		}
		else{
			_spriteStatic3DBox[i]->drawCube(corners, Color4F(0, 1, 0, 1));
		}
	}
}

void AABBOBB::updateOBBColl(float dt){
	cocos2d::OBB spriteStatic3DObb[2];
	_meshRotateValue += dt * 100;
	for (int i = 0; i < 2; i++){
		_spriteStatic3D[i]->setRotation3D(Vec3(0, _meshRotateValue, 0));
		cocos2d::AABB spriteStatic3DAABB = _spriteStatic3D[i]->getAABB();
		spriteStatic3DObb[i] = OBB(spriteStatic3DAABB);
	}
	Mat4 tRotateMat;
	tRotateMat.rotateY(_meshRotateValue);
	Vec3 tPosition = spriteStatic3DObb[0]._center;
	spriteStatic3DObb[0]._center = Vec3(0, 0, 0);
	spriteStatic3DObb[0].transform(tRotateMat);
	spriteStatic3DObb[0]._center = tPosition;

	bool bColl = spriteStatic3DObb[0].intersects(spriteStatic3DObb[1]);
	for (int i = 0; i < 2; i++){
		_spriteStatic3DBox[i]->clear();
		Vec3 corners[8] = {};
		spriteStatic3DObb[i].getCorners(corners);
		if (bColl){
			_spriteStatic3DBox[i]->drawCube(corners, Color4F(1, 0, 0, 1));
		}
		else{
			_spriteStatic3DBox[i]->drawCube(corners, Color4F(0, 1, 0, 1));
		}
	}
}

void AABBOBB::InitUI(){
	//加载UI界面
	auto rootNode = CSLoader::createNode("AABBOBB/MainScene.csb");
	addChild(rootNode);
	//进行界面适配
	auto visibleSize = Director::getInstance()->getVisibleSize();
	rootNode->setContentSize(visibleSize);
	ui::Helper::doLayout(rootNode);
	cocos2d::ui::Layout* rootPane = static_cast<cocos2d::ui::Layout*>(rootNode->getChildByName("Panel_1"));
	//获取控件
	auto Button_1 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_1"));
	//给设置按钮标记，已区分点击的是那个按钮
	Button_1->setTag(1);
	//添加按钮点击事件
	Button_1->addClickEventListener(CC_CALLBACK_1(AABBOBB::onButtonClick, this));
	//获取控件
	auto Button_2 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_2"));
	//给设置按钮标记，已区分点击的是那个按钮
	Button_2->setTag(2);
	//添加按钮点击事件
	Button_2->addClickEventListener(CC_CALLBACK_1(AABBOBB::onButtonClick, this));
}

void AABBOBB::onButtonClick(cocos2d::Ref* sender){
	cocos2d::ui::Button* btn = (cocos2d::ui::Button*)sender;
	if (btn)
	{
		if (btn->getTag() == 1){
			if (_meshCollType == 0){
				return;
			}
			unscheduleAllCallbacks();
			for (int i = 0; i < 2; i++){
				_spriteStatic3D[i]->setRotation3D(Vec3(0, 0, 0));
			}
			this->schedule(schedule_selector(AABBOBB::updateAABBColl));
			_meshCollType = 0;
		}
		else{
			if (_meshCollType == 1){
				return;
			}
			unscheduleAllCallbacks();
			this->schedule(schedule_selector(AABBOBB::updateOBBColl));
			_meshCollType = 1;
		}
	}
}