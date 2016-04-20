#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Particle3D/CCParticleSystem3D.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"

using namespace cocos2d::ui;
class Camera;
namespace cocos2d {
	class DrawNode3D;
}

class UVShadowMap : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
	CREATE_FUNC(UVShadowMap);
	
	// 更新影子
	void UpdateShadow(float dt);
private:
	//声明一个3D摄像机
	cocos2d::Camera* _camera;
	cocos2d::Sprite3D * _plane;
	//声明一个怪物模型
	cocos2d::Sprite3D* _sprite3D;
	cocos2d::GLProgramState * _state;
};

