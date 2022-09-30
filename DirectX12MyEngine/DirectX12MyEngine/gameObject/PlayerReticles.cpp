#include "PlayerReticles.h"

PlayerReticles* PlayerReticles::Create()
{
	//���@�t����2�̃��e�B�N������N���X�̃C���X�^���X�𐶐�
	PlayerReticles* playerReticles = new PlayerReticles();
	if (playerReticles == nullptr) {
		return nullptr;
	}

	// ������
	if (!playerReticles->Initialize()) {
		delete playerReticles;
		assert(0);
		return nullptr;
	}

	return playerReticles;
}

bool PlayerReticles::Initialize()
{
	//���e�B�N���𐶐�
	nearReticle.reset(PlayerNearReticle::Create(1, 15.0f, { 100, 100 }));
	farReticle.reset(PlayerFarReticle::Create(1, 25.0f, { 50, 50 }));

    return true;
}

void PlayerReticles::Update(const XMMATRIX& matWorld, const XMMATRIX& matView, const XMMATRIX& matProjection)
{
    //���e�B�N���X�V
	nearReticle->Update(matWorld, matView, matProjection);
    farReticle->Update(matWorld, matView, matProjection);
}

void PlayerReticles::Draw()
{
	//���e�B�N���`��
	nearReticle->Draw();
	farReticle->Draw();
}

void PlayerReticles::ChargeModeStart()
{
	//���e�B�N�����`���[�W��Ԃɂ���
	nearReticle->GetReticle2D()->ChargeModeStart();
	farReticle->GetReticle2D()->ChargeModeStart();
}

void PlayerReticles::ChargeModeEnd()
{
	nearReticle->GetReticle2D()->ChargeModeEnd();
	farReticle->GetReticle2D()->ChargeModeEnd();
}

void PlayerReticles::LockonEnemy(Enemy* enemy)
{
	//���e�B�N���̃`���[�W��Ԃ��I��������
	ChargeModeEnd();

	//�G�����b�N�I������
	farReticle->GetReticle2D()->LockonEnemy(enemy);
}

void PlayerReticles::UnlockonEnemy()
{
	//���e�B�N�����`���[�W��ԂȂ�
	if (farReticle->GetReticle2D()->GetIsChargeMode()) {
		//���e�B�N�����Ăу`���[�W��ԊJ�n��Ԃɂ���
		nearReticle->GetReticle2D()->ChargeModeStart();
		farReticle->GetReticle2D()->ChargeModeStart();
	}

	//���b�N�I��������
	farReticle->GetReticle2D()->UnlockonEnemy();
}
