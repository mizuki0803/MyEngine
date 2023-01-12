#pragma once
#include "ObjObject3d.h"

/// <summary>
/// �X�e�[�W01�o���V�[���p���@
/// </summary>
class Stage01SortiePlayer : public ObjObject3d
{
public:
	//�X�e�[�W01�o���s���t�F�[�Y
	enum class Stage01SortieActionPhase {
		Advance,	//�O�i
		Boost,		//�u�[�X�g
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�X�e�[�W01�o���V�[���p���@</returns>
	static Stage01SortiePlayer* Create(ObjModel* model, const Vector3& startPosition);

	//getter
	static const float GetAdvanceSpeed() { return Stage01SortiePlayer::advanceSpeed; }

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
	const Vector3& GetJetPos() { return jetPos; }
	Stage01SortieActionPhase GetSortieActionPhase() { return sortieActionPhase; }
	bool GetIsSortieEnd() { return isSortieEnd; }

private: //�����o�֐�
	/// <summary>
	/// �W�F�b�g���ˍ��W���X�V
	/// </summary>
	void UpdateJetPos();

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
	static void (Stage01SortiePlayer::* sortieActionFuncTable[])();
	//�ʏ�O�i�X�s�[�h
	static const float advanceSpeed;

private: //�����o�ϐ�
	//�W�F�b�g���ˍ��W
	Vector3 jetPos = {};
	//�o���s��
	Stage01SortieActionPhase sortieActionPhase = Stage01SortieActionPhase::Advance;
	//�o���s�����I��������
	bool isSortieEnd = false;
	//�u�[�X�g�o�ߎ��ԃ^�C�}�[
	int32_t boostTimer = 0;
	//�p�[�e�B�N���W�F�b�g�̑傫���ύX�p
	int32_t particleJetSizePhaseNum = 0;
};
