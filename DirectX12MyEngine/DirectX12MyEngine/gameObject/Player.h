#pragma once
#include "ObjObject3d.h"

/// <summary>
/// ���@
/// </summary>
class Player : public ObjObject3d
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>���@</returns>
	static Player* Create(ObjModel* model);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;


private: //�����o�֐�
	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

private:
};