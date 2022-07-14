#include "TitleScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Easing.h"
#include "FbxLoader.h"
#include <cassert>
#include <sstream>
#include <iomanip>

using namespace DirectX;


void TitleScene::Initialize()
{
	//�I�[�f�B�I�̃C���X�^���X���擾
	Audio* audio = Audio::GetInstance();
	//�T�E���h�f�[�^�ǂݍ���
	audio->LoadWave("BGM.wav");
	audio->LoadWave("BossDamage.wav");
	audio->LoadWave("Shot.wav");

	//���S�̂̃{�����[���ύX
	audio->ChangeVolume(soundVol);
	//audio->PlayWave("BGM.wav", true);

	//�J����������
	camera.reset(new Camera());
	camera->Initialize();
	camera->SetTarget({ 0, 2.5f, 0 });
	camera->SetDistance(8.0f);

	//���C�g����
	lightGroup.reset(LightGroup::Create());

	lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);
	pointLightPos[0] = 0.5f;
	pointLightPos[1] = 1.0f;
	pointLightPos[2] = 0.0f;
	lightGroup->SetPointLightActive(0, true);
	lightGroup->SetPointLightActive(1, true);
	lightGroup->SetPointLightActive(2, true);
	//lightGroup->SetSpotLightActive(0, true);
	lightGroup->SetCircleShadowActive(0, true);


	//�p�[�e�B�N������
	ParticleManager::SetCamera(camera.get());
	particleMan.reset(ParticleManager::Create());

	//�X�v���C�g���ʕ����̃C���X�^���X���擾
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	//�X�v���C�g�p�e�N�X�`���ǂݍ���
	spriteCommon->LoadTexture(1, "background.png");
	//spriteCommon->LoadTexture(2, "mario.jpg");

	//�X�v���C�g����
	sprite.reset(Sprite::Create(1, {0, 0}));

	//�X�v���C�g���W�ړ�
	//sprite->SetPosition({ 0, 0 });
	//�X�v���C�g�T�C�Y�ύX
	//sprite->SetSize({ 1280, 720 });
	//�X�v���C�g�e�N�X�`���T�C�Y�ύX
	//sprite->SetTexSize({ 1280, 720 });
	//�X�v���C�g�F�ύX
	//sprite->SetColor({ 1, 1, 1, 0.5f });


	//obj���烂�f���f�[�^��ǂݍ���
	modelMan.reset(ObjModel::LoadFromOBJ("man"));
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydome"));
	modelGround.reset(ObjModel::LoadFromOBJ("ground"));
	modelSphere.reset(ObjModel::LoadFromOBJ("sphere", true));
	modelFighter.reset(ObjModel::LoadFromOBJ("fighter", true));

	//obj�I�u�W�F�N�g����
	objMan.reset(ObjObject3d::Create(modelMan.get()));
	objGround.reset(ObjObject3d::Create(modelGround.get()));
	objSkydome.reset(ObjObject3d::Create(modelSkydome.get()));
	objSphere.reset(ObjObject3d::Create(modelSphere.get()));

	//�������W
	objMan->SetPosition({ 0, 1, 0 });
	objGround->SetPosition({ 0, -1, 0 });
	objSphere->SetPosition({ -1, 0, 0 });

	//�p�x�����l
	objMan->SetRotation({ 0, 90, 0 });

	//obj�I�u�W�F�N�g�ɃJ�������Z�b�g
	ObjObject3d::SetCamera(camera.get());
	//obj�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	ObjObject3d::SetLightGroup(lightGroup.get());


	//FBX�I�u�W�F�N�g�̃J�������Z�b�g
	FbxObject3d::SetCamera(camera.get());

	//���f�������w�肵�ăt�@�C���ǂݍ���
	//FbxLoader::GetInstance()->LoadModelFromFile("cube");
	//fbxModel1 = FbxLoader::GetInstance()->LoadModelFromFile("cube");
	fbxModel1.reset(FbxLoader::GetInstance()->LoadModelFromFile("boneTest"));

	//�I�u�W�F�N�g������FBX���f���̃Z�b�g
	fbxObject1.reset(new FbxObject3d);
	fbxObject1->Initialize();
	fbxObject1->SetFbxModel(fbxModel1.get());
	fbxObject1->PlayAnimation();
	fbxObject1->SetRotation({ 0, 90, 0 });


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

void TitleScene::Update()
{
	//���͂̃C���X�^���X���擾
	Input* input = Input::GetInstance();
	//�f�o�b�O�e�L�X�g�̃C���X�^���X���擾
	DebugText* debugText = DebugText::GetInstance();

	if (input->TriggerKey(DIK_I))
	{
		Audio::GetInstance()->PlayWave("Shot.wav", false);
	}
	if (input->TriggerKey(DIK_0))
	{
		Audio::GetInstance()->PlayWave("BGM.wav", true);
	}
	if (input->ReleaseKey(DIK_0))
	{
		Audio::GetInstance()->StopWave("BGM.wav");
	}
	if (input->TriggerKey(DIK_9))
	{
		Audio::GetInstance()->StopWave("BGM.wav");
	}

	if (input->PushGamePadTrigger(500))
	{
		Vector3 pos = objMan->GetPosition();
		pos.x += 0.1f;
		objMan->SetPosition(pos);
	}
	/*if (input->PushGamePadTrigger(-500))
	{
		Vector3 pos = objMan->GetPosition();
		pos.x -= 0.1f;
		objMan->SetPosition(pos);
	}*/

	if (input->GetMouseWheelVelocity() != 0.0f)
	{
		Vector3 pos = objMan->GetPosition();
		pos.x -= 0.1f * input->GetMouseWheelVelocity();
		objMan->SetPosition(pos);
	}

	if (input->ReleaseMouseButton(Input::MOUSE_WHEEL))
	{
		Vector3 pos = objMan->GetPosition();
		pos.x -= 0.1f;
		objMan->SetPosition(pos);
	}

	if (input->PushGamePadButton(Input::PAD_LEFT))
	{
		Vector3 pos = objMan->GetPosition();
		pos.x -= 0.1f;
		objMan->SetPosition(pos);
	}
	if (input->PushGamePadButton(Input::PAD_RIGHT))
	{
		Vector3 pos = objMan->GetPosition();
		pos.x += 0.1f;
		objMan->SetPosition(pos);
	}
	if (input->PushGamePadButton(Input::PAD_UP))
	{
		Vector3 pos = objMan->GetPosition();
		pos.y += 0.1f;
		objMan->SetPosition(pos);
	}
	if (input->PushGamePadButton(Input::PAD_DOWN))
	{
		Vector3 pos = objMan->GetPosition();
		pos.y -= 0.1f;
		objMan->SetPosition(pos);
	}


	//���f���ύX�ƃX�v���C�g�ω�
	if (input->PushKey(DIK_SPACE))
	{
		float rot = sprite->GetRotation();
		rot++;
		sprite->SetRotation(rot);
	}

	//�L�[���͂Ńv���C���[�̈ʒu��ύX
	if (input->PushKey(DIK_1) || input->PushKey(DIK_2) || input->PushKey(DIK_3) || input->PushKey(DIK_4) || input->PushKey(DIK_5) || input->PushKey(DIK_6))
	{
		Vector3 move = { 0, 0, 0 };
		if (input->PushKey(DIK_1)) { move.x += 0.1f; }
		if (input->PushKey(DIK_2)) { move.x -= 0.1f; }
		if (input->PushKey(DIK_3)) { move.y += 0.1f; }
		if (input->PushKey(DIK_4)) { move.y -= 0.1f; }
		if (input->PushKey(DIK_5)) { move.z += 0.1f; }
		if (input->PushKey(DIK_6)) { move.z -= 0.1f; }
		Vector3 playerPos = objMan->GetPosition();
		playerPos.x += move.x;
		playerPos.y += move.y;
		playerPos.z += move.z;
		objMan->SetPosition(playerPos);

		//�J�����������l��n���B�Ǐ]����
		camera->MoveVector(move);
	}

	////�X�v���C�g�̍��W��ǂݍ��݈ʒu���L�[���͂ł��炷
	//if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT))
	//{
	//	XMFLOAT2 pos = sprite->GetPosition();
	//	XMFLOAT2 leftTop = sprite->GetTexLeftTop();

	//	if (input->PushKey(DIK_RIGHT))
	//	{
	//		pos.x += 1.0f;
	//		leftTop.x += 1.0f;
	//	}

	//	if (input->PushKey(DIK_LEFT))
	//	{
	//		pos.x -= 1.0f;
	//		leftTop.x -= 1.0f;
	//	}

	//	sprite->SetPosition(pos);
	//	sprite->SetTexLeftTop(leftTop);
	//}


	//���C�ړ�
	{
		XMVECTOR moveZ = XMVectorSet(0, 0, 0.01f, 0);
		if (input->PushKey(DIK_W))
		{
			ray.start += moveZ;
		}
		else if (input->PushKey(DIK_S))
		{
			ray.start -= moveZ;
		}

		XMVECTOR moveX = XMVectorSet(0.01f, 0, 0, 0);
		if (input->PushKey(DIK_D))
		{
			ray.start += moveX;
		}
		else if (input->PushKey(DIK_A))
		{
			ray.start -= moveX;
		}
	}
	//stringstream�ŕϐ��̒l�𖄂ߍ���Ő��`����
	std::ostringstream raystr;
	raystr << "ray.start("
		<< std::fixed << std::setprecision(2)//�����_�ȉ�2���܂�
		<< ray.start.m128_f32[0] << ","	//x
		<< ray.start.m128_f32[1] << ","	//y
		<< ray.start.m128_f32[2] << ")",//z

		debugText->Print(raystr.str(), 50, 180, 1.0f);

	//���C�Ƌ��̓����蔻��
	XMVECTOR inter;
	float distance;
	bool hit = Collision::CheckRay2Sphere(ray, sphere, &distance, &inter);
	if (hit) {
		debugText->Print("HIT", 50, 260, 1.0f);

		//stringstream�ŕϐ��̒l�𖄂ߍ���Ő��`����
		raystr.str("");
		raystr.clear();
		raystr << "inter:("
			<< std::fixed << std::setprecision(2)//�����_�ȉ�2���܂�
			<< inter.m128_f32[0] << ","	//x
			<< inter.m128_f32[1] << ","	//y
			<< inter.m128_f32[2] << ")",//z

			debugText->Print(raystr.str(), 50, 280, 1.0f);

		raystr.str("");
		raystr.clear();
		raystr << "distance:(" << std::fixed << std::setprecision(2) << distance << ")";

		debugText->Print(raystr.str(), 50, 300, 1.0f);
	}

	//�I�u�W�F�N�g����]������
	Vector3 rot = objSphere->GetRotation();
	rot.y += 1.0f;
	objSphere->SetRotation(rot);

	//{
	//	//�������������l
	//	static XMVECTOR lightDir = { 0, 1, 5, 0 };

	//	if (input->PushKey(DIK_W)) { lightDir.m128_f32[1] += 1.0f; }
	//	else if (input->PushKey(DIK_S)) { lightDir.m128_f32[1] -= 1.0f; };
	//	if (input->PushKey(DIK_D)) { lightDir.m128_f32[0] += 1.0f; }
	//	else if (input->PushKey(DIK_A)) { lightDir.m128_f32[0] -= 1.0f; };

	//	//light->SetLightDir(lightDir);

	//	std::ostringstream debugstr;
	//	debugstr << "lightDirFactor("
	//		<< std::fixed << std::setprecision(2)
	//		<< lightDir.m128_f32[0] << ","
	//		<< lightDir.m128_f32[1] << ","
	//		<< lightDir.m128_f32[2] << ")",
	//		debugText->Print(debugstr.str(), 50, 50, 1.0f);

	//	debugstr.str("");
	//	debugstr.clear();

	//	const XMFLOAT3 &cameraPos = camera->GetEye();
	//	debugstr << "cameraPos("
	//		<< std::fixed << std::setprecision(2)
	//		<< cameraPos.x << ","
	//		<< cameraPos.y << ","
	//		<< cameraPos.z << ")",
	//		debugText->Print(debugstr.str(), 50, 70, 1.0f);
	//}


	{ // imgui����̃��C�g�p�����[�^�𔽉f
	//	lightGroup->SetAmbientColor(XMFLOAT3(ambientColor0));
	//	lightGroup->SetDirLightDir(0, XMVECTOR({ lightDir0[0], lightDir0[1], lightDir0[2], 0 }));
	//	lightGroup->SetDirLightColor(0, XMFLOAT3(lightColor0));
	//	lightGroup->SetDirLightDir(1, XMVECTOR({ lightDir1[0], lightDir1[1], lightDir1[2], 0 }));
	//	lightGroup->SetDirLightColor(1, XMFLOAT3(lightColor1));
	//	lightGroup->SetDirLightDir(2, XMVECTOR({ lightDir2[0], lightDir2[1], lightDir2[2], 0 }));
	//	lightGroup->SetDirLightColor(2, XMFLOAT3(lightColor2));
		lightGroup->SetPointLightPos(0, Vector3(pointLightPos));
		lightGroup->SetPointLightColor(0, XMFLOAT3(pointLightColor));
		lightGroup->SetPointLightAtten(0, XMFLOAT3(pointLightAtten));
		//	lightGroup->SetSpotLightDir(0, XMVECTOR({ spotLightDir[0], spotLightDir[1], spotLightDir[2], 0 }));
		//	lightGroup->SetSpotLightPos(0, XMFLOAT3(spotLightPos));
		//	lightGroup->SetSpotLightColor(0, XMFLOAT3(spotLightColor));
		//	lightGroup->SetSpotLightAtten(0, XMFLOAT3(spotLightAtten));
		//	lightGroup->SetSpotLightFactorAngleCos(0, XMFLOAT2(spotLightfactorAngleCos));
		lightGroup->SetCircleShadowDir(0, XMVECTOR({ circleShadowDir[0], circleShadowDir[1], circleShadowDir[2], 0 }));
		lightGroup->SetCircleShadowCasterPos(0, objMan->GetPosition());
		lightGroup->SetCircleShadowAtten(0, XMFLOAT3(circleShadowAtten));
		lightGroup->SetCircleShadowFactorAngleCos(0, XMFLOAT2(circleShadowFactorAngle));
	}

	//���C�g�X�V
	lightGroup->Update();


	// �J�����ړ�
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A) || input->PushKey(DIK_E) || input->PushKey(DIK_C))
	{
		Vector3 move = { 0, 0, 0 };
		if (input->PushKey(DIK_W)) { move.y += 0.1f; }
		else if (input->PushKey(DIK_S)) { move.y -= 0.1f; }
		if (input->PushKey(DIK_D)) { move.x += 0.1f; }
		else if (input->PushKey(DIK_A)) { move.x -= 0.1f; }
		if (input->PushKey(DIK_E)) { move.z += 0.1f; }
		else if (input->PushKey(DIK_C)) { move.z -= 0.1f; }
		camera->MoveVector(move);
	}
	/*if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
	{
		XMFLOAT3 move = { 0, 0, 0 };
		if (input->PushKey(DIK_UP)) { move.y += 0.1f; }
		else if (input->PushKey(DIK_DOWN)) { move.y -= 0.1f; }
		if (input->PushKey(DIK_RIGHT)) { move.x += 0.1f; }
		else if (input->PushKey(DIK_LEFT)) { move.x -= 0.1f; }
		camera->MoveVector(move);
	}*/


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


	//�J�����̃A���O����ύX����
	if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
	{
		Vector3 angle = camera->GetAngle();
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

	fbxObject1->Update();


	//�X�v���C�g�X�V
	sprite->Update();

	//�p�[�e�B�N���X�V
	particleMan->Update();

	//�f�o�b�N�e�L�X�g
	//X���W,Y���W,�k�ڂ��w�肵�ĕ\��
	debugText->Print("TITLE SCENE", 1000, 50);

	//�f�o�b�O�o��
	sprintf_s(str, "%f, %f\n", input->GetPadLStickAngle(), input->GetPadRStickAngle());
	OutputDebugStringA(str);

	if (input->TriggerKey(DIK_RETURN))
	{
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("GAME");
		Audio::GetInstance()->StopWave("BGM.wav");
	}
}

void TitleScene::Draw()
{
	//�w�i�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->DrawPrev();
	///-------�w�i�X�v���C�g�`�悱������-------///


	sprite->Draw();
	
	dxbase->ClearDepthBuffer();
	
	///-------�w�i�X�v���C�g�`�悱���܂�-------///


	//Object3d���ʃR�}���h
	ObjObject3d::DrawPrev();
	///-------Object3d�`�悱������-------///


	objMan->Draw();
	objGround->Draw();
	objSkydome->Draw();
	objSphere->Draw();

	fbxObject1->Draw();


	///-------Object3d�`�悱���܂�-------///


	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->DrawPrev();
	///-------�X�v���C�g�`�悱������-------///


	//sprite->Draw();


	///-------�X�v���C�g�`�悱���܂�-------///


	//�p�[�e�B�N�����ʃR�}���h
	ParticleManager::DrawPrev();
	///-------�p�[�e�B�N���`�悱������-------///


	//�p�[�e�B�N���`��
	//particleMan->Draw();


	///-------�p�[�e�B�N���`�悱���܂�-------///
}

