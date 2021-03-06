#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
class Camera;
class UVAnimLight : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
	CREATE_FUNC(UVAnimLight);
	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	//初始化UI界面
	void  InitUI();
	//按钮点击事件响应
	void onButtonClick(cocos2d::Ref* sender);
private:
	//声明一个3D摄像机
	cocos2d::Camera* _camera;
	//声明一个3D静态模型
	cocos2d::Sprite3D* _sprite3D;
	//摄像机触摸旋转角度
	float _angle;
	//声明一个环境光
	cocos2d::AmbientLight*  _ambientLight;
	//声明一个点光
	cocos2d::PointLight*    _pointLight;
	//声明一个方向光
	cocos2d::DirectionLight*    _directionalLight;
	//声明一个聚光
	cocos2d::SpotLight*    _spotLight;
};

