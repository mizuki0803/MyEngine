#pragma once
#include "Sprite.h"
#include <memory>

/// <summary>
/// �X�e�[�W����m�FUI
/// </summary>
class StageDecisionUI
{
public:
	//�s���t�F�[�Y
	enum class ActionPhase {
		Stay,	//��~
		Up,		//��ɏオ��
		Show,	//�\��
		Down,	//���ɉ�����
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�X�e�[�W����m�FUI</returns>
	static StageDecisionUI* Create();

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �e�L�X�g����ɓ������J�n
	/// </summary>
	/// <param name="stageNum">�X�e�[�W�ԍ�</param>
	void TextUpStart(int stageNum);

	/// <summary>
	/// �e�L�X�g�����ɓ������J�n
	/// </summary>
	void TextDownStart();

	/// <summary>
	/// �X�e�[�W���肷�邩��ύX�\
	/// </summary>
	/// <param name="isStageDecision">�X�e�[�W�����肷�邩</param>
	void ChangeStageDecision(bool isStageDecision);

	//getter
	ActionPhase GetActionPhase() { return phase; }
	bool GetIsStageDecision() { return isStageDecision; }

private: //�����o�֐�
	/// <summary>
	/// �e�L�X�g��ҋ@������
	/// </summary>
	void TextStay();

	/// <summary>
	/// �e�L�X�g����ɓ�����
	/// </summary>
	void TextUp();

	/// <summary>
	/// �e�L�X�g�����ɓ�����
	/// </summary>
	void TextDown();

private: //�ÓI�����o�ϐ�
	//�s���J��
	static void (StageDecisionUI::* actionFuncTable[])();
	//��~���W
	static const Vector2 stayPos;
	//��ʊO���W
	static const Vector2 outScreenPos;

private: //�����o�ϐ�
	//�s��
	ActionPhase phase = ActionPhase::Stay;
	//�X�e�[�W����m�F�X�v���C�g
	std::unique_ptr<Sprite> stageDecisionSprite;
	//�X�e�[�W�����肩
	bool isStageDecision = true;
	//�ړ����ԃ^�C�}�[
	int32_t moveTimer = 0;
	//��Ԃ�ύX�\��
	bool isCanChangeDecision = false;
	//�e�N�X�`���̍�����W�{��
	bool isSelectTitleStage = false;
};