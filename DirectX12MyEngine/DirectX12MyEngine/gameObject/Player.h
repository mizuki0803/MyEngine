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


private: //�����o�֐�
	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

private:
	//���@�e
	std::list<std::unique_ptr<PlayerBullet>> playerBullets;
};
