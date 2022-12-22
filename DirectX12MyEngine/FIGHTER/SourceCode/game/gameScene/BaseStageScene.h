#pragma once
#include "BaseGameScene.h"
#include "LightGroup.h"
#include "GameCamera.h"
#include "LightCamera.h"
#include "CollisionShape.h"
#include "Collision.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "BossWarning.h"
#include "BossDeadEffect.h"
#include "HealingItem.h"
#include "Skydome.h"
#include "GameGroundManager.h"
#include "GameBuildingManager.h"
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

protected: //�����o�ϐ�
	//���@�e
	std::list<std::unique_ptr<PlayerBullet>> playerBullets;
	//�G
	std::list<std::unique_ptr<Enemy>> enemys;
	//�G�e
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets;
	//�G�j��G�t�F�N�g
	std::list<std::unique_ptr<EnemyBreakEffect>> enemyBreakEffects;
};