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

	////�õ���Ļ�ߴ�
	auto s = Director::getInstance()->getWinSize();
	//����3D��
	auto layer3D = Layer::create();
	addChild(layer3D, 0);

	//����3D�����
	_camera = cocos2d::Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);
	//����������۾�λ��
	_camera->setPosition3D(Vec3(0, 5, 5));
	//����������۲��λ��
	_camera->lookAt(Vec3(0, 0, 0));
	_camera->setCameraFlag(CameraFlag::USER1);
	layer3D->addChild(_camera);
	layer3D->setCameraMask(2);

	//�������� 
	//���ȣ�������Ҫ������������Ҫ������ϸ��ͼ  ��һ����ϸ��ͼ��·�����ڶ�����ϸ��ͼ���ڵ����ϵ�ƽ�̳ߴ� �ڶ���������Ĭ��ֵ���ǿ�ѡ��
	Terrain::DetailMap r("UVRaycast/TerrainTest/dirt.jpg"), g("UVRaycast/TerrainTest/Grass2.jpg"), 
		b("UVRaycast/TerrainTest/road.jpg"), a("UVRaycast/TerrainTest/GreenSkin.jpg");
	//Ȼ������ʹ��ϸ��ͼ�����Terrain::TerrainData,Terrain::TerrainData �Ĺ��캯������������ʽ�������û���ͬ�Ĺ�����ε���������������ֻ������õ�һ�֣����캯���ĵ�һ������Ϊ�߶�ͼ��·�����ڶ�������ΪAlpha��ͼ��·�������������ĸ�����Ϊ֮ǰ��������ĸ�DetailMap�ṹ�塣
	Terrain::TerrainData data("UVRaycast/TerrainTest/heightmap16.jpg", "UVRaycast/TerrainTest/alphamap.png", r, g, b, a);
	//��󣬽����������TerrainData����Terrain::create���ɴ�����������
	//Terrain::create�ĵڶ�������Ϊ����LOD�ѷ�ķ첹������LOD����������ͨ����Զ���ĵ��ο��Mesh���Լ򻯣��Ӷ��ڲ�Ӱ���Ӿ�Ч��������£�������ܵķ�������������ٽ����������ο��LOD��ͬʱ�����Ե���ܻ�����ѷ죬Ϊ����ϷЧ�������ۣ�������Ҫ���ѷ�������Terrain�ṩ���ַ�ʽ���ѷ��������ֱ���ȹ���Լ����ߵķ���������������ʹ��ȹ�߷������ѷ�����
	auto terrain = Terrain::create(data, Terrain::CrackFixedType::SKIRT);
	terrain->setLODDistance(3.2f, 6.4f, 9.6f);
	terrain->setMaxDetailMapAmount(4);
	terrain->setCameraMask(2);
	terrain->setDrawWire(false);
	layer3D->addChild(terrain);
	_terrain = terrain;

	//����һ������ģ��
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

	//��Ӵ��������¼�
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(UVRaycast::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(UVRaycast::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(UVRaycast::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//����ģ��OBB��Χ��
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
			//ӳ�����׶�Ľ������Զ����
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