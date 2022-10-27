#include "Player.h"
#include "Input.h"
#include "Easing.h"
#include "GameScene.h"
#include "SpriteTextureLoader.h"
#include "StraightBullet.h"
#include "HomingBullet.h"
#include "ParticleEmitter.h"

void (Player::* Player::stageClearActionFuncTable[])() = {
	&Player::StageClearSideMove,
	&Player::StageClearReturn,
	&Player::StageClearUp,
	&Player::StageClearStay,
	&Player::StageClearBoost,
};

GameScene* Player::gameScene = nullptr;
ObjModel* Player::bulletModel = nullptr;
const float Player::homingBulletSize = 2.0f;
const Vector2 Player::rotLimit = { 35.0f, 25.0f };

Player* Player::Create(ObjModel* model)
{
	//���@�̃C���X�^���X�𐶐�
	Player* player = new Player();
	if (player == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	player->model = model;

	// ������
	if (!player->Initialize()) {
		delete player;
		assert(0);
		return nullptr;
	}

	return player;
}

bool Player::Initialize()
{
	position = { 0 ,1 ,15 };
	scale = { 1.5f, 1.5f, 1.5f };

	//3D�I�u�W�F�N�g�̏�����
	if (!ObjObject3d::Initialize()) {
		return false;
	}

	//���e�B�N���𐶐�
	reticles.reset(PlayerReticles::Create());

	//HP�o�[����
	const Vector2 hpBarPosition = { 20, 20 };
	hpBar.reset(PlayerHPBar::Create(SpriteTextureLoader::PlayerHPGaugeIn, hpBarPosition, maxHP));
	//HP�o�[�t���[������
	const float posDiff = 3.0f;	//HP�o�[�̍��W�Ƃ̍���
	const Vector2 hpFramePosition = { hpBarPosition.x - posDiff, hpBarPosition.y - posDiff };
	hpFrame.reset(PlayerHPFrame::Create(SpriteTextureLoader::PlayerHPGaugeOut, hpFramePosition));

	return true;
}

void Player::Update()
{
	//���S��ԂȂ甲����
	if (isDead) { return; }

	//�s��
	Action();

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();

	//���e�B�N���X�V
	reticles->Update(matWorld, camera->GetMatView(), camera->GetMatProjection());

	//HP�o�[�X�V
	hpBar->Update();
	//HP�o�[�t���[���X�V
	hpFrame->Update();

	//���@�̃W�F�b�g���ˉ��o�p�p�[�e�B�N������
	ParticleEmitter::GetInstance()->PlayerJet(matWorld);
}

void Player::Draw()
{
	//���S��ԂȂ甲����
	if (isDead) { return; }

	//�I�u�W�F�N�g�`��
	ObjObject3d::Draw();
}

void Player::DrawUI()
{
	//�ė���ԂȂ甲����
	if (isCrash) { return; }
	//�X�e�[�W�N���A��ԂȂ甲����
	if (isStageClearMode) { return; }

	//���e�B�N���`��
	reticles->Draw();

	//HP�o�[�t���[���`��
	hpFrame->Draw();
	//HP�o�[�`��
	hpBar->Draw();
}

void Player::OnCollisionDamage(const Vector3& subjectPos)
{
	//�_���[�W����炤
	Damage();

	//�m�b�N�o�b�N�����Z�b�g
	SetKnockback(subjectPos);

	//�_���[�W���������̂�HP�o�[�̒�����ύX����
	hpBar->SetChangeLength(HP);
}

void Player::OnCollisionHeal()
{
	//��
	Heal();

	//�_���[�W���������̂�HP�o�[�̒�����ύX����
	hpBar->SetChangeLength(HP);
}

void Player::StageClearModeStart()
{
	//�J�����Ǐ]������
	SetIsCameraFollow(false);
	//�J�����Ǐ]�����ɂ��A���[�J�����W�Ƀ��[���h���W����
	position = GetWorldPos();

	//�X�e�[�W�N���A�ړ����E���������߂�
	if (position.x < 0) { isStageClearMoveRight = false; }
	else { isStageClearMoveRight = true; }

	//�������x���Z�b�g
	stageClearMoveVelocity = { 0.2f, 0, 0.2f };
	//�����ړ����̂��߃}�C�i�X��n��
	if (!isStageClearMoveRight) {
		stageClearMoveVelocity.x = -stageClearMoveVelocity.x;
	}
	//��]���x���Z�b�g
	stageClearRota = rotation;

	//�X�e�[�W�N���A��̓���������
	isStageClearMode = true;
}

void Player::StageClearReturnStart(const Vector3& cameraPos)
{
	//�J�������W���擾
	stageClearCameraPos = cameraPos;

	//�J�����z�[�~���O�p���W���Z�b�g
	cameraHomingPos = cameraPos;
	cameraHomingPos.y += 2.0f;

	//�������x���Z�b�g
	stageClearMoveVelocity = { 0, 0, 2.0f };
	//�A�ҏ�Ԃɂ���
	stageClearModePhase = StageClearModePhase::Return;

	//�^�C�}�[������
	stageClearModeTimer = 0;
}

void Player::StageClearBoostStart()
{
	//�u�[�X�g��Ԃɂ���
	stageClearModePhase = StageClearModePhase::Boost;
}

Vector3 Player::GetWorldPos()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���s�ړ��������擾
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
}

void Player::Action()
{
	//�X�e�[�W�N���A���
	if (isStageClearMode) {
		//�X�e�[�W�N���A��s��
		(this->*stageClearActionFuncTable[static_cast<size_t>(stageClearModePhase)])();
	}
	//�ė����
	else if (isCrash) {
		Crash();
	}
	//����ȊO
	else {
		//��]
		Rotate();

		//�ً}���
		Roll();

		//�_���[�W��ԂȂ�m�b�N�o�b�N����
		if (isDamage) {
			Knockback();
		}
		//�_���[�W��ԂłȂ��Ȃ�ʏ�ړ�
		else {
			Move();
		}

		//�U��
		Attack();
	}
}

void Player::Damage()
{
	//�̗͂����炷
	HP -= 10;

	//HP��0�ȉ��ɂȂ�����ė�������
	if (HP <= 0) {
		CrashStart();

		//HP��0�ȉ��ɂȂ�Ȃ�
		HP = 0;
	}

	color = { 1,0,0,1 };

	//�_���[�W��Ԃɂ���
	isDamage = true;
}

void Player::CrashStart()
{
	//�J�����Ǐ]������
	SetIsCameraFollow(false);
	//�J�����Ǐ]�����ɂ��A���[�J�����W�Ƀ��[���h���W����
	position = GetWorldPos();

	//�ė���Ԃɂ���
	isCrash = true;
}

void Player::Crash()
{
	//���W�ړ�
	//�ė������x
	Vector3 crashAccel = { 0, -0.0075f, 0 };
	crashVel += crashAccel;
	position += crashVel;

	//Y���W��0�ȉ��ɂȂ�����
	if (position.y <= 0) {
		//���W��0�ȉ��ɂȂ�Ȃ�
		position.y = 0;

		//�ė��o�E���h�񐔂𑝉�������
		crashBoundCount++;

		//�o�E���h������
		const float boundStartVel = 0.4f;
		crashVel.y = boundStartVel;
	}

	//�ė���]����
	const Vector3 rotSpeed = { 0, 1, 5 };
	//�o�E���h����܂ł�Z������]
	if (crashBoundCount == 0) { rotation.z += rotSpeed.z; }
	//1��o�E���h������Z���E��] & Y����]
	else if (crashBoundCount == 1) {
		rotation -= rotSpeed;
	}

	//�ė��o�E���h�񐔂�2��ɒB�����玀�S
	const int maxCrashCount = 2;
	if (crashBoundCount >= maxCrashCount) {
		isDead = true;
	}
}

void Player::Heal()
{
	//�̗͂𑝂₷
	const int healNum = 10;
	HP += healNum;

	//HP�͍ő�HP�ȏ�ɂȂ�Ȃ�
	if (HP >= maxHP) {
		HP = maxHP;
	}
}

void Player::Rotate()
{
	Input* input = Input::GetInstance();

	//��]���x
	const float rotSpeed = 1.0f;
	//�p�x�C�����x
	const float backSpeed = rotSpeed / 1.5f;
	Vector3 rot = { 0, 0, 0 };


	//�ǂ��܂ŌX�����画����Ƃ邩
	const float stickNum = 200;
	//y����]
	{
		//�L�[���͂ŉ�]������
		if (input->PushKey(DIK_A) || input->PushKey(DIK_D)) {
			if (input->PushKey(DIK_A)) { rot.y -= rotSpeed; }
			if (input->PushKey(DIK_D)) { rot.y += rotSpeed; }
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
			//y����]�̌X�����C������
			if (rotation.y > 1.0f) {
				rot.y -= backSpeed;
			}
			else if (rotation.y < -1.0f) {
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
			//x����]�̌X�����C������
			if (rotation.x > 1.0f) {
				rot.x -= backSpeed;
			}
			else if (rotation.x < -1.0f) {
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
		const float rotZSpeed = 0.03f;
		const float rotZLimit = 0.8f;
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
	//�n�ʂɎ��@���߂荞�܂ރ��[���hY���W
	const float groundPos = 8.0f;
	//�n�ʂɂ߂荞�܂Ȃ��悤�ɂ���ꍇ�̊p�x���E�l����
	if (GetWorldPos().y <= groundPos) {
		//���[���hY���W�̌��E�l
		const float moveLimitWorldY = 4.0f;
		//�n�ʂ���p�x�����J�n���W�܂ł̊������v�Z
		const float ratio = (GetWorldPos().y - moveLimitWorldY) / (groundPos - moveLimitWorldY);

		//�����l��0�ȏ�̏ꍇ
		if (ratio >= 0) {
			//�n�ʂɋ߂Â��قǌX���Ȃ��悤�ɂ���(�ʏ�̊p�x�����l * �n�ʂɋ߂�����)
			const float groundRotLimit = rotLimit.x * ratio;
			rotation.x = min(rotation.x, groundRotLimit);
		}
		//0�����̏ꍇ
		else {
			//�������ɌX���Ȃ��悤�ɂ���
			rotation.x = min(rotation.x, 0);
		}
	}
	//�n�ʂɂ߂荞�ރ��[���hY���W�Ɗ֌W�Ȃ��ꏊ�ɂ���ꍇ�͒ʏ�ʂ�̊p�x�������s��
	else {
		rotation.x = min(rotation.x, +rotLimit.x);
	}
}

void Player::Move()
{
	//���@���X���Ă���p�x�Ɉړ�������
	Vector3 velocity = { 0, 0, 0 };
	const float moveSpeed = 0.1f;
	velocity.x = moveSpeed * (rotation.y / rotLimit.y);
	velocity.y = moveSpeed * -(rotation.x / rotLimit.x);
	position += velocity;

	//�ړ����E����o�Ȃ��悤�ɂ���
	const Vector2 moveLimitMax = { 10.0f, 6.0f };
	const Vector2 moveLimitMin = { -10.0f, -4.0f };
	position.x = max(position.x, moveLimitMin.x);
	position.x = min(position.x, moveLimitMax.x);
	position.y = max(position.y, moveLimitMin.y);
	position.y = min(position.y, moveLimitMax.y);
}

void Player::Roll()
{
	Input* input = Input::GetInstance();

	//�ً}������̓���
	if (isRoll) {
		//�^�C�}�[���X�V
		const float rollTime = 40;
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
	//�ً}����\��
	else {
		//�_���[�W��ԂȂ�ً}����͔����ł��Ȃ��Ŕ�����
		if (isDamage) { return; }

		//�ً}����L�[�������Ă��Ȃ���Δ�����
		if (!(input->TriggerKey(DIK_RSHIFT) || input->TriggerKey(DIK_LSHIFT) || input->TriggerGamePadButton(Input::PAD_RB)
			|| input->TriggerGamePadButton(Input::PAD_LB))) {
			return;
		}
		//�ً}�����Ԃɂ���
		isRoll = true;
		//�^�C�}�[��������
		rollTimer = 0;
		//�ً}����J�n����Z���p�x���L��
		rollStartRot = rotation.z;

		//�ً}����I������Z���p�x���Z�b�g
		const float rotAmount = 360; //��]��
		if (input->TriggerKey(DIK_RSHIFT) || input->TriggerGamePadButton(Input::PAD_RB)) { rollEndRot = -rotAmount; }		//�E��]
		else if (input->TriggerKey(DIK_LSHIFT) || input->TriggerGamePadButton(Input::PAD_LB)) { rollEndRot = rotAmount; }	//����]
	}
}

void Player::Attack()
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

void Player::PushAttackButton()
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

void Player::ReleaseAttackButton()
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

void Player::UpdateBulletShotPos()
{
	//���@�̒��S���W����̋���
	const Vector3 distancePos = { 0, -0.3f, 4.0f };
	//���s�ړ��s��̌v�Z
	XMMATRIX matTrans = XMMatrixTranslation(distancePos.x, distancePos.y, distancePos.z);

	//���[���h�s��̍���
	XMMATRIX bulletShotMatWorld;
	bulletShotMatWorld = XMMatrixIdentity();	//�ό`�����Z�b�g
	bulletShotMatWorld *= matTrans;	//���[���h�s��ɕ��s�ړ��𔽉f

	//���@�I�u�W�F�N�g�̃��[���h�s���������
	bulletShotMatWorld *= matWorld;

	//�e���ˍ��W���擾
	bulletShotPos = { bulletShotMatWorld.r[3].m128_f32[0], bulletShotMatWorld.r[3].m128_f32[1], bulletShotMatWorld.r[3].m128_f32[2] };
}

void Player::ShotStraightBullet()
{
	//���ˈʒu�����@�̃��[���h���W�ɐݒ�
	Vector3 shotPos = GetWorldPos();

	//�e�̑��x��ݒ�
	const float bulletSpeed = 5;
	//���@���烌�e�B�N���ւ̃x�N�g���ɍ��킹�Ĕ�΂�
	Vector3 velocity = reticles->GetNearReticleWorldPos() - GetWorldPos();
	velocity = velocity.normalize() * bulletSpeed;

	//���i�e�𐶐�
	std::unique_ptr<PlayerBullet> newBullet;
	newBullet.reset(StraightBullet::Create(bulletModel, bulletShotPos, velocity));
	gameScene->AddPlayerBullet(std::move(newBullet));
}

void Player::ShotHomingBullet()
{
	//���ˈʒu�����@�̃��[���h���W�ɐݒ�
	Vector3 shotPos = GetWorldPos();

	//���@���烌�e�B�N���ւ̃x�N�g���ɍ��킹�Ĕ�΂�
	Vector3 velocity = reticles->GetNearReticleWorldPos() - GetWorldPos();
	velocity.normalize();

	//�z�[�~���O�e�𐶐�
	std::unique_ptr<PlayerBullet> newBullet;
	newBullet.reset(HomingBullet::Create(bulletModel, bulletShotPos, velocity, homingBulletSize, reticles->GetLockonEnemy()));
	gameScene->AddPlayerBullet(std::move(newBullet));
}

void Player::SetKnockback(const Vector3& subjectPos)
{
	//�m�b�N�o�b�N������������߂�(���@�̃��[���h���W - �Ώۂ̃��[���h���W)
	knockbackVec = GetWorldPos() - subjectPos;
	//�x�N�g���𐳋K��
	knockbackVec.normalize();

	//�m�b�N�o�b�N�^�C�}�[��������
	knockbackTimer = 0;
}

void Player::Knockback()
{
	//�m�b�N�o�b�N���鎞��
	const float knockbackTime = 30;
	knockbackTimer++;
	const float time = knockbackTimer / knockbackTime;

	//���x���쐬
	knockbackVel = knockbackVec;
	//Z�������ɂ͈ړ����Ȃ��悤�ɂ���
	knockbackVel.z = 0;
	//�m�b�N�o�b�N���Ԃ𑬓x�ɂ����Č������ۂ�����
	knockbackVel *= (1 - time);

	//���@���m�b�N�o�b�N������
	const float speed = 0.2f;
	position += knockbackVel *= speed;

	//�ړ����E����o�Ȃ��悤�ɂ���
	const Vector2 moveLimitMax = { 10.0f, 6.0f };
	const Vector2 moveLimitMin = { -10.0f, -4.0f };
	position.x = max(position.x, moveLimitMin.x);
	position.x = min(position.x, moveLimitMax.x);
	position.y = max(position.y, moveLimitMin.y);
	position.y = min(position.y, moveLimitMax.y);

	//�m�b�N�o�b�N���I��������_���[�W��Ԃ���������
	if (knockbackTimer >= knockbackTime) {
		isDamage = false;
		color = { 1,1,1,1 };
	}
}

void Player::StageClearSideMove()
{
	//�^�C�}�[�X�V
	stageClearModeTimer++;

	//�����x��ݒ�
	Vector3 accel = { 0.003f, 0, 0.002f };
	//��]�p�x���Z�b�g
	float maxRotZ = -(360 + 65);
	//�����ړ����̐ݒ�
	if (!isStageClearMoveRight) { accel.x = -accel.x;  maxRotZ = -maxRotZ; }

	//���x�ɉ����x�����Z
	stageClearMoveVelocity += accel;
	//���W���ړ�
	position += stageClearMoveVelocity;

	//��]
	const float rotTime = 120;
	float time = stageClearModeTimer / rotTime;
	time = min(time, 1);
	rotation.x = Easing::OutQuart(stageClearRota.x, 0, time);
	rotation.y = Easing::OutQuart(stageClearRota.y, 0, time);
	rotation.z = Easing::OutQuart(stageClearRota.z, maxRotZ, time);
}

void Player::StageClearReturn()
{
	//�^�C�}�[�X�V
	const float maxTime = 250;
	stageClearModeTimer++;
	const float time = stageClearModeTimer / maxTime;

	//�w�肵�����Ԃ��߂�����
	float homingDistance = 100;
	const float distanceChangeStartTime = 120;
	if (stageClearModeTimer >= distanceChangeStartTime) {
		//�z�[�~���O�����ʒu�������炸�炷
		const float changeTime = (stageClearModeTimer - distanceChangeStartTime) / (maxTime - distanceChangeStartTime);
		homingDistance = Easing::LerpFloat(100, 0, changeTime);
	}
	cameraHomingPos.z = stageClearCameraPos.z + homingDistance;

	//�z�[�~���O�����ɍ��W���ړ�
	Vector3 toCamera = cameraHomingPos - GetWorldPos();
	toCamera.normalize();
	stageClearMoveVelocity.normalize();
	stageClearModeTimer = min(stageClearModeTimer, (int)maxTime);
	const float speed = 0.5f;
	stageClearMoveVelocity = Easing::LerpVec3(stageClearMoveVelocity, toCamera, time) * speed;
	position += stageClearMoveVelocity;

	//�i�s�����������悤�ɂ���
	rotation.y = XMConvertToDegrees(std::atan2(stageClearMoveVelocity.x, stageClearMoveVelocity.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 velocityZ = MatrixTransformDirection(stageClearMoveVelocity, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-velocityZ.y, velocityZ.z));

	//��]�p�xZ���Z�b�g
	const float downward = 180;
	float maxRotZ = (360 * 3 + downward);
	//�����ړ����̐ݒ�
	if (!isStageClearMoveRight) { maxRotZ = -maxRotZ; }
	rotation.z = Easing::OutQuad(stageClearRota.z, maxRotZ, time);

	//�J���������ւ������玟�̃t�F�[�Y��
	if (position.z <= stageClearCameraPos.z) {
		stageClearModePhase = StageClearModePhase::Up;

		//�p�x����]����������
		rotation.z = downward;

		//�^�C�}�[��������
		stageClearModeTimer = 0;
	}
}

void Player::StageClearUp()
{
	//�^�C�}�[�X�V
	const float upTime = 350;
	stageClearModeTimer++;

	//���������������
	const Vector3 rotSpeed = { -0.1f, 0, -1 };
	rotation += rotSpeed;
	const float upRotLimit = 40;
	rotation.x = max(rotation.x, -upRotLimit);
	rotation.z = max(rotation.z, 0);

	//���@���X���Ă���p�x�Ɉړ�������
	Vector3 velocity;
	velocity.x = stageClearMoveVelocity.x;
	velocity.y = stageClearMoveVelocity.z * (rotation.x / upRotLimit);
	velocity.z = stageClearMoveVelocity.z;
	position += velocity;

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (stageClearModeTimer >= upTime) {
		stageClearModePhase = StageClearModePhase::Stay;

		//z��������]������
		isRotZRight = true;
		swayZ = 0.0f;

		//�^�C�}�[������
		stageClearModeTimer = 0;
	}
}

void Player::StageClearStay()
{
	//�������Ȃ��Ǝ₵���̂ł���炳���Ă���
	const float rotZSpeed = 0.03f;
	const float rotZLimit = 0.8f;
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
	rotation.z = swayZ;


	//���x��0��
	const float decelerationTime = 120;
	if (stageClearModeTimer >= decelerationTime) { return; }
	stageClearModeTimer++;
	const float time = stageClearModeTimer / decelerationTime;
	Vector3 velocity = Easing::LerpVec3(stageClearMoveVelocity, { 0, 0, 0 }, time);

	//���񂾂񌸑�
	position += velocity;
}

void Player::StageClearBoost()
{
	//�u�[�X�g�ړ�
	//���@���X���Ă���p�x�Ɉړ�������
	Vector3 velocity = { 0, 0, 1.0f };
	velocity = MatrixTransformDirection(velocity, matWorld);
	position += velocity;

	//�X�e�[�W�N���A��̍s�����������Ă����甲����
	if (isStageClearModeCompletion) { return; }

	//�^�C�}�[�X�V
	const float stageClearModeCompletionTime = 120;
	stageClearModeTimer++;

	//�^�C�}�[���w�肵�����ԂɈȉ��Ȃ甲����
	if (stageClearModeTimer < stageClearModeCompletionTime) { return; }

	//�X�e�[�W�N���A��̍s���������������Ƃɂ���
	isStageClearModeCompletion = true;
}