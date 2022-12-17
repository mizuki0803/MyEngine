#include "GameBuildingManager.h"
#include "Player.h"
#include "GameCamera.h"

std::array<ObjModel*, 2> GameBuildingManager::buildingModels;
Player* GameBuildingManager::player = nullptr;
GameCamera* GameBuildingManager::gameCamera = nullptr;
bool GameBuildingManager::isScrollMode = false;

GameBuildingManager* GameBuildingManager::Create(float centerDistance, float objectDistance, int startNum, const Vector3& basePos)
{
	//�Q�[���Ŏg���r���Ǘ��̃C���X�^���X�𐶐�
	GameBuildingManager* gameBuildingManager = new GameBuildingManager();
	if (gameBuildingManager == nullptr) {
		return nullptr;
	}

	// ������
	if (!gameBuildingManager->Initialize(centerDistance, objectDistance, startNum, basePos)) {
		delete gameBuildingManager;
		assert(0);
		return nullptr;
	}

	return gameBuildingManager;
}

void GameBuildingManager::SetBuidingModel(int modelNum, ObjModel* model)
{
	//�r�����f�����Z�b�g
	assert(model);
	buildingModels[modelNum] = model;
}

bool GameBuildingManager::Initialize(float centerDistance, float objectDistance, int startNum, const Vector3& basePos)
{
	//��̍��W���Z�b�g
	this->basePos = basePos;
	//���S����̋������Z�b�g
	this->centerDistance = centerDistance;
	//�I�u�W�F�N�g���m�̋������Z�b�g
	this->objectDistance = objectDistance;
	//�r���̔ԍ����Z�b�g
	buildingNum = startNum;

	//�������炠��r�����Z�b�g
	for (int i = 0; i < buildingNum; i++) {
		std::unique_ptr<Building> newBuilding;
		const int leftModelNum = NextCreateModelNum(lastLeftSetModelNum); //�����ɐݒu���郂�f���ԍ����擾
		newBuilding.reset(Building::Create(buildingModels[leftModelNum], { basePos.x - centerDistance, basePos.y, basePos.z + (float)i * objectDistance }));
		//�����̓I�u�W�F�N�g�𔽓]������
		newBuilding->SetRotation({ 0, 180, 0 });
		buildings.push_back(std::move(newBuilding));
	}
	for (int i = 0; i < buildingNum; i++) {
		std::unique_ptr<Building> newBuilding;
		const int rightModelNum = NextCreateModelNum(lastRightSetModelNum); //�E���ɐݒu���郂�f���ԍ����擾
		newBuilding.reset(Building::Create(buildingModels[rightModelNum], { basePos.x + centerDistance, basePos.y, basePos.z + (float)i * objectDistance }));
		buildings.push_back(std::move(newBuilding));
	}

	return true;
}

void GameBuildingManager::Update()
{
	//�r���̐ݒu���\�ɂ���
	isCanCreate = true;

	//�r���̎��S�O����
	for (const std::unique_ptr<Building>& building : buildings) {
		//�V���Ƀr���̐ݒu���\�łȂ���Δ�����
		if (!isCanCreate) { break; }
		//�r�������S���Ă��Ȃ���Δ�΂�
		if (!building->GetIsDead()) { continue; }

		//�V���ȃr���ݒu
		CreateNewBuilding();
	}
	//���S�����r���̍폜
	buildings.remove_if([](std::unique_ptr<Building>& sprite) {
		return sprite->GetIsDead();
		});

	//�X�N���[�����
	ScrollMode();

	for (const std::unique_ptr<Building>& building : buildings) {
		//�I�u�W�F�N�g�X�V
		building->Update();

		//�r���ƑΏۂ̋������r����O�܂ōs������폜
		building->FrontOfScreenDelete(gameCamera->GetPosition());
	}
}

void GameBuildingManager::Draw()
{
	//�I�u�W�F�N�g�`��
	for (const std::unique_ptr<Building>& building : buildings) {
		building->Draw();
	}
}

void GameBuildingManager::DrawLightCameraView()
{
	//�e�p�������C�g���猩�����_�ł̕`��
	for (const std::unique_ptr<Building>& building : buildings) {
		building->DrawLightCameraView();
	}
}

void GameBuildingManager::CreateNewBuilding()
{
	//��Ԍ��̃r���̍��W���擾
	std::list<std::unique_ptr<Building>>::iterator itr = buildings.end();
	--itr;
	Building* lastBuilding = itr->get();

	//�V���ȃr������
	std::unique_ptr<Building> newBuildingLeft;
	const int leftModelNum = NextCreateModelNum(lastLeftSetModelNum);
	newBuildingLeft.reset(Building::Create(buildingModels[leftModelNum], { basePos.x - centerDistance, basePos.y, lastBuilding->GetPosition().z + objectDistance }));
	//�����̓I�u�W�F�N�g�𔽓]������
	newBuildingLeft->SetRotation({ 0, 180, 0 });
	buildings.push_back(std::move(newBuildingLeft));
	std::unique_ptr<Building> newBuildingRight;
	const int rightModelNum = NextCreateModelNum(lastRightSetModelNum);
	newBuildingRight.reset(Building::Create(buildingModels[rightModelNum], { basePos.x + centerDistance, basePos.y, lastBuilding->GetPosition().z + objectDistance }));
	buildings.push_back(std::move(newBuildingRight));

	//�r���̔ԍ����X�V
	buildingNum++;

	//�r���̐ݒu��s�\�ɂ���
	isCanCreate = false;
}

int GameBuildingManager::NextCreateModelNum(int& lastSetModelNum)
{
	//�ݒu���郂�f���ԍ�
	int createModelNum = lastSetModelNum;

	//�ݒu���郂�f���ԍ��������_���Őݒ�(�O��Ɠ������f���͑I�΂Ȃ��悤�ɂ���)
	while (createModelNum == lastSetModelNum) {
		createModelNum = rand() % (int)buildingModels.size();
	}

	//����I�����ꂽ���f���ԍ���O��̃��f���ԍ��ɃZ�b�g���X�V
	lastSetModelNum = createModelNum;

	return createModelNum;
}

void GameBuildingManager::ScrollMode()
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
	for (const std::unique_ptr<Building>& building : buildings) {
		Vector3 pos = building->GetPosition();
		pos.z -= moveSpeed;
		building->SetPosition(pos);
	}
}
