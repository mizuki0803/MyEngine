#pragma once
#include "ObjObject3d.h"
#include "PlayerBullet.h"
#include "Reticle.h"
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

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	void OnCollision();

	//getter
	Vector3 GetWorldPos();
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return playerBullets; };


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
	//���e�B�N��
	std::unique_ptr<Reticle> reticle;
};
