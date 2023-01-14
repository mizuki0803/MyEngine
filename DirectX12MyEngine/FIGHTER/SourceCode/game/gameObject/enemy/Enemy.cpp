#include "Enemy.h"
#include "BasePlayer.h"
#include "BaseGameCamera.h"
#include "BaseStageScene.h"
#include "EnemyDefeatCounter.h"
#include "ParticleEmitter.h"
#include "Easing.h"

BasePlayer* Enemy::player = nullptr;
BaseGameCamera* Enemy::gameCamera = nullptr;
BaseStageScene* Enemy::stageScene = nullptr;
ObjModel* Enemy::bulletModel = nullptr;
bool Enemy::isGroundMode = true;
const XMFLOAT4 Enemy::damageColor = { 1, 0.2f, 0.2f, 1 };

bool Enemy::Initialize()
{
	//�I�u�W�F�N�g������
	if (!ObjObject3d::Initialize()) {
		return false;
	}

	//�傫���ύX
	const float scaleNum = 2;
	scale = { scaleNum, scaleNum, scaleNum };

	//�ʏ�T�C�Y���Z�b�g
	normalSize = scale;
	//�_���[�W��Ԃ̃T�C�Y���Z�b�g
	damageSize = scale * 1.05f;

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

	//�_���[�W��Ԃ̏���
	if (isDamage) {
		DamageMode();
	}

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();
}

void Enemy::OnCollision(const int damageNum)
{
	//�����p�傫��
	float explosionSize = 1.5f;

	//���S��ԂłȂ���΃_���[�W�����킹��
	if (!isDead) {
		//HP���󂯂��_���[�W�ʂ̕����炷
		HP -= damageNum;
		//�_���[�W��Ԃɂ���
		isDamage = true;
		//�_���[�W��ԃ^�C�}�[������
		damageTimer = 0;
		//�F��ύX
		color = damageColor;

		//�T�C�Y�������傫������
		scale = damageSize;

		//HP��0�ȉ��ɂȂ����玀�S������
		if (HP <= 0) {
			isDead = true;

			//�|�������J�E���^�[�𑝂₷
			EnemyDefeatCounter::AddCounter();
		}
	}
	//���Ɏ��S��ԂŎ��S���o���̂Ƃ��͍폜����
	else {
		isDelete = true;

		//��e���č폜����̂Ńt���O�𗧂Ă�
		isHitDelete = true;

		//�j��
		Break();

		//�����̃T�C�Y��傫������
		explosionSize = 2.5f;
	}

	//�����蔻�肪���̃t���[���ō�p����
	isCollisionFrame = true;

	//�������o�p�p�[�e�B�N������
	ParticleEmitter::GetInstance()->Explosion(position, explosionSize);
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

void Enemy::FireLookAngle()
{
	//���W�����@����O�Ȃ甭�˂��Ȃ�
	if (GetWorldPos().z <= player->GetWorldPos().z) { return; }

	//�e�̑��x��ݒ�
	const float bulletSpeed = 1.0f;

	//�����Ă�������ɒe�𔭎�
	Vector3 velocity = { 0, 0, bulletSpeed };
	velocity = MatrixTransformDirection(velocity, matWorld);

	//�e�𐶐�
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(bulletModel, position, velocity));
	stageScene->AddEnemyBullet(std::move(newBullet));
}

void Enemy::FirePlayerAngle()
{
	//���@���Z�b�g����Ă��Ȃ���Δ�����
	if (!player) { return; }
	//���W�����@����O�Ȃ甭�˂��Ȃ�
	if (GetWorldPos().z <= player->GetWorldPos().z) { return; }

	//�e�̑��x��ݒ�
	const float bulletSpeed = 1.0f;

	Vector3 playerPos = player->GetWorldPos();
	Vector3 enemyPos = GetWorldPos();
	Vector3 DifferenceVec = playerPos - enemyPos;
	DifferenceVec.normalize();
	Vector3 velocity = DifferenceVec * bulletSpeed;

	//�e�𐶐�
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(bulletModel, position, velocity));
	stageScene->AddEnemyBullet(std::move(newBullet));
}

void Enemy::DamageMode()
{
	//�_���[�W��Ԃ̎���
	const int damageTime = 20;
	damageTimer++;

	//�傫�������T�C�Y��߂�
	DamageSizeReturn();

	//�_���[�W�F�؂�ւ�
	DamageColorMode();

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (damageTimer >= damageTime) {
		//�_���[�W��Ԃ��I��
		isDamage = false;

		//�F�����ɖ߂��Ă���
		color = { 1, 1, 1, 1 };
	}
}

void Enemy::DamageSizeReturn()
{
	//�傫�������T�C�Y�����ɖ߂�����
	const float sizeReturnTime = 10;
	//�w�肵�����Ԉȏ�Ȃ甲����
	if (damageTimer > sizeReturnTime) { return; }
	const float time = damageTimer / sizeReturnTime;

	//�傫�������ɖ߂�
	scale = Easing::LerpVec3(damageSize, normalSize, time);
}

void Enemy::DamageColorMode()
{
	//�_���[�W�F�؂�ւ�����
	const int damageColorChangeTime = 2;

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (damageTimer % damageColorChangeTime == 0) {
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
}

void Enemy::DeadSmokeEffect(const float size, const int smokeStartTime)
{
	//���S�^�C�}�[�X�V
	deadTimer++;

	//�^�C�}�[���������o���܂ł̎��Ԉȉ��Ȃ甲����
	if (deadTimer < smokeStartTime) { return; }

	//���t���[���o���Ƒ����̂ŊԊu��ݒ�
	const int smokeInterval = 5;
	//�w�肵���Ԋu�ȊO�Ȃ甲����
	if (deadTimer % smokeInterval != 0) { return; }

	//�����p�[�e�B�N������
	ParticleEmitter::GetInstance()->BlackSmoke(position, size);
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
	stageScene->AddEnemyBreakEffect(std::move(newBreakEffect));
}

void Enemy::FrontOfScreenDelete()
{
	//�Q�[���J�������Z�b�g����Ă��Ȃ���Δ�����
	if (!gameCamera) { return; }

	//���W���J��������O(��ʊO��O)�܂ōs������폜
	const float flontOfScreenDiffence = 20;
	const float deletePos = gameCamera->GetPosition().z - flontOfScreenDiffence;
	if (position.z <= deletePos) {
		isDelete = true;
	}
}
