#pragma once
#include "ObjObject3d.h"
#include "PlayerBullet.h"
#include "PlayerReticles.h"
#include "PlayerHPUI.h"
#include "PlayerSpeedChangeUI.h"
#include "PlayerDamageEffect.h"
#include "PlayerVaporEffect.h"
#include "PlayerRollingEffect.h"
#include <memory>
#include <list>

//�X�e�[�W�V�[���̑O���錾
class BaseStageScene;

/// <summary>
/// ���@���
/// </summary>
class BasePlayer : public ObjObject3d
{
public:
	//�ړ����x���
	enum class MoveSpeedPhase {
		NormalSpeed,	//�ʏ�ړ�
		HighSpeed,		//�����ړ�
		SlowSpeed,		//�x���ړ�
		ReturnNormalSpeed, //�ʏ�ړ��ɖ߂����
	};

public: //�ÓI�����o�֐�

	//getter
	static const Vector2& GetRotLimit() { return rotLimit; }
	static const float GetMoveBaseSpeed() { return moveBaseSpeed; }
	static const float GetKnockbackBaseSpeed() { return knockbackBaseSpeed; }

	//setter
	static void SetStageScene(BaseStageScene* stageScene) { BasePlayer::stageScene = stageScene; }
	static void SetBulletModel(ObjModel* model) { BasePlayer::bulletModel = model; }

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="startHP">�J�n��HP</param>
	/// <param name="maxHP">�ő�HP</param>
	/// <param name="isVaporCreate">��s�@�_���o�𐶐����邩</param>
	/// <returns>����</returns>
	virtual bool Initialize(ObjModel* model, const int startHP, const int maxHP, const bool isVaporCreate);

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() override;

	/// <summary>
	/// UI�X�V
	/// </summary>
	virtual void UpdateUI();

	/// <summary>
	/// UI�`��
	/// </summary>
	virtual void DrawUI();

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�(�G�Ȃǃ_���[�W����炤)
	/// </summary>
	/// <param name="subjectPos">�Փ˂�������̍��W</param>
	virtual void OnCollisionDamage(const Vector3& subjectPos);

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�(�A�C�e���Ȃǉ񕜂���)
	/// </summary>
	virtual void OnCollisionHeal();

	//getter
	const XMMATRIX& GetMatWorld() { return matWorld; }
	const int GetHP() { return HP; }
	const Vector3& GetBasePos() { return basePos; }
	const Vector3& GetJetPos() { return jetPos; }
	const bool GetIsDamageKnockback() { return isDamageKnockBack; }
	const bool GetIsCrash() { return isCrash; }
	const bool GetIsDead() { return isDead; }
	const bool GetIsRoll() { return isRoll; }
	PlayerVaporEffect* GetVaporEffect() { return vaporEffect.get(); }
	MoveSpeedPhase GetMoveSpeedPhase() { return moveSpeedPhase; }
	const Vector2& GetMoveLimitMax() { return moveLimitMax; }
	const Vector2& GetMoveLimitMin() { return moveLimitMin; }
	const Vector3& GetKnockbackVel() { return knockbackVel; }
	PlayerReticles* GetReticles() { return reticles.get(); }
	const bool GetIsChargeShotMode() { return isChargeShotMode; }
	const bool GetIsStageClearModeCompletion() { return isStageClearModeCompletion; }

protected: //�����o�֐�
	/// <summary>
	/// �W�F�b�g���ˍ��W���X�V
	/// </summary>
	void UpdateJetPos();

	/// <summary>
	/// �����̍��W���X�V
	/// </summary>
	void UpdateWingPos();

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
	virtual void CrashStart();

	/// <summary>
	/// �ė�
	/// </summary>
	virtual void Crash() = 0;

	/// <summary>
	/// ��
	/// </summary>
	void Heal();

	/// <summary>
	/// ��]
	/// </summary>
	virtual void Rotate();

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
	/// �X�e�[�W�N���A�s��
	/// </summary>
	virtual void StageClear() = 0;

protected: //�ÓI�����o�ϐ�
	//�X�e�[�W�V�[��
	static BaseStageScene* stageScene;
	//���@�e�̃��f��
	static ObjModel* bulletModel;
	//�z�[�~���O�e�̑傫��
	static const float homingBulletSize;
	//���@�̉�]���E
	static const Vector2 rotLimit;
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

protected: //�����o�ϐ�
	//�ő�̗�
	int maxHP = 0;
	//�̗�
	int HP = 0;
	//HPUI
	std::unique_ptr<PlayerHPUI> hpUI;
	//��̍��W
	Vector3 basePos = {};
	//�W�F�b�g���ˍ��W
	Vector3 jetPos = {};
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
	//���S�t���O
	bool isDead = false;
	//�ً}��𒆂�
	bool isRoll = false;
	//�ً}���p���o
	std::list<std::unique_ptr<PlayerRollingEffect>> rollingEffects;
	//�ً}���p�^�C�}�[
	int32_t rollTimer = 0;
	//�ً}����J�n����Z���p�x
	float rollStartRot = 0;
	//�ً}����I������Z���p�x
	float rollEndRot = 0;
	//��s�@�_���o���o����
	bool isVaporCreate = false;
	//��s�@�_���o
	std::unique_ptr<PlayerVaporEffect> vaporEffect;
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
	//���@�̈ړ����E
	Vector2 moveLimitMax = {};
	Vector2 moveLimitMin = {};
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
	//�X�e�[�W�N���A��̍s��������������
	bool isStageClearModeCompletion = false;
};
