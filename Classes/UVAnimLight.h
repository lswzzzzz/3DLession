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
	//��ʼ��UI����
	void  InitUI();
	//��ť����¼���Ӧ
	void onButtonClick(cocos2d::Ref* sender);
private:
	//����һ��3D�����
	cocos2d::Camera* _camera;
	//����һ��3D��̬ģ��
	cocos2d::Sprite3D* _sprite3D;
	//�����������ת�Ƕ�
	float _angle;
	//����һ��������
	cocos2d::AmbientLight*  _ambientLight;
	//����һ�����
	cocos2d::PointLight*    _pointLight;
	//����һ�������
	cocos2d::DirectionLight*    _directionalLight;
	//����һ���۹�
	cocos2d::SpotLight*    _spotLight;
};

