#include "Player.h"
#include "Input.h"
#include "GameScene.h"
#include "StraightBullet.h"
#include "HomingBullet.h"

GameScene* Player::gameScene = nullptr;
ObjModel* Player::bulletModel = nullptr;

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
	position = { 0 ,-5 ,10 };
	scale = { 0.5f, 0.5f, 0.5f };

	//3D�I�u�W�F�N�g�̏�����
	if (!ObjObject3d::Initialize())
	{
		return false;
	}

	//���e�B�N���𐶐�
	reticle.reset(Reticle::Create(1, 5.0f, { 100, 100 }));
	reticle2.reset(Reticle::Create(1, 25.0f, { 50, 50 }));

	return true;
}

void Player::Update()
{
	//��]
	Rotate();

	//�ړ�
	Move();

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();

	//���e�B�N���X�V
	reticle->Update(matWorld, camera->GetMatView(), camera->GetMatProjection());
	reticle2->Update(matWorld, camera->GetMatView(), camera->GetMatProjection());

	//�U��
	Attack();
}

void Player::DrawUI()
{
	//���e�B�N���`��
	reticle->Draw();
	reticle2->Draw();
}

void Player::OnCollision()
{
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

void Player::Move()
{
	Input* input = Input::GetInstance();

	//���͂ňړ�������
	Vector3 move = { 0, 0, 0 };
	float moveSpeed = 0.15f;
	if (input->PushKey(DIK_RIGHT)) { move.x += moveSpeed; }
	if (input->PushKey(DIK_LEFT)) { move.x -= moveSpeed; }
	if (input->PushKey(DIK_UP)) { move.y += moveSpeed; }
	if (input->PushKey(DIK_DOWN)) { move.y -= moveSpeed; }

	//�X�e�B�b�N�X���̔�������
	const float stickNum = 500;
	if (input->TiltGamePadLStickX(stickNum) || input->TiltGamePadLStickX(-stickNum) || input->TiltGamePadLStickY(stickNum) || input->TiltGamePadLStickY(-stickNum)) {
		//�v���C���[�̓X�e�B�b�N��|���������ɓ���
		float padRota = input->GetPadLStickAngle();
		float moveAngle = XMConvertToRadians(padRota);
		move.x = moveSpeed * cosf(moveAngle);
		move.y = moveSpeed * -sinf(moveAngle);
	}

	//position += move;

	//�ړ����E������o�Ȃ��悤�ɂ���
	const XMFLOAT2 moveLimit = { 10.0f, 5.0f };

	position.x = max(position.x, -moveLimit.x);
	position.x = min(position.x, +moveLimit.x);
	position.y = max(position.y, -moveLimit.y);
	position.y = min(position.y, +moveLimit.y);
}

void Player::Rotate()
{
	Input* input = Input::GetInstance();

	//���͂ŉ�]������
	Vector3 rot = { 0, 0, 0 };
	float rotSpeed = 1.0f;
	if (input->PushKey(DIK_W)) { rot.x -= rotSpeed; }
	if (input->PushKey(DIK_S)) { rot.x += rotSpeed; }
	if (input->PushKey(DIK_A)) { rot.y -= rotSpeed; }
	if (input->PushKey(DIK_D)) { rot.y += rotSpeed; }

	//�X�e�B�b�N�X���̔�������
	const float stickNum = 200;
	if (input->TiltGamePadLStickX(stickNum) || input->TiltGamePadLStickX(-stickNum)) {
		//�v���C���[�̓X�e�B�b�N��|���������ɓ���
		float padRota = input->GetPadLStickAngle();
		const float padStickIncline = input->GetPadLStickIncline().x;
		float moveAngle = XMConvertToRadians(padRota);
		rot.y = rotSpeed * cosf(moveAngle) * fabsf(padStickIncline);
	}
	else {
		if (rotation.y > 0.1f) {
			rot.y -= rotSpeed / 5;
		}
		else if (rotation.y < -0.1f) {
			rot.y += rotSpeed / 5;
		}
	}

	if (input->TiltGamePadLStickY(stickNum) || input->TiltGamePadLStickY(-stickNum)) {
		//�v���C���[�̓X�e�B�b�N��|���������ɓ���
		float padRota = input->GetPadLStickAngle();
		const float padStickIncline = input->GetPadLStickIncline().y;
		float moveAngle = XMConvertToRadians(padRota);
		rot.x = rotSpeed * sinf(moveAngle) * fabsf(padStickIncline);
	}

	else {
		if (rotation.x > 0.1f) {
			rot.x -= rotSpeed / 5;
		}
		else if (rotation.x < -0.1f) {
			rot.x += rotSpeed / 5;
		}
	}

	rotation += rot;
	rotation.z = -rotation.y / 4;

	//�p�x�̌��E�l��ݒ�
	const XMFLOAT2 rotLimit = { 35.0f, 20.0f };

	rotation.x = max(rotation.x, -rotLimit.x);
	rotation.x = min(rotation.x, +rotLimit.x);
	rotation.y = max(rotation.y, -rotLimit.y);
	rotation.y = min(rotation.y, +rotLimit.y);




	//�p�x�ɒǏ]���Ĉړ�������
	Vector3 move = { 0, 0, 0 };
	float moveSpeed = 0.15f;
	{
		//�v���C���[�̓X�e�B�b�N��|���������ɓ���
		float padRota = input->GetPadLStickAngle();
		float moveAngle = XMConvertToRadians(padRota);
		move.x = moveSpeed * (rotation.y / rotLimit.y);
		move.y = moveSpeed * -(rotation.x / rotLimit.x);
	}

	position += move;

	//�ړ����E������o�Ȃ��悤�ɂ���
	const XMFLOAT2 moveLimit = { 30.0f, 15.0f };

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
			const int32_t changeModeTime = 40;
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
	Vector3 velocity = reticle->GetWorldPos() - GetWorldPos();
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
	Vector3 velocity = reticle->GetWorldPos() - GetWorldPos();
	velocity.normalize();

	//�z�[�~���O�e�𐶐�
	std::unique_ptr<PlayerBullet> newBullet;
	newBullet.reset(HomingBullet::Create(bulletModel, shotPos, velocity, enemy));
	gameScene->AddPlayerBullet(std::move(newBullet));
}
