#pragma once
#include "Vector3.h"
#include <functional>
#include <vector>

/// <summary>
/// ���@�ً̋}����G�t�F�N�g�Ǘ�
/// </summary>
class PlayerRollingEffect
{
public:
	//�����p�t�F�[�Y
	enum class CreatePhase {
		Phase01, //�t�F�[�Y01
		Phase02, //�t�F�[�Y02
	};

	//�G�t�F�N�g1�ɕK�v�ȏ��
	struct EffectParam {
		//��������
		int life = 0;
		//�Ǐ]�Ώۂ���̋���
		Vector3 localPos = {};
		//��]�p
		float rotation = 0;
		//�傫��
		float size = 0;
		//�F
		DirectX::XMFLOAT4 color = {};
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="getTargetPos">�Ǐ]�Ώۂ̍��W�擾�֐�</param>
	/// <param name="isRollRight">�E��]��</param>
	/// <returns>���@�ً̋}����G�t�F�N�g�Ǘ�</returns>
	static PlayerRollingEffect* Create(std::function<Vector3()> getTargetPos, const bool isRollRight);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	//getter
	const bool GetIsDelete() { return isDelete; }

private: //�����o�֐�
	/// <summary>
	/// �����t�F�[�Y01�̍s��
	/// </summary>
	void Phase01Create();

	/// <summary>
	/// �����t�F�[�Y02�̍s��
	/// </summary>
	void Phase02Create();

	/// <summary>
	/// �G�t�F�N�g�̃p�����[�^�\���̂����ɂ�����������
	/// </summary>
	void CreateEffects(const std::vector<EffectParam>& effectParams);

private: //�ÓI�����o�ϐ�
	//�����t�F�[�Y�֐�
	static void (PlayerRollingEffect::* createPhaseFuncTable[])();

private: //�����o�ϐ�
	//�Ǐ]�Ώۂ̍��W�擾�֐�
	std::function<Vector3()> getTargetPos;
	//�����t�F�[�Y
	CreatePhase createPhase = CreatePhase::Phase01;
	//�����p�^�C�}�[
	int32_t createTimer = 0;
	//�E��]��
	bool isRollRight = true;
	//�폜�t���O
	bool isDelete = false;
};