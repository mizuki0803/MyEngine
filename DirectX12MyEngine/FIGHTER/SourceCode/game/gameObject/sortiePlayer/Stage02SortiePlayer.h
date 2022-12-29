#pragma once
#include "ObjObject3d.h"
#include "PlayerBullet.h"

//�X�e�[�W�V�[���̑O���錾
class BaseStageScene;

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

	//setter
	static void SetStageScene(BaseStageScene* stageScene) { Stage02SortiePlayer::stageScene = stageScene; }
	static void SetBulletModel(ObjModel* model) { Stage02SortiePlayer::bulletModel = model; }

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
	Vector3 GetWorldPos();
	Stage02SortieActionPhase GetSortieActionPhase() { return sortieActionPhase; }
	bool GetIsSortieEnd() { return isSortieEnd; }

private: //�����o�֐�
	/// <summary>
	/// �e���ˍ��W���X�V
	/// </summary>
	void UpdateBulletShotPos();

	/// <summary>
	/// ���i�e����
	/// </summary>
	void ShotStraightBullet();

	/// <summary>
	/// �e����
	/// </summary>
	void Fire();

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
	//�X�e�[�W�V�[��
	static BaseStageScene* stageScene;
	//���@�e�̃��f��
	static ObjModel* bulletModel;
	//�ʏ�O�i�X�s�[�h
	static const float advanceSpeed;

private: //�����o�ϐ�
	//�o���s��
	Stage02SortieActionPhase sortieActionPhase = Stage02SortieActionPhase::Advance;
	//�e���ˍ��W
	Vector3 bulletShotPos;
	//�e���˃^�C�}�[
	int32_t fireTimer = 0;
	//�e���ˉ�
	int fireCount = 0;
	//�o���s�����I��������
	bool isSortieEnd = false;
	//�u�[�X�g�o�ߎ��ԃ^�C�}�[
	int32_t boostTimer = 0;
	//�p�[�e�B�N���W�F�b�g�̑傫���ύX�p
	int32_t particleJetSizePhaseNum = 0;
};
