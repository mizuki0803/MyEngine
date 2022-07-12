#pragma once
#include "ObjObject3d.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>

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

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	//getter
	Vector3 GetWorldPos();


private: //�����o�֐�
	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// ��]
	/// </summary>
	void Rotate();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

private:
	//���@�e
	std::list<std::unique_ptr<PlayerBullet>> playerBullets;
};
