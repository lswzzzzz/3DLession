#pragma once

#include "cocos2d.h"
class Camera;
class VSTree : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
	CREATE_FUNC(VSTree);
	void meshUVTimeUpdate(float dt);
	//��ʼ��UI
	void InitUI();
	//��ť�����Ӧ
	void onButtonClick(cocos2d::Ref* sender);
private:
	//����һ��3D�����
	cocos2d::Camera* _camera;
	float _meshUVRunTime;
	//����һ����̬ģ��
	cocos2d::Sprite3D* _sprite3D;
	//����һ��GL��̵�״̬
	cocos2d::GLProgramState* _state;
};

