#pragma once

#include "cocos2d.h"

class BillBoardTest : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
	CREATE_FUNC(BillBoardTest);
	//初始化UI界面
	void  InitUI();
	//按钮点击事件响应
	void onButtonClick(cocos2d::Ref* sender);
	//旋转摄像机 
	void  rotCamera(float dt);
	//平移摄像机
	void  moveCamera(float dt);
	//拉远拉近摄像机
	void  zoomCamera(float dt);
private:
	//管理3D物件的类
	cocos2d::Layer*	 _layer3D;
	//声明一个3D摄像机
	cocos2d::Camera* _camera;
	//默认
	cocos2d::BillBoard*	_Logo;
	//树木
	cocos2d::BillBoard*	_Tree[30];
	//雪花
	cocos2d::BillBoard*	_Snow[50];
	//血条
	cocos2d::BillBoard*	_Blood;
	//声明一个观察点节点
	cocos2d::Node*  _Tank_Base;
	cocos2d::Node*  _Tank_Gun;

};

