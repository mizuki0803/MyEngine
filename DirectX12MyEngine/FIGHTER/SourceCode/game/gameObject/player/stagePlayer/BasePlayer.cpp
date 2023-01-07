#include "BasePlayer.h"
#include "Input.h"
#include "Easing.h"
#include "BaseStageScene.h"
#include "SpriteTextureLoader.h"
#include "StraightBullet.h"
#include "HomingBullet.h"
#include "ParticleEmitter.h"
#include "GamePostEffect.h"

BaseStageScene* BasePlayer::stageScene = nullptr;
ObjModel* BasePlayer::bulletModel = nullptr;
const float BasePlayer::homingBulletSize = 2.5f;
const Vector2 BasePlayer::rotLimit = { 35.0f, 25.0f };
const XMFLOAT4 BasePlayer::damageColor = { 1, 0, 0, 1 };
const float BasePlayer::moveBaseSpeed = 0.16f;
const float BasePlayer::knockbackBaseSpeed = 0.25f;
const float BasePlayer::maxSpeedChangeGauge = 100.0f;
const float BasePlayer::normalSpeedBlurStrength = 0.03f;

bool BasePlayer::Initialize(ObjModel* model, const int startHP, const int maxHP, const bool isVaporCreate)
{
	//3D�I�u�W�F�N�g�̏�����
	if (!ObjObject3d::Initialize()) {
		return false;
	}

	//�������W���Z�b�g
	position = basePos;
	//�傫�����Z�b�g
	scale = { 1.5f, 1.5f, 1.5f };

	//���f�����Z�b�g
	assert(model);
	this->model = model;

	//���e�B�N���𐶐�
	reticles.reset(PlayerReticles::Create());

	//HP���Z�b�g
	HP = startHP;
	//�ő�HP���Z�b�g
	this->maxHP = maxHP;

	//��s�@�_���o�𐶐����邩�Z�b�g
	this->isVaporCreate = isVaporCreate;

	//HPUI����
	const Vector2 hpUIPosition = { 20, 20 };
	hpUI.reset(PlayerHPUI::Create(hpUIPosition, HP, maxHP));
	//���x�ύX�Q�[�WUI����
	const Vector2 speedChangeUIPosition = { 20, 70 };
	speedChangeUI.reset(PlayerSpeedChangeUI::Create(speedChangeUIPosition, maxSpeedChangeGauge));
	//�_���[�W���o����
	damageEffect.reset(PlayerDamageEffect::Create());

	//��s�@�_���o����
	if (isVaporCreate) { vaporEffect.reset(new PlayerVaporEffect()); }

	//�|�X�g�G�t�F�N�g�Ƀ��W�A���u���[��������
	if (!GamePostEffect::GetPostEffect()->GetIsRadialBlur()) {
		GamePostEffect::GetPostEffect()->SetRadialBlur(true);

		//�ʏ�ړ����̃u���[�̋������Z�b�g
		GamePostEffect::GetPostEffect()->SetRadialBlurStrength(normalSpeedBlurStrength);
	}

	return true;
}

void BasePlayer::Update()
{
	//�폜��Ԃً̋}������o���폜
	rollingEffects.remove_if([](std::unique_ptr<PlayerRollingEffect>& rollingEffect) {
		return rollingEffect->GetIsDelete();
		});
	//�ً}������o�X�V
	for (const std::unique_ptr<PlayerRollingEffect>& rollingEffect : rollingEffects) {
		rollingEffect->Update();
	}

	//���@�̃W�F�b�g���ˉ��o�p�p�[�e�B�N������
	JetEffectManager();

	//���S��ԂȂ甲����
	if (isDead) { return; }

	//�s��
	Action();

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();

	//�����̍��W�X�V
	if (isVaporCreate) { UpdateWingPos(); }

	//���e�B�N���X�V
	reticles->Update(matWorld, camera->GetMatView(), camera->GetMatProjection());
}

void BasePlayer::Draw()
{
	//���S��ԂȂ甲����
	if (isDead) { return; }

	//�I�u�W�F�N�g�`��
	ObjObject3d::Draw();
}

void BasePlayer::UpdateUI()
{
	//���S��ԂȂ甲����
	if (isDead) { return; }

	//HPUI�X�V
	hpUI->Update();
	//���x�ύX�Q�[�WUI�X�V
	speedChangeUI->Update();
	//�_���[�W���o�X�V
	damageEffect->Update();
}

void BasePlayer::DrawUI()
{
	//�ė���ԂłȂ��Ȃ�&&�X�e�[�W�N���A��ԂłȂ��Ȃ�
	if (!isCrash && !isStageClearMode) {
		//���e�B�N���`��
		reticles->Draw();

		//HPUI�`��
		hpUI->Draw();

		//���x�ύX�Q�[�WUI�`��
		speedChangeUI->Draw();
	}

	//�_���[�W���o�`��
	damageEffect->Draw();
}

void BasePlayer::OnCollisionDamage(const Vector3& subjectPos)
{
	//�_���[�W����炤
	Damage();

	//�m�b�N�o�b�N�����Z�b�g
	SetDamageKnockback(subjectPos);

	//�_���[�W���������̂�HP�o�[�̒�����ύX����
	hpUI->Damage(HP);

	//�_���[�W���o���J�n����
	damageEffect->DamageEffectStart(maxHP, HP);
}

void BasePlayer::OnCollisionHeal()
{
	//��
	Heal();

	//�񕜃A�C�e�����l�������̂�HPUI���񕜏�Ԃɂ���
	hpUI->ItemGet(HP);
}

void BasePlayer::UpdateWingPos()
{
	//���@�̒��S���W����̋���
	const Vector3 leftDistancePos = { -2.0f, -0.2f, -0.9f };
	const Vector3 rightDistancePos = { 2.0f, -0.2f, -0.9f };
	//�������W���擾
	const Vector3 leftWingPos = LocalTranslation(leftDistancePos, matWorld);
	//�E�����W���擾
	const Vector3 rightWingPos = LocalTranslation(rightDistancePos, matWorld);

	vaporEffect->Update(leftWingPos, rightWingPos);
}

void BasePlayer::Action()
{
	//�X�e�[�W�N���A���
	if (isStageClearMode) {
		//�X�e�[�W�N���A��s��
		StageClear();
	}
	//�X�e�[�W�N���A�ȊO�̏��
	else {
		//�ė����
		if (isCrash) {
			Crash();
		}
		//����ȊO
		else {
			//��]
			Rotate();

			//�ً}���
			Roll();

			//���x�ύX
			SpeedChange();

			//�_���[�W�m�b�N�o�b�N���
			if (isDamageKnockBack) {
				DamageKnockback();
			}
			//�_���[�W��ԂłȂ��Ȃ�ʏ�ړ�
			else {
				Move();
			}

			//�U��
			Attack();
		}
	}

	//�_���[�W���
	if (isDamage) {
		DamageMode();
	}
}

void BasePlayer::JetEffectManager()
{
	//�ė���ԈȊO�Ȃ�ʏ�W�F�b�g���o
	if (!isCrash) {
		ParticleEmitter::GetInstance()->PlayerJet(matWorld, (int)moveSpeedPhase);
	}
	//�ė���ԂȂ獕�����W�F�b�g�̑���ɏo��
	else {
		CrashBlackSmoke();
	}
}

void BasePlayer::CrashBlackSmoke()
{
	//�ė��J�n���Ă��獕�����o���܂ł̎���
	const int smokeStartTime = 30;
	//�����p�^�C�}�[�X�V
	blackSmokeTimer++;

	//�^�C�}�[���������o���܂ł̎��Ԉȉ��Ȃ甲����
	if (blackSmokeTimer < smokeStartTime) { return; }

	//���t���[���o���Ƒ����̂ŊԊu��ݒ�
	const int smokeInterval = 4;
	//�w�肵���Ԋu�ȊO�Ȃ甲����
	if (blackSmokeTimer % smokeInterval != 0) { return; }

	//�����p�[�e�B�N������
	ParticleEmitter::GetInstance()->PlayerBlackSmokeJet(matWorld);
}

void BasePlayer::Damage()
{
	//�̗͂����炷
	HP -= 10;

	//HP��0�ȉ��ɂȂ�����ė�������
	if (HP <= 0) {
		CrashStart();

		//HP��0�ȉ��ɂȂ�Ȃ�
		HP = 0;

		//�������o�p�p�[�e�B�N������
		const float explosionSize = 0.5f;
		const int explosionTime = 20;
		ParticleEmitter::GetInstance()->Explosion(position, explosionSize, explosionTime);
	}

	//�_���[�W��Ԃɂ���
	isDamage = true;
	//�_���[�W��ԃ^�C�}�[������
	damageTimer = 0;
	//�m�b�N�o�b�N��Ԃɂ���
	isDamageKnockBack = true;
	//�_���[�W��ԐF�ɕύX
	color = damageColor;
}

void BasePlayer::DamageMode()
{
	//�_���[�W��Ԃ̎���
	const int damageTime = 80;
	//�_���[�W��ԃ^�C�}�[���X�V
	damageTimer++;

	//�_���[�W�F�؂�ւ�
	DamageColorChange();

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (damageTimer >= damageTime) {
		//�_���[�W��Ԃ��I��
		isDamage = false;

		//�F�����ɖ߂��Ă���
		color = { 1, 1, 1, 1 };
	}
}

void BasePlayer::SetDamageKnockback(const Vector3& subjectPos)
{
	//�m�b�N�o�b�N������������߂�(���@�̃��[���h���W - �Ώۂ̃��[���h���W)
	knockbackVec = GetWorldPos() - subjectPos;
	//�x�N�g���𐳋K��
	knockbackVec.normalize();
}

void BasePlayer::DamageKnockback()
{
	//�m�b�N�o�b�N���鎞��
	const float knockbackTime = 35;
	//�w�肵�����Ԉȏ�Ȃ甲����
	if (damageTimer > knockbackTime) {
		//�m�b�N�o�b�N��ԂȂ�������Ă��甲����
		if (isDamageKnockBack) { isDamageKnockBack = false; }
		return;
	}

	const float time = damageTimer / knockbackTime;

	//���x���쐬(�m�b�N�o�b�N���x * ��̑��x * ���Ԃɂ�錸��)
	knockbackVel = knockbackVec * knockbackBaseSpeed * (1 - time);
	//Z�������ɂ͈ړ����Ȃ��悤�ɂ���
	knockbackVel.z = 0;

	//���@���m�b�N�o�b�N������
	position += knockbackVel;

	//�ړ����E����o�Ȃ��悤�ɂ���
	position.x = max(position.x, moveLimitMin.x);
	position.x = min(position.x, moveLimitMax.x);
	position.y = max(position.y, moveLimitMin.y);
	position.y = min(position.y, moveLimitMax.y);
}

void BasePlayer::DamageColorChange()
{
	//�_���[�W�F�؂�ւ��Ԋu����
	const int colorChangeInterval = 3;

	//�^�C�}�[���w�肵���Ԋu�ȊO�Ȃ甲����
	if (damageTimer % colorChangeInterval != 0) { return; }

	//�_���[�W�F��Ԃ�؂�ւ���
	if (isDamageColor) {
		isDamageColor = false;

		//�F�����ɖ߂�
		color = { 1, 1, 1, 1 };
	}
	else {
		isDamageColor = true;

		//�_���[�W�F�ɂ���
		color = damageColor;
	}
}

void BasePlayer::CrashStart()
{
	//�J�����Ǐ]������
	SetIsCameraFollow(false);
	//�J�����Ǐ]�����ɂ��A���[�J�����W�Ƀ��[���h���W����
	position = GetWorldPos();

	//�p�[�e�B�N���̑傫���𓝈ꂷ�邽�߁A�ړ��͂������Ȃ����ʏ�ړ���Ԃɂ��Ă���
	moveSpeedPhase = MoveSpeedPhase::NormalSpeed;

	//��s�@�_�̐������I������
	if (isVaporCreate) { vaporEffect->VaporEnd(); }

	//�ė���Ԃɂ���
	isCrash = true;
}

void BasePlayer::Heal()
{
	//�̗͂𑝂₷
	const int healNum = 10;
	HP += healNum;

	//HP�͍ő�HP�ȏ�ɂȂ�Ȃ�
	if (HP >= maxHP) {
		HP = maxHP;
	}
}

void BasePlayer::Rotate()
{
	Input* input = Input::GetInstance();

	//��]���x
	const float rotSpeed = 1.0f;
	//�p�x�C������x
	const float backBaseSpeed = rotSpeed / 1.5f;
	Vector3 rot = { 0, 0, 0 };


	//�ǂ��܂ŌX�����画����Ƃ邩
	const float stickNum = 200;
	//y����]
	{
		//�L�[���͂ŉ�]������
		if (input->PushKey(DIK_A) || input->PushKey(DIK_D)) {
			if (input->PushKey(DIK_A)) { rot.y -= rotSpeed; }
			else if (input->PushKey(DIK_D)) { rot.y += rotSpeed; }
		}
		//�p�b�h�X�e�B�b�NX���̔�������
		else if (input->TiltGamePadLStickX(stickNum) || input->TiltGamePadLStickX(-stickNum)) {
			//���@�̓X�e�B�b�N��|���������ɓ���
			const float padRota = input->GetPadLStickAngle();
			const float moveAngle = XMConvertToRadians(padRota);
			const float padStickIncline = input->GetPadLStickIncline().x;
			rot.y = rotSpeed * cosf(moveAngle) * fabsf(padStickIncline);
		}
		//�L�[���͂Ȃ�&�X�e�B�b�N��|���Ă��Ȃ��ꍇ
		else {
			//�p�x�C�����x�{��
			float backSpeedRatio = fabsf(rotation.y / (rotLimit.y * 2)) + 0.5f;
			//�p�x�C�����x
			const float backSpeed = backBaseSpeed * backSpeedRatio;
			//y����]�̌X�����C������
			const float rotMin = 0.5f;
			if (rotation.y > rotMin) {
				rot.y -= backSpeed;
			}
			else if (rotation.y < -rotMin) {
				rot.y += backSpeed;
			}
			else {
				rotation.y = 0;
			}
		}
	}
	//x����]
	{
		//�L�[���͂ŉ�]������
		if (input->PushKey(DIK_W) || input->PushKey(DIK_S)) {
			if (input->PushKey(DIK_W)) { rot.x -= rotSpeed; }
			if (input->PushKey(DIK_S)) { rot.x += rotSpeed; }
		}
		//�p�b�h�X�e�B�b�NY���̔�������
		else if (input->TiltGamePadLStickY(stickNum) || input->TiltGamePadLStickY(-stickNum)) {
			//���@�̓X�e�B�b�N��|���������ɓ���
			const float padRota = input->GetPadLStickAngle();
			const float moveAngle = XMConvertToRadians(padRota);
			const float padStickIncline = input->GetPadLStickIncline().y;
			rot.x = rotSpeed * sinf(moveAngle) * fabsf(padStickIncline);
		}
		//�L�[���͂Ȃ�&�X�e�B�b�N��|���Ă��Ȃ��ꍇ
		else {
			//�p�x�C�����x�{��
			float backSpeedRatio = fabsf(rotation.x / (rotLimit.x * 2)) + 0.5f;
			//�p�x�C�����x
			const float backSpeed = backBaseSpeed * backSpeedRatio;
			//x����]�̌X�����C������
			const float rotMin = 0.5f;
			if (rotation.x > rotMin) {
				rot.x -= backSpeed;
			}
			else if (rotation.x < -rotMin) {
				rot.x += backSpeed;
			}
			else {
				rotation.x = 0;
			}
		}
	}
	//z����]
	{
		//�������Ȃ��Ǝ₵���̂ł���炳���Ă���
		const float rotZSpeed = 0.04f;
		const float rotZLimit = 1.0f;
		//�E��]
		if (isRotZRight) {
			swayZ += rotZSpeed;
			if (swayZ >= rotZLimit) {
				isRotZRight = false;
			}
		}
		//����]
		else {
			swayZ -= rotZSpeed;
			if (swayZ <= -rotZLimit) {
				isRotZRight = true;
			}
		}

		rotation.z = -rotation.y + swayZ;
	}

	//��]�̍X�V
	rotation += rot;

	//�p�x�̌��E�l����͂ݏo���Ȃ�
	rotation.y = max(rotation.y, -rotLimit.y);
	rotation.y = min(rotation.y, +rotLimit.y);
	rotation.x = max(rotation.x, -rotLimit.x);
	rotation.x = min(rotation.x, +rotLimit.x);
}

void BasePlayer::Move()
{
	//���@���X���Ă���p�x�Ɉړ�������
	Vector3 velocity = { 0, 0, 0 };
	velocity.x = moveBaseSpeed * (rotation.y / rotLimit.y);
	velocity.y = moveBaseSpeed * -(rotation.x / rotLimit.x);
	position += velocity;

	//�ړ����E����o�Ȃ��悤�ɂ���
	position.x = max(position.x, moveLimitMin.x);
	position.x = min(position.x, moveLimitMax.x);
	position.y = max(position.y, moveLimitMin.y);
	position.y = min(position.y, moveLimitMax.y);
}

void BasePlayer::Roll()
{
	//�ً}������̓���
	if (isRoll) {
		RollMode();
	}
	//�ً}����\��
	else {
		//�J�n���邩����
		RollStart();
	}
}

void BasePlayer::RollStart()
{
	//�_���[�W��ԂȂ�ً}����͔����ł��Ȃ��Ŕ�����
	if (isDamage) { return; }

	Input* input = Input::GetInstance();
	//�ړ������̓��͂������������ً̋}�����͂����Ă��Ȃ���Δ�����
	bool isInputRightRoll = (input->TriggerKey(DIK_RSHIFT) || input->TriggerGamePadButton(Input::PAD_RB));
	bool isInputLeftRoll = (input->TriggerKey(DIK_LSHIFT) || input->TriggerGamePadButton(Input::PAD_LB));

	if (!(isInputRightRoll || isInputLeftRoll)) { return; }

	//�ً}�����Ԃɂ���
	isRoll = true;
	//�^�C�}�[��������
	rollTimer = 0;
	//�ً}����J�n����Z���p�x���L��
	rollStartRot = rotation.z;

	//�ً}����I������Z���p�x���Z�b�g
	const float rotAmount = 360; //��]��
	if (isInputRightRoll) { rollEndRot = -rotAmount; }		//�E��]
	else if (isInputLeftRoll) { rollEndRot = rotAmount; }	//����]

	//�ً}������o����
	std::unique_ptr<PlayerRollingEffect> newRollingEffect;
	std::function<Vector3()> getTergetPos = std::bind(&BasePlayer::GetWorldPos, this);
	newRollingEffect.reset(PlayerRollingEffect::Create(getTergetPos, true));
	rollingEffects.push_back(std::move(newRollingEffect));
}

void BasePlayer::RollMode()
{
	//�^�C�}�[���X�V
	const float rollTime = 45;
	rollTimer++;
	const float time = rollTimer / rollTime;

	//Z����]����
	const float endRot = rollEndRot - rotation.y + swayZ;
	rotation.z = Easing::OutBack(rollStartRot, endRot, time);

	//�^�C�}�[���w�肵�����ԂɂȂ�����ً}����I��
	if (rollTimer >= rollTime) {
		isRoll = false;
	}
}

void BasePlayer::SpeedChange()
{
	Input* input = Input::GetInstance();

	//��������
	const bool isPushHighSpeedInput = (input->PushGamePadButton(Input::PAD_X) || input->PushKey(DIK_X));
	//��������
	const bool isPushSlowSpeedInput = (input->PushGamePadButton(Input::PAD_A) || input->PushKey(DIK_Z));

	//���x�ύX�J�n�\��
	if (isSpeedChangeStartPossible) {
		//�J�n���邩����
		SpeedChangeStart(isPushHighSpeedInput, isPushSlowSpeedInput);
	}
	//���x�ύX�J�n�\�ł͂Ȃ��Ƃ�
	else {
		//���x�ύX���̂Ƃ�
		if (isSpeedChange) {
			//���x�ύX��(����or����)�̏���
			SpeedChangeMode(isPushHighSpeedInput, isPushSlowSpeedInput);
		}
		//���x�ύX���I���A���̑��x�ɖ߂�Ƃ�
		else {
			//�Q�[�W�����ɖ߂��Ă���
			SpeedChangeModeEnd();
		}

		//�Q�[�W�����Ȃ��悤�ɂ���
		speedChangeGauge = max(speedChangeGauge, 0);
		speedChangeGauge = min(speedChangeGauge, maxSpeedChangeGauge);
		//�Q�[�W�ύX����
		speedChangeUI->ChangeLength(speedChangeGauge);
	}

	//����or�����ǂ���ł��Ȃ��ꍇ�A���̈ʒu�ɖ߂�
	SpeedChangeNormalSpeed();
	//�u���[�̋�����ʏ�ړ����̋����ɖ߂��Ă���
	SpeedChangeNormalBlur();
}

void BasePlayer::SpeedChangeStart(bool isPushHighSpeedInput, bool isPushSlowSpeedInput)
{
	//����or�����̃L�[�{�^���̓��͂��Ȃ���Δ�����
	if (!(isPushHighSpeedInput || isPushSlowSpeedInput)) { return; }

	//������Ԃ��Z�b�g
	if (isPushHighSpeedInput) {
		moveSpeedPhase = MoveSpeedPhase::HighSpeed;

		//��s�@�_�̐������J�n����
		if (isVaporCreate) { vaporEffect->VaporStart(); }

		//�|�X�g�G�t�F�N�g�̃��W�A���u���[�����߂�
		if (GamePostEffect::GetPostEffect()->GetIsRadialBlur()) {
			const float highSpeedBlur = 0.4f;
			GamePostEffect::GetPostEffect()->SetRadialBlurStrength(highSpeedBlur);
		}
	}
	else if (isPushSlowSpeedInput) {
		moveSpeedPhase = MoveSpeedPhase::SlowSpeed;

		//�|�X�g�G�t�F�N�g�̃��W�A���u���[����߂�
		if (GamePostEffect::GetPostEffect()->GetIsRadialBlur()) {
			const float slowSpeedBlur = 0.005f;
			GamePostEffect::GetPostEffect()->SetRadialBlurStrength(slowSpeedBlur);
		}
	}

	//���x�ύX��Ԃɂ���
	isSpeedChange = true;
	//���x�ύX�J�n�\�ł͂Ȃ�����
	isSpeedChangeStartPossible = false;
	//���x�ύX�^�C�}�[��������
	speedChangeTimer = 0;
}

void BasePlayer::SpeedChangeMode(bool isPushHighSpeedInput, bool isPushSlowSpeedInput)
{
	//�^�C�}�[�X�V
	speedChangeTimer++;

	//�Q�[�W�����炷����
	const float gaugeDecSpeed = 1.3f;
	//������Ԃ̂Ƃ�
	if (moveSpeedPhase == MoveSpeedPhase::HighSpeed) {
		//�������͂𑱂��Ă���ꍇ�́A�Q�[�W�����炵������
		if (isPushHighSpeedInput) {
			speedChangeGauge -= gaugeDecSpeed;

			//��������̂őO�Ɉړ�������
			SpeedChangeHighSpeed();
		}
		//�������͂���߂��u�Ԃɑ��x�ύX���I��
		else {
			//���x�ύX�I��
			SetSpeedChangeModeEnd();
		}
	}
	//������Ԃ̂Ƃ�
	else if (moveSpeedPhase == MoveSpeedPhase::SlowSpeed) {
		//�������͂𑱂��Ă���ꍇ�́A�Q�[�W�����炵������
		if (isPushSlowSpeedInput) {
			speedChangeGauge -= gaugeDecSpeed;

			//��������̂Ō��Ɉړ�������
			SpeedChangeSlowSpeed();
		}
		//�������͂���߂��u�Ԃɑ��x�ύX���I��
		else {
			//���x�ύX�I��
			SetSpeedChangeModeEnd();
		}
	}

	//�Q�[�W���Ȃ��Ȃ�����
	if (!(speedChangeGauge <= 0)) { return; }

	//���x�ύX�I��
	SetSpeedChangeModeEnd();
}

void BasePlayer::SetSpeedChangeModeEnd()
{
	//���x�ύX���I��
	isSpeedChange = false;

	//��s�@�_�̐������I������
	if (isVaporCreate) { vaporEffect->VaporEnd(); }

	//�ړ����x��ʏ�ɖ߂���Ԃɂ���
	moveSpeedPhase = MoveSpeedPhase::ReturnNormalSpeed;
}

void BasePlayer::SpeedChangeModeEnd()
{
	//���炵���Q�[�W�𑝂₵�Ă���
	const float gaugeIncSpeed = 0.55f;
	speedChangeGauge += gaugeIncSpeed;

	//�Q�[�W���ő�܂ŗ��܂��Ă��Ȃ���Δ�����
	if (speedChangeGauge < maxSpeedChangeGauge) { return; }

	//���x�ύX�J�n�\�ɂ���
	isSpeedChangeStartPossible = true;

	//�Q�[�W�����܂������}�ɁA�ʏ�ړ���Ԃɂ��Ă���
	moveSpeedPhase = MoveSpeedPhase::NormalSpeed;
}

void BasePlayer::SpeedChangeHighSpeed()
{
	//�������Ɉړ�����ő���E���W
	const float highSpeedMaxPosZ = 25.0f;

	//���x�ύX����
	const int speedChangeTime = 30;
	float timeRatio = ((float)speedChangeTimer / (float)speedChangeTime);
	timeRatio = min(timeRatio, 1);
	//�^�C�}�[�̒l����ړ����x�Ɋɋ}������
	float speedRatio = 1 - timeRatio;
	speedRatio = max(speedRatio, 0.2f);

	//�O�Ɉړ�������
	const float moveSpeed = 0.4f;
	position.z += moveSpeed * speedRatio;

	//�ő���E���W���z���Ȃ��悤�ɂ���
	position.z = min(position.z, highSpeedMaxPosZ);
}

void BasePlayer::SpeedChangeSlowSpeed()
{
	//�������Ɉړ�����ŏ����E���W
	const float slowSpeedMinPosZ = 10.0f;

	//���x�ύX����
	const int speedChangeTime = 30;
	float timeRatio = ((float)speedChangeTimer / (float)speedChangeTime);
	timeRatio = min(timeRatio, 1);
	//�^�C�}�[�̒l����ړ����x�Ɋɋ}������
	float speedRatio = 1 - timeRatio;
	speedRatio = max(speedRatio, 0.15f);

	//���Ɉړ�������
	const float moveSpeed = 0.2f;
	position.z -= moveSpeed * speedRatio;

	//�ŏ����E���W���z���Ȃ��悤�ɂ���
	position.z = max(position.z, slowSpeedMinPosZ);
}

void BasePlayer::SpeedChangeNormalSpeed()
{
	//�ʏ�ړ��ɖ߂���ԂłȂ���Δ�����
	if (!(moveSpeedPhase == MoveSpeedPhase::NormalSpeed || moveSpeedPhase == MoveSpeedPhase::ReturnNormalSpeed)) { return; }

	//�ړ������邩���肷�鍷��
	const float distanceBasePos = 0.02f;
	//�������Ȃ���Ԃ������甲����
	if (position.z < basePos.z + distanceBasePos && position.z > basePos.z - distanceBasePos) {
		//����W�ɂ��Ă���
		position.z = basePos.z;

		//������
		return;
	}

	//���̍��W�ɖ߂��Ă���
	const float moveSpeed = 0.08f;
	if (position.z >= basePos.z + distanceBasePos) {
		position.z -= moveSpeed;
	}
	else if (position.z <= basePos.z - distanceBasePos) {
		position.z += moveSpeed;
	}
}

void BasePlayer::SpeedChangeNormalBlur()
{
	//�ʏ�ړ��ɖ߂���ԂłȂ���Δ�����
	if (!(moveSpeedPhase == MoveSpeedPhase::NormalSpeed || moveSpeedPhase == MoveSpeedPhase::ReturnNormalSpeed)) { return; }

	//�u���[�������Ȃ���ԂȂ甲����
	if (!(GamePostEffect::GetPostEffect()->GetIsRadialBlur())) { return; }

	//�u���[�̋����ύX�����邩���肷�鍷��
	const float distanceBlurStrength = 0.01f;
	//�������Ȃ���Ԃ������甲����
	float blurStrength = GamePostEffect::GetPostEffect()->GetRadialBlurStrength();
	if (blurStrength < normalSpeedBlurStrength + distanceBlurStrength && blurStrength > normalSpeedBlurStrength - distanceBlurStrength) {
		//�ʏ�ړ����̃u���[�̋����ɂ��Ă���
		GamePostEffect::GetPostEffect()->SetRadialBlurStrength(normalSpeedBlurStrength);

		//������
		return;
	}

	//�ʏ�ړ����̃u���[�̋����ɖ߂��Ă���
	const float changeSpeed = 0.01f;
	if (blurStrength >= normalSpeedBlurStrength + distanceBlurStrength) {
		blurStrength -= distanceBlurStrength;
	}
	else if (blurStrength <= normalSpeedBlurStrength - distanceBlurStrength) {
		blurStrength += distanceBlurStrength;
	}
	//�ύX�����������Z�b�g
	GamePostEffect::GetPostEffect()->SetRadialBlurStrength(blurStrength);
}

void BasePlayer::Attack()
{
	Input* input = Input::GetInstance();

	//�e���ˍ��W���X�V
	UpdateBulletShotPos();

	//�U���{�^������������
	if (input->PushKey(DIK_SPACE) || input->PushGamePadButton(Input::PAD_B)) {
		//�U���{�^���������Ă���Ƃ��̏���
		PushAttackButton();
	}
	//�U���{�^���𗣂�����
	else if (input->ReleaseKey(DIK_SPACE) || input->ReleaseGamePadButton(Input::PAD_B)) {
		//�U���{�^���𗣂����Ƃ��̏���
		ReleaseAttackButton();
	}
}

void BasePlayer::PushAttackButton()
{
	//�z�[�~���O�e�ɐ؂�ւ�鎞��
	const int32_t changeModeTime = 50;
	//�`���[�W���Ԃ����Z
	chargeTimer++;

	//���e�B�N�����`���[�W��ԂłȂ��Ƃ� && ���e�B�N�������b�N�I����ԂłȂ��Ƃ�
	if (!reticles->GetIsChargeMode() && !reticles->GetIsLockon()) {
		//�^�C�}�[���w�肵�����ԂɂȂ�����
		if (chargeTimer >= changeModeTime) {
			//���e�B�N�����`���[�W��Ԃɂ���
			reticles->ChargeModeStart();
		}
	}

	//�`���[�W��������
	if (!isChargeShotMode) {
		//�^�C�}�[���w�肵�����ԂɂȂ�����
		if (chargeTimer >= changeModeTime) {
			//�`���[�W�V���b�g��Ԃɂ���
			isChargeShotMode = true;
		}

		//���i�e���ˑҋ@����
		if (isStraightShotWait) {
			straightShotWaitTimer--;
			if (straightShotWaitTimer <= 0) {
				//�ҋ@�I��
				isStraightShotWait = false;
			}
			return;
		}

		//���i�e����
		ShotStraightBullet();

		//���i�e���ˑ҂����Ԃ�ݒ�
		const int32_t waitTime = 10;
		//�ҋ@�J�n
		isStraightShotWait = true;
		straightShotWaitTimer = waitTime;
	}
	//�`���[�W������
	else {
		//�`���[�W���o���ő�̑傫���ɂȂ�܂ł̃t���[����
		const int maxRatioFrame = 5;
		//�`���[�W�V���b�g���o�p�p�[�e�B�N���̑傫�������߂�(�`���[�W�������Ԃő傫���Ȃ�)
		float sizeRatio = (float)(chargeTimer - changeModeTime) / maxRatioFrame;
		//�ő�䗦1.0���z���Ȃ�
		const float maxRatio = 1.0f;
		if (sizeRatio >= maxRatio) { sizeRatio = maxRatio; }
		const float particleSize = homingBulletSize * sizeRatio;

		//�`���[�W�V���b�g���o�p�p�[�e�B�N������
		ParticleEmitter::GetInstance()->ChargeShot(bulletShotPos, particleSize);
	}
}

void BasePlayer::ReleaseAttackButton()
{
	//�`���[�W������
	if (isChargeShotMode) {
		//�z�[�~���O�e����
		ShotHomingBullet();

		//���e�B�N�����G�����b�N�I�����Ă��Ȃ���΁A���˂Ɠ����Ƀ`���[�W��ԏI��
		if (!reticles->GetIsLockon()) {
			reticles->ChargeModeEnd();
		}
	}

	//���ɔ��˃{�^�������������ɂ������˂ł���悤���i�e�̔��ˑҋ@�����Z�b�g
	isStraightShotWait = false;
	straightShotWaitTimer = 0;
	//�`���[�W�V���b�g��Ԃ����Z�b�g
	isChargeShotMode = false;
	chargeTimer = 0;
}

void BasePlayer::UpdateBulletShotPos()
{
	//���@�̒��S���W����̋���
	const Vector3 distancePos = { 0, -0.3f, 4.0f };

	//�e���ˍ��W���擾
	bulletShotPos = LocalTranslation(distancePos, matWorld);
}

void BasePlayer::ShotStraightBullet()
{
	//�e�̑��x��ݒ�
	const float bulletSpeed = 6;
	//���@���烌�e�B�N���ւ̃x�N�g���ɍ��킹�Ĕ�΂�
	Vector3 velocity = reticles->GetNearReticleWorldPos() - GetWorldPos();
	velocity = velocity.normalize() * bulletSpeed;

	//���i�e�𐶐�
	std::unique_ptr<PlayerBullet> newBullet;
	newBullet.reset(StraightBullet::Create(bulletModel, bulletShotPos, velocity));
	stageScene->AddPlayerBullet(std::move(newBullet));

	//�V���b�g���o�p�p�[�e�B�N������
	ParticleEmitter::GetInstance()->Shot(bulletShotPos);
}

void BasePlayer::ShotHomingBullet()
{
	//�e�̑��x��ݒ�
	const float bulletSpeed = 1.6f;
	//���@���烌�e�B�N���ւ̃x�N�g���ɍ��킹�Ĕ�΂�
	Vector3 velocity = reticles->GetNearReticleWorldPos() - GetWorldPos();
	velocity = velocity.normalize() * bulletSpeed;

	//�z�[�~���O�e�𐶐�
	std::unique_ptr<PlayerBullet> newBullet;
	newBullet.reset(HomingBullet::Create(bulletShotPos, velocity, homingBulletSize, reticles->GetLockonEnemy()));
	stageScene->AddPlayerBullet(std::move(newBullet));
}
