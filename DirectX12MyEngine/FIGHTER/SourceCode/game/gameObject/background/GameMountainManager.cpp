#include "GameMountainManager.h"
#include "Player.h"
#include "GameCamera.h"

Player* GameMountainManager::player = nullptr;
GameCamera* GameMountainManager::gameCamera = nullptr;
bool GameMountainManager::isScrollMode = false;

GameMountainManager* GameMountainManager::Create(ObjModel* model, float distance, int startNum)
{
	//�R�̃C���X�^���X�𐶐�
	GameMountainManager* gameMountainManager = new GameMountainManager();
	if (gameMountainManager == nullptr) {
		return nullptr;
	}

	// ������
	if (!gameMountainManager->Initialize(model, distance, startNum)) {
		delete gameMountainManager;
		assert(0);
		return nullptr;
	}

	return gameMountainManager;
}

bool GameMountainManager::Initialize(ObjModel* model, float distance, int startNum)
{
	//���f�����Z�b�g
	assert(model);
	this->model = model;
	//���S����̋������Z�b�g
	this->distance = distance;
	//�R�̔ԍ����Z�b�g
	mountainNum = startNum;

	//�������炠��R���Z�b�g
	for (int i = 0; i < mountainNum; i++) {
		std::unique_ptr<Mountain> newMountain;
		newMountain.reset(Mountain::Create(model, { -distance, 0, 0 + (float)i * 40 }));
		mountains.push_back(std::move(newMountain));
	}
	for (int i = 0; i < mountainNum; i++) {
		std::unique_ptr<Mountain> newMountain;
		newMountain.reset(Mountain::Create(model, { distance, 0, 0 + (float)i * 40 }));
		mountains.push_back(std::move(newMountain));
	}

	return true;
}

void GameMountainManager::Update()
{
	//�R�̐ݒu���\�ɂ���
	isCanCreate = true;

	//�R�̎��S�O����
	for (const std::unique_ptr<Mountain>& mountain : mountains) {
		//�V���ɎR�̐ݒu���\�łȂ���Δ�����
		if (!isCanCreate) { break; }
		//�R�����S���Ă��Ȃ���Δ�΂�
		if (!mountain->GetIsDead()) { continue; }

		//�V���ȎR�ݒu
		CreateNewMountain();
	}
	//���S�����R�̍폜
	mountains.remove_if([](std::unique_ptr<Mountain>& sprite) {
		return sprite->GetIsDead();
		});

	//�X�N���[�����
	ScrollMode();

	for (const std::unique_ptr<Mountain>& mountain : mountains) {
		//�I�u�W�F�N�g�X�V
		mountain->Update();

		//�R�ƑΏۂ̋������r����O�܂ōs������폜
		mountain->FrontOfScreenDelete(gameCamera->GetPosition());
	}
}

void GameMountainManager::Draw()
{
	//�I�u�W�F�N�g�`��
	for (const std::unique_ptr<Mountain>& mountain : mountains) {
		mountain->Draw();
	}
}

void GameMountainManager::DrawLightCameraView()
{
	//�e�p�������C�g���猩�����_�ł̕`��
	for (const std::unique_ptr<Mountain>& mountain : mountains) {
		mountain->DrawLightCameraView();
	}
}

void GameMountainManager::CreateNewMountain()
{
	//��Ԍ��̎R�̍��W���擾
	std::list<std::unique_ptr<Mountain>>::iterator itr = mountains.end();
	--itr;
	Mountain* lastMountain = itr->get();

	//�V���ȎR����
	const float mountainSize = 40;
	std::unique_ptr<Mountain> newMountainLeft;
	newMountainLeft.reset(Mountain::Create(model, { -distance, 0, lastMountain->GetPosition().z + mountainSize }));
	mountains.push_back(std::move(newMountainLeft));
	std::unique_ptr<Mountain> newMountainRight;
	newMountainRight.reset(Mountain::Create(model, { distance, 0, lastMountain->GetPosition().z + mountainSize }));
	mountains.push_back(std::move(newMountainRight));

	//�R�̔ԍ����X�V
	mountainNum++;

	//�R�̐ݒu��s�\�ɂ���
	isCanCreate = false;
}

void GameMountainManager::ScrollMode()
{
	//�X�N���[����ԂłȂ���Δ�����
	if (!isScrollMode) { return; }
	//���@�ƃJ�����̃C���X�^���X���Ȃ���Δ�����
	if (!player || !gameCamera) { return; }

	//���@�̈ړ����x��Ԃɂ���ĎR���J�����ňړ����Ă������x�œ�����
	float moveSpeed = GameCamera::GetAdvanceSpeed();
	if (player->GetMoveSpeedPhase() == Player::MoveSpeedPhase::HighSpeed) { moveSpeed *= GameCamera::GetHighSpeedMagnification(); }
	else if (player->GetMoveSpeedPhase() == Player::MoveSpeedPhase::SlowSpeed) { moveSpeed *= GameCamera::GetSlowSpeedMagnification(); }

	//�v�Z�����ړ����x�ňړ�������
	for (const std::unique_ptr<Mountain>& mountain : mountains) {
		Vector3 pos = mountain->GetPosition();
		pos.z -= moveSpeed;
		mountain->SetPosition(pos);
	}
}
