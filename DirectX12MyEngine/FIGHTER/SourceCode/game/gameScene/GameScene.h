#pragma once
#include "BaseGameScene.h"
#include "Sprite.h"
#include "ObjObject3d.h"
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
#include "HowToPlayUI.h"
#include "MultiHitUI.h"
#include "StageStartUI.h"
#include "StageClearText.h"
#include "StageResultUI.h"

#include <array>
#include <sstream>

/// <summary>
/// �Q�[���V�[��
/// </summary>
class GameScene :public BaseGameScene
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

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
	/// �G����������
	/// </summary>
	void InitializeEnemy();

	/// <summary>
	/// �G�����f�[�^�ǂݍ���
	/// </summary>
	void LoadEnemySetData(const std::string& fileName);

	/// <summary>
	/// �G�����R�}���h�̍X�V
	/// </summary>
	void UpdateEnemySetCommands();

	/// <summary>
	/// �V�ѕ�
	/// </summary>
	void HowToPlay();

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
	std::unique_ptr<GameCamera> gameCamera;
	//�e�p�����J����
	std::unique_ptr<LightCamera> lightCamera;
	//���ォ��̉e�p�����J����
	std::unique_ptr<LightCamera> topLightCamera;

	//���C�g
	std::unique_ptr<LightGroup> lightGroup;
	float ambientColor0[3] = { 1,1,1 };
	// �������������l
	float lightDir0[3] = { 2,-1,1 };
	float lightColor0[3] = { 1,1,1 };
	float lightDir1[3] = { -2,-1,-6 };
	float lightColor1[3] = { 0.3f,0.3f,0.3f };

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
	std::unique_ptr<Player> player;
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
	//�{�X
	std::unique_ptr<Boss> boss;
	//�{�X�풆��
	bool isBossBattle = false;
	//�{�X�o��x��
	std::unique_ptr<BossWarning> bossWarning;
	//�{�X���S��̉��o�Ǘ�
	std::unique_ptr<BossDeadEffect> bossDeadEffect;
	//�񕜃A�C�e��
	std::list<std::unique_ptr<HealingItem>> healingItems;
	//�V��
	std::unique_ptr<Skydome> skydome;
	//�n��
	std::unique_ptr<GameGroundManager> gameGroundManager;
	//�w�i�p(�r���Ǘ�)
	std::unique_ptr<GameBuildingManager> gameBuildingManager;
	//�V�ѕ�UI
	std::unique_ptr<HowToPlayUI> howToPlayUI;
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