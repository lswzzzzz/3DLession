#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
class Camera;

class UVEquiReload : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
	CREATE_FUNC(UVEquiReload);
	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	//应用当前皮肤
	void applyCurSkin();
	//初始化UI界面
	void  InitUI();
	//按钮点击事件响应
	void onButtonClick(cocos2d::Ref* sender);
private:
	//声明一个3D摄像机
	cocos2d::Camera* _camera;
	//摄像机触摸旋转角度
	float _angle;
	//换装类型
	enum class SkinType
	{
		UPPER_BODY = 0,	//上身
		PANTS,		//裤子
		SHOES,		//鞋子
		HAIR,		//头发
		FACE,		//脸
		HAND,		//手
		GLASSES,	//眼镜
		MAX_TYPE,
	};
	//全部皮肤
	std::vector<std::string> _skins[(int)SkinType::MAX_TYPE];
	//当前换装皮肤索引
	int                      _curSkin[(int)SkinType::MAX_TYPE];
	//换装皮肤模型
	cocos2d::Sprite3D*     _spriteMeshReskin;
};

