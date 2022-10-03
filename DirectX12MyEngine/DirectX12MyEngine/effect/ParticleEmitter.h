#pragma once
#include "ParticleManager.h"

/// <summary>
/// �p�[�e�B�N���G�~�b�^�[
/// </summary>
class ParticleEmitter final
{
private: //�V���O���g����
	//�R���X�g���N�^���B��
	ParticleEmitter() = default;
	//�f�X�g���N�^���B��
	~ParticleEmitter() = default;
public:
	//�R�s�[�R���X�g���N�^�𖳌���
	ParticleEmitter(const ParticleEmitter& particleEmitter) = delete;
	//������Z�q�𖳌���
	void operator = (const ParticleEmitter& particleEmitter) = delete;

public: //�����o�֐�
	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns>�f�o�b�O�e�L�X�g</returns>
	static ParticleEmitter* GetInstance();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// ��ĕ`��
	/// </summary>
	void DrawAll();

	/// <summary>
	/// �������p���o
	/// </summary>
	void DemoEffect();

	/// <summary>
	/// �������p���o
	/// </summary>
	void DemoEffect2();

private: //�����o�ϐ�
	//�p�[�e�B�N���}�l�[�W���[
	std::unique_ptr<ParticleManager> particleManager;
};
