#pragma once
#include "ObjObject3d.h"

/// <summary>
/// �{�X
/// </summary>
class Boss : public ObjObject3d
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
	/// <returns>�~���G</returns>
	static Boss* Create(ObjModel* model, const Vector3& position);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

private: //�����o�֐�
	/// <summary>
	/// �s��
	/// </summary>
	void Action();

protected: //�����o�ϐ�
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
