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

class EnvirMapper : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
	CREATE_FUNC(EnvirMapper);
	
	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	//初始化UI
	void InitUI();
	//按钮点击响应
	void onButtonClick(cocos2d::Ref* sender);
private:
	//声明一个3D摄像机
	cocos2d::Camera* _camera;
	//声明一个静态模型
	cocos2d::Sprite3D* _sprite3D;
	//声明一个GL编程的状态
	cocos2d::GLProgramState* _state;
	//声明天空盒纹理
	cocos2d::TextureCube*  _textureCube;
};

