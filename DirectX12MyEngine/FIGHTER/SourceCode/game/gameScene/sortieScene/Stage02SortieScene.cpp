#include "Stage02SortieScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Easing.h"
#include "ParticleEmitter.h"
#include "SceneChangeEffect.h"
#include "GamePostEffect.h"
#include "MeteoriteEnemy.h"
#include <cassert>
#include <fstream>
#include <iomanip>

void Stage02SortieScene::Initialize()
{
	//���C�g����
	lightGroup.reset(LightGroup::Create());
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, false);

	//obj���烂�f���f�[�^��ǂݍ���
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydomeStage02"));
	modelFighter.reset(ObjModel::LoadFromOBJ("fighter"));
	modelPlayerBullet.reset(ObjModel::LoadFromOBJ("playerBullet", true));
	modelMeteoriteBrown.reset(ObjModel::LoadFromOBJ("meteoriteBrown", true));

	//�|�X�g�G�t�F�N�g�̃u���[���������Ă���
	GamePostEffect::GetPostEffect()->SetRadialBlur(false);

	//���@����
	player.reset(Stage02SortiePlayer::Create(modelFighter.get(), { 0, 0, -360 }));
	//���@�ɕK�v�ȏ����Z�b�g
	Stage02SortiePlayer::SetStageScene(this);
	Stage02SortiePlayer::SetBulletModel(modelPlayerBullet.get()); //�ʏ�e�p���f�����Z�b�g
	PlayerBullet::SetIsGroundMode(false); //���@�e�̒n�ʂ���s����OFF�ɂ���


	//�J����������
	sortieCamera.reset(new Stage02SortieCamera());
	sortieCamera->SetSortiePlayer(player.get());
	sortieCamera->Initialize();
	//�e�p�����J����������
	lightCamera.reset(new LightCamera());
	lightCamera->Initialize({ 0, 500, 0 });
	lightCamera->SetProjectionNum({ 500, 500 }, { -500, -500 });
	//���ォ��̉e�p�����J����������
	topLightCamera.reset(new LightCamera());
	topLightCamera->Initialize({ 0, 500, 0 });
	topLightCamera->SetProjectionNum({ 100, 100 }, { -100, -100 });

	//�S�G����������
	InitializeEnemy();
	//�G�����R�}���h�X�V
	UpdateEnemySetCommands(player->GetPosition());

	//�V������
	skydome.reset(Skydome::Create(modelSkydome.get()));
	//���h���������p�x�ɕύX���Ă���
	skydome->SetRotation({ 0, 215, 0 });

	//obj�I�u�W�F�N�g�ɃJ�������Z�b�g
	ObjObject3d::SetCamera(sortieCamera.get());
	ObjObject3d::SetLightCamera(lightCamera.get());
	ObjObject3d::SetTopLightCamera(topLightCamera.get());
	//obj�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	ObjObject3d::SetLightGroup(lightGroup.get());

	//�p�[�e�B�N���ɃJ�������Z�b�g
	ParticleManager::SetCamera(sortieCamera.get());
	//��ʂɃp�[�e�B�N�����c�邱�Ƃ�����̂őS�č폜���Ă���
	ParticleEmitter::GetInstance()->AllDelete();
}

void Stage02SortieScene::Update()
{
	//�f�o�b�O�e�L�X�g�̃C���X�^���X���擾
	DebugText* debugText = DebugText::GetInstance();

	//�I�u�W�F�N�g���
	ObjectRelease();
	//�o�������Ǘ�
	SortieAction();

	//�J�����X�V
	sortieCamera->Update();

	//���C�g�X�V
	lightGroup->SetAmbientColor(XMFLOAT3(ambientColor0));
	lightGroup->SetDirLightDir(0, XMVECTOR({ lightDir0[0], lightDir0[1], lightDir0[2], 0 }));
	lightGroup->SetDirLightColor(0, XMFLOAT3(lightColor0));
	lightGroup->SetDirLightDir(1, XMVECTOR({ lightDir1[0], lightDir1[1], lightDir1[2], 0 }));
	lightGroup->SetDirLightColor(1, XMFLOAT3(lightColor1));
	lightGroup->Update();

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
	//�G�j��G�t�F�N�g
	for (const std::unique_ptr<EnemyBreakEffect>& breakEffect : enemyBreakEffects) {
		breakEffect->Update();
	}
	//�V��
	skydome->Update();

	//3D�Փ˔���Ǘ�
	CollisionCheck3d();

	//�p�[�e�B�N���X�V
	ParticleEmitter::GetInstance()->Update();


	//���@�̏o���s��������������
	if (player->GetIsSortieEnd()) {
		//�X�e�[�W02�փV�[���ύX���J�n����
		SceneChangeStart({ 1,1,1,0 }, 10, 0, 20, "STAGE02");
	}
	//���[�r�[�X�L�b�v�@�\
	MovieSkip();

	//�V�[���ύX���
	SceneChangeMode();

	//�V�[���ύX���o�X�V
	SceneChangeEffect::Update();
}

void Stage02SortieScene::DrawBackSprite()
{
}

void Stage02SortieScene::Draw3D()
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
	//�G�j��G�t�F�N�g
	for (const std::unique_ptr<EnemyBreakEffect>& breakEffect : enemyBreakEffects) {
		breakEffect->Draw();
	}
	//�V��
	skydome->Draw();

	///-------Object3d�`�悱���܂�-------///

	///-------�p�[�e�B�N���`�悱������-------///

	//�p�[�e�B�N���`��
	ParticleEmitter::GetInstance()->DrawAll();

	///-------�p�[�e�B�N���`�悱���܂�-------///
}

void Stage02SortieScene::Draw3DLightView()
{
}

void Stage02SortieScene::Draw3DTopLightView()
{
	//Object3d���ʃR�}���h
	ObjObject3d::DrawLightViewPrev();
	///-------Object3d�`�悱������-------///

	//���@
	player->DrawTopLightCameraView();

	///-------Object3d�`�悱���܂�-------///
}

void Stage02SortieScene::DrawFrontSprite()
{
	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->DrawPrev();
	///-------�X�v���C�g�`�悱������-------///

	//�V�[���ύX���o�`��
	SceneChangeEffect::Draw();


	///-------�X�v���C�g�`�悱���܂�-------///
}

void Stage02SortieScene::ObjectRelease()
{
	//���S�������@�e�̍폜
	playerBullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->GetIsDead();
		});

	//�폜��Ԃ̓G�̍폜
	enemys.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->GetIsDelete();
		});

	//�폜��Ԃ̓G�j��G�t�F�N�g�̍폜
	enemyBreakEffects.remove_if([](std::unique_ptr<EnemyBreakEffect>& breakEffect) {
		return breakEffect->GetIsDelete();
		});
}

void Stage02SortieScene::CollisionCheck3d()
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
			enemy->OnCollision(bullet->GetDamageNum());
			//���@�e�̃R�[���o�b�N�֐����Ăяo��
			bullet->OnCollision(posB, radiusB);

			//�e�͈�̓G�����|���Ȃ��̂�enemy���[�v�𔲂���
			break;
		}
	}
#pragma endregion
}

void Stage02SortieScene::InitializeEnemy()
{
	//�G�z�u�X�N���v�g�̓ǂݍ���
	LoadSetData(enemySetCommands, "Resources/csv/EnemySetSortie02.csv");

	//�S�G�ɕK�v�ȏ����Z�b�g
	Enemy::SetStageScene(this); //�S�G�ɃX�e�[�W�V�[����������
	Enemy::SetPlayer(nullptr); //���@���Z�b�g
	Enemy::SetGameCamera(nullptr); //�Q�[���J�������Z�b�g
	Enemy::SetBulletModel(nullptr); //�e�̃��f�����Z�b�g
	Enemy::SetIsGroundMode(false); //�n�ʂ���s����OFF�ɂ���
	EnemyBreakEffect::SetIsGroundMode(false); //�j��G�t�F�N�g�̒n�ʂ���s����OFF�ɂ���
	EnemyBreakEffect::SetIsGravityMode(false); //�j��G�t�F�N�g�̏d�͂���s����OFF�ɂ���
	EnemyBreakEffect::SetGameCamera(nullptr); //�j��G�t�F�N�g�ɃQ�[���J�������Z�b�g

	//�j��\覐�
	MeteoriteEnemy::SetModel(modelMeteoriteBrown.get()); //���f�����Z�b�g
}

void Stage02SortieScene::SortieAction()
{
	//���Ƀu�[�X�g��ԂȂ甲����
	if (player->GetSortieActionPhase() == Stage02SortiePlayer::Stage02SortieActionPhase::Boost) { return; }
	//�J�����̎��@�Y�[�����I�����Ă��Ȃ���Δ�����
	if (!sortieCamera->GetIsZoomEnd()) { return; }

	//���@���u�[�X�g��Ԃɂ���
	player->BoostStart();
}

void Stage02SortieScene::MovieSkip()
{
	//�X�L�b�v���͂��Ȃ���Δ�����
	if (!(Input::GetInstance()->TriggerKey(DIK_RETURN) || Input::GetInstance()->TriggerGamePadButton(Input::PAD_START))) { return; }

	//�X�e�[�W02�փV�[���ύX���J�n����
	SceneChangeStart({ 0,0,0,0 }, 15, 0, 15, "STAGE02");
}
