#include "GameMountainManager.h"

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

void GameMountainManager::Update(const Vector3& targetPos)
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

	//�I�u�W�F�N�g�X�V
	for (const std::unique_ptr<Mountain>& mountain : mountains) {
		mountain->Update();

		//�R�ƑΏۂ̋������r����O�܂ōs������폜
		mountain->FrontOfScreenDelete(targetPos);
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
	//�V���ȎR����
	std::unique_ptr<Mountain> newMountainLeft;
	newMountainLeft.reset(Mountain::Create(model, { -distance, 0, 0 + (float)mountainNum * 40 }));
	mountains.push_back(std::move(newMountainLeft));
	std::unique_ptr<Mountain> newMountainRight;
	newMountainRight.reset(Mountain::Create(model, { distance, 0, 0 + (float)mountainNum * 40 }));
	mountains.push_back(std::move(newMountainRight));

	//�R�̔ԍ����X�V
	mountainNum++;

	//�R�̐ݒu��s�\�ɂ���
	isCanCreate = false;
}
