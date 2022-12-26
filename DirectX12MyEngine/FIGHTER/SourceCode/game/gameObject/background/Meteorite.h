#pragma once
#include "ObjObject3d.h"
#include "BaseGameCamera.h"

/// <summary>
/// 覐�(�j��s�w�i�p)
/// </summary>
class Meteorite : public ObjObject3d
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="rotation">��]�p</param>
	/// <param name="size">�T�C�Y</param>
	/// <param name="velocity">�ړ����x</param>
	/// <param name="rotSpeed">��]���x</param>
	/// <returns>覐�(�j��s�w�i�p)</returns>
	static Meteorite* Create(const Vector3& position, const Vector3& rotation, float size, const Vector3& velocity, const Vector3& rotSpeed);

	//setter
	static void SetMeteoriteModel(ObjModel* model) { Meteorite::meteoriteModel = model; };
	static void SetGameCamera(BaseGameCamera* gameCamera) { Meteorite::gameCamera = gameCamera; }

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	//getter
	bool GetIsDelete() { return isDelete; }

private: //�����o�֐�
	/// <summary>
	/// ��ʎ�O�܂ōs������폜���鏈��
	/// </summary>
	void FrontOfScreenDelete();

private: //�ÓI�����o�ϐ�
	//覐�(�j��s�w�i�p)���f��
	static ObjModel* meteoriteModel;
	//�Q�[���J����
	static BaseGameCamera* gameCamera;

private: //�����o�ϐ�
	//�ړ����x
	Vector3 velocity = {};
	//��]���x
	Vector3 rotSpeed = {};
	//�폜�t���O
	bool isDelete = false;
};
