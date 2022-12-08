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
	/// <param name="playerSpeedPhase">���@�ړ����x�t�F�[�Y</param>
	void PlayerJet(const XMMATRIX& playerMatWorld, const int playerSpeedPhase = 0);

	/// <summary>
	/// ���@�̎��S�W�F�b�g�������ˉ��o
	/// </summary>
	/// <param name="matWorld">���@���[���h�s��</param>
	void PlayerBlackSmokeJet(const XMMATRIX& playerMatWorld);

	/// <summary>
	/// �V���b�g���S���o
	/// </summary>
	/// <param name="position">�p�[�e�B�N���������W</param>
	void ShotDead(const Vector3& position, const float size);

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
	void ChargeShotDead(const Vector3& position, const float size);

	/// <summary>
	/// �{�X�`���[�W���o
	/// </summary>
	/// <param name="position">�p�[�e�B�N���������W</param>
	/// <param name="size">�傫��</param>
	void BossCharge(const Vector3& position, const float size);

	/// <summary>
	/// �{�X�`���[�W�V���b�g���o
	/// </summary>
	/// <param name="position">�p�[�e�B�N���������W</param>
	/// <param name="size">�傫��</param>
	void BossChargeShot(const Vector3& position, const float size);

	/// <summary>
	/// �{�X�`���[�W�V���b�g���S���o
	/// </summary>
	/// <param name="position">�p�[�e�B�N���������W</param>
	void BossChargeShotDead(const Vector3& position, const float size);

	/// <summary>
	/// �������o
	/// </summary>
	/// <param name="position">�p�[�e�B�N���������W</param>
	void Explosion(const Vector3& position, const float size = 1, const int time = 10);

	/// <summary>
	/// �{�X���S��p�������o
	/// </summary>
	/// <param name="position">�p�[�e�B�N���������W</param>
	void BossDeadExplosion(const Vector3& position);

	/// <summary>
	/// �������o
	/// </summary>
	/// <param name="position">�p�[�e�B�N���������W</param>
	void BlackSmoke(const Vector3& position, const float size);

	/// <summary>
	/// �S�폜
	/// </summary>
	void AllDelete();

private: //�����o�֐�
	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	void LoadTexture();

private: //�����o�ϐ�
	//�p�[�e�B�N���}�l�[�W���[(��)
	std::unique_ptr<ParticleManager> circleParticle;
	//�p�[�e�B�N���}�l�[�W���[(����)
	std::unique_ptr<ParticleManager> explosionParticle;
	//�p�[�e�B�N���}�l�[�W���[(����)
	std::unique_ptr<ParticleManager> blackSmokeParticle;
};
