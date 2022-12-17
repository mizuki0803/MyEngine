#include "GameGroundManager.h"
#include "Player.h"
#include "GameCamera.h"

Player* GameGroundManager::player = nullptr;
GameCamera* GameGroundManager::gameCamera = nullptr;
bool GameGroundManager::isScrollMode = false;

GameGroundManager* GameGroundManager::Create(ObjModel* model)
{
	//�Q�[���Ŏg���n�ʊǗ��̃C���X�^���X�𐶐�
	GameGroundManager* gameGroundManager = new GameGroundManager();
	if (gameGroundManager == nullptr) {
		return nullptr;
	}

	// ������
	if (!gameGroundManager->Initialize(model)) {
		delete gameGroundManager;
		assert(0);
		return nullptr;
	}

	return gameGroundManager;
}

bool GameGroundManager::Initialize(ObjModel* model)
{
	//���f�����Z�b�g
	assert(model);
	this->model = model;

	//�n�ʂ𐶐�
	for (int i = 0; i < 2; i++) {
		grounds[i].reset(Ground::Create(model));
	}
	grounds[1]->SetPosition({ 0, 0, grounds[0]->GetPosition().z + grounds[0]->GetScale().z * 100 });

	return true;
}

void GameGroundManager::Update()
{
	//�X�N���[�����
	ScrollMode();

	//���ɍs�����n�ʂ�O�Ɏ����Ă���
	SwapGroundPos();

	//�I�u�W�F�N�g�X�V
	for (int i = 0; i < 2; i++) {
		grounds[i]->Update();
	}
}

void GameGroundManager::Draw()
{
	//�I�u�W�F�N�g�`��
	for (int i = 0; i < 2; i++) {
		grounds[i]->Draw();
	}
}
void GameGroundManager::SwapGroundPos()
{
	//�J�����̃C���X�^���X���Ȃ���Δ�����
	if (!gameCamera) { return; }

	//���W�Ƒ傫���𑫂����l�����@����O(��ʊO��O)�܂ōs�������O�Ɏ����Ă���
	const float flontOfScreenDiffence = 50;
	const float swapPos = gameCamera->GetPosition().z - flontOfScreenDiffence;
	for (int i = 0; i < 2; i++) {
		if (grounds[i]->GetPosition().z + grounds[i]->GetScale().z * 100 <= swapPos) {
			if (i == 0) { grounds[0]->SetPosition({ 0, 0, grounds[1]->GetPosition().z + grounds[1]->GetScale().z * 100 }); }
			else if (i == 1) { grounds[1]->SetPosition({ 0, 0, grounds[0]->GetPosition().z + grounds[0]->GetScale().z * 100 }); }
		}
	}
}

void GameGroundManager::ScrollMode()
{
	//�X�N���[����ԂłȂ���Δ�����
	if (!isScrollMode) { return; }
	//���@�ƃJ�����̃C���X�^���X���Ȃ���Δ�����
	if (!player || !gameCamera) { return; }

	//���@�̈ړ����x��Ԃɂ���ăr�����J�����ňړ����Ă������x�œ�����
	float moveSpeed = GameCamera::GetAdvanceSpeed();
	if (player->GetMoveSpeedPhase() == Player::MoveSpeedPhase::HighSpeed) { moveSpeed *= GameCamera::GetHighSpeedMagnification(); }
	else if (player->GetMoveSpeedPhase() == Player::MoveSpeedPhase::SlowSpeed) { moveSpeed *= GameCamera::GetSlowSpeedMagnification(); }

	//�v�Z�����ړ����x�ňړ�������
	for (int i = 0; i < 2; i++) {
		Vector3 pos = grounds[i]->GetPosition();
		pos.z -= moveSpeed;
		grounds[i]->SetPosition(pos);
	}
}
