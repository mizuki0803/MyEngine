#pragma once
#include "ObjObject3d.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>

/// <summary>
/// ���e�B�N����
/// </summary>
class Reticle : public ObjObject3d
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>���e�B�N��</returns>
	static Reticle* Create(ObjModel* model);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	//bool Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	//void Draw() override;

	//setter
	void SetFollowMatWorld(const XMMATRIX& followMatWorld) { this->followMatWorld = followMatWorld; }

	//getter
	Vector3 GetWorldPos();

private:
	XMMATRIX followMatWorld = {};
};

