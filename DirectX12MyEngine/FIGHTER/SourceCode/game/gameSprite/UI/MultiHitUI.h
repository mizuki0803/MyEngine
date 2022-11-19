#pragma once
#include "NumberSprite.h"
#include "Camera.h"
#include <vector>
#include <memory>

/// <summary>
/// �������jUI
/// </summary>
class MultiHitUI
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�������jUI</returns>
	static MultiHitUI* Create(const Vector3& position, Camera* camera, const int enemyHitNum);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(const Vector3& position, Camera* camera, const int enemyHitNum);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//getter
	bool GetIsDead() { return isDead; }

private: //�����o�֐�
	/// <summary>
	/// �X�N���[�����W�擾
	/// </summary>
	Vector2 GetScreenPos(const Vector3& position, Camera* camera);

	/// <summary>
	/// �e�L�X�g����ɓ�����
	/// </summary>
	void TextMove();

private: //�ÓI�����o�ϐ�
	//��������
	static const int32_t lifeTime = 180;

private: //�����o�ϐ�
	//�J����
	Camera* camera = nullptr;
	//�uHIT�v�e�L�X�g�X�v���C�g
	std::unique_ptr<Sprite> hitTextSprite;
	//�����X�v���C�g
	std::vector<std::unique_ptr<NumberSprite>> numberSprites;
	//���[���h���W
	Vector3 hitWorldPos;
	//�擾�p���j��
	int hitNum = 0;
	//�擾�p���j���̌���
	int hitNumDigit = 0;
	//�����^�C�}�[
	int32_t deadTimer = 0;
	//���S�t���O
	bool isDead = false;
};