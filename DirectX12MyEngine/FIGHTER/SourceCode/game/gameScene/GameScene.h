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
#include "HealingItem.h"
#include "Skydome.h"
#include "GameGroundManager.h"
#include "GameMountainManager.h"
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
	/// �O�i�X�v���C�g�`��
	/// </summary>
	void DrawFrontSprite() override;

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

	//���C�g
	std::unique_ptr<LightGroup> lightGroup;
	float ambientColor0[3] = { 1,1,1 };
	// �������������l
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	float pointLightPos[3] = { 0,0,0 };
	float pointLightColor[3] = { 1,1,1 };
	float pointLightAtten[3] = { 0.3f, 0.1f, 0.1f };

	float spotLightDir[3] = { 0, -1, 0 };
	float spotLightPos[3] = { 0, 5, 0 };
	float spotLightColor[3] = { 1, 1, 1 };
	float spotLightAtten[3] = { 0.0f, 0.0f, 0.0f };
	float spotLightfactorAngleCos[2] = { 20.0f, 30.0f };

	float circleShadowDir[3] = { 0, -1, 0 };
	float circleShadowAtten[3] = { 0.5f, 0.6f, 0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.5f };

	//obj���f���f�[�^
	std::unique_ptr<ObjModel> modelSkydome;
	std::unique_ptr<ObjModel> modelGround;
	std::unique_ptr<ObjModel> modelMountain;
	std::unique_ptr<ObjModel> modelSphere;
	std::unique_ptr<ObjModel> modelFighter;
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
	//�񕜃A�C�e��
	std::list<std::unique_ptr<HealingItem>> healingItems;
	//�V��
	std::unique_ptr<Skydome> skydome;
	//�n��
	std::unique_ptr<GameGroundManager> gameGroundManager;
	//�w�i�p(�R�Ǘ�)
	std::unique_ptr<GameMountainManager> gameMountainManager;
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