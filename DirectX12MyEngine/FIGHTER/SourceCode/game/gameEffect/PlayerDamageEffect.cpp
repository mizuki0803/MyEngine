#include "PlayerDamageEffect.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"
#include "WindowApp.h"

PlayerDamageEffect* PlayerDamageEffect::Create()
{
	//���@�_���[�W���o�̃C���X�^���X�𐶐�
	PlayerDamageEffect* playerDamageEffect = new PlayerDamageEffect();
	if (playerDamageEffect == nullptr) {
		return nullptr;
	}

	// ������
	if (!playerDamageEffect->Initialize()) {
		delete playerDamageEffect;
		assert(0);
		return nullptr;
	}

	return playerDamageEffect;
}

bool PlayerDamageEffect::Initialize()
{
	//���o�p�X�v���C�g����
	effectSprite.reset(Sprite::Create(SpriteTextureLoader::White, { 0, 0 }));
	effectSprite->SetSize({ WindowApp::window_width, WindowApp::window_height });
	effectSprite->SetTexSize({ 1, 1 });
	effectSprite->SetColor({ 1, 0, 0, 0 });

    return true;
}

void PlayerDamageEffect::Update()
{
	//�_���[�W���o
	DamageEffect();

	//���o�p�X�v���C�g�X�V
	effectSprite->Update();
}

void PlayerDamageEffect::Draw()
{
	//�`���ԂłȂ���Δ�����
	if (!isDraw) { return; }

	//���o�p�X�v���C�g�`��
	effectSprite->Draw();
}

void PlayerDamageEffect::DamageEffectStart(const int maxHP, const int HP)
{
	//���݂�HP�̊������Z�o
	const float ratioHP = (float)HP / (float)maxHP;
	//�����ɂ���ĕω�����F�̔Z���͈̔�
	const float ratioDeepRange = 0.4f;
	//�x�[�X�ƂȂ�F�̔Z��(���̐����Ɋ����ŎZ�o�����Z�������Z����)
	const float baseDeepNum = 0.15f;
	//�F�̔Z�����Z�b�g(�c��HP�����Ȃ��قǐF���Z���Ȃ�)
	const float deepNum = ratioDeepRange * (1 - ratioHP) + baseDeepNum;
	colorDeepNum = deepNum;

	//�F���Z�b�g
	DirectX::XMFLOAT4 color = effectSprite->GetColor();
	color.w = colorDeepNum;
	effectSprite->SetColor(color);

	//�^�C�}�[��������
	drawTimer = 0;

	//�`���Ԃɂ���
	isDraw = true;
}

void PlayerDamageEffect::DamageEffect()
{
	//�`���ԂłȂ���Δ�����
	if (!isDraw) { return; }

	//�F�𔖂����鎞��
	const float returnTime = 50;
	//�^�C�}�[�X�V
	drawTimer++;
	const float time = drawTimer / returnTime;

	//�F�𔖂����Ă���
	DirectX::XMFLOAT4 color = effectSprite->GetColor();
	color.w = Easing::LerpFloat(colorDeepNum, 0, time);
	effectSprite->SetColor(color);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (drawTimer >= returnTime) {
		//�`��I��
		isDraw = false;
	}
}
