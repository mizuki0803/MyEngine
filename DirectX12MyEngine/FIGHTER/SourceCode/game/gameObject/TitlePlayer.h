#pragma once
#include "ObjObject3d.h"
#include "PlayerVaporEffect.h"

/// <summary>
/// �^�C�g���V�[���p���@
/// </summary>
class TitlePlayer : public ObjObject3d
{
public:
	//�^�C�g���o���s���t�F�[�Y
	enum class SortieModePhase {
		Stay,		//��~
		Boost,		//�u�[�X�g
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�^�C�g���V�[���p���@</returns>
	static TitlePlayer* Create(ObjModel* model, const Vector3& startPosition);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(ObjModel* model, const Vector3& startPosition);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	//getter
	bool GetIsSortie() { return isSortie; }
	const Vector3& GetJetPos() { return jetPos; }

	//setter
	void SetIsSortie(bool isSortie) { this->isSortie = isSortie; }

private: //�����o�֐�
	/// <summary>
	/// �o��
	/// </summary>
	void Sortie();

	/// <summary>
	/// �o����Ԃ̒�~�s��
	/// </summary>
	void SortieStay();

	/// <summary>
	/// �o����Ԃ̃u�[�X�g�s��
	/// </summary>
	void SortieBoost();

	/// <summary>
	/// �W�F�b�g���ˍ��W���X�V
	/// </summary>
	void UpdateJetPos();

	/// <summary>
	/// �����̍��W���X�V
	/// </summary>
	void UpdateWingPos();

private: //�ÓI�����o�ϐ�
	//�o����ԍs���J��
	static void (TitlePlayer::* sortieActionFuncTable[])();

private: //�����o�ϐ�
	//�o�����邩
	bool isSortie = false;
	//�o���p���ԃ^�C�}�[
	int32_t sortieTimer = 0;
	//�o����Ԃ̍s��
	SortieModePhase sortieModePhase = SortieModePhase::Stay;
	//�W�F�b�g���ˍ��W
	Vector3 jetPos = {};
	//��s�@�_���o
	std::unique_ptr<PlayerVaporEffect> vaporEffect;
};
