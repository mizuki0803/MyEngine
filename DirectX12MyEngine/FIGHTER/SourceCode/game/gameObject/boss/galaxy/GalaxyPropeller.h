#pragma once
#include "ObjObject3d.h"

/// <summary>
/// �M�����N�V�[(�v���y��)
/// </summary>
class GalaxyPropeller : public ObjObject3d
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="parent">�e�I�u�W�F�N�g</param> 
	/// <param name="position">���W</param>
	/// <param name="rotation">������]�p</param>
	/// <param name="rotSpeed">��]���x</param>
	/// <param name="size">�傫��</param>
	/// <returns>�M�����N�V�[(�v���y��)</returns>
	static GalaxyPropeller* Create(ObjObject3d* parent, const Vector3& position, const Vector3& rotation, const float rotSpeed, const float size);

	//setter
	static void SetPropellerModel(ObjModel* model) { GalaxyPropeller::propellerModel = model; }

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="parent">�e�I�u�W�F�N�g</param> 
	/// <param name="position">���W</param>
	/// <param name="rotation">������]�p</param>
	/// <param name="rotSpeed">��]���x</param>
	/// <param name="size">�傫��</param>
	/// <returns>����</returns>
	bool Initialize(ObjObject3d* parent, const Vector3& position, const Vector3& rotation, const float rotSpeed, const float size);

	/// <summary>
	/// ������
	/// </summary>
	void Update() override;

	//setter
	void SetIsRotation(const float isRotation) { this->isRotation = isRotation; }

protected: //�����o�֐�
	/// <summary>
	/// ��]
	/// </summary>
	void Rotation();

protected: //�ÓI�����o�ϐ�
	//�v���y���̃��f��
	static ObjModel* propellerModel;
	//�_���[�W��Ԃ̐F
	static const XMFLOAT4 damageColor;

protected: //�����o�ϐ�
	//��]���邩
	bool isRotation = true;
	//��]���x
	float rotSpeed = 0;
	//�_���[�W�t���O
	bool isDamage = false;
	//�_���[�W��ԃ^�C�}�[
	int32_t damageTimer = 0;
	//�_���[�W�F��
	bool isDamageColor = false;
};