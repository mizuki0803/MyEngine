#pragma once
#include "ObjObject3d.h"
#include "PlayerBullet.h"
#include "PlayerReticles.h"
#include "PlayerHPUI.h"
#include "PlayerSpeedChangeUI.h"
#include "PlayerDamageEffect.h"
#include <memory>
#include <list>

//�Q�[���V�[���̑O���錾
class GameScene;

/// <summary>
/// ���@
/// </summary>
class Player : public ObjObject3d
{
public:
	//�ړ����x���
	enum class MoveSpeedPhase {
		NormalSpeed,	//�ʏ�ړ�
		HighSpeed,		//�����ړ�
		SlowSpeed,		//�x���ړ�
		ReturnNormalSpeed, //�ʏ�ړ��ɖ߂����
	};

	//�X�e�[�W�N���A��s���t�F�[�Y
	enum class StageClearModePhase {
		SideMove,	//������ړ�
		Return,		//����A��
		Up,			//�㏸
		Stay,		//��~
		Boost,		//�u�[�X�g
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="startHP">�J�n��HP</param>
	/// <param name="maxHP">�ő�HP</param>
	/// <returns>���@</returns>
	static Player* Create(ObjModel* model, const int startHP, const int maxHP);

	//getter
	static const Vector2& GetRotLimit() { return rotLimit; }
	static const Vector2& GetMoveLimitMax() { return moveLimitMax; }
	static const Vector2& GetMoveLimitMin() { return moveLimitMin; }
	static const float GetMoveBaseSpeed() { return moveBaseSpeed; }
	static const float GetKnockbackBaseSpeed() { return knockbackBaseSpeed; }

	//setter
	static void SetGameScene(GameScene* gameScene) { Player::gameScene = gameScene; }
	static void SetBulletModel(ObjModel* model) { Player::bulletModel = model; }

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="startHP">�J�n��HP</param>
	/// <param name="maxHP">�ő�HP</param>
	/// <returns>����</returns>
	bool Initialize(ObjModel* model, const int startHP, const int maxHP);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// UI�X�V
	/// </summary>
	void UpdateUI();

	/// <summary>
	/// UI�`��
	/// </summary>
	void DrawUI();

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�(�G�Ȃǃ_���[�W����炤)
	/// </summary>
	void OnCollisionDamage(const Vector3& subjectPos);

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�(�A�C�e���Ȃǉ񕜂���)
	/// </summary>
	void OnCollisionHeal();

	/// <summary>
	/// �X�e�[�W�N���A��̓������J�n����
	/// </summary>
	void StageClearModeStart();

	/// <summary>
	/// �X�e�[�W�N���A��̋A�҂��J�n����
	/// </summary>
	/// <param name="cameraPos">�J�������W</param>
	void StageClearReturnStart(const Vector3& cameraPos);

	/// <summary>
	/// �X�e�[�W�N���A��̃u�[�X�g���J�n����
	/// </summary>
	void StageClearBoostStart();

	//getter
	Vector3 GetWorldPos();
	const XMMATRIX& GetMatWorld() { return matWorld; }
	const int GetHP() { return HP; }
	const bool GetIsDamageKnockback() { return isDamageKnockBack; }
	const bool GetIsCrash() { return isCrash; }
	const int GetCrashBoundCount() { return crashBoundCount; }
	const bool GetIsDead() { return isDead; }
	const bool GetIsRoll() { return isRoll; }
	MoveSpeedPhase GetMoveSpeedPhase() { return moveSpeedPhase; }
	const Vector3& GetKnockbackVel() { return knockbackVel; }
	PlayerReticles* GetReticles() { return reticles.get(); }
	const bool GetIsChargeShotMode() { return isChargeShotMode; }
	StageClearModePhase GetStageClearModePhase() { return stageClearModePhase; }
	const bool GetIsStageClearModeCompletion() { return isStageClearModeCompletion; }

private: //�����o�֐�
	/// <summary>
	/// �s��
	/// </summary>
	void Action();

	/// <summary>
	/// ���@�̃W�F�b�g�G�t�F�N�g�Ǘ�
	/// </summary>
	void JetEffectManager();

	/// <summary>
	/// �ė����̍����G�t�F�N�g
	/// </summary>
	void CrashBlackSmoke();

	/// <summary>
	/// �_���[�W����炤
	/// </summary>
	void Damage();

	/// <summary>
	/// �_���[�W��Ԃ̏���
	/// </summary>
	void DamageMode();

	/// <summary>
	/// �_���[�W��Ԃ̃m�b�N�o�b�N�����Z�b�g
	/// </summary>
	/// <param name="subjectPos">�Ώۂ̍��W</param>
	void SetDamageKnockback(const Vector3& subjectPos);

	/// <summary>
	/// �_���[�W��Ԃ̃m�b�N�o�b�N
	/// </summary>
	void DamageKnockback();

	/// <summary>
	/// �_���[�W����������Ԃ̐F���`�J�`�J�����鏈��
	/// </summary>
	void DamageColorChange();

	/// <summary>
	/// �ė��J�n
	/// </summary>
	void CrashStart();

	/// <summary>
	/// �ė�
	/// </summary>
	void Crash();

	/// <summary>
	/// ��
	/// </summary>
	void Heal();

	/// <summary>
	/// ��]
	/// </summary>
	void Rotate();

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// ����
	/// </summary>
	void Roll();

	/// <summary>
	/// ����J�n
	/// </summary>
	void RollStart();

	/// <summary>
	/// ������
	/// </summary>
	void RollMode();

	/// <summary>
	/// ���x�ύX
	/// </summary>
	void SpeedChange();

	/// <summary>
	/// ���x�ύX�J�n
	/// </summary>
	/// <param name="isPushHighSpeedInput">�������͂�������</param>
	/// <param name="isPushSlowSpeedInput">�������͂�������</param>
	void SpeedChangeStart(bool isPushHighSpeedInput, bool isPushSlowSpeedInput);

	/// <summary>
	/// ���x�ύX��(����or����)�̏���
	/// </summary>
	/// <param name="isPushHighSpeedInput">�������͂�������</param>
	/// <param name="isPushSlowSpeedInput">�������͂�������</param>
	void SpeedChangeMode(bool isPushHighSpeedInput, bool isPushSlowSpeedInput);

	/// <summary>
	/// ���x�ύX�I�����Z�b�g
	/// </summary>
	void SetSpeedChangeModeEnd();

	/// <summary>
	/// ���x�ύX�I��(���̑��x�ɖ߂��Ă���)�̏���
	/// </summary>
	void SpeedChangeModeEnd();

	/// <summary>
	/// �ړ����x�𑬂�����Ƃ��̏���
	/// </summary>
	void SpeedChangeHighSpeed();

	/// <summary>
	/// �ړ����x��x������Ƃ��̏���
	/// </summary>
	void SpeedChangeSlowSpeed();

	/// <summary>
	/// �ړ����x��ʏ�ɖ߂��Ƃ��̏���
	/// </summary>
	void SpeedChangeNormalSpeed();

	/// <summary>
	/// �ʏ�ړ����̃u���[�̋����ɖ߂��Ƃ��̏���
	/// </summary>
	void SpeedChangeNormalBlur();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// �U���{�^���������Ă���Ƃ��̏���
	/// </summary>
	void PushAttackButton();

	/// <summary>
	/// �U���{�^���𗣂����Ƃ��̏���
	/// </summary>
	void ReleaseAttackButton();

	/// <summary>
	/// �e���ˍ��W���X�V
	/// </summary>
	void UpdateBulletShotPos();

	/// <summary>
	/// ���i�e����
	/// </summary>
	void ShotStraightBullet();

	/// <summary>
	/// �z�[�~���O�e����
	/// </summary>
	void ShotHomingBullet();

	/// <summary>
	/// �X�e�[�W�N���A��̉��ړ�
	/// </summary>
	void StageClearSideMove();

	/// <summary>
	/// �X�e�[�W�N���A��̋A��
	/// </summary>
	void StageClearReturn();
	
	/// <summary>
	/// �X�e�[�W�N���A��̏㏸
	/// </summary>
	void StageClearUp();

	/// <summary>
	/// �X�e�[�W�N���A��̒�~
	/// </summary>
	void StageClearStay();

	/// <summary>
	/// �X�e�[�W�N���A��̃u�[�X�g
	/// </summary>
	void StageClearBoost();

private: //�ÓI�����o�ϐ�
	//�Q�[���V�[��
	static GameScene* gameScene;
	//���@�e�̃��f��
	static ObjModel* bulletModel;
	//�z�[�~���O�e�̑傫��
	static const float homingBulletSize;
	//���@�̊���W
	static const Vector3 basePos;
	//���@�̉�]���E
	static const Vector2 rotLimit;
	//���@�̈ړ����E
	static const Vector2 moveLimitMax;
	static const Vector2 moveLimitMin;
	//�_���[�W��Ԃ̐F
	static const XMFLOAT4 damageColor;
	//���@�̈ړ��̊�̑���
	static const float moveBaseSpeed;
	//���@�̃m�b�N�o�b�N�̊�̑���
	static const float knockbackBaseSpeed;
	//���x�ύX�ő�Q�[�W��
	static const float maxSpeedChangeGauge;
	//�ʏ�ړ����̃u���[�̋���
	static const float normalSpeedBlurStrength;
	//�X�e�[�W�N���A��s���J��
	static void (Player::* stageClearActionFuncTable[])();

private: //�����o�ϐ�
	//�ő�̗�
	int maxHP = 0;
	//�̗�
	int HP = 0;
	//HPUI
	std::unique_ptr<PlayerHPUI> hpUI;
	//�_���[�W���o
	std::unique_ptr<PlayerDamageEffect> damageEffect;
	//�_���[�W�t���O
	bool isDamage = false;
	//�_���[�W��ԃ^�C�}�[
	int32_t damageTimer = 0;
	//�_���[�W�m�b�N�o�b�N��Ԃ�
	bool isDamageKnockBack = false;
	//�m�b�N�o�b�N����
	Vector3 knockbackVec;
	//�m�b�N�o�b�N���x
	Vector3 knockbackVel;
	//�_���[�W�F��
	bool isDamageColor = false;
	//�ė�����
	bool isCrash = false;
	//�����p�^�C�}�[
	int32_t blackSmokeTimer = 0;
	//�ė��o�E���h��
	int crashBoundCount = 0;
	//�ė����x
	Vector3 crashVel = { 0, 0, 0.2f };
	//���S�t���O
	bool isDead = false;
	//�ً}��𒆂�
	bool isRoll = false;
	//�ً}���p�^�C�}�[
	int32_t rollTimer = 0;
	//�ً}����J�n����Z���p�x
	float rollStartRot = 0;
	//�ً}����I������Z���p�x
	float rollEndRot = 0;
	//���x�ύX�Q�[�W
	float speedChangeGauge = maxSpeedChangeGauge;
	//���x�ύX�Q�[�WUI
	std::unique_ptr<PlayerSpeedChangeUI> speedChangeUI;
	//���x�ύX�J�n�\��
	bool isSpeedChangeStartPossible = true;
	//���x�ύX����
	bool isSpeedChange = false;
	//�ړ����x
	MoveSpeedPhase moveSpeedPhase = MoveSpeedPhase::NormalSpeed;
	//���x�ύX�p�^�C�}�[
	int32_t speedChangeTimer = 0;
	//���e�B�N��
	std::unique_ptr<PlayerReticles> reticles;
	//�e���ˍ��W
	Vector3 bulletShotPos;
	//���i�e�̔��ˑҋ@����
	bool isStraightShotWait = false;
	//���i�e�̔��ˑҋ@�^�C�}�[
	int32_t straightShotWaitTimer = 0;
	//�`���[�W�V���b�g����
	bool isChargeShotMode = false;
	//�`���[�W��������
	int32_t chargeTimer = 0;
	//z��������]���E��]��
	bool isRotZRight = true;
	//z��������]�p
	float swayZ = 0.0f;
	//�X�e�[�W�N���A��̓��������邩
	bool isStageClearMode = false;
	//�X�e�[�W�N���A��s��
	StageClearModePhase stageClearModePhase = StageClearModePhase::SideMove;
	//�X�e�[�W�N���A��Ɏg�p����^�C�}�[
	int32_t stageClearModeTimer = 0;
	//�X�e�[�W�N���A�ړ��������E��
	bool isStageClearMoveRight = true;
	//�X�e�[�W�N���A�ړ����x
	Vector3 stageClearMoveVelocity;
	//�X�e�[�W�N���A���p�x
	Vector3 stageClearRota;
	//�X�e�[�W�N���A��Ɏg�p����J�����z�[�~���O�p���W
	Vector3 stageClearCameraPos;
	//�J�����z�[�~���O�p���W
	Vector3 cameraHomingPos;
	//�X�e�[�W�N���A��̍s��������������
	bool isStageClearModeCompletion = false;
};
