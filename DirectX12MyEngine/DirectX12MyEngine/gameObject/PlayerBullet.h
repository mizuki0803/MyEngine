#pragma once
#include "ObjObject3d.h"

/// <summary>
/// ���@�e
/// </summary>
class PlayerBullet : public ObjObject3d
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>���@�e</returns>
	static PlayerBullet* Create(ObjModel* model, XMFLOAT3 position);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;
};