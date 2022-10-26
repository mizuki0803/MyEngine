#pragma once
#include "ObjObject3d.h"

/// <summary>
/// �o���V�[���p���@
/// </summary>
class SortiePlayer : public ObjObject3d
{
public:
	//�o���s���t�F�[�Y
	enum class SortieActionPhase {
		Advance,	//�O�i
		Boost,		//�u�[�X�g
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�o���V�[���p���@</returns>
	static SortiePlayer* Create(ObjModel* model, const Vector3& startPosition);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �u�[�X�g��Ԃɂ���
	/// </summary>
	void BoostStart();

	//getter
	SortieActionPhase GetSortieActionPhase() { return sortieActionPhase; }
	bool GetIsSortieEnd() { return isSortieEnd; }

private: //�����o�֐�
	/// <summary>
	/// �o���O�i
	/// </summary>
	void SortieAdvance();

	/// <summary>
	/// �o���u�[�X�g
	/// </summary>
	void SortieBoost();

private: //�ÓI�����o�ϐ�
	//�o���s���J��
	static void (SortiePlayer::* sortieActionFuncTable[])();

private: //�����o�ϐ�
	//�o���s��
	SortieActionPhase sortieActionPhase = SortieActionPhase::Advance;
	//�o���s�����I��������
	bool isSortieEnd = false;
	//�u�[�X�g�o�ߎ��ԃ^�C�}�[
	int32_t boostTimer = 0;
};
