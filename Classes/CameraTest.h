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

	//��ʼ��UI����
	void  InitUI();
	//��ť����¼���Ӧ
	void onButtonClick(cocos2d::Ref* sender);
	//��һ�˳��ӽ� 
	void  FirstView(float dt);
	//�����˳��ӽ�
	void  ThirdView(float dt);
private:
	//����3D�������
	cocos2d::Layer*	 _layer3D;
	//����һ��3D�����
	cocos2d::Camera* _camera;
	//����һ���۲��ڵ�
	cocos2d::Node*  _Tank_Base;
	cocos2d::Node*  _Tank_Gun;
};

