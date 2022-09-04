#pragma once
#include "BossMainBody.h"
#include "BossAvatar.h"
#include "BossHPBar.h"
#include "BossHPFrame.h"
#include "BossBehaviorTree.h"

//���@�N���X��O���錾
class Player;

/// <summary>
/// �{�X
/// </summary>
class Boss
{
public:
	//�s���t�F�[�Y
	enum class Phase {
		Fall,	//�~��
		Attack,	//�U��
		Wait,	//�ҋ@
		Dead,	//���S
	};

	//�s���t�F�[�Y
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
	static void SetPlayer(Player* player) { Boss::player = player; }

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
	/// UI�`��
	/// </summary>
	void DrawUI();

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�(�{��)
	/// </summary>
	/// <param name="bossAvatars">�Փ˂������g</param>
	/// <param name="damageNum">�_���[�W��</param>
	void OnCollisionMainBody(const int damageNum);

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�(���g)
	/// </summary>
	/// <param name="avatars">�Փ˂������g</param>
	/// <param name="damageNum">�_���[�W��</param>
	void OnCollisionAvatar(BossAvatar* avatar, const int damageNum);

	/// <summary>
	/// �~�����
	/// </summary>
	bool FallMode();

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
	/// �U�����
	/// </summary>
	bool AttackModeCount();

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
	/// ���S���
	/// </summary>
	bool DeadMode();

	//getter
	const Phase& GetPhase() { return this->phase; }
	BossMainBody* GetMainBody() { return mainBody.get(); }
	const std::list<std::unique_ptr<BossAvatar>>& GetAvatars() { return avatars; };
	const bool GetIsDead() { return isDead; }

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


private: //�ÓI�����o�ϐ�
	//�v���C���[���@
	static Player* player;
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
	Phase phase = Phase::Fall;
	//�~����Ԏ��ԃ^�C�}�[
	int32_t fallModeTimer = 0;
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
	//�̗�
	int HP = 0;
	//HP�o�[
	std::unique_ptr<BossHPBar> hpBar;
	//HP�o�[�t���[��
	std::unique_ptr<BossHPFrame> hpFrame;
	//���S�t���O
	bool isDead = false;
};
