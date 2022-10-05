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

	/// <summary>
	/// ���@�̃W�F�b�g���ˉ��o
	/// </summary>
	/// <param name="matWorld">���@���[���h�s��</param>
	void PlayerJet(const XMMATRIX& matWorld);

	/// <summary>
	/// �`���[�W�V���b�g���o
	/// </summary>
	/// <param name="position">�p�[�e�B�N���������W</param>
	/// <param name="size">�傫��</param>
	void ChargeShot(const Vector3& position, const float size);

	/// <summary>
	/// �`���[�W�V���b�g���S���o
	/// </summary>
	/// <param name="position">�p�[�e�B�N���������W</param>
	void ChargeShotDead(const Vector3& position);

	/// <summary>
	/// �S�폜
	/// </summary>
	void AllDelete();

private: //�����o�ϐ�
	//�p�[�e�B�N���}�l�[�W���[
	std::unique_ptr<ParticleManager> particleManager;
	//�p�[�e�B�N���}�l�[�W���[
	std::unique_ptr<ParticleManager> particleA;
};
