#pragma once
#include "BaseStageScene.h"
#include "Stage02GameCamera.h"
#include "Collision.h"
#include "Stage02Player.h"
#include "Boss.h"
#include "BossDeadEffect.h"
#include "Meteorite.h"

/// <summary>
/// �X�e�[�W02�V�[��
/// </summary>
class Stage02Scene :public BaseStageScene
{
public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �w�i�X�v���C�g�`��
	/// </summary>
	void DrawBackSprite() override;

	/// <summary>
	/// 3D�V�[���`��
	/// </summary>
	void Draw3D() override;

	/// <summary>
	/// 3D�V�[���`��(���C�g���猩�����_)
	/// </summary>
	void Draw3DLightView() override;

	/// <summary>
	/// 3D�V�[���`��(���ォ��̃��C�g���猩�����_)
	/// </summary>
	void Draw3DTopLightView() override;

	/// <summary>
	/// �O�i�X�v���C�g�`��
	/// </summary>
	void DrawFrontSprite() override;

	/// <summary>
	/// �e�����p���C�g�J�����X�V
	/// </summary>
	void LightCameraUpdate();

	/// <summary>
	/// �I�u�W�F�N�g�̉��
	/// </summary>
	void ObjectRelease();

	/// <summary>
	/// 3D�I�u�W�F�N�g�̏Փ˔���
	/// </summary>
	void CollisionCheck3d();

	/// <summary>
	/// 2D�I�u�W�F�N�g�̏Փ˔���
	/// </summary>
	void CollisionCheck2d();

	/// <summary>
	/// �G����������
	/// </summary>
	void InitializeEnemy();

	/// <summary>
	/// �w�i�p覐Δ����R�}���h�̍X�V
	/// </summary>
	void UpdateMeteoriteSetCommands(const Vector3& targetPosition);

	/// <summary>
	/// �{�X�o�g���J�n���菈��
	/// </summary>
	void BossBattleStart();

	/// <summary>
	/// �X�e�[�W�N���A
	/// </summary>
	void StageClear();

	/// <summary>
	/// �X�e�[�W���U���g
	/// </summary>
	void StageResult();

	/// <summary>
	/// �X�e�[�W�N���A���̉e�̌����ɕύX
	/// </summary>
	void StageClearSetLightCameraPos();

	/// <summary>
	/// �X�e�[�W�N���A�e�L�X�g�����Ɖ��
	/// </summary>
	void StageClearTextCreateAndRelease();

	/// <summary>
	/// �X�e�[�W���U���gUI�����Ɖ��
	/// </summary>
	void StageResultUICreateAndRelease();

	/// <summary>
	/// �^�C�g���V�[���ɖ߂鏈��
	/// </summary>
	void ReturnTitleScene();

	/// <summary>
	/// �Q�[���I�[�o�[
	/// </summary>
	void GameOver();

private: //�����o�ϐ�
	//�Q�[���J����
	std::unique_ptr<Stage02GameCamera> gameCamera;

	//obj���f���f�[�^
	std::unique_ptr<ObjModel> modelSkydome;
	std::unique_ptr<ObjModel> modelGround;
	std::array<std::unique_ptr<ObjModel>, 2> modelBuilding;
	std::unique_ptr<ObjModel> modelSphere;
	std::unique_ptr<ObjModel> modelPlayerBullet;
	std::unique_ptr<ObjModel> modelFighter;
	std::unique_ptr<ObjModel> modelEnemyBullet;
	std::unique_ptr<ObjModel> modelEnemyFighter;
	std::array<std::unique_ptr<ObjModel>, 5> modelEnemyFighterBreak;
	std::unique_ptr<ObjModel> modelEnemyMiniRobot;
	std::array<std::unique_ptr<ObjModel>, 5> modelEnemyMiniRobotBreak;
	std::unique_ptr<ObjModel> modelBossMainBody;
	std::unique_ptr<ObjModel> modelBossMainBodyDamage;
	std::unique_ptr<ObjModel> modelBossMainBodySleep;
	std::unique_ptr<ObjModel> modelBossMainBodyDead;
	std::unique_ptr<ObjModel> modelBossAvatar;
	std::unique_ptr<ObjModel> modelBossAvatarDamage;
	std::unique_ptr<ObjModel> modelBossAvatarSleep;
	std::unique_ptr<ObjModel> modelBossAvatarDead;
	std::unique_ptr<ObjModel> modelHealingItem;

	//���@
	std::unique_ptr<Stage02Player> player;
	//�{�X
	std::unique_ptr<Boss> boss;
	//�{�X���S��̉��o�Ǘ�
	std::unique_ptr<BossDeadEffect> bossDeadEffect;

	//�w�i�p覐�(�j��s��)
	std::list<std::unique_ptr<Meteorite>> meteorites;
	//�w�i�p覐Δ����R�}���h
	std::stringstream meteoriteSetCommands;
	//�w�i�p覐Δ����R�}���h�ҋ@����
	bool isWaitMeteorite = false;
	//�ҋ@�p �������s�����@���W(���@���������s�����W�ȏ�ɂȂ�����w�i�p覐ΐ������s��)
	float waitMeteoriteSetPlayerPosition = 0;

	//�X�e�[�W�N���A�p�̉e��Ԃ�
	bool isStageClearShadow = false;
	//�^�[�Q�b�g�ƌ����J�����̋���
	Vector3 lightCameraTargetDistance = { -300, 200, -150 };
};