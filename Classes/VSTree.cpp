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
	////得到屏幕尺寸
	auto s = Director::getInstance()->getWinSize();
	//创建3D层
	auto layer3D = Layer::create();
	addChild(layer3D, 0);

	//创建3D摄像机
	_camera = cocos2d::Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);
	//设置摄像机眼睛位置
	_camera->setPosition3D(Vec3(0, 40, 40));
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
	line->setGlobalZOrder(-1);
	layer3D->addChild(line);

	
	//创建静态模型
	auto sprite3D = Sprite3D::create("VSTree/tree.c3t");
	_sprite3D = sprite3D;
	//使用自定义默认shader
	auto shader = GLProgram::createWithFilenames("VSTree/tree.vert", "VSTree/tree.frag");
	//创建一个GL编程的状态通过shader
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
	//加载UI界面
	auto rootNode = CSLoader::createNode("VSTree/MainScene.csb");
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
	Button_1->addClickEventListener(CC_CALLBACK_1(VSTree::onButtonClick, this));
	//获取控件
	auto Button_2 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_2"));
	//给设置按钮标记，已区分点击的是那个按钮
	Button_2->setTag(2);
	//添加按钮点击事件
	Button_2->addClickEventListener(CC_CALLBACK_1(VSTree::onButtonClick, this));
}
void VSTree::onButtonClick(cocos2d::Ref* sender)
{
	cocos2d::ui::Button* btn = (cocos2d::ui::Button*)sender;
	if (btn)
	{
		//卸载掉所有定时器
		unscheduleAllCallbacks();
		if (btn->getTag() == 1)
		{
			//创建模型自定义shader
			auto shader = GLProgram::createWithFilenames("VSTree/tree.vert", "VSTree/tree.frag");
			_state = GLProgramState::create(shader);
			//全部子模型应用默认效果
			_sprite3D->setGLProgramState(_state);
		}
		else 
		{
			
			
			//使用自定义默认shader
			auto shader = GLProgram::createWithFilenames("VSTree/treeSwing.vert", "VSTree/tree.frag");
			//创建一个GL编程的状态通过shader
			_state = GLProgramState::create(shader);
			//获取模型树叶子模型
			if (_sprite3D->getMeshByIndex(0))
			{
				//树叶子模型应用摇摆效果
				_sprite3D->getMeshByIndex(0)->setGLProgramState(_state);
			}
			//创建定时器，更新树叶摇摆时间
			schedule(schedule_selector(VSTree::meshUVTimeUpdate));
		}
	}

}