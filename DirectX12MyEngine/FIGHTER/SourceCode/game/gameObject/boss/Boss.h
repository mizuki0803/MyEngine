#pragma once
#include "BossMainBody.h"
#include "BossAvatar.h"
#include "BossNameUI.h"
#include "BossHPUI.h"
#include "BossBehaviorTree.h"

//���@�N���X��O���錾
class BasePlayer;

/// <summary>
/// �{�X
/// </summary>
class Boss
{
public:
	//�s���t�F�[�Y
	enum class Phase {
		Appear,	//�o��
		Attack,	//�U��
		Wait,	//�ҋ@
		Dead,	//���S
	};

	//�U�����
	enum class AttackType {
		None,				//���ݒ�
		Tracking,			//�Ǐ]
		SuperGiantBullet,	//������e
		Rotate,				//��]����̔���
		AvatarBodyBlow,		//���g�̓�����
		AvatarGatling,		//���g�K�g�����O�C
		AvatarGiantBullet,	//���g����e
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�{�X</returns>
	static Boss* Create(const Vector3& position);

	//setter
	static void SetPlayer(BasePlayer* player) { Boss::player = player; }

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(const Vector3& position);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �e�p�������C�g���猩�����_�ł̕`��
	/// </summary>
	void DrawLightCameraView();

	/// <summary>
	/// ���ォ��̉e�p�������C�g���猩�����_�ł̕`��
	/// </summary>
	void DrawTopLightCameraView();

	/// <summary>
	/// UI�`��
	/// </summary>
	void DrawUI();

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�(�{��)
	/// </summary>
	/// <param name="bossAvatars">�Փ˂������g</param>
	/// <param name="damageNum">�_���[�W��</param>
	void OnCollisionMainBody(const int damageNum, const Vector3& collisionPos, const Vector3& subjectVel);

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�(���g)
	/// </summary>
	/// <param name="avatars">�Փ˂������g</param>
	/// <param name="damageNum">�_���[�W��</param>
	void OnCollisionAvatar(BossAvatar* avatar, const int damageNum, const Vector3& collisionPos, const Vector3& subjectVel);

	/// <summary>
	/// �o����
	/// </summary>
	bool AppearModeCount();

	/// <summary>
	/// �o��~��
	/// </summary>
	bool AppearFall();

	/// <summary>
	/// �U�����
	/// </summary>
	bool AttackModeCount();

	/// <summary>
	/// �e���ˍ��W���X�V
	/// </summary>
	bool UpdateBulletShotPos();

	/// <summary>
	/// �U�����e�ݒ���J�n���邩
	/// </summary>
	/// <returns></returns>
	bool AttackTypeSelectStart();

	/// <summary>
	/// �U�����e:�Ǐ]��ݒ肷�邩
	/// </summary>
	/// <returns></returns>
	bool AttackTypeTrackingSelect();

	/// <summary>
	/// �U�����e:������e��ݒ肷�邩
	/// </summary>
	/// <returns></returns>
	bool AttackTypeSuperGiantBulletSelect();

	/// <summary>
	/// �U�����e:��]��ݒ肷�邩
	/// </summary>
	/// <returns></returns>
	bool AttackTypeRotateSelect();

	/// <summary>
	/// �U�����e:���g�̓������ݒ肷�邩
	/// </summary>
	/// <returns></returns>
	bool AttackTypeAvatarBodyBlowSelect();

	/// <summary>
	/// �U�����e:���g�K�g�����O�C��ݒ肷�邩
	/// </summary>
	/// <returns></returns>
	bool AttackTypeAvatarGatlingSelect();

	/// <summary>
	/// �U�����e:���g����e��ݒ肷�邩
	/// </summary>
	/// <returns></returns>
	bool AttackTypeAvatarGiantBulletSelect();

	/// <summary>
	/// �U�����e;�Ǐ]
	/// </summary>
	/// <returns></returns>
	bool AttackTypeTracking();

	/// <summary>
	/// �U�����e:������e
	/// </summary>
	/// <returns></returns>
	bool AttackTypeSuperGiantBullet();

	/// <summary>
	/// �U�����e:��]
	/// </summary>
	/// <returns></returns>
	bool AttackTypeRotate();

	/// <summary>
	/// �U�����e:���g�̓�����
	/// </summary>
	/// <returns></returns>
	bool AttackTypeAvatarBodyBlow();

	/// <summary>
	/// �U�����e:���g�K�g�����O�C
	/// </summary>
	/// <returns></returns>
	bool AttackTypeAvatarGatling();

	/// <summary>
	/// �U�����e:���g����e
	/// </summary>
	/// <returns></returns>
	bool AttackTypeAvatarGiantBullet();

	/// <summary>
	/// �U����ԗp�̊p�x�ɖ{�̉�]
	/// </summary>
	bool AttackModeMainBodyRota();

	/// <summary>
	/// �U����ԗp�̊p�x�ɕ��g��]
	/// </summary>
	bool AttackModeAvatarRota();

	/// <summary>
	/// �ҋ@���
	/// </summary>
	bool WaitModeCount();

	/// <summary>
	/// ��ʒu�ɖ߂�
	/// </summary>
	/// <returns></returns>
	bool ReturnBasePosition();

	/// <summary>
	/// �ҋ@��ԗp�̊p�x�ɖ{�̉�]
	/// </summary>
	bool WaitModeMainBodyRota();

	/// <summary>
	/// �ҋ@��ԗp�̊p�x�ɕ��g��]
	/// </summary>
	bool WaitModeAvatarRota();

	/// <summary>
	/// ���S����
	/// </summary>
	bool DeadExplosion();

	/// <summary>
	/// ���S����
	/// </summary>
	bool DeadFall();

	//getter
	const Phase& GetPhase() { return this->phase; }
	BossMainBody* GetMainBody() { return mainBody.get(); }
	const std::list<std::unique_ptr<BossAvatar>>& GetAvatars() { return avatars; };
	const bool GetIsDead() { return isDead; }
	const bool GetIsDelete() { return isDelete; }

private: //�����o�֐�
	/// <summary>
	/// �_���[�W����炤
	/// </summary>
	/// <param name="damageNum">�_���[�W��</param>
	void Damage(const int damageNum);

	/// <summary>
	/// ���g���S�ł������`�F�b�N����
	/// </summary>
	void CheckAllAvatarDead();

	/// <summary>
	/// �U����Ԃ��I������̂ŕK�v�ȏ����Z�b�g
	/// ��ʒu�ɖ߂�Ƃ��̏o�����W���L�^����A�e���ˏ�ԉ���
	/// </summary>
	void AttackEnd();

	/// <summary>
	/// ��ʒu�ɖ߂�Ƃ��̏o�����W���L�^����
	/// </summary>
	void SetReturnBasePosition();

private: //�ÓI�����o�ϐ�
	//�v���C���[���@
	static BasePlayer* player;
	//�o���Ԏ���
	static const float appearModeTime;
	//�U����Ԏ���
	static const float attackModeTime;
	//�ҋ@��Ԏ���
	static const float waitModeTime;
	//���[�h�`�F���W��]�ɗv���鎞��
	static const float changeModeTime;
	//��ʒu�ɖ߂邽�߂ɗv���鎞��
	static const float returnBasePositionTime;

private: //�����o�ϐ�
	//�{�X�̍s���J�ڃr�w�C�r�A�c���[
	std::unique_ptr<BossBehaviorTree> behaviorTree;
	//�{��
	std::unique_ptr<BossMainBody> mainBody;
	//���g
	std::list<std::unique_ptr<BossAvatar>> avatars;
	//�{�̂��U�������Ԃ�
	bool isMainBodyAttackMode = false;
	//�s��
	Phase phase = Phase::Appear;
	//�o���Ԏ��ԃ^�C�}�[
	int32_t appearModeTimer = 0;
	//�U�����e
	AttackType attackType = AttackType::None;
	//1�O�ɔ��������U�����e
	AttackType preAttackType = AttackType::None;
	//�U����Ԏ��ԃ^�C�}�[
	int32_t attackModeTimer = 0;
	//�ҋ@��Ԏ��ԃ^�C�}�[
	int32_t waitModeTimer = 0;
	//���S��Ԏ��ԃ^�C�}�[
	int32_t deadModeTimer = 0;
	//�������o�����^�C�}�[
	int32_t explosionTimer = 0;
	//����������
	int32_t explosionCount = 0;
	//�{�X���\��UI
	std::unique_ptr<BossNameUI> bossNameUI;
	//HPUI
	std::unique_ptr<BossHPUI> hpUI;
	//�̗�
	int HP = 0;
	//���S�t���O
	bool isDead = false;
	//�폜�t���O
	bool isDelete = false;
};
