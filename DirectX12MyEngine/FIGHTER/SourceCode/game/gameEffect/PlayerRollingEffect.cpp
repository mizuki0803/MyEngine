#include "PlayerRollingEffect.h"
#include "ParticleEmitter.h"

void (PlayerRollingEffect::* PlayerRollingEffect::createPhaseFuncTable[])() = {
	&PlayerRollingEffect::Phase01Create,
	&PlayerRollingEffect::Phase02Create,
};

PlayerRollingEffect* PlayerRollingEffect::Create(std::function<Vector3()> getTargetPos, const bool isRollRight)
{
	//���@�ً̋}����G�t�F�N�g�Ǘ��̃C���X�^���X�𐶐�
	PlayerRollingEffect* playerRollingEffect = new PlayerRollingEffect();
	if (playerRollingEffect == nullptr) {
		return nullptr;
	}

	//�Ǐ]�Ώۂ̍��W�擾�֐����Z�b�g
	playerRollingEffect->getTargetPos = getTargetPos;
	//��]�������Z�b�g
	playerRollingEffect->isRollRight = isRollRight;

	return playerRollingEffect;
}

void PlayerRollingEffect::Update()
{
	//�����t�F�[�Y
	(this->*createPhaseFuncTable[static_cast<size_t>(createPhase)])();
}

void PlayerRollingEffect::Phase01Create()
{
	//���̃t�F�[�Y�Ő�������G�t�F�N�g�̏����Z�b�g
	std::vector<EffectParam> effectParams = {
		{20, {2, 2, -1}, 125, 5, { 0.4f, 0.5f, 0.5f, 1.0f }},
		{20, {2, 2, -1}, 135, 5, { 0.4f, 0.5f, 0.5f, 1.0f }},
		{20, {-2, -2, -1}, 305, 5, { 0.4f, 0.5f, 0.5f, 1.0f }},
		{20, {-2, -2, -1}, 315, 5, { 0.4f, 0.5f, 0.5f, 1.0f }},

		{25, {2, 2, -1}, 125, 7.5f, { 0.1f, 0.5f, 0.5f, 1.0f }},
		{25, {2, 2, -1}, 135, 7.5f, { 0.1f, 0.5f, 0.5f, 1.0f }},
		{25, {-2, -2, -1}, 305, 7.5f, { 0.1f, 0.5f, 0.5f, 1.0f }},
		{25, {-2, -2, -1}, 315, 7.5f, { 0.1f, 0.5f, 0.5f, 1.0f }},

		{30, {2, 2, -1}, 125, 10, { 0.05f, 0.25f, 0.25f, 1.0f }},
		{30, {2, 2, -1}, 135, 10, { 0.05f, 0.25f, 0.25f, 1.0f }},
		{30, {-2, -2, -1}, 305, 10, { 0.05f, 0.25f, 0.25f, 1.0f }},
		{30, {-2, -2, -1}, 315, 10, { 0.05f, 0.25f, 0.25f, 1.0f }},
	};
	//�p�����[�^�����ɃG�t�F�N�g�𐶐�
	CreateEffects(effectParams);

	//���̃t�F�[�Y��
	createPhase = CreatePhase::Phase02;
}

void PlayerRollingEffect::Phase02Create()
{
	//���̃t�F�[�Y�Ő����܂ł����鎞��
	const int phaseTime = 5;
	//�^�C�}�[�X�V
	createTimer++;

	//�^�C�}�[���w�肵�����ԂɈȉ��Ȃ甲����
	if (createTimer < phaseTime) { return; }

	//���̃t�F�[�Y�Ő�������G�t�F�N�g�̏����Z�b�g
	std::vector<EffectParam> effectParams = {
		{20, {2, -2, -1}, 35, 5, { 0.4f, 0.5f, 0.5f, 1.0f }},
		{20, {2, -2, -1}, 45, 5, { 0.4f, 0.5f, 0.5f, 1.0f }},
		{20, {-2, 2, -1}, 215, 5, { 0.4f, 0.5f, 0.5f, 1.0f }},
		{20, {-2, 2, -1}, 225, 5, { 0.4f, 0.5f, 0.5f, 1.0f }},

		{25, {2, -2, -1}, 35, 7.5f, { 0.1f, 0.5f, 0.5f, 1.0f }},
		{25, {2, -2, -1}, 45, 7.5f, { 0.1f, 0.5f, 0.5f, 1.0f }},
		{25, {-2, 2, -1}, 215, 7.5f, { 0.1f, 0.5f, 0.5f, 1.0f }},
		{25, {-2, 2, -1}, 225, 7.5f, { 0.1f, 0.5f, 0.5f, 1.0f }},

		{30, {2, -2, -1}, 35, 10, { 0.05f, 0.25f, 0.25f, 1.0f }},
		{30, {2, -2, -1}, 45, 10, { 0.05f, 0.25f, 0.25f, 1.0f }},
		{30, {-2, 2, -1}, 215, 10, { 0.05f, 0.25f, 0.25f, 1.0f }},
		{30, {-2, 2, -1}, 225, 10, { 0.05f, 0.25f, 0.25f, 1.0f }},
	};
	
	//�p�����[�^�����ɃG�t�F�N�g�𐶐�
	CreateEffects(effectParams);

	//�폜��Ԃɂ���
	isDelete = true;
}

void PlayerRollingEffect::CreateEffects(const std::vector<EffectParam>& effectParams)
{
	//�����̗v�f�����A�G�t�F�N�g�𐶐�����
	for (int i = 0; i < (int)effectParams.size(); i++) {
		ParticleEmitter::GetInstance()->PlayerRolling(effectParams[i].life, getTargetPos,
			effectParams[i].localPos, effectParams[i].rotation, effectParams[i].size, effectParams[i].color);
	}
}
