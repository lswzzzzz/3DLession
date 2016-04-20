#include "CubeSkyBox.h"
#include "cocostudio/CocoStudio.h"
#include "DrawNode3D.h"
#include "ui/CocosGUI.h"
#include "Particle3D/CCParticleSystem3D.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* CubeSkyBox::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = CubeSkyBox::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool CubeSkyBox::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	////得到屏幕尺寸
	auto s = Director::getInstance()->getWinSize();
	//创建3D层
	auto layer3D = Layer::create();
	addChild(layer3D, 0);

	//创建3D摄像机
	_camera = cocos2d::Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);
	//设置摄像机眼睛位置
	_camera->setPosition3D(Vec3(0, 0, 50));
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

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(CubeSkyBox::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(CubeSkyBox::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(CubeSkyBox::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	auto textureCube = TextureCube::create("CubSkyBox/skybox/left.jpg", "CubSkyBox/skybox/right.jpg", "CubSkyBox/skybox/top.jpg",
		"CubSkyBox/skybox/bottom.jpg", "CubSkyBox/skybox/front.jpg", "CubSkyBox/skybox/back.jpg");
	textureCube->retain();

	Texture2D::TexParams tRepeatParams;
	tRepeatParams.magFilter = GL_LINEAR;
	tRepeatParams.minFilter = GL_LINEAR;
	tRepeatParams.wrapS = GL_CLAMP_TO_EDGE;
	tRepeatParams.wrapT = GL_CLAMP_TO_EDGE;
	textureCube->setTexParameters(tRepeatParams);

	auto skyBox = Skybox::create();
	skyBox->setTexture(textureCube);
	skyBox->setCameraMask(2);
	layer3D->addChild(skyBox);

	return true;
}

void CubeSkyBox::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}

void CubeSkyBox::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
	if (touches.size())
	{
		auto touch = touches[0];
		auto delta = touch->getDelta();
		static  float angle = 0;
		angle -= CC_DEGREES_TO_RADIANS(delta.x);

		static  float deltaY = 0;
		deltaY += delta.y;

		if (_camera)
		{
			_camera->setPosition3D(Vec3(50 * sinf(angle), 0.0f, 50 * cosf(angle)));
			_camera->lookAt(Vec3(0.0f, deltaY, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
		}
	}
}

void CubeSkyBox::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}

void CubeSkyBox::InitUI()
{
	
}

void CubeSkyBox::onButtonClick(cocos2d::Ref* sender)
{
	cocos2d::ui::Button* btn = (cocos2d::ui::Button*)sender;
	if (btn)
	{
		
	}
}