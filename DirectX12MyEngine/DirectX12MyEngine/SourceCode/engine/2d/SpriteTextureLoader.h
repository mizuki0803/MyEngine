#pragma once

/// <summary>
/// �X�v���C�g�e�N�X�`���ǂݍ��݃N���X
/// </summary>
class SpriteTextureLoader
{
public: //�e�N�X�`����
	enum TextureName {
		DebugFont,			//�f�o�b�O�p�t�H���g
		White,				//�^����
		Number,				//����
		TitleLogo,			//�^�C�g�����S
		BButton,			//B�{�^��
		HowToPlayCharge,	//�V�ѕ��u�`���[�W�v
		Reticle,			//���e�B�N��
		PlayerHPGaugeIn,	//���@HP�o�[
		PlayerHPGaugeOut,	//���@HP�o�[�t���[��
		BossHPGaugeIn,		//�{�XHP�o�[
		BossHPGaugeOut,		//�{�XHP�o�[�t���[��
		HitPlusText,		//HIT+�e�L�X�g
		HitPlusNumber,		//HIT+����
		StageClearText,		//�X�e�[�W�N���A�e�L�X�g(�U������)
		DefeatNumText,		//���j���̃e�L�X�g�Ƙg
	};


public: //�ÓI�����o�֐�
	/// <summary>
	/// �e�N�X�`���S�ǂݍ���
	/// </summary>
	static void TextureLoad();
};