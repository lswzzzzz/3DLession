#pragma once
#include "cocos2d.h"

USING_NS_CC;
class Camera;

class UVAnim : public Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(UVAnim);

	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void meshUVTimeUpdate(float dt);
	//��ʼ��UI
	void InitUI();
	//��ť�����Ӧ
	void onButtonClick(cocos2d::Ref* sender);
private:
	//����һ��3D�����
	cocos2d::Camera* _camera;
	//�����������ת�Ƕ�
	float _angle;
	float _meshUVRunTime;
	//����һ����̬ģ��
	cocos2d::Sprite3D* _sprite3D;
	//����һ��GL��̵�״̬
	cocos2d::GLProgramState* _state;
};

