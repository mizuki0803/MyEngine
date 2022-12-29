#pragma once
#include "ObjObject3d.h"

/// <summary>
/// �X�e�[�W02�o���V�[���p���@
/// </summary>
class Stage02SortiePlayer : public ObjObject3d
{
public:
	//�X�e�[�W02�o���s���t�F�[�Y
	enum class Stage02SortieActionPhase {
		Advance,	//�O�i
		Boost,		//�u�[�X�g
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�X�e�[�W02�o���V�[���p���@</returns>
	static Stage02SortiePlayer* Create(ObjModel* model, const Vector3& startPosition);

	//getter
	static const float GetAdvanceSpeed() { return Stage02SortiePlayer::advanceSpeed; }

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
	Stage02SortieActionPhase GetSortieActionPhase() { return sortieActionPhase; }
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
	static void (Stage02SortiePlayer::* sortieActionFuncTable[])();
	//�ʏ�O�i�X�s�[�h
	static const float advanceSpeed;

private: //�����o�ϐ�
	//�o���s��
	Stage02SortieActionPhase sortieActionPhase = Stage02SortieActionPhase::Advance;
	//�o���s�����I��������
	bool isSortieEnd = false;
	//�u�[�X�g�o�ߎ��ԃ^�C�}�[
	int32_t boostTimer = 0;
	//�p�[�e�B�N���W�F�b�g�̑傫���ύX�p
	int32_t particleJetSizePhaseNum = 0;
};
