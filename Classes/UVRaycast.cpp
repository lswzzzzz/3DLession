#include "UVRaycast.h"
#include "cocostudio/CocoStudio.h"
#include "DrawNode3D.h"
#include "ui/CocosGUI.h"
#include "Particle3D/CCParticleSystem3D.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* UVRaycast::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = UVRaycast::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool UVRaycast::init()
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
	_camera->setPosition3D(Vec3(0, 5, 5));
	//设置摄像机观察点位置
	_camera->lookAt(Vec3(0, 0, 0));
	_camera->setCameraFlag(CameraFlag::USER1);
	layer3D->addChild(_camera);
	layer3D->setCameraMask(2);

	//创建地形 
	//首先，我们需要构建出地形需要的所有细节图  第一个是细节图的路径，第二个是细节图的在地形上的平铺尺寸 第二个参数有默认值，是可选的
	Terrain::DetailMap r("UVRaycast/TerrainTest/dirt.jpg"), g("UVRaycast/TerrainTest/Grass2.jpg"), 
		b("UVRaycast/TerrainTest/road.jpg"), a("UVRaycast/TerrainTest/GreenSkin.jpg");
	//然后，我们使用细节图构造出Terrain::TerrainData,Terrain::TerrainData 的构造函数有若干种形式以满足用户不同的构造地形的需求，在这里我们只给出最常用的一种，构造函数的第一个参数为高度图的路径，第二个参数为Alpha贴图的路径，接下来的四个参数为之前所构造的四个DetailMap结构体。
	Terrain::TerrainData data("UVRaycast/TerrainTest/heightmap16.jpg", "UVRaycast/TerrainTest/alphamap.png", r, g, b, a);
	//最后，将构造出来的TerrainData传给Terrain::create即可创建出地形了
	//Terrain::create的第二个参数为地形LOD裂缝的缝补方法，LOD简单来讲就是通过将远处的地形块的Mesh加以简化，从而在不影响视觉效果的情况下，提高性能的方法。但是如果临近的两个地形块的LOD不同时，其边缘可能会出现裂缝，为了游戏效果的美观，我们需要对裂缝进行填补。Terrain提供两种方式对裂缝进行填补，分别是裙边以及补边的方法，在这里我们使用裙边法来对裂缝进行填补
	auto terrain = Terrain::create(data, Terrain::CrackFixedType::SKIRT);
	terrain->setLODDistance(3.2f, 6.4f, 9.6f);
	terrain->setMaxDetailMapAmount(4);
	terrain->setCameraMask(2);
	terrain->setDrawWire(false);
	layer3D->addChild(terrain);
	_terrain = terrain;

	//创建一个动画模型
	_sprite3D = Sprite3D::create("UVRaycast/orc.c3b");
	_sprite3D->setScale(0.1f);
	_sprite3D->setRotation3D(Vec3(0, 180, 0));
	_sprite3D->setPosition3D(Vec3(0, 2, 0));
	_sprite3D->setCameraMask(2);
	layer3D->addChild(_sprite3D);
	auto animation = Animation3D::create("UVRaycast/orc.c3b");
	if (animation)
	{
		auto animate = Animate3D::create(animation);
		auto repeate = RepeatForever::create(animate);
		_sprite3D->runAction(repeate);
	}

	//添加触摸监听事件
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(UVRaycast::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(UVRaycast::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(UVRaycast::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//构建模型OBB包围盒
	AABB aabb = _sprite3D->getAABB();
	_obbt = OBB(aabb);

	return true;
}

void UVRaycast::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
	auto touch = touches[0];
	auto location = touch->getLocationInView();
	if (_camera){
		if (_terrain){
			Vec3 nearP(location.x, location.y, 0.0f), farP(location.x, location.y, 1.0f);
			auto size = Director::getInstance()->getWinSize();
			//映射成视锥的近截面和远截面
			_camera->unproject(size, &nearP, &nearP);
			_camera->unproject(size, &farP, &farP);
			Vec3 dir = farP - nearP;
			dir.normalize();
			Vec3 collisionPoint(-999, -999, -999);
			Ray ray(nearP, dir);

			Mat4 mat = _sprite3D->getNodeToWorldTransform();
			mat.getRightVector(&_obbt._xAxis);
			_obbt._xAxis.normalize();

			mat.getUpVector(&_obbt._yAxis);
			_obbt._yAxis.normalize();

			mat.getForwardVector(&_obbt._zAxis);
			_obbt._zAxis.normalize();

			_obbt._center = _sprite3D->getPosition3D();
			if (ray.intersects(_obbt)){
				if (_sprite3D){
					_sprite3D->setOpacity(128);
				}
			}
			else{
				bool isInTerrain = _terrain->getIntersectionPoint(ray, collisionPoint);
				if (isInTerrain){
					if (_sprite3D){
						_sprite3D->setPosition3D(collisionPoint);
					}
				}
			}
		}
	}
}

void UVRaycast::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}

void UVRaycast::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
	if (_sprite3D)
	{
		_sprite3D->setOpacity(255);
	}
}