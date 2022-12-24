#pragma once
#include "BaseGameScene.h"
#include "PlayerBullet.h"
#include "LightGroup.h"
#include "LightCamera.h"
#include "Enemy.h"
#include "BossWarning.h"
#include "HealingItem.h"
#include "Skydome.h"
#include "MultiHitUI.h"
#include "StageStartUI.h"
#include "StageClearText.h"
#include "StageResultUI.h"

#include <array>
#include <sstream>

/// <summary>
/// �S�X�e�[�W�̊��V�[��
/// </summary>
class BaseStageScene :public BaseGameScene
{
public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �w�i�X�v���C�g�`��
	/// </summary>
	virtual void DrawBackSprite() = 0;

	/// <summary>
	/// 3D�V�[���`��
	/// </summary>
	virtual void Draw3D() = 0;

	/// <summary>
	/// 3D�V�[���`��(���C�g���猩�����_)
	/// </summary>
	virtual void Draw3DLightView() = 0;

	/// <summary>
	/// 3D�V�[���`��(���ォ��̃��C�g���猩�����_)
	/// </summary>
	virtual void Draw3DTopLightView() = 0;

	/// <summary>
	/// �O�i�X�v���C�g�`��
	/// </summary>
	virtual void DrawFrontSprite() = 0;

	/// <summary>
	/// ���@�e��ǉ�����
	/// </summary>
	/// <param name="playerBullet">���@�e</param>
	void AddPlayerBullet(std::unique_ptr<PlayerBullet> playerBullet);

	/// <summary>
	/// �G�e��ǉ�����
	/// </summary>
	/// <param name="enemyBullet">�G�e</param>
	void AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet);

	/// <summary>
	/// �G�j��G�t�F�N�g��ǉ�����
	/// </summary>
	/// <param name="enemyBreakEffect">�G�j��G�t�F�N�g</param>
	void AddEnemyBreakEffect(std::unique_ptr<EnemyBreakEffect> enemyBreakEffect);

	/// <summary>
	/// �G�����f�[�^�ǂݍ���
	/// </summary>
	void LoadEnemySetData(const std::string& fileName);

	/// <summary>
	/// �G�����R�}���h�̍X�V
	/// </summary>
	void UpdateEnemySetCommands(const Vector3& targetPosition);

protected: //�����o�ϐ�
	//�e�p�����J����
	std::unique_ptr<LightCamera> lightCamera;
	//���ォ��̉e�p�����J����
	std::unique_ptr<LightCamera> topLightCamera;

	//���C�g
	std::unique_ptr<LightGroup> lightGroup;
	float ambientColor0[3] = { 1,1,1 };
	// �������������l
	float lightDir0[3] = { 2,-1,1 };
	float lightColor0[3] = { 0.9f,0.9f,0.9f };
	float lightDir1[3] = { -2,-1,-6 };
	float lightColor1[3] = { 0.3f,0.3f,0.3f };

	//���@�e
	std::list<std::unique_ptr<PlayerBullet>> playerBullets;
	//�G
	std::list<std::unique_ptr<Enemy>> enemys;
	//�G�e
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets;
	//�G�j��G�t�F�N�g
	std::list<std::unique_ptr<EnemyBreakEffect>> enemyBreakEffects;
	//�G�����R�}���h
	std::stringstream enemySetCommands;
	//�ҋ@����
	bool isWait = false;
	//�ҋ@�p �������s�����@���W(���@���������s�����W�ȏ�ɂȂ�����G�������s��)
	float waitEnemySetPlayerPosition = 0;
	//�{�X�풆��
	bool isBossBattle = false;
	//�{�X�o��x��
	std::unique_ptr<BossWarning> bossWarning;
	//�񕜃A�C�e��
	std::list<std::unique_ptr<HealingItem>> healingItems;
	//�V��
	std::unique_ptr<Skydome> skydome;
	//�ꌂ�ŕ����̏����p��UI
	std::list<std::unique_ptr<MultiHitUI>> multiHitUIs;
	//�X�e�[�W�J�nUI
	std::unique_ptr<StageStartUI> stageStartUI;
	//�X�e�[�W�N���A�e�L�X�g
	std::unique_ptr<StageClearText> stageClearText;
	//�X�e�[�W���U���gUI
	std::unique_ptr<StageResultUI> stageResultUI;
	//�X�e�[�W�N���A��
	bool isStageClear = false;
	//�Q�[���I�[�o�[��
	bool isGameOver = false;
	//�Q�[���I�[�o�[�^�C�}�[
	int32_t gameOverTimer = 0;
};