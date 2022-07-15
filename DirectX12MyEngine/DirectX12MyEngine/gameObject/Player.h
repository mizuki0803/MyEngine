#pragma once
#include "ObjObject3d.h"
#include "PlayerBullet.h"
#include "Reticle3D.h"
#include "Reticle2D.h"
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
	/// UI�`��
	/// </summary>
	void DrawUI();

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
	//���e�B�N��3D(���W�v�Z�p)
	std::unique_ptr<Reticle3D> reticle3d;
	//���e�B�N��2D(�\���p)
	std::unique_ptr<Reticle2D> reticle2d;
};
