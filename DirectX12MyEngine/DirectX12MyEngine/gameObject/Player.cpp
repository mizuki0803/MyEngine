#include "Player.h"
#include "Input.h"
#include "GameScene.h"
#include "StraightBullet.h"
#include "HomingBullet.h"

GameScene* Player::gameScene = nullptr;
ObjModel* Player::bulletModel = nullptr;
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
	position = { 0 ,1 ,10 };
	scale = { 1.5f, 1.5f, 1.5f };

	//3D�I�u�W�F�N�g�̏�����
	if (!ObjObject3d::Initialize()) {
		return false;
	}

	//���e�B�N���𐶐�
	reticle.reset(Reticle::Create(1, 15.0f, { 100, 100 }));
	reticle2.reset(Reticle::Create(1, 25.0f, { 50, 50 }));


	//HP�o�[����
	const Vector2 hpBarPosition = { 20, 20 };
	hpBar.reset(PlayerHPBar::Create(2, hpBarPosition, maxHP));
	//HP�o�[�t���[������
	const float posDiff = 3.0f;	//HP�o�[�̍��W�Ƃ̍���
	const Vector2 hpFramePosition = { hpBarPosition.x - posDiff, hpBarPosition.y - posDiff };
	hpFrame.reset(PlayerHPFrame::Create(3, hpFramePosition));

	return true;
}

void Player::Update()
{
	//��]
	Rotate();

	//�_���[�W��ԂȂ�m�b�N�o�b�N����
	if (isDamage) {
		Knockback();
	}
	//�_���[�W��ԂłȂ��Ȃ�ʏ�ړ�
	else {
		Move();
	}

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();

	//���e�B�N���X�V
	reticle->Update(matWorld, camera->GetMatView(), camera->GetMatProjection());
	reticle2->Update(matWorld, camera->GetMatView(), camera->GetMatProjection());

	//HP�o�[�X�V
	hpBar->Update();
	//HP�o�[�t���[���X�V
	hpFrame->Update();

	//�U��
	Attack();
}

void Player::DrawUI()
{
	//���e�B�N���`��
	reticle->Draw();
	reticle2->Draw();

	//HP�o�[�t���[���`��
	hpFrame->Draw();
	//HP�o�[�`��
	hpBar->Draw();
}

void Player::OnCollisionDamage(const Vector3& subjectPos)
{
	//�_���[�W��ԂȂ甲����
	if (isDamage) { return; }

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

void Player::Damage()
{
	//�̗͂����炷
	HP -= 10;

	//HP��0�ȉ��ɂȂ����玀�S������
	if (HP <= 0) {
		isDead = true;

		//HP��0�ȉ��ɂȂ�Ȃ�
		HP = 0;
	}

	color = { 1,0,0,1 };

	//�_���[�W��Ԃɂ���
	isDamage = true;
}

void Player::Heal()
{
	//�̗͂𑝂₷
	HP += 10;

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
	rotation.x = max(rotation.x, -rotLimit.x);
	rotation.x = min(rotation.x, +rotLimit.x);
	rotation.y = max(rotation.y, -rotLimit.y);
	rotation.y = min(rotation.y, +rotLimit.y);
}

void Player::Move()
{
	//���@���X���Ă���p�x�Ɉړ�������
	Vector3 move = { 0, 0, 0 };
	const float moveSpeed = 0.15f;
	move.x = moveSpeed * (rotation.y / rotLimit.y);
	move.y = moveSpeed * -(rotation.x / rotLimit.x);
	position += move;

	//�ړ����E����o�Ȃ��悤�ɂ���
	const Vector2 moveLimit = { 10.0f, 5.0f };
	position.x = max(position.x, -moveLimit.x);
	position.x = min(position.x, +moveLimit.x);
	position.y = max(position.y, -moveLimit.y);
	position.y = min(position.y, +moveLimit.y);
}

void Player::Attack()
{
	Input* input = Input::GetInstance();
	//���˃L�[����������
	if (input->PushKey(DIK_SPACE) || input->PushGamePadButton(Input::PAD_B)) {
		//�`���[�W���Ԃ����Z
		chargeTimer++;

		//�`���[�W��������
		if (!isChargeShotMode) {
			//�z�[�~���O�e�ɐ؂�ւ�鎞��
			const int32_t changeModeTime = 60;
			if (chargeTimer >= changeModeTime) {
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
	}
	//���˃L�[�𗣂�����
	else if (input->ReleaseKey(DIK_SPACE) || input->ReleaseGamePadButton(Input::PAD_B)) {
		//�`���[�W������
		if (isChargeShotMode) {
			//�z�[�~���O�e����
			ShotHomingBullet();
		}

		//���ɔ��˃{�^�������������ɂ������˂ł���悤���i�e�̔��ˑҋ@�����Z�b�g
		isStraightShotWait = false;
		straightShotWaitTimer = 0;
		//�`���[�W�V���b�g��Ԃ����Z�b�g
		isChargeShotMode = false;
		chargeTimer = 0;
	}
}

void Player::ShotStraightBullet()
{
	//���ˈʒu�����@�̃��[���h���W�ɐݒ�
	Vector3 shotPos = GetWorldPos();

	//�e�̑��x��ݒ�
	const float bulletSpeed = 5;
	//���@���烌�e�B�N���ւ̃x�N�g���ɍ��킹�Ĕ�΂�
	Vector3 velocity = reticle->GetReticle3D()->GetWorldPos() - GetWorldPos();
	velocity = velocity.normalize() * bulletSpeed;

	//���i�e�𐶐�
	std::unique_ptr<PlayerBullet> newBullet;
	newBullet.reset(StraightBullet::Create(bulletModel, shotPos, velocity));
	gameScene->AddPlayerBullet(std::move(newBullet));
}

void Player::ShotHomingBullet()
{
	//���ˈʒu�����@�̃��[���h���W�ɐݒ�
	Vector3 shotPos = GetWorldPos();

	//���@���烌�e�B�N���ւ̃x�N�g���ɍ��킹�Ĕ�΂�
	Vector3 velocity = reticle->GetReticle3D()->GetWorldPos() - GetWorldPos();
	velocity.normalize();

	//�z�[�~���O�e�𐶐�
	std::unique_ptr<PlayerBullet> newBullet;
	newBullet.reset(HomingBullet::Create(bulletModel, shotPos, velocity, reticle2->GetReticle2D()->GetLockonEnemy()));
	gameScene->AddPlayerBullet(std::move(newBullet));
}

void Player::SetKnockback(const Vector3& subjectPos)
{
	//�m�b�N�o�b�N������������߂�(�Ώۂ̃��[���h���W - ���@�̃��[���h���W)
	knockbackVec = subjectPos - GetWorldPos();
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
	knockbackVel.normalize();
	//Z�������ɂ͈ړ����Ȃ��悤�ɂ���
	knockbackVel.z = 0;
	//�m�b�N�o�b�N���Ԃ𑬓x�ɂ����Č������ۂ�����
	knockbackVel *= (1 - time);

	//���@���m�b�N�o�b�N������
	const float speed = 0.2f;
	position += knockbackVel *= speed;

	//�ړ����E����o�Ȃ��悤�ɂ���
	const Vector2 moveLimit = { 10.0f, 5.0f };
	position.x = max(position.x, -moveLimit.x);
	position.x = min(position.x, +moveLimit.x);
	position.y = max(position.y, -moveLimit.y);
	position.y = min(position.y, +moveLimit.y);

	//�m�b�N�o�b�N���I��������_���[�W��Ԃ���������
	if (knockbackTimer >= knockbackTime) {
		isDamage = false;
		color = { 1,1,1,1 };
	}
}
