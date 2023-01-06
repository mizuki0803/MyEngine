#pragma once
#include "GalaxyBody.h"
#include "BossNameUI.h"
#include "BossHPUI.h"
#include "GalaxyBehaviorTree.h"

//���@�N���X��O���錾
class BasePlayer;

/// <summary>
/// �{�X �X�e�[�W2�{�X
/// </summary>
class Galaxy
{
public:
	//�s���t�F�[�Y
	enum class Phase {
		Appear,	//�o��
		Wait,	//�ҋ@
		Dead,	//���S
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>02</returns>
	static Galaxy* Create(const Vector3& position);

	//setter
	static void SetPlayer(BasePlayer* player) { Galaxy::player = player; }

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
	/// �Փˎ��R�[���o�b�N�֐�(����)
	/// </summary>
	/// <param name="damageNum">�_���[�W��</param>
	void OnCollisionBody(const int damageNum, const Vector3& collisionPos, const Vector3& subjectVel);

	/// <summary>
	/// �o����
	/// </summary>
	bool AppearModeCount();

	/// <summary>
	/// �o��~��
	/// </summary>
	bool AppearFall();

	/// <summary>
	/// �ҋ@���
	/// </summary>
	bool WaitMode();

	/// <summary>
	/// ���S����
	/// </summary>
	bool DeadExplosion();

	//getter
	const Phase& GetPhase() { return this->phase; }
	GalaxyBody* GetBody() { return body.get(); }
	const bool GetIsDead() { return isDead; }
	const bool GetIsDelete() { return isDelete; }

private: //�����o�֐�
	/// <summary>
	/// �_���[�W����炤
	/// </summary>
	/// <param name="damageNum">�_���[�W��</param>
	void Damage(const int damageNum);

private: //�ÓI�����o�ϐ�
	//�v���C���[���@
	static BasePlayer* player;
	//�o���Ԏ���
	static const float appearModeTime;
	//�ҋ@��Ԏ���
	static const float waitModeTime;

private: //�����o�ϐ�
	//����
	std::unique_ptr<GalaxyBody> body;
	//�X�e�[�W2�{�X�̍s���J�ڃr�w�C�r�A�c���[
	std::unique_ptr<GalaxyBehaviorTree> behaviorTree;
	//�s��
	Phase phase = Phase::Appear;
	//�o���Ԏ��ԃ^�C�}�[
	int32_t appearModeTimer = 0;
	//�ҋ@��Ԏ��ԃ^�C�}�[
	int32_t waitModeTimer = 0;
	//���S��Ԏ��ԃ^�C�}�[
	int32_t deadModeTimer = 0;
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
