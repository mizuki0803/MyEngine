#include "StageSelectScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Easing.h"
#include "ParticleEmitter.h"
#include "SceneChangeEffect.h"
#include <cassert>
#include <fstream>
#include <iomanip>

StageSelectScene::SelectStage StageSelectScene::selectStage = StageSelectScene::SelectStage::Title;

void StageSelectScene::Initialize()
{
	//���C�g����
	lightGroup.reset(LightGroup::Create());

	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);
	pointLightPos[0] = 0.5f;
	pointLightPos[1] = 1.0f;
	pointLightPos[2] = 0.0f;
	lightGroup->SetPointLightActive(0, false);
	lightGroup->SetPointLightActive(1, false);
	lightGroup->SetPointLightActive(2, false);
	//lightGroup->SetSpotLightActive(0, true);
	lightGroup->SetCircleShadowActive(0, true);

	//obj���烂�f���f�[�^��ǂݍ���
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydomeSpace"));
	modelGround.reset(ObjModel::LoadFromOBJ("ground"));
	modelMountain.reset(ObjModel::LoadFromOBJ("mountain"));
	modelSphere.reset(ObjModel::LoadFromOBJ("sphere", true));
	modelFighter.reset(ObjModel::LoadFromOBJ("fighter"));
	modelStageSelect.reset(ObjModel::LoadFromOBJ("stageSelect", true));
	modelPlanetBasis.reset(ObjModel::LoadFromOBJ("planetBasis", true));
	modelPlanetMautan.reset(ObjModel::LoadFromOBJ("planetMautan", true));
	modelPlanetSoon.reset(ObjModel::LoadFromOBJ("planetSoon", true));

	//�X�e�[�W�I���t�B�[���h����
	stageSelectField.reset(ObjObject3d::Create(modelStageSelect.get()));
	stageSelectField->SetScale({ 5, 5, 5 });

	//�f������
	CreatePlanets();

	//���@����
	player.reset(StageSelectPlayer::Create(modelFighter.get(), planets[(int)selectStage]->GetPosition()));
	player->GooutPlanetStart(stageSelectFieldPos[(int)selectStage]);

	//�J����������
	stageSelectCamera.reset(new StageSelectCamera());
	stageSelectCamera->SetStageSelectPlayer(player.get());
	stageSelectCamera->Initialize();
	stageSelectCamera->GooutPlayerLookActionStart(planets[(int)selectStage]->GetPosition(), stageSelectFieldPos[(int)selectStage]);

	//�V������
	skydome.reset(Skydome::Create(modelSkydome.get()));
	skydome->SetScale({ 10,10,10 });

	//obj�I�u�W�F�N�g�ɃJ�������Z�b�g
	ObjObject3d::SetCamera(stageSelectCamera.get());
	//obj�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	ObjObject3d::SetLightGroup(lightGroup.get());

	//�X�e�[�W�I���V�[��UI
	stageSelectUI.reset(StageSelectUI::Create());
	//�X�e�[�W����m�FUI����
	stageDecisionUI.reset(StageDecisionUI::Create());

	//�p�[�e�B�N���ɃJ�������Z�b�g
	ParticleManager::SetCamera(stageSelectCamera.get());
	//��ʂɃp�[�e�B�N�����c�邱�Ƃ�����̂őS�č폜���Ă���
	ParticleEmitter::GetInstance()->AllDelete();
}

void StageSelectScene::Update()
{
	//�f�o�b�O�e�L�X�g�̃C���X�^���X���擾
	DebugText* debugText = DebugText::GetInstance();

	//�f������o�Ă���s��
	GooutPlanet();
	//�X�e�[�W�I��
	StageSelect();
	//�X�e�[�W����
	StageDicision();
	//�f���ɓ���s��
	EnterPlanet();

	//�J�����X�V
	stageSelectCamera->Update();

	//�I�u�W�F�N�g�X�V
	//���@
	player->Update();
	//�f��
	for (const std::unique_ptr<StageSelectPlanet>& planet : planets) {
		planet->Update();
	}
	//�V��
	Vector3 rot = skydome->GetRotation();
	rot.y += 0.04f;
	skydome->SetRotation(rot);
	skydome->Update();
	//�X�e�[�W�I���t�B�[���h
	stageSelectField->Update();

	//�X�v���C�g�X�V
	//�X�e�[�W�I���V�[��UI
	stageSelectUI->Update();
	//�X�e�[�W����m�FUI
	stageDecisionUI->Update();

	//�p�[�e�B�N���X�V
	ParticleEmitter::GetInstance()->Update();


	//���@�̏o���s��������������
	//if (player->GetIsSortieEnd()) {
		//�Q�[���V�[���փV�[���ύX���J�n����
		//SceneChangeStart({ 1,1,1,0 }, 10, 0, 20, "GAME");
	//}
	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("GAME");
	}

	//�V�[���ύX���
	SceneChangeMode();

	//�V�[���ύX���o�X�V
	SceneChangeEffect::Update();
}

void StageSelectScene::DrawBackSprite()
{
}

void StageSelectScene::Draw3D()
{
	//Object3d���ʃR�}���h
	ObjObject3d::DrawPrev();
	///-------Object3d�`�悱������-------///

	//���@
	player->Draw();
	//�f��
	for (const std::unique_ptr<StageSelectPlanet>& planet : planets) {
		planet->Draw();
	}
	//�V��
	skydome->Draw();
	//�X�e�[�W�I���t�B�[���h
	stageSelectField->Draw();

	///-------Object3d�`�悱���܂�-------///


	//�p�[�e�B�N�����ʃR�}���h
	ParticleManager::DrawPrev();
	///-------�p�[�e�B�N���`�悱������-------///

	//�p�[�e�B�N���`��
	ParticleEmitter::GetInstance()->DrawAll();

	///-------�p�[�e�B�N���`�悱���܂�-------///
}

void StageSelectScene::DrawFrontSprite()
{
	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->DrawPrev();
	///-------�X�v���C�g�`�悱������-------///

	//�X�e�[�W�I���V�[��UI
	stageSelectUI->Draw();
	//�X�e�[�W����m�FUI
	stageDecisionUI->Draw();

	//�V�[���ύX���o�`��
	SceneChangeEffect::Draw();


	///-------�X�v���C�g�`�悱���܂�-------///
}

void StageSelectScene::CreatePlanets()
{
	//�X�e�[�W�I���t�B�[���h���W���Z�b�g
	const float fieldDistance = 20.0f;
	for (int i = 0; i < 3; i++) {
		Vector3 pos = { stageSelectField->GetScale().x * fieldDistance * i, 0, 0 };
		stageSelectFieldPos.push_back(pos);
	}

	//�f���ƃX�e�[�W�I���t�B�[���h�̋���
	const Vector3 planetFieldDistance = { 0, 0, 50 };

	//�^�C�g���p�f������
	std::unique_ptr<StageSelectPlanet> titlePlanet;
	titlePlanet.reset(StageSelectPlanet::Create(modelPlanetBasis.get(), stageSelectFieldPos[0] + planetFieldDistance));
	planets.push_back(std::move(titlePlanet));

	//�X�e�[�W01�p�f������
	std::unique_ptr<StageSelectPlanet> stage01Planet;
	stage01Planet.reset(StageSelectPlanet::Create(modelPlanetMautan.get(), stageSelectFieldPos[1] + planetFieldDistance));
	planets.push_back(std::move(stage01Planet));

	//�܂��Ȃ��X�e�[�W�p�f������
	std::unique_ptr<StageSelectPlanet> comingSoonPlanet;
	comingSoonPlanet.reset(StageSelectPlanet::Create(modelPlanetSoon.get(), stageSelectFieldPos[2] + planetFieldDistance));
	planets.push_back(std::move(comingSoonPlanet));
}

void StageSelectScene::GooutPlanet()
{
	//�J�������f������o�Ă��鎩�@������s����ԂłȂ���Δ�����
	if (!(stageSelectCamera->GetActionPhase() == StageSelectCamera::ActionPhase::GooutPlayerLook)) { return; }

	//���@�̘f������o�Ă���ړ����I�������
	if (!player->GetIsGooutPlanetEnd()) { return; }

	//�J�����̃X�e�[�W�I���J�n��Ԃ��J�n����
	stageSelectCamera->StageSelectStartActionStart();
	//�X�e�[�W�I���V�[��UI����ʓ��ɕ\��������
	stageSelectUI->TextInScreenStart((int)selectStage);
}

void StageSelectScene::StageSelect()
{
	//�X�e�[�W�I���m�F���Ȃ甲����
	if (isStageDecisionCheck) { return; }
	//���@���X�e�[�W�I����ԂłȂ���Δ�����
	if (!(player->GetActionPhase() == StageSelectPlayer::ActionPhase::StageSelect)) { return; }
	//�J�������X�e�[�W�I����ԂłȂ���Δ�����
	if (!(stageSelectCamera->GetActionPhase() == StageSelectCamera::ActionPhase::StageSelect)) { return; }
	//�X�e�[�W�I���Ŏ��@���ړ����Ȃ甲����
	if (player->GetIsStageSelectMove()) { return; }

	//���@�̈ړ����������ăX�e�[�W�I���V�[��UI��\�����Ă��Ȃ��ꍇ�͕\�����J�n����
	if (!stageSelectUI->GetIsShow()) { stageSelectUI->TextInScreenStart((int)selectStage); }

	//���͂̃C���X�^���X���擾
	Input* input = Input::GetInstance();

	//�X�e�B�b�N���ǂ��܂ŌX�����画����Ƃ邩
	const float stickNum = 200;
	//�E�����̓��͂��Ȃ���Δ�����
	const bool isInputRight = (input->PushKey(DIK_D) || input->TiltGamePadLStickX(stickNum) || input->PushGamePadButton(Input::PAD_RIGHT));
	const bool isInputLeft = (input->PushKey(DIK_A) || input->TiltGamePadLStickX(-stickNum) || input->PushGamePadButton(Input::PAD_LEFT));
	if (!(isInputRight || isInputLeft)) { return; }

	//�E�����ɓ��͂��Ă�����I������X�e�[�W���E�ɕύX
	if (isInputRight) { StageSelectChangeRight(); }
	//�������ɓ��͂��Ă�����I������X�e�[�W�����ɕύX
	else if (isInputLeft) { StageSelectChangeLeft(); }
}

void StageSelectScene::StageSelectChangeRight()
{
	//�ړ��ł�����
	bool isMove = false;

	//�I������X�e�[�W�ԍ���1�i�߂�
	if (selectStage == SelectStage::Title) { selectStage = SelectStage::Stage01; isMove = true; }
	else if (selectStage == SelectStage::Stage01) { selectStage = SelectStage::ComingSoon; isMove = true; }

	//�ړ��ł��Ȃ���Δ�����
	if (!isMove) { return; }

	//���@���X�e�[�W�I���ړ�������
	player->StageSelectMoveStart(stageSelectFieldPos[(int)selectStage], true);

	//�X�e�[�W�I���V�[��UI����ʊO�Ɉړ�������
	stageSelectUI->TextOutScreenStart();
}

void StageSelectScene::StageSelectChangeLeft()
{
	//�ړ��ł�����
	bool isMove = false;

	//�I������X�e�[�W�ԍ���1�߂�
	if (selectStage == SelectStage::ComingSoon) { selectStage = SelectStage::Stage01; isMove = true; }
	else if (selectStage == SelectStage::Stage01) { selectStage = SelectStage::Title; isMove = true; }

	//�ړ��ł��Ȃ���Δ�����
	if (!isMove) { return; }

	//���@���X�e�[�W�I���ړ�������
	player->StageSelectMoveStart(stageSelectFieldPos[(int)selectStage], false);

	//�X�e�[�W�I���V�[��UI����ʊO�Ɉړ�������
	stageSelectUI->TextOutScreenStart();
}

void StageSelectScene::StageDicision()
{
	//���͂̃C���X�^���X���擾
	Input* input = Input::GetInstance();

	//�X�e�[�W����m�F���łȂ����
	if (!isStageDecisionCheck) {
		//�I���Ă���X�e�[�W�����쒆�Ȃ甲����
		if (selectStage == SelectStage::ComingSoon) { return; }

		//���@���X�e�[�W�I����ԂłȂ���Δ�����
		if (!(player->GetActionPhase() == StageSelectPlayer::ActionPhase::StageSelect)) { return; }
		//�J�������X�e�[�W�I����ԂłȂ���Δ�����
		if (!(stageSelectCamera->GetActionPhase() == StageSelectCamera::ActionPhase::StageSelect)) { return; }
		//�X�e�[�W�I���Ŏ��@���ړ����Ȃ甲����
		if (player->GetIsStageSelectMove()) { return; }
		//������͂��Ȃ���Δ�����
		if (!(input->TriggerKey(DIK_SPACE) || input->TriggerGamePadButton(Input::PAD_B))) { return; }

		//�X�e�[�W����m�F���ɂ���
		isStageDecisionCheck = true;

		//�J�������X�e�[�W����m�F��Ԃɂ���
		stageSelectCamera->StageDecisionCheckActionStart();
		//�X�e�[�W�I���V�[��UI����ʊO�Ɉړ�������
		stageSelectUI->TextOutScreenStart();
		//�X�e�[�W����m�FUI����ʂɏo��
		stageDecisionUI->TextUpStart();
	}
	//�X�e�[�W����m�F���Ȃ�
	else {
		//�X�e�[�W����m�FUI���\����ԂłȂ���Δ�����
		if (!(stageDecisionUI->GetActionPhase() == StageDecisionUI::ActionPhase::Show)) { return; }

		//�X�e�[�W���肷�邩�I��
		SelectStageDicision();

		//������͂��Ȃ���Δ�����
		if (!(input->TriggerKey(DIK_SPACE) || input->TriggerGamePadButton(Input::PAD_B) || input->TriggerGamePadButton(Input::PAD_A))) { return; }

		//A���͂͊m��ŃX�e�[�W�ɓ���Ȃ�
		if (input->TriggerGamePadButton(Input::PAD_A)) {
			stageDecisionUI->ChangeStageDecision(false);

			//�X�e�[�W����m�F��������
			isStageDecisionCheck = false;
			//�X�e�[�W���肵�Ȃ���Ԃɂ��Ă���
			stageDecisionUI->ChangeStageDecision(false);
			//�X�e�[�W����m�FUI����ʊO�ɖ߂�
			stageDecisionUI->TextDownStart();
		}

		//�X�e�[�W����m�F��������
		isStageDecisionCheck = false;

		//�X�e�[�W����m�FUI����ʊO�ɖ߂�
		stageDecisionUI->TextDownStart();

		//�X�e�[�W����Ȃ玩�@�ƃJ������f���ɓ���s���ɐ؂�ւ���
		if (stageDecisionUI->GetIsStageDecision()) {
			player->EnterPlanetStart(planets[(int)selectStage]->GetPosition());
		}
		//�X�e�[�W���肵�Ȃ��Ȃ�
		else {
			//�J�������X�e�[�W�I����Ԃɖ߂�
			stageSelectCamera->ReturnSelectActionStart();
			//�X�e�[�W�I���V�[��UI����ʓ��ɕ\��������
			stageSelectUI->TextInScreenStart((int)selectStage);
		}
	}
}

void StageSelectScene::SelectStageDicision()
{
	//���͂̃C���X�^���X���擾
	Input* input = Input::GetInstance();

	//�X�e�B�b�N���ǂ��܂ŌX�����画����Ƃ邩
	const float stickNum = 200;
	//�E�����̓��͂��Ȃ���Δ�����
	const bool isInputUp = (input->PushKey(DIK_W) || input->TiltGamePadLStickY(-stickNum) || input->PushGamePadButton(Input::PAD_UP));
	const bool isInputDown = (input->PushKey(DIK_S) || input->TiltGamePadLStickY(stickNum) || input->PushGamePadButton(Input::PAD_DOWN));
	const bool isInputA = (input->TriggerGamePadButton(DIK_A));
	if (!(isInputUp || isInputDown)) { return; }

	//���͂Ő؂�ւ���
	if (isInputUp) { stageDecisionUI->ChangeStageDecision(true); }
	else if (isInputDown) { stageDecisionUI->ChangeStageDecision(false); }
}

void StageSelectScene::EnterPlanet()
{
	//���@���f���ɓ����ԂłȂ���Δ�����
	if (!(player->GetActionPhase() == StageSelectPlayer::ActionPhase::EnterPlanet)) { return; }

	//���@���X�e�[�W�I���ōs���S�Ă̍s�����I���Ă��Ȃ���Δ�����
	if (!player->GetIsStageSelectModeEnd()) { return; }

	//�V�[����؂�ւ���
	ChangeSelectScene();
}

void StageSelectScene::ChangeSelectScene()
{
	//�I�����Ă���X�e�[�W�ɉ����ăV�[���ύX
	if (selectStage == SelectStage::Title) { SceneChangeStart({ 0,0,0,0 }, 40, 60, 60, "TITLE"); }
	else if (selectStage == SelectStage::Stage01) { SceneChangeStart({ 0,0,0,0 }, 40, 60, 120, "SORTIE"); }
}
