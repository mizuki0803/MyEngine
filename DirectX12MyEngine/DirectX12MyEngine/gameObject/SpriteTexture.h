#pragma once

class SpriteTexture
{
public: //�e�N�X�`����
	enum TextureName {
		DebugFont,			//�f�o�b�O�p�t�H���g
		Number,				//����
		Reticle,			//���e�B�N��
		PlayerHPGaugeIn,	//���@HP�o�[
		PlayerHPGaugeOut,	//���@HP�o�[�t���[��
		BossHPGaugeIn,		//�{�XHP�o�[
		BossHPGaugeOut,		//�{�XHP�o�[�t���[��
	};


public: //�ÓI�����o�֐�
	/// <summary>
	/// �e�N�X�`���S�ǂݍ���
	/// </summary>
	static void TextureLoad();
};