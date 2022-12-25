#pragma once
#include "ObjObject3d.h"
#include "Player.h"
#include "GameCamera.h"

/// <summary>
/// �F���o�G�t�F�N�g
/// </summary>
class SpaceDustEffect : public ObjObject3d
{
public: // �ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�F���o�G�t�F�N�g</returns>
	static SpaceDustEffect* Create(const Vector3& position, const Vector3& rotation);

	//setter
	static void SetSpaceDustModel(ObjModel* model) { SpaceDustEffect::spaceDustModel = model; };
	static void SetPlayer(Player* player) { SpaceDustEffect::player = player; }
	static void SetGameCamera(GameCamera* gameCamera) { SpaceDustEffect::gameCamera = gameCamera; }
	static void SetIsScrollMode(const bool isScrollMode) { SpaceDustEffect::isScrollMode = isScrollMode; }

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	//getter
	bool GetIsDelete() const { return isDelete; }

private: //�����o�֐�
	/// <summary>
	/// ��ʎ�O�܂ōs������폜���鏈��
	/// </summary>
	void FrontOfScreenDelete();

protected: //�ÓI�����o�ϐ�
	//�o���f��
	static ObjModel* spaceDustModel;
	//���@
	static Player* player;
	//�Q�[���J����
	static GameCamera* gameCamera;
	//1���̑傫��
	static const float spaceDustSize;
	//�F���o�G�t�F�N�g���X�N���[����Ԃ�
	static bool isScrollMode;

protected: //�����o�ϐ�
	//�폜�t���O
	bool isDelete = false;
};