#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
class Camera;
namespace cocos2d {
	class DrawNode3D;
}

class AABBOBB : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
	CREATE_FUNC(AABBOBB);
	
	//AABB包围盒碰撞检测函数
	void updateAABBColl(float dt);
	// OBB包围盒碰撞检测函数
	void updateOBBColl(float dt);
	//动画结束回调
	void aniEndCallBack(cocos2d::Node* seedNode);
	//初始化UI
	void InitUI();
	//按钮点击响应
	void onButtonClick(cocos2d::Ref* sender);

private:
	//定义2个静态模型，用于演示模型碰撞
	cocos2d::Sprite3D*   _spriteStatic3D[2];
	//定义2个用于渲染静态模型的包围盒对象
	cocos2d::DrawNode3D* _spriteStatic3DBox[2];
	//定义碰撞检测类型  0 AABB碰撞模式 1 OBB碰撞模式
	int         _meshCollType;
	//定义按钮
	cocos2d::ui::Button* _collTypeButtonn[2];
	//旋转数值
	float				_meshRotateValue;
};

