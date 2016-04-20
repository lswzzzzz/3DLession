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
	//Ӧ�õ�ǰƤ��
	void applyCurSkin();
	//��ʼ��UI����
	void  InitUI();
	//��ť����¼���Ӧ
	void onButtonClick(cocos2d::Ref* sender);
private:
	//����һ��3D�����
	cocos2d::Camera* _camera;
	//�����������ת�Ƕ�
	float _angle;
	//��װ����
	enum class SkinType
	{
		UPPER_BODY = 0,	//����
		PANTS,		//����
		SHOES,		//Ь��
		HAIR,		//ͷ��
		FACE,		//��
		HAND,		//��
		GLASSES,	//�۾�
		MAX_TYPE,
	};
	//ȫ��Ƥ��
	std::vector<std::string> _skins[(int)SkinType::MAX_TYPE];
	//��ǰ��װƤ������
	int                      _curSkin[(int)SkinType::MAX_TYPE];
	//��װƤ��ģ��
	cocos2d::Sprite3D*     _spriteMeshReskin;
};

