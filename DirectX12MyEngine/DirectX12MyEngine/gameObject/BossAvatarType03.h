#pragma once
#include "BossAvatar.h"

/// <summary>
/// �{�X(���g�F�^�C�v03)
/// </summary>
class BossAvatarType03 : public BossAvatar
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�{�X(���g�F�^�C�v03)</returns>
	static BossAvatarType03* Create(ObjModel* model, ObjObject3d* parent, const Vector3& position);

public: //�����o�֐�
	/// <summary>
	/// �������p
	/// </summary>
	void Otamesi() override;
	void Otamesi2() override;
	void Otamesi3() override;
	void Otamesi4() override;
};
