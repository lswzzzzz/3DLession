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
	
	//AABB��Χ����ײ��⺯��
	void updateAABBColl(float dt);
	// OBB��Χ����ײ��⺯��
	void updateOBBColl(float dt);
	//���������ص�
	void aniEndCallBack(cocos2d::Node* seedNode);
	//��ʼ��UI
	void InitUI();
	//��ť�����Ӧ
	void onButtonClick(cocos2d::Ref* sender);

private:
	//����2����̬ģ�ͣ�������ʾģ����ײ
	cocos2d::Sprite3D*   _spriteStatic3D[2];
	//����2��������Ⱦ��̬ģ�͵İ�Χ�ж���
	cocos2d::DrawNode3D* _spriteStatic3DBox[2];
	//������ײ�������  0 AABB��ײģʽ 1 OBB��ײģʽ
	int         _meshCollType;
	//���尴ť
	cocos2d::ui::Button* _collTypeButtonn[2];
	//��ת��ֵ
	float				_meshRotateValue;
};

