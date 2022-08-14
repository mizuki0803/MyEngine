#pragma once
#include "BossBody.h"
#include "BossHead.h"
#include "BossHPBar.h"
#include "BossHPFrame.h"
#include "BossBehaviorTree.h"


/// <summary>
/// �{�X
/// </summary>
class Boss
{
public:
	//�s���t�F�[�Y
	enum class Phase {
		Fall,	//�~��
		Stay,	//��~
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�{�X</returns>
	static Boss* Create(ObjModel* bodyModel, ObjModel* headModel, const Vector3& position);

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
	/// �������p
	/// </summary>
	/// <returns></returns>
	static bool Otamesi();

	/// <summary>
	/// �������p
	/// </summary>
	/// <returns></returns>
	static bool Otamesi2();

private: //�����o�֐�
	/// <summary>
	/// �s��
	/// </summary>
	void Action();

private: //�ÓI�����o�ϐ�
	//�ő�̗�
	static const int maxHP = 100;

protected: //�����o�ϐ�
	//�{�X�̍s���J�ڃr�w�C�r�A�c���[
	std::unique_ptr<BossBehaviorTree> behaviorTree;
	//��
	std::unique_ptr<BossBody> bossBody;
	//��
	std::unique_ptr<BossHead> bossHead;
	//�s��
	Phase phase = Phase::Fall;
	//�������W
	Vector3 startPos;
	//�~�����鎞�ԃ^�C�}�[
	int32_t fallTimer = 0;
	//�̗�
	int HP = maxHP;
	//HP�o�[
	std::unique_ptr<BossHPBar> hpBar;
	//HP�o�[�t���[��
	std::unique_ptr<BossHPFrame> hpFrame;
	//���S�t���O
	bool isDead = false;
};
