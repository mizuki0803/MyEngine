#pragma once
#include "PlayerNearReticle.h"
#include "PlayerFarReticle.h"

/// <summary>
/// ���@�t����2�̃��e�B�N������N���X
/// </summary>
class PlayerReticles
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns> ���@�t����2�̃��e�B�N��</returns>
	static PlayerReticles* Create();

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="matWorld">���[���h�s��</param>
	/// <param name="matView">�r���[�s��</param>
	/// <param name="matProjection">�v���W�F�N�V�����s��</param>
	void Update(const XMMATRIX& matWorld, const XMMATRIX& matView, const XMMATRIX& matProjection);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �`���[�W��Ԃ��J�n����
	/// </summary>
	void ChargeModeStart();

	/// <summary>
	/// �`���[�W��Ԃ��I������
	/// </summary>
	void ChargeModeEnd();

	/// <summary>
	/// �G���b�N�I��
	/// </summary>
	/// <param name="enemy">���b�N�I���Ώۂ̓G</param>
	void LockonEnemy(Enemy* enemy);

	/// <summary>
	/// �G���b�N�I������
	/// </summary>
	void UnlockonEnemy();

	//getter
	const Vector3 GetNearReticleWorldPos() { return nearReticle->GetReticle3D()->GetWorldPos(); }
	const Vector2& GetFarReticleScreenPosition() { return farReticle->GetReticle2D()->GetPosition(); }
	const float GetFarReticleSize() { return farReticle->GetReticle2D()->GetSize().x; }
	const bool GetIsChargeMode() { return farReticle->GetReticle2D()->GetIsChargeMode(); }
	const bool GetIsLockon() { return farReticle->GetReticle2D()->GetIsLockon(); }
	Enemy* GetLockonEnemy() { return farReticle->GetReticle2D()->GetLockonEnemy(); }

private:
	//���e�B�N��(��)
	std::unique_ptr<PlayerNearReticle> nearReticle;
	//���e�B�N��(��)
	std::unique_ptr<PlayerFarReticle> farReticle;
};