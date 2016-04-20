#include "UVShadowMap.h"
#include "cocostudio/CocoStudio.h"
#include "DrawNode3D.h"
#include "ui/CocosGUI.h"
#include "Particle3D/CCParticleSystem3D.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* UVShadowMap::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = UVShadowMap::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool UVShadowMap::init()
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
	_camera->setPosition3D(Vec3(0, 20, 25));
	_camera->setRotation3D(Vec3(-60, 0, 0));
	//设置摄像机观察点位置
	_camera->lookAt(Vec3(0, 0, 0));
	//_camera->setRotation3D(Vec3(-60, 0, 0));
	_camera->setCameraFlag(CameraFlag::USER1);
	layer3D->addChild(_camera);
	layer3D->setCameraMask(2);

	_plane = Sprite3D::create("UVShadowMap/plane.c3t");
	_plane->setRotation3D(Vec3(90, 0, 0));
	_plane->setCameraMask(2);
	layer3D->addChild(_plane);
	auto shader = GLProgram::createWithFilenames("UVShadowMap/simple_shadow.vert", "UVShadowMap/simple_shadow.frag");
	_state = GLProgramState::create(shader);
	_plane->setGLProgramState(_state);

	long offset = 0;
	auto attributeCount = _plane->getMesh()->getMeshVertexAttribCount();
	for (auto i = 0; i < attributeCount; i++){
		auto meshattribute = _plane->getMesh()->getMeshVertexAttribute(i);
		_state->setVertexAttribPointer(s_attributeNames[meshattribute.vertexAttrib],
			meshattribute.size,
			meshattribute.type,
			GL_FALSE,
			_plane->getMesh()->getVertexSizeInBytes(),
			(GLvoid*)offset);
		offset += meshattribute.attribSizeBytes;
	}
	_state->setUniformMat4("u_model_matrix", _plane->getNodeToWorldTransform());


	//创建阴影纹理
	auto shadowTexture = Director::getInstance()->getTextureCache()->addImage("UVShadowMap/shadowCircle.png");
	Texture2D::TexParams tRepeatParams;
	tRepeatParams.magFilter = GL_LINEAR;
	tRepeatParams.minFilter = GL_LINEAR;
	//超过边缘会被自动裁剪掉
	tRepeatParams.wrapS = GL_CLAMP_TO_EDGE;
	tRepeatParams.wrapT = GL_CLAMP_TO_EDGE;
	shadowTexture->setTexParameters(tRepeatParams);
	_state->setUniformTexture("u_shadowTexture", shadowTexture);

	//创建一个动画模型
	_sprite3D = Sprite3D::create("UVShadowMap/orc.c3b");
	_sprite3D->setScale(0.2f);
	_sprite3D->setRotation3D(Vec3(0, 180, 0));
	_sprite3D->setPosition3D(Vec3(0, 0, 0));
	_sprite3D->setCameraMask(2);
	auto animation = Animation3D::create("/UVShadowMaporc.c3b");
	if (animation){
		auto animate = Animate3D::create(animation);
		auto repeate = RepeatForever::create(animate);
		_sprite3D->runAction(repeate);
	}

	//进行位移动画
	_sprite3D->runAction(RepeatForever::create(Sequence::create(
		MoveTo::create(1.0f, Vec3(0, 0, 0)), RotateTo::create(1.0f, Vec3(0, 180, 0)),
		MoveTo::create(1.0f, Vec3(0, 0, 10)), RotateTo::create(1.0f, Vec3(0, 270, 0)),
		MoveTo::create(1.0f, Vec3(10, 0, 10)), RotateTo::create(1.0f, Vec3(0, 360, 0)),
		MoveTo::create(1.0f, Vec3(10, 0, 0)), RotateTo::create(1.0f, Vec3(0, 90, 0)),
		nullptr)));

	layer3D->addChild(_sprite3D);
	layer3D->setCameraMask(2);

	schedule(schedule_selector(UVShadowMap::UpdateShadow));
	return true;
}

void UVShadowMap::UpdateShadow(float dt)
{
	if (_state){
		_plane->getGLProgramState()->setUniformVec3("u_target_pos", _sprite3D->getPosition3D());
	}
}