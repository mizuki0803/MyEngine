#include "GameScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Easing.h"
#include "ParticleEmitter.h"
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

	//�p�[�e�B�N������
	ParticleManager::SetCamera(railCamera.get());

	//�X�v���C�g���ʕ����̃C���X�^���X���擾
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	//�X�v���C�g�p�e�N�X�`���ǂݍ���
	spriteCommon->LoadTexture(1, "reticle.png");
	spriteCommon->LoadTexture(2, "HPGaugeIn.png");
	spriteCommon->LoadTexture(3, "HPGaugeOut.png");
	spriteCommon->LoadTexture(4, "bossHPGaugeIn.png");
	spriteCommon->LoadTexture(5, "bossHPGaugeOut.png");


	//obj���烂�f���f�[�^��ǂݍ���
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydome"));
	modelGround.reset(ObjModel::LoadFromOBJ("ground"));
	modelMountain.reset(ObjModel::LoadFromOBJ("mountain"));
	modelSphere.reset(ObjModel::LoadFromOBJ("sphere", true));
	modelFighter.reset(ObjModel::LoadFromOBJ("fighter", true));
	modelEnemyFighter.reset(ObjModel::LoadFromOBJ("enemyFighter", true));
	modelBossMainBody.reset(ObjModel::LoadFromOBJ("bossMainBody", true));
	modelBossMainBodyDamage.reset(ObjModel::LoadFromOBJ("bossMainBodyDamage", true));
	modelBossMainBodySleep.reset(ObjModel::LoadFromOBJ("bossMainBodySleep", true));
	modelBossAvatar.reset(ObjModel::LoadFromOBJ("bossAvatar", true));
	modelBossAvatarDamage.reset(ObjModel::LoadFromOBJ("bossAvatarDamage", true));
	modelBossAvatarSleep.reset(ObjModel::LoadFromOBJ("bossAvatarSleep", true));

	//���@�ɕK�v�ȏ����Z�b�g
	Player::SetGameScene(this);
	Player::SetBulletModel(modelSphere.get());
	//���@����
	player.reset(Player::Create(modelFighter.get()));
	player->SetIsCameraFollow(true);

	//���[���J�����Ɏ��@�̃|�C���^���Z�b�g
	railCamera->SetPlayer(player.get());

	//�S�G�ɕK�v�ȏ����Z�b�g
	Enemy::SetGameScene(this);
	Enemy::SetPlayer(player.get());
	Enemy::SetBulletModel(modelSphere.get());
	//�e��ނ̓G�ɕK�v�ȏ����Z�b�g
	CannonEnemy::SetBreakModel(modelSphere.get());
	//�G�z�u�X�N���v�g�̓ǂݍ���
	LoadEnemySetData();

	//�{�X�ɕK�v�ȏ����Z�b�g
	Boss::SetPlayer(player.get());
	BossMainBody::SetGameScene(this);
	BossMainBody::SetBossMainBodyModel(modelBossMainBody.get());
	BossMainBody::SetBossMainBodyDamageModel(modelBossMainBodyDamage.get());
	BossMainBody::SetBossMainBodySleepModel(modelBossMainBodySleep.get());
	BossMainBody::SetBulletModel(modelSphere.get());
	BossAvatar::SetGameScene(this);
	BossAvatar::SetAvatarModel(modelBossAvatar.get());
	BossAvatar::SetAvatarDamageModel(modelBossAvatarDamage.get());
	BossAvatar::SetAvatarSleepModel(modelBossAvatarSleep.get());
	BossAvatar::SetBulletModel(modelSphere.get());

	//�񕜃A�C�e���ɕK�v�ȏ����Z�b�g
	HealingItem::SetPlayer(player.get());

	std::unique_ptr<HealingItem> healingItem;
	healingItem.reset(HealingItem::Create(modelSphere.get(), { 0, 0, 35 }));
	healingItems.push_back(std::move(healingItem));


	//�V������
	skydome.reset(Skydome::Create(modelSkydome.get()));

	//�n�ʐ���
	ground.reset(Ground::Create(modelGround.get()));

	//�w�i�p(�R)����
	for (int i = 0; i < 20; i++) {
		std::unique_ptr<Mountain> newMountain;
		newMountain.reset(Mountain::Create(modelMountain.get(), { -70, 0, 0 + (float)i * 40 }));
		mountains.push_back(std::move(newMountain));
	}
	for (int i = 0; i < 20; i++) {
		std::unique_ptr<Mountain> newMountain;
		newMountain.reset(Mountain::Create(modelMountain.get(), { 70, 0, 0 + (float)i * 40 }));
		mountains.push_back(std::move(newMountain));
	}

	//obj�I�u�W�F�N�g�ɃJ�������Z�b�g
	ObjObject3d::SetCamera(railCamera.get());
	//obj�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	ObjObject3d::SetLightGroup(lightGroup.get());
}

void GameScene::Update()
{
	//���͂̃C���X�^���X���擾
	Input* input = Input::GetInstance();
	//�f�o�b�O�e�L�X�g�̃C���X�^���X���擾
	DebugText* debugText = DebugText::GetInstance();

	//���S�������@�e�̍폜�O����
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//���S���Ă��Ȃ���Δ�΂�
		if (!bullet->GetIsDead()) { continue; }

		//���e�B�N�������b�N�I�����������ꍇ�A�z�[�~���O�e�����b�N�I���ȊO�̓G�ɓ������Ă��܂��\��������̂Ń��b�N�I���������Ă���
		if (!(bullet->GetBulletType() == PlayerBullet::BulletType::Homing)) { continue; }
		if (!player->GetReticles()->GetIsLockon()) { continue; }

		player->GetReticles()->UnlockonEnemy();
	}
	//���S�������@�e�̍폜
	playerBullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->GetIsDead();
		});

	//���S�����G�̏���
	for (const std::unique_ptr<Enemy>& enemy : enemys) {
		//���S���Ă��Ȃ���Δ�΂�
		if (!enemy->GetIsDead()) { continue; }

		//���e�B�N���̃��b�N�I���Ώۂ������ꍇ�A���b�N�I������
		if (player->GetReticles()->GetLockonEnemy() == enemy.get()) {
			player->GetReticles()->UnlockonEnemy();
		}
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
			//���@�e�̃z�[�~���O�Ώۂ������ꍇ
			if (bullet->GetEnemy() != enemy.get()) { continue; }
			//�z�[�~���O����
			bullet->SetEnemy(nullptr);
		}
	}
	//�폜��Ԃ̓G�̍폜
	enemys.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->GetIsDelete();
		});

	//���S�����G�e�̍폜
	enemyBullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->GetIsDead();
		});

	//���S�����񕜃A�C�e���̍폜
	healingItems.remove_if([](std::unique_ptr<HealingItem>& healingItem) {
		return healingItem->GetIsDead();
		});

	//���@���ė����J�n������Q�[���I�[�o�[
	if (!isGameOver) {
		if (player->GetIsCrash()) {
			//���[���J������ė���Ԃɂ���
			railCamera->CrashStart();

			//�Q�[���I�[�o�[
			isGameOver = true;
		}
	}
	if (boss) {
		//���S�����{�X�̉��
		if (boss->GetIsDead()) {
			boss.reset();
			//�X�e�[�W�N���A
			isStageClear = true;
		}
	}


	//�G�����R�}���h�X�V
	UpdateEnemySetCommands();
	//�{�X�o�g���J�n���菈��
	BossBattleStart();
	//�Q�[���I�[�o�[
	GameOver();

	//�J�����X�V
	normalCamera->Update();
	railCamera->Update();

	//�I�u�W�F�N�g�X�V
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
	//�{�X
	if (boss) {
		boss->Update();
	}
	//�G�e
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		bullet->Update();
	}
	//�񕜃A�C�e��
	for (const std::unique_ptr<HealingItem>& healingItem : healingItems) {
		healingItem->Update();
	}
	//�V��
	skydome->Update();
	//�n��
	ground->Update();
	//�w�i�p(�R)
	for (const std::unique_ptr<Mountain>& mountain : mountains) {
		mountain->Update();
	}

	//�Փ˔���Ǘ�
	CollisionCheck3d();
	CollisionCheck2d();

	//�p�[�e�B�N���X�V
	ParticleEmitter::GetInstance()->Update();

	//�f�o�b�N�e�L�X�g
	//X���W,Y���W,�k�ڂ��w�肵�ĕ\��
	//debugText->Print("GAME SCENE", 1000, 50);
	std::string enemyNum = std::to_string(enemys.size());
	DebugText::GetInstance()->Print("EnemyNum : " + enemyNum, 100, 200);
	if (isStageClear) {
		DebugText::GetInstance()->Print("STAGE CLEAR", 100, 250);
	}
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
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}

void GameScene::Draw()
{
	//Object3d���ʃR�}���h
	ObjObject3d::DrawPrev();
	///-------Object3d�`�悱������-------///

	//���@
	player->Draw();
	//���@�e
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		if (!(bullet->GetBulletType() == PlayerBullet::BulletType::Straight)) { continue; }
		bullet->Draw();
	}
	//�G
	for (const std::unique_ptr<Enemy>& enemy : enemys) {
		enemy->Draw();
	}
	//�G�e
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		bullet->Draw();
	}
	//�{�X
	if (boss) {
		boss->Draw();
	}
	//�񕜃A�C�e��
	for (const std::unique_ptr<HealingItem>& healingItem : healingItems) {
		healingItem->Draw();
	}
	//�V��
	skydome->Draw();
	//�n��
	ground->Draw();
	//�w�i�p(�R)
	for (const std::unique_ptr<Mountain>& mountain : mountains) {
		mountain->Draw();
	}

	///-------Object3d�`�悱���܂�-------///


	//�p�[�e�B�N�����ʃR�}���h
	ParticleManager::DrawPrev();
	///-------�p�[�e�B�N���`�悱������-------///

	//�p�[�e�B�N���`��
	ParticleEmitter::GetInstance()->DrawAll();

	///-------�p�[�e�B�N���`�悱���܂�-------///


	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->DrawPrev();
	///-------�X�v���C�g�`�悱������-------///

	//���@��UI�`��
	player->DrawUI();

	//�{�X��UI�`��
	if (boss) {
		boss->DrawUI();
	}

	///-------�X�v���C�g�`�悱���܂�-------///
}

void GameScene::CollisionCheck3d()
{
	//����Ώۂ̍��W
	Vector3 posA, posB;
	float radiusA, radiusB;

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
			//�Փ˂��Ă��Ȃ���Δ�΂�
			if (!isCollision) { continue; }

			//�G�̃R�[���o�b�N�֐����Ăяo��
			enemy->OnCollision();
			//���@�e�̃R�[���o�b�N�֐����Ăяo��
			bullet->OnCollision();

			break;
		}
	}
#pragma endregion

	//���@�����S���Ă����炱�̐�̏����͍s��Ȃ�
	if (player->GetIsDead()) { return; }

#pragma region ���@�ƓG�̏Փ˔���
	//���@���ً}��������Ă��Ȃ���Δ��肷��
	if (!player->GetIsRoll()) {
		//���@���W
		posA = player->GetWorldPos();
		//���@���a
		radiusA = player->GetScale().x;

		//���@�ƑS�Ă̓G�̏Փ˔���
		for (const std::unique_ptr<Enemy>& enemy : enemys) {
			//���@���_���[�W��ԂȂ��΂�
			if (player->GetIsDamage()) { continue; }

			//�G���W
			posB = enemy->GetWorldPos();
			//�G���a
			radiusB = enemy->GetScale().x;

			//���Ƌ��̏Փ˔�����s��
			bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);
			//�Փ˂��Ă��Ȃ���Δ�΂�
			if (!isCollision) { continue; }

			//���@�̃_���[�W�p�R�[���o�b�N�֐����Ăяo��
			player->OnCollisionDamage(posB);
			//�J�������V�F�C�N������
			railCamera->ShakeStart();
		}
	}
#pragma endregion

#pragma region ���@�ƓG�e�̏Փ˔���
	//���@���ً}��������Ă��Ȃ���Δ��肷��
	if (!player->GetIsRoll()) {
		//���@���W
		posA = player->GetWorldPos();
		//���@���a
		radiusA = player->GetScale().x;

		//���@�ƑS�Ă̓G�e�̏Փ˔���
		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
			//���@���_���[�W��ԂȂ��΂�
			if (player->GetIsDamage()) { continue; }

			//�G�e���W
			posB = bullet->GetWorldPos();
			//�G�e���a
			radiusB = bullet->GetScale().x;

			//���Ƌ��̏Փ˔�����s��
			bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);
			//�Փ˂��Ă��Ȃ���Δ�΂�
			if (!isCollision) { continue; }

			//���@�̃_���[�W�p�R�[���o�b�N�֐����Ăяo��
			player->OnCollisionDamage(posB);
			//�G�e�̃R�[���o�b�N�֐����Ăяo��
			bullet->OnCollision();
			//�J�������V�F�C�N������
			railCamera->ShakeStart();
		}
	}
#pragma endregion

#pragma region ���@�Ɖ񕜃A�C�e���̏Փ˔���
	//���@���ė���ԂłȂ���Δ��肷��
	if (!player->GetIsCrash()) {
		//���@���W
		posA = player->GetWorldPos();
		//���@���a
		radiusA = player->GetScale().x;

		//���@�ƑS�Ẳ񕜃A�C�e���̏Փ˔���
		for (const std::unique_ptr<HealingItem>& healingItem : healingItems) {
			//�񕜃A�C�e�����W
			posB = healingItem->GetWorldPos();
			//�񕜃A�C�e�����a
			radiusB = healingItem->GetScale().x;

			//���Ƌ��̏Փ˔�����s��
			bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);
			//�Փ˂��Ă��Ȃ���Δ�΂�
			if (!isCollision) { continue; }

			//���@�̉񕜗p�R�[���o�b�N�֐����Ăяo��
			player->OnCollisionHeal();
			//�񕜃A�C�e���̃R�[���o�b�N�֐����Ăяo��
			healingItem->OnCollision();
		}
	}
#pragma endregion

	//�{�X�̑��݂��Ȃ���΂��̐�̏����͍s��Ȃ�
	if (!boss) { return; }

#pragma region ���@�ƃ{�X���g�̏Փ˔���
	//���@���ً}��������Ă��Ȃ���Δ��肷��
	if (!player->GetIsRoll()) {
		//���@���W
		posA = player->GetWorldPos();
		//���@���a
		radiusA = player->GetScale().x;

		//�{�X���g�̃��X�g�������Ă���
		const std::list<std::unique_ptr<BossAvatar>>& bossAvatars = boss->GetAvatars();
		for (const std::unique_ptr<BossAvatar>& bossAvatar : bossAvatars) {
			//���@���_���[�W��ԂȂ��΂�
			if (player->GetIsDamage()) { continue; }

			//�{�X���g���W
			posB = bossAvatar->GetWorldPos();
			//�{�X���g���a
			radiusB = bossAvatar->GetScale().x;

			//���Ƌ��̏Փ˔�����s��
			bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);
			//�Փ˂��Ă��Ȃ���Δ�΂�
			if (!isCollision) { continue; }

			//���@�̃_���[�W�p�R�[���o�b�N�֐����Ăяo��
			player->OnCollisionDamage(posB);
			//�J�������V�F�C�N������
			railCamera->ShakeStart();
		}
	}
#pragma endregion

#pragma region ���@�e�ƃ{�X�{�̂̏Փ˔���
	//�{�̍��W
	posA = boss->GetMainBody()->GetWorldPos();
	//�{�̔��a
	radiusA = boss->GetMainBody()->GetScale().x;

	//�S�Ď��@�e�ƃ{�X�{�̂̏Փ˔���
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//���@�e���W
		posB = bullet->GetWorldPos();
		//���@�e�e���a
		radiusB = bullet->GetScale().x;

		//���Ƌ��̏Փ˔�����s��
		bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);
		//�Փ˂��Ă��Ȃ���Δ�΂�
		if (!isCollision) { continue; }

		//�{�X�̃R�[���o�b�N�֐����Ăяo��
		const int attackPower = 2;
		boss->OnCollisionMainBody(attackPower);
		//���@�e�̃R�[���o�b�N�֐����Ăяo��
		bullet->OnCollision();

		break;
	}
#pragma endregion

#pragma region ���@�e�ƃ{�X���g�̏Փ˔���
	//�S�Ẵ{�X���g�ƑS�Ă̎��@�e�̏Փ˔���
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//���@�e���W
		posA = bullet->GetWorldPos();
		//���@�e���a
		radiusA = bullet->GetScale().x;

		//�{�X���g�̃��X�g�������Ă���
		const std::list<std::unique_ptr<BossAvatar>>& bossAvatars = boss->GetAvatars();
		for (const std::unique_ptr<BossAvatar>& bossAvatar : bossAvatars) {
			//�{�X���g���W
			posB = bossAvatar->GetWorldPos();
			//�{�X���g���a(�e�q�\���ׁ̈A�{�̂̑傫���~���g�̑傫���Ő������傫����������)
			radiusB = boss->GetMainBody()->GetScale().x * bossAvatar->GetScale().x;

			//���Ƌ��̏Փ˔�����s��
			bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);
			//�Փ˂��Ă��Ȃ���Δ�΂�
			if (!isCollision) { continue; }

			//�{�X�̃R�[���o�b�N�֐����Ăяo��
			const int attackPower = 2;
			boss->OnCollisionAvatar(bossAvatar.get(), attackPower);
			//���@�e�̃R�[���o�b�N�֐����Ăяo��
			bullet->OnCollision();

			break;
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
		if (player->GetReticles()->GetIsLockon()) { return; }

		//���e�B�N�����W
		posA = player->GetReticles()->GetFarReticleScreenPosition();
		//���e�B�N�����a
		radiusA = player->GetReticles()->GetFarReticleSize();

		//���e�B�N���ƑS�Ă̓G�̏Փ˔���
		for (const std::unique_ptr<Enemy>& enemy : enemys) {
			//�G�����S��ԂȂ��΂�
			if (enemy->GetIsDead()) { continue; }

			//�G�̃��[���h���W�����@�̃��[���h���W����O�ɂ����画�肵�Ȃ��Ŕ�΂�
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
			//�Փ˂��Ă��Ȃ���Δ�΂�
			if (!isCollision) { continue; }

			//���e�B�N���̃��b�N�I���Ώۂ��m�肳����
			player->GetReticles()->LockonEnemy(enemy.get());
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
				newEnemy.reset(DemoEnemy::Create(modelEnemyFighter.get(), { x, y, z }, { velX, velY, velZ }));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::Cannon) {
				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(CannonEnemy::Create(modelEnemyFighter.get(), { x, y, z }));
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
				newEnemy.reset(CircularEnemy::Create(modelEnemyFighter.get(), { x, y, z }, angle, length, rotSpeed));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::Fall) {
				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(FallEnemy::Create(modelEnemyFighter.get(), { x, y, z }));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::UpDown) {
				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(UpDownEnemy::Create(modelEnemyFighter.get(), { x, y, z }));
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
				newEnemy.reset(ComeGoEnemy::Create(modelEnemyFighter.get(), { x, y, z }, { comeX, comeY, comeZ }, { goX, goY, goZ }, time));
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

void GameScene::BossBattleStart()
{
	//���Ƀ{�X�o�g���Ȃ甲����
	if (isBossBattle) { return; }

	//���@���{�X�o�g���J�n�Ƃ�����W�܂Ői�񂾂�J�n
	const float isBossBattleStartPos = 350;
	const bool isBossBattleStart = player->GetWorldPos().z >= isBossBattleStartPos;
	if (!isBossBattleStart) { return; }

	//�{�X����
	const Vector3 bossBasePos = { 0, 20, 410 };
	boss.reset(Boss::Create(bossBasePos));

	//���[���J�����̑O�i���~�߂�
	railCamera->SetIsAdvance(false);

	//�{�X�o�g����Ԃɂ���
	isBossBattle = true;
}

void GameScene::GameOver()
{
	//�Q�[���I�[�o�[�łȂ���Δ�����
	if (!player->GetIsDead()) { return; }

	//�^�C�}�[�X�V
	const float gameOverTime = 180;
	gameOverTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ�����Q�[���V�[������蒼��
	if (gameOverTimer >= gameOverTime) {
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}
