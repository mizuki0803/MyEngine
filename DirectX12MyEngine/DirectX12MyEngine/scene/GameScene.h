#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"
#include "ParticleManager.h"
#include "LightGroup.h"
#include "Camera.h"
#include "CollisionPrimitive.h"
#include "Collision.h"

/// <summary>
/// �Q�[���V�[��
/// </summary>
class GameScene :public BaseScene
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

private: //�����o�ϐ�
	//�J����
	Camera* camera = nullptr;

	//���C�g
	LightGroup* lightGroup = nullptr;
	float ambientColor0[3] = { 1,1,1 };
	// �������������l
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	float pointLightPos[3] = { 0,0,0 };
	float pointLightColor[3] = { 1,1,1 };
	float pointLightAtten[3] = { 0.3f, 0.1f, 0.1f };

	float spotLightDir[3] = { 0, -1, 0 };
	float spotLightPos[3] = { 0, 5, 0 };
	float spotLightColor[3] = { 1, 1, 1 };
	float spotLightAtten[3] = { 0.0f, 0.0f, 0.0f };
	float spotLightfactorAngleCos[2] = { 20.0f, 30.0f };

	float circleShadowDir[3] = { 0, -1, 0 };
	float circleShadowAtten[3] = { 0.5f, 0.6f, 0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.5f };

	//�p�[�e�B�N��
	ParticleManager* particleMan = nullptr;

	//�X�v���C�g
	Sprite* sprite = nullptr;
	Sprite* sprite2 = nullptr;

	//���f���f�[�^
	Model* modelMan = nullptr;
	Model* modelSkydome = nullptr;
	Model* modelGround = nullptr;
	Model* modelSphere = nullptr;
	Model* modelFighter = nullptr;

	//3D�I�u�W�F�N�g
	Object3d* objMan = nullptr;
	Object3d* objGround = nullptr;
	Object3d* objSkydome = nullptr;
	Object3d* objSphere = nullptr;

	//�����蔻�� ��
	Sphere sphere;
	//�����蔻�� ����
	Plane plane;
	//�����蔻�� �O�p�`
	Triangle triangle;
	//�����蔻�� ���C
	Ray ray;

	//���̑��ϐ�
	float soundVol = 0.1f;
	char str[100];
};