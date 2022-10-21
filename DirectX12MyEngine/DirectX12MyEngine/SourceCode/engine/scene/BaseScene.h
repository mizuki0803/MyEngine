#pragma once
#include <DirectXBase.h>

class SceneManager;

/// <summary>
/// ���V�[��
/// </summary>
class BaseScene
{
public:
	/// <summary>
	/// ���z�f�X�g���N�^
	/// </summary>
	virtual ~BaseScene() = default;

	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() = 0;

	//setter
	static void SetDirectXbase(DirectXBase* dxbase) { BaseScene::dxbase = dxbase; }

protected:
	static DirectXBase* dxbase;
};

