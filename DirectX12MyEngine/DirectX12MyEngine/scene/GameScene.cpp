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

	//�������W
	objMan->SetPosition({ 0, 1, 0 });
	objGround->SetPosition({ 0, -1, 0 });
	objSphere->SetPosition({ -1, 0, 0 });

	player.reset(Player::Create(modelSphere));

	//�G�̑��x��ݒ�
	const Vector3 position(5, 0, 50);
	const float enemySpeed = 0.1f;
	Vector3 velocity(0, 0, enemySpeed);
	enemy.reset(Enemy::Create(modelSphere, position, velocity));

	//�G�Ɏ��@�̃A�h���X��n��
	enemy->SetPlayer(player.get());

	//�p�x�����l
	objMan->SetRotation({ 0, 90, 0 });

	//obj�I�u�W�F�N�g�ɃJ�������Z�b�g
	ObjObject3d::SetCamera(camera);
	//obj�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	ObjObject3d::SetLightGroup(lightGroup);
}

void GameScene::Finalize()
{
	//�J�������
	delete camera;

	//���C�g���
	delete lightGroup;

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

	////�L�[���͂Ńv���C���[�̈ʒu��ύX
	//if (input->PushKey(DIK_1) || input->PushKey(DIK_2) || input->PushKey(DIK_3) || input->PushKey(DIK_4) || input->PushKey(DIK_5) || input->PushKey(DIK_6))
	//{
	//	XMFLOAT3 move = { 0, 0, 0 };
	//	if (input->PushKey(DIK_1)) { move.x += 0.1f; }
	//	if (input->PushKey(DIK_2)) { move.x -= 0.1f; }
	//	if (input->PushKey(DIK_3)) { move.y += 0.1f; }
	//	if (input->PushKey(DIK_4)) { move.y -= 0.1f; }
	//	if (input->PushKey(DIK_5)) { move.z += 0.1f; }
	//	if (input->PushKey(DIK_6)) { move.z -= 0.1f; }
	//	XMFLOAT3 playerPos = objMan->GetPosition();
	//	playerPos.x += move.x;
	//	playerPos.y += move.y;
	//	playerPos.z += move.z;
	//	objMan->SetPosition(playerPos);
	//}
	//

	////���C�g�X�V
	//lightGroup->Update();


	//// �J�����ړ�
	//if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A) || input->PushKey(DIK_E) || input->PushKey(DIK_C))
	//{
	//	XMFLOAT3 move = { 0, 0, 0 };
	//	if (input->PushKey(DIK_W)) { move.y += 0.1f; }
	//	else if (input->PushKey(DIK_S)) { move.y -= 0.1f; }
	//	if (input->PushKey(DIK_D)) { move.x += 0.1f; }
	//	else if (input->PushKey(DIK_A)) { move.x -= 0.1f; }
	//	if (input->PushKey(DIK_E)) { move.z += 0.1f; }
	//	else if (input->PushKey(DIK_C)) { move.z -= 0.1f; }
	//	camera->MoveVector(move);
	//}

	////�J�����̃A���O����ύX����
	//if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
	//{
	//	XMFLOAT3 angle = camera->GetAngle();
	//	if (input->PushKey(DIK_LEFT)) { angle.y += 1.0f; }
	//	if (input->PushKey(DIK_RIGHT)) { angle.y -= 1.0f; }
	//	if (input->PushKey(DIK_UP)) { angle.x += 1.0f; }
	//	if (input->PushKey(DIK_DOWN)) { angle.x -= 1.0f; }
	//	camera->SetAngle(angle);
	//}

	////�J�����̋�����ύX����
	//if (input->PushKey(DIK_M) || input->PushKey(DIK_N))
	//{
	//	float dis = camera->GetDistance();
	//	if (input->PushKey(DIK_M)) { dis += 0.1f; }
	//	if (input->PushKey(DIK_N)) { dis -= 0.1f; }
	//	camera->SetDistance(dis);
	//}

	//�J�����X�V
	camera->Update();

	//Object3d�X�V
	objMan->Update();
	objGround->Update();
	objSkydome->Update();
	objSphere->Update();
	player->Update();

	if (enemy)
	{
		enemy->Update();
	}

	//�Փ˔���Ǘ�
	CollisionCheck();

	//�f�o�b�N�e�L�X�g
	//X���W,Y���W,�k�ڂ��w�肵�ĕ\��
	debugText->Print("GAME SCENE", 1000, 50);

	//if (input->TriggerKey(DIK_RETURN))
	//{
	//	//�V�[���؂�ւ�
	//	SceneManager::GetInstance()->ChangeScene("TITLE");
	//}
}

void GameScene::Draw()
{
	//Object3d���ʃR�}���h
	ObjObject3d::DrawPrev();
	///-------Object3d�`�悱������-------///


	//objMan->Draw();
	//objGround->Draw();
	//objSkydome->Draw();
	//objSphere->Draw();
	player->Draw();

	if (enemy)
	{
		enemy->Draw();
	}


	///-------Object3d�`�悱���܂�-------///


	//�X�v���C�g���ʃR�}���h
	//SpriteCommon::GetInstance()->DrawPrev();
	///-------�X�v���C�g�`�悱������-------///




	///-------�X�v���C�g�`�悱���܂�-------///


	//�p�[�e�B�N�����ʃR�}���h
	//ParticleManager::DrawPrev();
	///-------�p�[�e�B�N���`�悱������-------///


	//�p�[�e�B�N���`��
	//particleMan->Draw();


	///-------�p�[�e�B�N���`�悱���܂�-------///
}

void GameScene::CollisionCheck()
{
	//����Ώۂ̍��W
	Vector3 posA, posB;
	float radiusA, radiusB;

	//���@�e���X�g�擾
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
	//�G�e���X�g�擾
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();

#pragma region ���@�ƓG�e�̏Փ˔���
	//���@���W
	posA = player->GetWorldPos();
	//���@���a
	radiusA = player->GetScale().x;

	//���@�ƑS�Ă̓G�e�̏Փ˔���
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		//�G�e���W
		posB = bullet->GetWorldPos();
		//�G�e���a
		radiusB = bullet->GetScale().x;

		//���Ƌ��̏Փ˔�����s��
		bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);

		//�Փ˂��Ă�����
		if (isCollision) {
			//���@�̃R�[���o�b�N�֐����Ăяo��
			player->OnCollision();
			//�G�e�̃R�[���o�b�N�֐����Ăяo��
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region �G�Ǝ��@�e�̏Փ˔���
	//�G���W
	posA = enemy->GetWorldPos();
	//�G���a
	radiusA = enemy->GetScale().x;

	//�G�ƑS�Ă̎��@�e�̏Փ˔���
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//�G�e���W
		posB = bullet->GetWorldPos();
		//�G�e���a
		radiusB = bullet->GetScale().x;

		//���Ƌ��̏Փ˔�����s��
		bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);

		//�Փ˂��Ă�����
		if (isCollision) {
			//�G�̃R�[���o�b�N�֐����Ăяo��
			enemy->OnCollision();
			//���@�e�̃R�[���o�b�N�֐����Ăяo��
			bullet->OnCollision();
		}
	}
#pragma endregion
}
