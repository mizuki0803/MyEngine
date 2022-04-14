#include "TurretScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Easing.h"
#include <cassert>
#include <sstream>
#include <iomanip>

using namespace DirectX;


void TurretScene::Initialize()
{
	//�I�[�f�B�I�̃C���X�^���X���擾
	Audio* audio = Audio::GetInstance();
	//�T�E���h�f�[�^�ǂݍ���
	audio->LoadWave("BGM.wav");
	audio->LoadWave("BossDamage.wav");
	//audio->LoadWave("Shot.wav");

	//���S�̂̃{�����[���ύX
	audio->ChangeVolume(soundVol);
	//audio->PlayWave("BGM.wav", true);


	//�J����������
	camera = new Camera();
	camera->Initialize();


	//���C�g����
	lightGroup = LightGroup::Create();

	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);
	pointLightPos[0] = 0.5f;
	pointLightPos[1] = 1.0f;
	pointLightPos[2] = 0.0f;
	lightGroup->SetPointLightActive(0, false);
	lightGroup->SetPointLightActive(1, false);
	lightGroup->SetPointLightActive(2, false);
	//lightGroup->SetSpotLightActive(0, true);
	lightGroup->SetCircleShadowActive(0, true);


	//�p�[�e�B�N������
	ParticleManager::SetCamera(camera);
	particleMan = ParticleManager::Create();

	//�X�v���C�g���ʕ����̃C���X�^���X���擾
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	//�X�v���C�g�p�e�N�X�`���ǂݍ���
	spriteCommon->LoadTexture(1, "pad1.png");
	spriteCommon->LoadTexture(2, "mario.jpg");

	//�X�v���C�g����
	sprite = Sprite::Create(2, { 0, 0 });
	sprite2 = Sprite::Create(1);

	//�X�v���C�g���W�ړ�
	sprite->SetPosition({ 100, 100 });
	sprite2->SetPosition({ 800, 300 });
	//�X�v���C�g��]
	sprite2->SetRotation(90);
	//�X�v���C�g�T�C�Y�ύX
	sprite->SetSize({ 100, 100 });
	sprite2->SetSize({ 428, 150 });
	//�X�v���C�g�e�N�X�`���T�C�Y�ύX
	sprite->SetTexSize({ 64, 478 });
	sprite2->SetTexSize({ 428, 150 });
	//�X�v���C�g�F�ύX
	sprite2->SetColor({ 1, 0, 0, 0.8f });


	//obj���烂�f���f�[�^��ǂݍ���
	modelSkydome = Model::LoadFromOBJ("skydome");
	modelGround = Model::LoadFromOBJ("ground");
	modelSphere = Model::LoadFromOBJ("sphere", true);
	modelFighter = Model::LoadFromOBJ("fighter", true);

	// 3D�I�u�W�F�N�g����
	objGround = Object3d::Create(modelGround);
	objSkydome = Object3d::Create(modelSkydome);
	objTurret = Turret::Create(modelFighter, modelSphere);

	//�������W
	objGround->SetPosition({ 0, -1, 0 });

	//�T�C�Y�ύX
	objGround->SetScale({ 20, 20, 20 });
	objSkydome->SetScale({ 20, 20, 20 });

	//3D�I�u�W�F�N�g�ɃJ�������Z�b�g
	Object3d::SetCamera(camera);
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3d::SetLightGroup(lightGroup);


	//�����蔻�� �� �̏����l��ݒ�
	sphere.center = XMVectorSet(0, 2, 0, 1); //���S�_���W
	sphere.radius = 1.0f; //���a

	//�����蔻�� ���� �̏����l��ݒ�
	plane.normal = XMVectorSet(0, 1, 0, 0); //�@���x�N�g��
	plane.distance = 0.0f; //���_(0, 0, 0)����̋���

	//�����蔻�� �O�p�` �̏����l��ݒ�
	triangle.p0 = XMVectorSet(-1.0f, +1.0, -1.0f, 1);	//����O
	triangle.p1 = XMVectorSet(-1.0f, +1.0, +1.0f, 1);	//����
	triangle.p2 = XMVectorSet(+1.0f, +1.0, -1.0f, 1);	//�E��O
	triangle.normal = XMVectorSet(0.0f, 1.0f, 0.0f, 0);	//�����

	//�����蔻�� ���C �̏����l��ݒ�
	ray.start = XMVectorSet(0, 5, 0, 1);	//���_����
	ray.dir = XMVectorSet(0, -1, 0, 0);		//������
}

void TurretScene::Finalize()
{
	//�J�������
	delete camera;

	//���C�g���
	delete lightGroup;

	//�p�[�e�B�N�����
	delete particleMan;

	//�X�v���C�g���
	delete sprite;
	delete sprite2;

	//���f�����
	delete modelSkydome;
	delete modelGround;
	delete modelSphere;
	delete modelFighter;

	//3D�I�u�W�F�N�g���
	delete objGround;
	delete objSkydome;
	delete objTurret;
}

void TurretScene::Update()
{
	//���͂̃C���X�^���X���擾
	Input* input = Input::GetInstance();
	//�f�o�b�O�e�L�X�g�̃C���X�^���X���擾
	DebugText* debugText = DebugText::GetInstance();


	{ // imgui����̃��C�g�p�����[�^�𔽉f
	//	lightGroup->SetAmbientColor(XMFLOAT3(ambientColor0));
	//	lightGroup->SetDirLightDir(0, XMVECTOR({ lightDir0[0], lightDir0[1], lightDir0[2], 0 }));
	//	lightGroup->SetDirLightColor(0, XMFLOAT3(lightColor0));
	//	lightGroup->SetDirLightDir(1, XMVECTOR({ lightDir1[0], lightDir1[1], lightDir1[2], 0 }));
	//	lightGroup->SetDirLightColor(1, XMFLOAT3(lightColor1));
	//	lightGroup->SetDirLightDir(2, XMVECTOR({ lightDir2[0], lightDir2[1], lightDir2[2], 0 }));
	//	lightGroup->SetDirLightColor(2, XMFLOAT3(lightColor2));
	//	lightGroup->SetPointLightPos(0, XMFLOAT3(pointLightPos));
	//	lightGroup->SetPointLightColor(0, XMFLOAT3(pointLightColor));
	//	lightGroup->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));
	//	lightGroup->SetSpotLightDir(0, XMVECTOR({ spotLightDir[0], spotLightDir[1], spotLightDir[2], 0 }));
	//	lightGroup->SetSpotLightPos(0, XMFLOAT3(spotLightPos));
	//	lightGroup->SetSpotLightColor(0, XMFLOAT3(spotLightColor));
	//	lightGroup->SetSpotLightAtten(0, XMFLOAT3(spotLightAtten));
	//	lightGroup->SetSpotLightFactorAngleCos(0, XMFLOAT2(spotLightfactorAngleCos));
		lightGroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
		lightGroup->SetCircleShadowCasterPos(0, objTurret->GetBulletPosition());
		lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
		lightGroup->SetCircleShadowFactorAngleCos(0, XMFLOAT2(circleShadowFactorAngle));
	}

	//���C�g�X�V
	lightGroup->Update();


	//Object3d�X�V
	objGround->Update();
	objSkydome->Update();
	objTurret->Update();

	XMFLOAT3 eye = objTurret->GetBulletPosition();
	eye.z -= 200;
	camera->SetEye(eye);
	camera->SetTarget(objTurret->GetBulletPosition());

	//�J�����X�V
	camera->Update();

	//�f�o�b�N�e�L�X�g
	//X���W,Y���W,�k�ڂ��w�肵�ĕ\��
	debugText->Print("TurretShot", 100, 50);
	debugText->Print("R key Reset", 100, 100);
	debugText->Print("ENTER FallSphereScene", 100, 130);


	//�G���^�[�L�[�ŏd�͗����V�[����
	if(input->TriggerKey(DIK_RETURN))
	{
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}

void TurretScene::Draw()
{
	//Object3d���ʃR�}���h
	Object3d::DrawPrev();
	///-------Object3d�`�悱������-------///


	objGround->Draw();
	objSkydome->Draw();
	objTurret->Draw();


	///-------Object3d�`�悱���܂�-------///


	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->DrawPrev();
	///-------�X�v���C�g�`�悱������-------///


	//sprite->Draw();
	//sprite2->Draw();	


	///-------�X�v���C�g�`�悱���܂�-------///


	//�p�[�e�B�N�����ʃR�}���h
	ParticleManager::DrawPrev();
	///-------�p�[�e�B�N���`�悱������-------///


	//�p�[�e�B�N���`��
	//particleMan->Draw();


	///-------�p�[�e�B�N���`�悱���܂�-------///
}
