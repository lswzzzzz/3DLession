#pragma once

#include "cocos2d.h"

class CameraTest : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
	CREATE_FUNC(CameraTest);

	//初始化UI界面
	void  InitUI();
	//按钮点击事件响应
	void onButtonClick(cocos2d::Ref* sender);
	//第一人称视角 
	void  FirstView(float dt);
	//第三人称视角
	void  ThirdView(float dt);
private:
	//管理3D物件的类
	cocos2d::Layer*	 _layer3D;
	//声明一个3D摄像机
	cocos2d::Camera* _camera;
	//声明一个观察点节点
	cocos2d::Node*  _Tank_Base;
	cocos2d::Node*  _Tank_Gun;
};

