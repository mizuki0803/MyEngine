#include "GameScene.h"
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


void GameScene::Initialize()
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
	modelMan = ObjModel::LoadFromOBJ("man");
	modelSkydome = ObjModel::LoadFromOBJ("skydome");
	modelGround = ObjModel::LoadFromOBJ("ground");
	modelSphere = ObjModel::LoadFromOBJ("sphere", true);
	modelFighter = ObjModel::LoadFromOBJ("fighter", true);

	//obj�I�u�W�F�N�g����
	objMan = ObjObject3d::Create(modelMan);
	objGround = ObjObject3d::Create(modelGround);
	objSkydome = ObjObject3d::Create(modelSkydome);
	objSphere = ObjObject3d::Create(modelSphere);

	//���f�����蓖��
	/*objMan->SetModel(modelMan);
	objGround->SetModel(modelGround);
	objSkydome->SetModel(modelSkydome);
	objSphere->SetModel(modelSphere);*/

	//�������W
	objMan->SetPosition({ 0, 1, 0 });
	objGround->SetPosition({ 0, -1, 0 });
	objSphere->SetPosition({ -1, 0, 0 });

	//�p�x�����l
	objMan->SetRotation({ 0, 90, 0 });

	//obj�I�u�W�F�N�g�ɃJ�������Z�b�g
	ObjObject3d::SetCamera(camera);
	//obj�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	ObjObject3d::SetLightGroup(lightGroup);


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

void GameScene::Finalize()
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
	delete modelMan;
	delete modelSkydome;
	delete modelGround;
	delete modelSphere;
	delete modelFighter;

	//obj�I�u�W�F�N�g���
	delete objMan;
	delete objGround;
	delete objSkydome;
	delete objSphere;
}

void GameScene::Update()
{
	//���͂̃C���X�^���X���擾
	Input* input = Input::GetInstance();
	//�f�o�b�O�e�L�X�g�̃C���X�^���X���擾
	DebugText* debugText = DebugText::GetInstance();


	//���f���ύX�ƃX�v���C�g�ω�
	if (input->PushKey(DIK_SPACE))
	{
		objMan->SetModel(modelFighter);

		sprite2->SetTexNumber(2);
		sprite->SetIsFlipX(false);
	}
	else
	{
		objMan->SetModel(modelMan);

		sprite2->SetTexNumber(1);
		sprite->SetIsFlipX(true);
	}	

	//���C�g�X�V
	lightGroup->Update();


	//�F
	const XMFLOAT4 purple = { 1.0f, 0.0f, 1.0f, 0.8f }; //��
	const XMFLOAT4 lightBlue = { 0.0f, 1.0f, 1.0f, 0.8f }; //���F
	//�p�[�e�B�N���X�V
	for (int i = 0; i < 10; i++)
	{
		//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z
		const float md_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
		const float md_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
		XMFLOAT3 acc{};
		const float md_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * md_acc;

		//�ǉ�
		particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f, purple, lightBlue);
	}


	// �J�����ړ�
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A) || input->PushKey(DIK_E) || input->PushKey(DIK_C))
	{
		XMFLOAT3 move = { 0, 0, 0 };
		if (input->PushKey(DIK_W)) { move.y += 0.1f; }
		else if (input->PushKey(DIK_S)) { move.y -= 0.1f; }
		if (input->PushKey(DIK_D)) { move.x += 0.1f; }
		else if (input->PushKey(DIK_A)) { move.x -= 0.1f; }
		if (input->PushKey(DIK_E)) { move.z += 0.1f; }
		else if (input->PushKey(DIK_C)) { move.z -= 0.1f; }
		camera->MoveVector(move);
	}

	//�J�����̃A���O����ύX����
	if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
	{
		XMFLOAT3 angle = camera->GetAngle();
		if (input->PushKey(DIK_LEFT)) { angle.y += 1.0f; }
		if (input->PushKey(DIK_RIGHT)) { angle.y -= 1.0f; }
		if (input->PushKey(DIK_UP)) { angle.x += 1.0f; }
		if (input->PushKey(DIK_DOWN)) { angle.x -= 1.0f; }
		camera->SetAngle(angle);
	}

	//�J�����̋�����ύX����
	if (input->PushKey(DIK_M) || input->PushKey(DIK_N))
	{
		float dis = camera->GetDistance();
		if (input->PushKey(DIK_M)) { dis += 0.1f; }
		if (input->PushKey(DIK_N)) { dis -= 0.1f; }
		camera->SetDistance(dis);
	}

	//�J�����X�V
	camera->Update();

	//Object3d�X�V
	objMan->Update();
	objGround->Update();
	objSkydome->Update();
	objSphere->Update();


	//�X�v���C�g�X�V
	sprite->Update();
	sprite2->Update();

	//�p�[�e�B�N���X�V
	particleMan->Update();

	//�f�o�b�N�e�L�X�g
	//X���W,Y���W,�k�ڂ��w�肵�ĕ\��
	debugText->Print("SCENE2", 50, 50);
	debugText->Print("[1] monochromatic", 50, 70);
	debugText->Print("[2] ADS", 50, 90);
	debugText->Print("[3] Toon", 50, 110);
	//�f�o�b�O�o��
	//sprintf_s(str, "%f\n", soundVol);
	//OutputDebugStringA(str);

	if (input->TriggerKey(DIK_RETURN))
	{
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("TITLE");
		Audio::GetInstance()->StopWave("BGM.wav");
	}
}

void GameScene::Draw()
{
	//Object3d���ʃR�}���h
	ObjObject3d::DrawPrev();
	///-------Object3d�`�悱������-------///


	objMan->Draw();
	objGround->Draw();
	objSkydome->Draw();
	objSphere->Draw();


	///-------Object3d�`�悱���܂�-------///


	//�X�v���C�g���ʃR�}���h
	//SpriteCommon::GetInstance()->DrawPrev();
	///-------�X�v���C�g�`�悱������-------///


	//sprite->Draw();
	//sprite2->Draw();


	///-------�X�v���C�g�`�悱���܂�-------///


	//�p�[�e�B�N�����ʃR�}���h
	ParticleManager::DrawPrev();
	///-------�p�[�e�B�N���`�悱������-------///


	//�p�[�e�B�N���`��
	particleMan->Draw();


	///-------�p�[�e�B�N���`�悱���܂�-------///
}
