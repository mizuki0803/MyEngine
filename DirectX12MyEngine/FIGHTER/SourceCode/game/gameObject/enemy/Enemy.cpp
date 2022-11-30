#include "Enemy.h"
#include "Player.h"
#include "GameScene.h"
#include "EnemyDefeatCounter.h"
#include "ParticleEmitter.h"

Player* Enemy::player = nullptr;
GameScene* Enemy::gameScene = nullptr;
ObjModel* Enemy::bulletModel = nullptr;

bool Enemy::Initialize()
{
	//�I�u�W�F�N�g������
	if (!ObjObject3d::Initialize()) {
		return false;
	}

	//�傫���ύX
	const float scaleNum = 2;
	scale = { scaleNum, scaleNum, scaleNum };

	//��]
	rotation.y = 180;

	return true;
}

void Enemy::Update()
{
	//�����蔻�肪��p�����̂͑O�t���[���ɂȂ�̂ŁAfalse�ɖ߂��Ă���
	if (isCollisionFrame) {
		isCollisionFrame = false;
	}

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();
}

void Enemy::OnCollision()
{
	//�����p�傫��
	float explosionSize = 0.8f;

	//���S��ԂłȂ���Ύ��S������
	if (!isDead) {
		isDead = true;

		//�|�������J�E���^�[�𑝂₷
		EnemyDefeatCounter::AddCounter();
	}
	//���Ɏ��S��ԂŎ��S���o���̂Ƃ��͍폜����
	else {
		isDelete = true;

		//��e���č폜����̂Ńt���O�𗧂Ă�
		isHitDelete = true;

		//�j��
		Break();

		//�����̃T�C�Y��傫������
		explosionSize = 1.6f;
	}

	//�����蔻�肪���̃t���[���ō�p����
	isCollisionFrame = true;

	//�������o�p�p�[�e�B�N������
	ParticleEmitter::GetInstance()->Explosion(position, explosionSize);
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

void Enemy::Break()
{
	//�j��p�G�t�F�N�g�ǉ�
	for (int i = 0; i < 5; i++) {
		//�����_���ŃG�t�F�N�g�̑��x���Z�b�g
		const Vector3 randVel = { 2, 2, 2 };
		Vector3 velocity;
		velocity.x = (float)((rand() % (int)randVel.x) - randVel.x / 2);
		velocity.y = (float)((rand() % (int)randVel.y));
		velocity.z = (float)((rand() % (int)randVel.z) - randVel.z / 2);

		//�l���傫���̂Ŋ���Z���ď���������
		const float div = 10;
		velocity /= div;

		const Vector3 scale = { 0.5f, 0.5f, 0.5f };
		BreakEffect(bulletModel, velocity, {}, scale);
	}
}

void Enemy::BreakEffect(ObjModel* model, const Vector3& velocity, const Vector3& rotSpeed, const Vector3& scale)
{
	//�j��p�G�t�F�N�g�𐶐�
	std::unique_ptr<EnemyBreakEffect> newBreakEffect;
	newBreakEffect.reset(EnemyBreakEffect::Create(model, position, velocity, rotSpeed, scale));
	gameScene->AddEnemyBreakEffect(std::move(newBreakEffect));
}

void Enemy::FrontOfScreenDelete()
{
	//���W�����@����O(��ʊO��O)�܂ōs������폜
	const float flontOfScreenDiffence = 50;
	const float deletePos = player->GetWorldPos().z - flontOfScreenDiffence;
	const Vector3 worldPos = GetWorldPos();

	if (worldPos.z <= deletePos) {
		isDelete = true;
	}
}
