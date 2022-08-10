#pragma once
#include "BossBody.h"
#include "BossHead.h"

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

	//�`��
	void Draw();

private: //�����o�֐�
	/// <summary>
	/// �s��
	/// </summary>
	void Action();

protected: //�����o�ϐ�
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
	int HP = 1;
	//���S�t���O
	bool isDead = false;
};
