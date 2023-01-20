#include "GameGroundManager.h"
#include "BasePlayer.h"
#include "BaseGameCamera.h"

BasePlayer* GameGroundManager::player = nullptr;
BaseGameCamera* GameGroundManager::gameCamera = nullptr;
const float GameGroundManager::modelScaleMagnification = 100.0f;

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
	//2���ڂ̒n�ʂ͈ꖇ�ڂɒn�ʂ̌��ɐڒn
	grounds[1]->SetPosition({ 0, 0, grounds[0]->GetPosition().z + grounds[0]->GetScale().z * modelScaleMagnification });

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
		if (grounds[i]->GetPosition().z + grounds[i]->GetScale().z * modelScaleMagnification <= swapPos) {
			if (i == 0) { grounds[0]->SetPosition({ 0, 0, grounds[1]->GetPosition().z + grounds[1]->GetScale().z * modelScaleMagnification }); }
			else if (i == 1) { grounds[1]->SetPosition({ 0, 0, grounds[0]->GetPosition().z + grounds[0]->GetScale().z * modelScaleMagnification }); }
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
	float moveSpeed = gameCamera->GetAdvanceSpeed();
	if (player->GetMoveSpeedPhase() == BasePlayer::MoveSpeedPhase::HighSpeed) { moveSpeed *= BaseGameCamera::GetHighSpeedMagnification(); }
	else if (player->GetMoveSpeedPhase() == BasePlayer::MoveSpeedPhase::SlowSpeed) { moveSpeed *= BaseGameCamera::GetSlowSpeedMagnification(); }

	//�v�Z�����ړ����x�ňړ�������
	for (int i = 0; i < 2; i++) {
		Vector3 pos = grounds[i]->GetPosition();
		pos.z -= moveSpeed;
		grounds[i]->SetPosition(pos);
	}
}
