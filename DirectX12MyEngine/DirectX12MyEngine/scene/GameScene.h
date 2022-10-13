#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include "ObjObject3d.h"
#include "LightGroup.h"
#include "GameCamera.h"
#include "CollisionShape.h"
#include "Collision.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "HealingItem.h"
#include "Skydome.h"
#include "Ground.h"
#include "Mountain.h"

#include <sstream>

/// <summary>
/// �Q�[���V�[��
/// </summary>
class GameScene :public BaseScene
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
	/// �`��
	/// </summary>
	void Draw() override;

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
	/// �G�����f�[�^�ǂݍ���
	/// </summary>
	void LoadEnemySetData();

	/// <summary>
	/// �G�����R�}���h�̍X�V
	/// </summary>
	void UpdateEnemySetCommands();

	/// <summary>
	/// �{�X�o�g���J�n���菈��
	/// </summary>
	void BossBattleStart();

	/// <summary>
	/// �X�e�[�W�N���A
	/// </summary>
	void StageClear();

	/// <summary>
	/// �Q�[���I�[�o�[
	/// </summary>
	void GameOver();

private: //�����o�ϐ�
	//���[���J����
	std::unique_ptr<GameCamera> gameCamera;

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
	std::unique_ptr<ObjModel> modelBossMainBody;
	std::unique_ptr<ObjModel> modelBossMainBodyDamage;
	std::unique_ptr<ObjModel> modelBossMainBodySleep;
	std::unique_ptr<ObjModel> modelBossAvatar;
	std::unique_ptr<ObjModel> modelBossAvatarDamage;
	std::unique_ptr<ObjModel> modelBossAvatarSleep;

	//���@
	std::unique_ptr<Player> player;
	//���@�e
	std::list<std::unique_ptr<PlayerBullet>> playerBullets;
	//�G
	std::list<std::unique_ptr<Enemy>> enemys;
	//�G�e
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets;
	//�G�����R�}���h
	std::stringstream enemySetCommands;
	//�ҋ@����
	bool isWait = false;
	//�ҋ@�^�C�}�[
	int32_t waitTimer = 0;
	//�{�X
	std::unique_ptr<Boss> boss;
	//�{�X�풆��
	bool isBossBattle = false;
	//�񕜃A�C�e��
	std::list<std::unique_ptr<HealingItem>> healingItems;
	//�V��
	std::unique_ptr<Skydome> skydome;
	//�n��
	std::unique_ptr<Ground> ground;
	//�w�i�p(�R)
	std::list<std::unique_ptr<Mountain>> mountains;
	//�X�e�[�W�N���A��
	bool isStageClear = false;
	//�Q�[���I�[�o�[��
	bool isGameOver = false;
	//�Q�[���I�[�o�[�^�C�}�[
	int32_t gameOverTimer = 0;

	//���̑��ϐ�
	float soundVol = 0.1f;
	char str[100];
};