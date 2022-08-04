#include "GameScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Easing.h"
#include "DemoEnemy.h"
#include "CannonEnemy.h"
#include "CircularEnemy.h"
#include "FallEnemy.h"
#include "UpDownEnemy.h"
#include "ComeGoEnemy.h"
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

	//���[���J�����Ɏ��@�̃|�C���^���Z�b�g
	RailCamera::SetPlayer(player.get());

	//�S�G�ɕK�v�ȏ����Z�b�g
	Enemy::SetGameScene(this);
	Enemy::SetPlayer(player.get());
	Enemy::SetBulletModel(modelSphere.get());

	//�V������
	objSkydome.reset(Skydome::Create(modelSkydome.get()));

	//obj�I�u�W�F�N�g�ɃJ�������Z�b�g
	ObjObject3d::SetCamera(railCamera.get());
	//obj�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	ObjObject3d::SetLightGroup(lightGroup.get());

	//�G�z�u�X�N���v�g�̓ǂݍ���
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

	//���S�����G�̍폜�O����
	for (const std::unique_ptr<Enemy>& enemy : enemys) {
		//���S���Ă��Ȃ���Δ�΂�
		if (!enemy->GetIsDead()) { continue; }

		//���e�B�N���̃��b�N�I���Ώۂ������ꍇ�A���b�N�I������
		if (player->GetReticle()->GetReticle2D()->GetLockonEnemy() == enemy.get()) {
			player->GetReticle()->GetReticle2D()->UnlockonEnemy();
		}
		//���@�e�̃z�[�~���O�Ώۂ������ꍇ�A�z�[�~���O����
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
			if (bullet->GetEnemy() == enemy.get()) {
				bullet->SetEnemy(nullptr);
			}
		}
	}
	//���S�����G�̍폜
	enemys.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->GetIsDead();
		});

	//���S�����G�e�̍폜
	enemyBullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->GetIsDead();
		});


	//�G�����R�}���h�X�V
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
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		bullet->Update();
	}
	//�G
	for (const std::unique_ptr<Enemy>& enemy : enemys) {
		enemy->Update();
	}
	//�G�e
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		bullet->Update();
	}

	//�Փ˔���Ǘ�
	CollisionCheck3d();
	CollisionCheck2d();

	//�f�o�b�N�e�L�X�g
	//X���W,Y���W,�k�ڂ��w�肵�ĕ\��
	//debugText->Print("GAME SCENE", 1000, 50);
	std::string enemyNum = std::to_string(enemys.size());
	DebugText::GetInstance()->Print("EnemyNum : " + enemyNum, 200, 200);
	/*std::string playerHP = std::to_string(player->GetHP());
	if (!player->GetIsDead()) {
		DebugText::GetInstance()->Print("PlayerHP : " + playerHP, 200, 200);
	}
	else {
		DebugText::GetInstance()->Print("PlayerDead", 200, 200);
	}*/


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

void GameScene::CollisionCheck3d()
{
	//����Ώۂ̍��W
	Vector3 posA, posB;
	float radiusA, radiusB;

#pragma region ���@�ƓG�̏Փ˔���
	//���@���W
	posA = player->GetWorldPos();
	//���@���a
	radiusA = player->GetScale().x;

	//���@�ƑS�Ă̓G�̏Փ˔���
	for (const std::unique_ptr<Enemy>& enemy : enemys) {
		//�G���W
		posB = enemy->GetWorldPos();
		//�G���a
		radiusB = enemy->GetScale().x;

		//���Ƌ��̏Փ˔�����s��
		bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);

		//�Փ˂��Ă�����
		if (isCollision) {
			//���@�̃R�[���o�b�N�֐����Ăяo��
			player->OnCollision(posB);
		}
	}
#pragma endregion

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
			player->OnCollision(posB);
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

void GameScene::CollisionCheck2d()
{
	//����Ώۂ̍��W
	Vector2 posA, posB;
	float radiusA, radiusB;

#pragma region ���e�B�N���ƓG�̏Փ˔���
	//�v���C���[���`���[�W�V���b�g��ԂȂ�
	if (player->GetIsChargeShotMode()) {
		//���e�B�N�������b�N�I����ԂȂ甲����
		if (player->GetReticle()->GetReticle2D()->GetIsLockon()) { return; }

		//���e�B�N�����W
		posA = player->GetReticle()->GetReticle2D()->GetPosition();
		//���e�B�N�����a
		radiusA = player->GetReticle()->GetReticle2D()->GetSize().x;

		//���e�B�N���ƑS�Ă̓G�̏Փ˔���
		for (const std::unique_ptr<Enemy>& enemy : enemys) {
			//�G�̃��[���h���W�����@�̃��[���h���W����O�ɂ����画�肵�Ȃ��Ŕ�����
			const float enemyToPlayerPosZ = enemy->GetWorldPos().z - player->GetWorldPos().z;
			if (enemyToPlayerPosZ < 0) { continue; }

			//�G���W
			posB = enemy->GetScreenPos();
			//�G���a
			radiusB = enemy->GetScale().x;

			//�G���W����ʊO�Ȃ珈���͔�΂�
			const bool isOutsideScreen = (posB.x > WindowApp::window_width || posB.x < 0 || posB.y > WindowApp::window_height || posB.y < 0);
			if (isOutsideScreen) { continue; }

			//���Ƌ��̏Փ˔�����s��
			bool isCollision = Collision::CheckCircleToCircle(posA, posB, radiusA, radiusB);

			//�Փ˂��Ă�����
			if (isCollision) {
				//���e�B�N���̃��b�N�I���Ώۂ��m�肳����
				player->GetReticle()->GetReticle2D()->LockonEnemy(enemy.get());
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
			//�G�̎��
			getline(line_stream, word, ',');
			int type = (int)std::atof(word.c_str());

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
			if (type == Enemy::EnemyType::Demo) {
				//x�������x
				getline(line_stream, word, ',');
				float velX = (float)std::atof(word.c_str());

				//y�������x
				getline(line_stream, word, ',');
				float velY = (float)std::atof(word.c_str());

				//z�������x
				getline(line_stream, word, ',');
				float velZ = (float)std::atof(word.c_str());

				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(DemoEnemy::Create(modelSphere.get(), { x, y, z }, { velX, velY, velZ }));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::Cannon) {
				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(CannonEnemy::Create(modelFighter.get(), { x, y, z }));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::Circular) {
				//�p�x
				getline(line_stream, word, ',');
				float angle = (float)std::atof(word.c_str());

				//���a�̒���
				getline(line_stream, word, ',');
				float length = (float)std::atof(word.c_str());

				//��]���x
				getline(line_stream, word, ',');
				float rotSpeed = (float)std::atof(word.c_str());

				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(CircularEnemy::Create(modelSphere.get(), { x, y, z }, angle, length, rotSpeed));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::Fall) {
				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(FallEnemy::Create(modelSphere.get(), { x, y, z }));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::UpDown) {
				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(UpDownEnemy::Create(modelSphere.get(), { x, y, z }));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::ComeGo) {
				//x�������W
				getline(line_stream, word, ',');
				float comeX = (float)std::atof(word.c_str());
				//y�������W
				getline(line_stream, word, ',');
				float comeY = (float)std::atof(word.c_str());
				//z�������W
				getline(line_stream, word, ',');
				float comeZ = (float)std::atof(word.c_str());

				//x�o�����W
				getline(line_stream, word, ',');
				float goX = (float)std::atof(word.c_str());
				//y�o�����W
				getline(line_stream, word, ',');
				float goY = (float)std::atof(word.c_str());
				//z�o�����W
				getline(line_stream, word, ',');
				float goZ = (float)std::atof(word.c_str());

				//�U������
				getline(line_stream, word, ',');
				int time = (int)std::atof(word.c_str());

				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(ComeGoEnemy::Create(modelSphere.get(), { x, y, z }, {comeX, comeY, comeZ}, {goX, goY, goZ}, time));
				enemys.push_back(std::move(newEnemy));
			}
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
