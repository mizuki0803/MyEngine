#pragma once
#include "NumberSprite.h"
#include <memory>
#include <array>

/// <summary>
/// �X�e�[�W�I�����UI
/// </summary>
class StageSelectUI
{
public:
	//�s���t�F�[�Y
	enum class ActionPhase {
		Stay,		//��~
		InScreen,	//��ɏオ��
		Show,		//�\��
		OutScreen,	//���ɉ�����
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�X�e�[�W�I�����UI</returns>
	static StageSelectUI* Create();

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
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
	/// �e�L�X�g����ʓ��ɓ������J�n
	/// </summary>
	/// <param name="stageNum">�\������X�e�[�W�ԍ�</param>
	void TextInScreenStart(const int stageNum);

	/// <summary>
	/// �e�L�X�g����ʊO�֓������J�n
	/// </summary>
	void TextOutScreenStart();

	//getter
	bool GetIsShow() { return isShow; }

private: //�����o�֐�
	/// <summary>
	/// �e�L�X�g��ҋ@������
	/// </summary>
	void TextStay();

	/// <summary>
	/// �e�L�X�g����ʓ��ɓ������J�n
	/// </summary>
	void TextInScreen();

	/// <summary>
	/// �e�L�X�g����ʊO�֓�����
	/// </summary>
	void TextOutScreen();

private: //�ÓI�����o�ϐ�
	//�s���J��
	static void (StageSelectUI::* actionFuncTable[])();
	//��ʓ�Y���W
	static const float inScreenPosY;
	//��ʊOY���W
	static const float outScreenPosY;
	//�����̌���
	static const int highScoreDigit = 3;
	//�g�̒��S�Ɛ�����Y���W�̋���
	static const float frameNumberDistanceY;
	//�g�̒��S�ƃ����N���_����Y���W�̋���
	static const float frameRankMedalDistanceY;

private: //�����o�ϐ�
	//�s��
	ActionPhase phase = ActionPhase::Stay;
	//�f�����X�v���C�g
	std::unique_ptr<Sprite> planetNameSprite;
	//�n�C�X�R�A�g�X�v���C�g
	std::unique_ptr<Sprite> highScoreFrameSprite;
	//�����X�v���C�g�z��
	std::array<std::unique_ptr<NumberSprite>, highScoreDigit> numberSprites;
	//�����N���_���X�v���C�g
	std::unique_ptr<Sprite> rankMedalSprite;
	//��������X�v���C�g
	std::unique_ptr<Sprite> howToPlaySprite;
	//�`�悷��X�e�[�WUI
	int stageNum = 0;
	//�n�C�X�R�A��\�����邩
	bool isHighScoreShow = false;
	//�ړ����ԃ^�C�}�[
	int32_t moveTimer = 0;
	//��ʊO�Ɉړ��J�n������W
	Vector2 moveOutScreenStartPos;
	//�\�����Ă��邩
	bool isShow = false;
};