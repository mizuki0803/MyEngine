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
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�{�X</returns>
	static Boss* Create(ObjModel* bodyModel, ObjModel* headModel, const Vector3& position);

	//setter
	static void SetPlayer(Player* player) { Boss::player = player; }

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(ObjModel* bodyModel, ObjModel* headModel, const Vector3& position);

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
	void OnCollisionAvator(BossAvatar* avatar, const int damageNum);

	/// <summary>
	/// �~��
	/// </summary>
	bool Fall();

	/// <summary>
	/// �U�����
	/// </summary>
	bool AttackMode();

	/// <summary>
	/// �U����ԗp�̊p�x�ɕύX
	/// </summary>
	bool ChangeRotAttackMode();

	/// <summary>
	/// �ҋ@���
	/// </summary>
	bool WaitMode();

	/// <summary>
	/// �ҋ@��ԗp�̊p�x�ɕύX
	/// </summary>
	bool ChangeRotWaitMode();

	//getter
	const Phase& GetPhase() { return this->phase; }
	BossMainBody* GetMainBody() { return mainBody.get(); }
	const std::list<std::unique_ptr<BossAvatar>>& GetAvators() { return avatars; };

private: //�����o�֐�
	/// <summary>
	/// �_���[�W����炤
	/// </summary>
	/// <param name="damageNum">�_���[�W��</param>
	void Damage(const int damageNum);

protected: //�ÓI�����o�ϐ�
	//�v���C���[���@
	static Player* player;
	//�U����Ԏ���
	static const float attackModeTime;
	//�ҋ@��Ԏ���
	static const float waitModeTime;

private: //�����o�ϐ�
	//�{�X�̍s���J�ڃr�w�C�r�A�c���[
	std::unique_ptr<BossBehaviorTree> behaviorTree;
	//�{��
	std::unique_ptr<BossMainBody> mainBody;
	//���g
	std::list<std::unique_ptr<BossAvatar>> avatars;
	//�s��
	Phase phase = Phase::Fall;
	//�~�����鎞�ԃ^�C�}�[
	int32_t fallTimer = 0;
	//�U����Ԏ��ԃ^�C�}�[
	int32_t attackModeTimer = 0;
	//�ҋ@��Ԏ��ԃ^�C�}�[
	int32_t waitModeTimer = 0;
	//�̗�
	int HP = 0;
	//HP�o�[
	std::unique_ptr<BossHPBar> hpBar;
	//HP�o�[�t���[��
	std::unique_ptr<BossHPFrame> hpFrame;
	//���S�t���O
	bool isDead = false;
};
