#include "Player.h"
#include "Input.h"

Player* Player::Create(ObjModel* model)
{
	//���@�̃C���X�^���X�𐶐�
	Player* player = new Player();
	if (player == nullptr) {
		return nullptr;
	}

	// ������
	if (!player->Initialize()) {
		delete player;
		assert(0);
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	player->SetModel(model);

	return player;
}

bool Player::Initialize()
{
	position = { 1,1,0 };
	scale = { 0.5f, 0.5f, 0.5f };

	//3D�I�u�W�F�N�g�̏�����
	if (!ObjObject3d::Initialize())
	{
		return false;
	}

	return true;
}

void Player::Update()
{
	//���S�����e�̍폜
	playerBullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->GetIsDead();
		});

	//�ړ�
	Move();

	//�U��
	Attack();

	//3D�I�u�W�F�N�g�̍X�V
	ObjObject3d::Update();

	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		bullet->Update();
	}
}

void Player::Draw()
{
	ObjObject3d::Draw();

	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		bullet->Draw();
	}
}

void Player::Move()
{
	Input* input = Input::GetInstance();

	//���͂ňړ�������
	XMFLOAT3 move = { 0, 0, 0 };
	float moveSpeed = 0.1f;
	if (input->PushKey(DIK_RIGHT)) { move.x += moveSpeed; }
	if (input->PushKey(DIK_LEFT)) { move.x -= moveSpeed; }
	if (input->PushKey(DIK_UP)) { move.y += moveSpeed; }
	if (input->PushKey(DIK_DOWN)) { move.y -= moveSpeed; }

	position.x += move.x;
	position.y += move.y;

	//�ړ����E������o�Ȃ��悤�ɂ���
	const XMFLOAT2 moveLimit = { 5.0f, 5.0f };

	position.x = max(position.x, -moveLimit.x);
	position.x = min(position.x, +moveLimit.x);
	position.y = max(position.y, -moveLimit.y);
	position.y = min(position.y, +moveLimit.y);
}

void Player::Attack()
{
	Input* input = Input::GetInstance();
	//���˃L�[����������
	if (input->TriggerKey(DIK_SPACE))
	{
		//�e�̑��x��ݒ�
		const float bulletSpeed = 0.05f;
		XMFLOAT3 velocity(0, 0, bulletSpeed);


		//�e�𐶐�
		std::unique_ptr<PlayerBullet> newBullet;
		newBullet.reset(PlayerBullet::Create(model, position, velocity));
		playerBullets.push_back(std::move(newBullet));
	}
}
