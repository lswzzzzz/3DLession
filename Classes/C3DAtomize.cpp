#include "C3DAtomize.h"
#include "cocostudio/CocoStudio.h"
#include "DrawNode3D.h"
#include "ui/CocosGUI.h"
#include "Particle3D/CCParticleSystem3D.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* C3DAtomize::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = C3DAtomize::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool C3DAtomize::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	InitUI();

	////�õ���Ļ�ߴ�
	auto s = Director::getInstance()->getWinSize();
	//����3D��
	auto layer3D = Layer::create();
	addChild(layer3D, 0);

	//����3D�����
	_camera = cocos2d::Camera::createPerspective(60, (GLfloat)s.width / s.height, 1, 1000);
	//����������۾�λ��
	_camera->setPosition3D(Vec3(0, 5, 5));
	//����������۲��λ��
	_camera->lookAt(Vec3(0, 0, 0));
	_camera->setCameraFlag(CameraFlag::USER1);
	layer3D->addChild(_camera);
	layer3D->setCameraMask(2);

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(C3DAtomize::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(C3DAtomize::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(C3DAtomize::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//�������� 
	//���ȣ�������Ҫ������������Ҫ������ϸ��ͼ  ��һ����ϸ��ͼ��·�����ڶ�����ϸ��ͼ���ڵ����ϵ�ƽ�̳ߴ� �ڶ���������Ĭ��ֵ���ǿ�ѡ��
	Terrain::DetailMap r("C3DAtomize/TerrainTest/dirt.jpg"), g("C3DAtomize/TerrainTest/Grass2.jpg"), 
		b("C3DAtomize/TerrainTest/road.jpg"), a("C3DAtomize/TerrainTest/GreenSkin.jpg");
	//Ȼ������ʹ��ϸ��ͼ�����Terrain::TerrainData,Terrain::TerrainData �Ĺ��캯������������ʽ�������û���ͬ�Ĺ�����ε���������������ֻ������õ�һ�֣����캯���ĵ�һ������Ϊ�߶�ͼ��·�����ڶ�������ΪAlpha��ͼ��·�������������ĸ�����Ϊ֮ǰ��������ĸ�DetailMap�ṹ�塣
	Terrain::TerrainData data("C3DAtomize/TerrainTest/heightmap16.jpg", "C3DAtomize/TerrainTest/alphamap.png", r, g, b, a);
	//��󣬽����������TerrainData����Terrain::create���ɴ�����������
	//Terrain::create�ĵڶ�������Ϊ����LOD�ѷ�ķ첹������LOD����������ͨ����Զ���ĵ��ο��Mesh���Լ򻯣��Ӷ��ڲ�Ӱ���Ӿ�Ч��������£�������ܵķ�������������ٽ����������ο��LOD��ͬʱ�����Ե���ܻ�����ѷ죬Ϊ����ϷЧ�������ۣ�������Ҫ���ѷ�������Terrain�ṩ���ַ�ʽ���ѷ��������ֱ���ȹ���Լ����ߵķ���������������ʹ��ȹ�߷������ѷ�����
	auto terrain = Terrain::create(data, Terrain::CrackFixedType::SKIRT);
	terrain->setLODDistance(3.2f, 6.4f, 9.6f);
	terrain->setMaxDetailMapAmount(4);
	terrain->setPosition3D(Vec3(0.17, 0.015, -0.74));
	terrain->setCameraMask(2);
	terrain->setDrawWire(false);

	layer3D->addChild(terrain);
	_terrain = terrain;


	auto state = GLProgramState::create(GLProgram::createWithFilenames("C3DAtomize/ccShader_3D_Terrain.vert", "C3DAtomize/ccShader_3D_Terrain.frag"));
	state->setUniformVec4("u_fogColor", Vec4(0.0f, 1.0f, 1.0f, 1.0f));
	state->setUniformFloat("u_fogStart", 3.0f);
	state->setUniformFloat("u_fogEnd", 20.0f);
	state->setUniformInt("u_fogEquation", 0);
	state->applyUniforms();
	_terrain->setGLProgramState(state);

	return true;
}

void C3DAtomize::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}

void C3DAtomize::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
	if (touches.size())
	{
		Vec2 prelocation = touches[0]->getPreviousLocationInView();
		Vec2 location = touches[0]->getLocationInView();
		Vec2 newPos = prelocation - location;

		Vec3 cameraDir;
		Vec3 cameraRightDir;
		_camera->getNodeToWorldTransform().getForwardVector(&cameraDir);
		cameraDir.normalize();
		cameraDir.y = 0;
		_camera->getNodeToWorldTransform().getRightVector(&cameraRightDir);
		cameraRightDir.normalize();
		cameraRightDir.y = 0;
		Vec3 cameraPos = _camera->getPosition3D();
		cameraPos -= cameraDir*newPos.y*0.1f;
		cameraPos += cameraRightDir*newPos.x*0.1f;
		_camera->setPosition3D(cameraPos);
	}
}

void C3DAtomize::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{

}

void C3DAtomize::InitUI()
{
	//����UI����
	auto rootNode = CSLoader::createNode("C3DAtomize/MainScene.csb");
	addChild(rootNode);
	//���н�������
	auto visibleSize = Director::getInstance()->getVisibleSize();
	rootNode->setContentSize(visibleSize);
	rootNode->setScale(0.5);
	rootNode->setPosition(visibleSize.width / 3, visibleSize.height / 2);
	ui::Helper::doLayout(rootNode);
	cocos2d::ui::Layout* rootPane = static_cast<cocos2d::ui::Layout*>(rootNode->getChildByName("Panel_1"));
	if (rootPane)
	{
		//��ȡ�ؼ�
		auto Button_1 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_1"));
		//�����ð�ť��ǣ������ֵ�������Ǹ���ť
		Button_1->setTag(1);
		//��Ӱ�ť����¼�
		Button_1->addClickEventListener(CC_CALLBACK_1(C3DAtomize::onButtonClick, this));
		//��ȡ�ؼ�
		auto Button_2 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_2"));
		//�����ð�ť��ǣ������ֵ�������Ǹ���ť
		Button_2->setTag(2);
		//��Ӱ�ť����¼�
		Button_2->addClickEventListener(CC_CALLBACK_1(C3DAtomize::onButtonClick, this));

		//��ȡ�ؼ�
		auto Button_3 = static_cast<cocos2d::ui::Button*>(cocos2d::ui::Helper::seekWidgetByName(rootPane, "Button_3"));
		//�����ð�ť��ǣ������ֵ�������Ǹ���ť
		Button_3->setTag(3);
		//��Ӱ�ť����¼�
		Button_3->addClickEventListener(CC_CALLBACK_1(C3DAtomize::onButtonClick, this));
	}
}

void C3DAtomize::onButtonClick(cocos2d::Ref* sender)
{
	cocos2d::ui::Button* btn = (cocos2d::ui::Button*)sender;
	if (btn)
	{
		if (btn->getTag() == 1)
		{
			auto _state = GLProgramState::create(GLProgram::createWithFilenames("C3DAtomize/ccShader_3D_Terrain.vert", "C3DAtomize/ccShader_3D_Terrain.frag"));
			_state->setUniformVec4("u_fogColor", Vec4(0.0, 1.0, 1.0, 1.0));
			_state->setUniformFloat("u_fogStart", 3.0);
			_state->setUniformFloat("u_fogEnd", 20.0);
			_state->setUniformInt("u_fogEquation", 0);
			_state->applyUniforms();
			_terrain->setGLProgramState(_state);
		}

		else if (btn->getTag() == 2)
		{
			auto _state = GLProgramState::create(GLProgram::createWithFilenames("C3DAtomize/ccShader_3D_Terrain.vert", "C3DAtomize/ccShader_3D_Terrain.frag"));
			_state->setUniformVec4("u_fogColor", Vec4(0.0, 1.0, 1.0, 1.0));
			_state->setUniformFloat("u_fogDensity", 0.1f);
			_state->setUniformInt("u_fogEquation", 1);
			_state->applyUniforms();
			_terrain->setGLProgramState(_state);
		}
		else
		{
			auto _state = GLProgramState::create(GLProgram::createWithFilenames("C3DAtomize/ccShader_3D_Terrain.vert", "C3DAtomize/ccShader_3D_Terrain.frag"));
			_state->setUniformVec4("u_fogColor", Vec4(0.0, 1.0, 1.0, 1.0));
			_state->setUniformFloat("u_fogDensity", 0.1f);
			_state->setUniformInt("u_fogEquation", 2);
			_state->applyUniforms();
			_terrain->setGLProgramState(_state);
		}
	}
}