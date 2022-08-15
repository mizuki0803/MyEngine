#include "Enemy.h"
#include "Player.h"
#include "GameScene.h"

Player* Enemy::player = nullptr;
GameScene* Enemy::gameScene = nullptr;
ObjModel* Enemy::bulletModel = nullptr;

void Enemy::OnCollision()
{
	//���S������
	isDead = true;
}

Vector3 Enemy::GetWorldPos()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���s�ړ��������擾
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
}

Vector2 Enemy::GetScreenPos()
{
	Vector3 worldPos = GetWorldPos();

	//�r���[�|�[�g�s��
	XMMATRIX matViewport = {
		{1280 / 2, 0, 0, 0 },
		{0, -720 / 2, 0, 0},
		{0, 0, 1, 0},
		{1280 / 2, 720 / 2, 0, 1}
	};

	//�r���[�s��ƃv���W�F�N�V�����s��ƃr���[�|�[�g�s�������
	XMMATRIX matViewProjectionViewport =
		camera->GetMatView() * camera->GetMatProjection() * matViewport;

	//���[���h���W����X�N���[�����W�ɕϊ�
	worldPos = MatrixTransformWDivision(worldPos, matViewProjectionViewport);
	Vector2 screenPos = { worldPos.x, worldPos.y };

	return screenPos;
}

void Enemy::Fire()
{
	//���W�����@����O�Ȃ甭�˂��Ȃ�
	if (GetWorldPos().z <= player->GetWorldPos().z) { return; }

	//�e�̑��x��ݒ�
	const float bulletSpeed = 0.5f;

	Vector3 playerPos = player->GetWorldPos();
	Vector3 enemyPos = GetWorldPos();
	Vector3 DifferenceVec = playerPos - enemyPos;
	DifferenceVec.normalize();
	Vector3 velocity = DifferenceVec * bulletSpeed;

	//�e�𐶐�
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(bulletModel, position, velocity));
	gameScene->AddEnemyBullet(std::move(newBullet));
}

void Enemy::FrontOfScreenDelete()
{
	//���W�����@����O(��ʊO��O)�܂ōs������폜
	const float flontOfScreenDiffence = 50;
	const float deletePos = player->GetWorldPos().z - flontOfScreenDiffence;
	const Vector3 worldPos = GetWorldPos();

	if (worldPos.z <= deletePos) {
		isDead = true;
	}
}
