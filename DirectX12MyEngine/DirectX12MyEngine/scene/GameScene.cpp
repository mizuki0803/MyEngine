#include "GameScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Easing.h"
#include "DemoEnemy.h"
#include "Cannon.h"
#include <cassert>
#include <fstream>
#include <iomanip>

using namespace DirectX;


void GameScene::Initialize()
{
	//�J����������
	normalCamera.reset(new Camera());
	normalCamera->Initialize();
	railCamera.reset(new RailCamera());
	railCamera->Initialize();

	//���C�g����
	lightGroup.reset(LightGroup::Create());

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

	//�X�v���C�g���ʕ����̃C���X�^���X���擾
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	//�X�v���C�g�p�e�N�X�`���ǂݍ���
	spriteCommon->LoadTexture(1, "reticle.png");


	//obj���烂�f���f�[�^��ǂݍ���
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydome"));
	modelSphere.reset(ObjModel::LoadFromOBJ("sphere", true));
	modelFighter.reset(ObjModel::LoadFromOBJ("fighter", true));

	//���@�ɕK�v�ȏ����Z�b�g
	Player::SetGameScene(this);
	Player::SetBulletModel(modelSphere.get());
	//���@����
	player.reset(Player::Create(modelFighter.get()));
	player->SetIsCameraFollow(true);

	//�S�G�ɕK�v�ȏ����Z�b�g
	Enemy::SetGameScene(this);
	Enemy::SetPlayer(player.get());
	Enemy::SetBulletModel(modelSphere.get());
	//�G�̑��x��ݒ�
	const Vector3 position(5, 0, 50);
	std::unique_ptr<Enemy> newEnemy;
	newEnemy.reset(Cannon::Create(modelFighter.get(), position));
	enemys.push_back(std::move(newEnemy));

	//�V������
	objSkydome.reset(Skydome::Create(modelSkydome.get()));

	//obj�I�u�W�F�N�g�ɃJ�������Z�b�g
	ObjObject3d::SetCamera(railCamera.get());
	//obj�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	ObjObject3d::SetLightGroup(lightGroup.get());

	LoadEnemySetData();
}

void GameScene::Update()
{
	//���͂̃C���X�^���X���擾
	Input* input = Input::GetInstance();
	//�f�o�b�O�e�L�X�g�̃C���X�^���X���擾
	DebugText* debugText = DebugText::GetInstance();

	//���S�������@�e�̍폜
	playerBullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->GetIsDead();
		});

	//���S�����G�̍폜
	enemys.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->GetIsDead();
		});

	//���S�����G�e�̍폜
	enemyBullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->GetIsDead();
		});

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

	if (input->TriggerKey(DIK_1))
	{
		const Vector3 position(5, 0, 50);
		std::unique_ptr<Enemy> newEnemy;
		newEnemy.reset(Cannon::Create(modelFighter.get(), position));
		enemys.push_back(std::move(newEnemy));
	}

	if (input->TriggerKey(DIK_2))
	{
		const Vector3 position(-5, 0, 50);
		std::unique_ptr<Enemy> newEnemy;
		const float enemySpeed = 0.5f;
		Vector3 velocity(0, 0, enemySpeed);
		newEnemy.reset(DemoEnemy::Create(modelFighter.get(), position, velocity));
		enemys.push_back(std::move(newEnemy));
	}

	UpdateEnemySetCommands();

	//�J�����X�V
	normalCamera->Update();
	railCamera->Update();

	//�I�u�W�F�N�g�X�V
	//�V��
	objSkydome->Update();
	//���@
	player->Update();
	//���@�e
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		bullet->Update();
	}
	//�G
	for (const std::unique_ptr<Enemy>& enemy : enemys)
	{
		enemy->Update();
	}
	//�G�e
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		bullet->Update();
	}

	//�Փ˔���Ǘ�
	CollisionCheck();

	//�f�o�b�N�e�L�X�g
	//X���W,Y���W,�k�ڂ��w�肵�ĕ\��
	debugText->Print("GAME SCENE", 1000, 50);
	std::string enemyNum = std::to_string(enemys.size());
	DebugText::GetInstance()->Print("EnemyNum : " + enemyNum, 200, 200);

	if (input->TriggerKey(DIK_RETURN))
	{
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void GameScene::Draw()
{
	//Object3d���ʃR�}���h
	ObjObject3d::DrawPrev();
	///-------Object3d�`�悱������-------///


	//�V��
	objSkydome->Draw();
	//���@
	player->Draw();
	//���@�e
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		bullet->Draw();
	}
	//�G
	for (const std::unique_ptr<Enemy>& enemy : enemys)
	{
		enemy->Draw();
	}
	//�G�e
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		bullet->Draw();
	}


	///-------Object3d�`�悱���܂�-------///


	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->DrawPrev();
	///-------�X�v���C�g�`�悱������-------///

	player->DrawUI();


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
	//�S�Ă̓G�ƑS�Ă̎��@�e�̏Փ˔���
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//���@�e���W
		posA = bullet->GetWorldPos();
		//���@�e���a
		radiusA = bullet->GetScale().x;

		for (const std::unique_ptr<Enemy>& enemy : enemys) {
			//�G���W
			posB = enemy->GetWorldPos();
			//�G���a
			radiusB = enemy->GetScale().x;

			//���Ƌ��̏Փ˔�����s��
			bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);

			//�Փ˂��Ă�����
			if (isCollision) {
				//�G�̃R�[���o�b�N�֐����Ăяo��
				enemy->OnCollision();
				//���@�e�̃R�[���o�b�N�֐����Ăяo��
				bullet->OnCollision();

				break;
			}
		}
	}
#pragma endregion
}

void GameScene::AddPlayerBullet(std::unique_ptr<PlayerBullet> playerBullet)
{
	//���@�e���X�g�ɓo�^
	playerBullets.push_back(std::move(playerBullet));
}

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet)
{
	//�G�e���X�g�ɓo�^
	enemyBullets.push_back(std::move(enemyBullet));
}

void GameScene::LoadEnemySetData()
{
	//�t�@�C�����J��
	std::ifstream file;
	file.open("Resources/EnemySet.csv");
	assert(file.is_open());

	//�t�@�C���̓��e�𕶎���X�g���[���ɃR�s�[
	enemySetCommands << file.rdbuf();

	//�t�@�C�������
	file.close();
}

void GameScene::UpdateEnemySetCommands()
{
	//�ҋ@����
	if (isWait) {
		waitTimer--;
		if (waitTimer <= 0) {
			//�ҋ@�I��
			isWait = false;
		}
		return;
	}

	//1�s���̕����������ϐ�
	std::string line;

	//�R�}���h�����s���郋�[�v
	while (getline(enemySetCommands, line)) {
		//1�s���̕�������X�g���[���ϊ����ĉ�͂��₷��
		std::istringstream line_stream(line);

		std::string word;
		//�u,�v��؂�ōs�̐擪�������擾
		getline(line_stream, word, ',');

		//"//"����n�߂�s�̓R�����g
		if (word.find("//") == 0) {
			//�R�����g�s���΂�
			continue;
		}

		//POP�R�}���h
		if (word.find("POP") == 0) {
			//x���W
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			//y���W
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			//z���W
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//�G�𔭐�
			std::unique_ptr<Enemy> newEnemy;
			newEnemy.reset(Cannon::Create(modelFighter.get(), { x, y, z }));
			enemys.push_back(std::move(newEnemy));
		}

		//WAIT�R�}���h
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			//�҂�����
			int32_t waitTime = atoi(word.c_str());

			//�ҋ@�J�n
			isWait = true;
			waitTimer = waitTime;

			//�R�}���h���[�v�𔲂���
			break;
		}
	}
}
