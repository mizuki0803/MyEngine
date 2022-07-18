#include "Player.h"
#include "Input.h"

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
	position = { 0 ,-5 ,25 };
	scale = { 2, 2, 2 };

	//3D�I�u�W�F�N�g�̏�����
	if (!ObjObject3d::Initialize())
	{
		return false;
	}

	//���e�B�N���𐶐�
	reticle.reset(Reticle::Create(1, 25.0f, { 100, 100 }));
	reticle2.reset(Reticle::Create(1, 50.0f, { 50, 50 }));

	return true;
}

void Player::Update()
{
	//���S�����e�̍폜
	playerBullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->GetIsDead();
		});

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

	//�e�X�V
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		bullet->Update();
	}
}

void Player::Draw()
{
	//�I�u�W�F�N�g�`��
	ObjObject3d::Draw();

	//���@�e�`��
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		bullet->Draw();
	}
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
	float moveSpeed = 0.1f;
	if (input->PushKey(DIK_RIGHT)) { move.x += moveSpeed; }
	if (input->PushKey(DIK_LEFT)) { move.x -= moveSpeed; }
	if (input->PushKey(DIK_UP)) { move.y += moveSpeed; }
	if (input->PushKey(DIK_DOWN)) { move.y -= moveSpeed; }

	position += move;

	//�ړ����E������o�Ȃ��悤�ɂ���
	const XMFLOAT2 moveLimit = { 5.0f, 5.0f };

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

	rotation += rot;
}

void Player::Attack()
{
	Input* input = Input::GetInstance();
	//���˃L�[����������
	if (input->TriggerKey(DIK_SPACE))
	{
		//���ˈʒu�����@�̃��[���h���W�ɐݒ�
		Vector3 shotPos = GetWorldPos();

		//�e�̑��x��ݒ�
		const float bulletSpeed = 0.5f;
		//���@���烌�e�B�N���ւ̃x�N�g���ɍ��킹�Ĕ�΂�
		Vector3 velocity = reticle->GetWorldPos() - GetWorldPos();
		velocity = velocity.normalize() * bulletSpeed;

		//�e�𐶐�
		std::unique_ptr<PlayerBullet> newBullet;
		newBullet.reset(PlayerBullet::Create(model, shotPos, velocity));
		playerBullets.push_back(std::move(newBullet));
	}
}
