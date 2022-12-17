#include "TitleScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Easing.h"
#include "ParticleEmitter.h"
#include "SceneChangeEffect.h"
#include "GamePostEffect.h"
#include <cassert>
#include <fstream>
#include <iomanip>

void TitleScene::Initialize()
{
	//���C�g����
	lightGroup.reset(LightGroup::Create());
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, false);

	//obj���烂�f���f�[�^��ǂݍ���
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydomeBase"));
	modelFighter.reset(ObjModel::LoadFromOBJ("fighter"));
	modelbaseField.reset(ObjModel::LoadFromOBJ("baseField"));
	modelbaseField02.reset(ObjModel::LoadFromOBJ("baseField02"));
	modelRoad.reset(ObjModel::LoadFromOBJ("road"));
	modelWarehouse01.reset(ObjModel::LoadFromOBJ("warehouse01"));
	modelWarehouse02.reset(ObjModel::LoadFromOBJ("warehouse02", true));
	modelControlTower.reset(ObjModel::LoadFromOBJ("controlTower", true));
	modelControlRoom.reset(ObjModel::LoadFromOBJ("controlRoom", true));

	//�w�i�}�b�v���x���f�[�^����
	backgroundMapData.reset(LevelDataLoader::Create("titleMap.json"));
	backgroundMapData->InsertModel("fighter", modelFighter.get());
	backgroundMapData->InsertModel("baseField", modelbaseField.get());
	backgroundMapData->InsertModel("baseField02", modelbaseField02.get());
	backgroundMapData->InsertModel("road", modelRoad.get());
	backgroundMapData->InsertModel("controlTower", modelControlTower.get());
	backgroundMapData->InsertModel("controlRoom", modelControlRoom.get());
	backgroundMapData->InsertModel("warehouse01", modelWarehouse01.get());
	backgroundMapData->InsertModel("warehouse02", modelWarehouse02.get());
	backgroundMapData->CreateLevelDataObjects();

	//�|�X�g�G�t�F�N�g�̃u���[���������Ă���
	GamePostEffect::GetPostEffect()->SetRadialBlur(false);

	//���@����
	player.reset(TitlePlayer::Create(modelFighter.get(), { 0, 2, 0 }));

	//�J����������
	titleCamera.reset(new TitleCamera());
	titleCamera->SetTitlePlayer(player.get());
	titleCamera->Initialize();
	//�e�p�����J����������
	lightCamera.reset(new LightCamera());
	lightCamera->Initialize({ -100, 100, -300 });
	lightCamera->SetProjectionNum({ 400, 400 }, { -400, -400 });
	//���ォ��̉e�p�����J����������
	topLightCamera.reset(new LightCamera());
	topLightCamera->Initialize({ 0, 300, 10 });
	topLightCamera->SetProjectionNum({ 100, 100 }, { -100, -100 });

	//�V������
	skydome.reset(Skydome::Create(modelSkydome.get()));

	//obj�I�u�W�F�N�g�ɃJ�������Z�b�g
	ObjObject3d::SetCamera(titleCamera.get());
	ObjObject3d::SetLightCamera(lightCamera.get());
	ObjObject3d::SetTopLightCamera(topLightCamera.get());
	//obj�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	ObjObject3d::SetLightGroup(lightGroup.get());

	//�p�[�e�B�N���ɃJ�������Z�b�g
	ParticleManager::SetCamera(titleCamera.get());
	//��ʂɃp�[�e�B�N�����c�邱�Ƃ�����̂őS�č폜���Ă���
	ParticleEmitter::GetInstance()->AllDelete();

	//�^�C�g��UI����
	titleUI.reset(TitleUI::Create());
}

void TitleScene::Update()
{
	//���͂̃C���X�^���X���擾
	Input* input = Input::GetInstance();
	//�f�o�b�O�e�L�X�g�̃C���X�^���X���擾
	DebugText* debugText = DebugText::GetInstance();

	//�o���J�n
	SortieStart();


	//�J�����X�V
	titleCamera->Update();
	lightCamera->Update();
	topLightCamera->Update();

	//���C�g�X�V
	lightGroup->SetAmbientColor(XMFLOAT3(ambientColor0));
	lightGroup->SetDirLightDir(0, XMVECTOR({ lightDir0[0], lightDir0[1], lightDir0[2], 0 }));
	lightGroup->SetDirLightColor(0, XMFLOAT3(lightColor0));
	lightGroup->SetDirLightDir(1, XMVECTOR({ lightDir1[0], lightDir1[1], lightDir1[2], 0 }));
	lightGroup->SetDirLightColor(1, XMFLOAT3(lightColor1));
	lightGroup->Update();

	//�I�u�W�F�N�g�X�V
	//�w�i�}�b�v���x���f�[�^
	backgroundMapData->Update();
	//���@
	player->Update();
	//�V��
	skydome->Update();

	//�X�v���C�g�X�V
	//�^�C�g��UI�X�V
	titleUI->Update();


	//�p�[�e�B�N���X�V
	ParticleEmitter::GetInstance()->Update();

	//���@����܂ōs���Č����Ȃ��Ȃ�����
	const float stageChangePlayerPos = 550;
	const bool isPlayerSky = (player->GetPosition().y >= stageChangePlayerPos);
	if (isPlayerSky) {
		//�X�e�[�W�I���փV�[���ύX���J�n����
		SceneChangeStart({ 0,0,0,0 }, 40, 60, 60, "STAGESELECT");
	}
	//���[�r�[�X�L�b�v�@�\
	MovieSkip();

	//�V�[���ύX���
	SceneChangeMode();

	//�V�[���ύX���o�X�V
	SceneChangeEffect::Update();

	if (input->TriggerKey(DIK_Q)) {
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
	if (input->TriggerKey(DIK_W)) {
		SceneManager::GetInstance()->ChangeScene("SORTIE");
	}
}

void TitleScene::DrawBackSprite()
{
}

void TitleScene::Draw3D()
{
	//Object3d���ʃR�}���h
	ObjObject3d::DrawPrev();
	///-------Object3d�`�悱������-------///

	//�w�i�}�b�v���x���f�[�^
	backgroundMapData->Draw();
	//���@
	player->Draw();
	//�V��
	skydome->Draw();

	///-------Object3d�`�悱���܂�-------///

	///-------�p�[�e�B�N���`�悱������-------///

	//�p�[�e�B�N���`��
	ParticleEmitter::GetInstance()->DrawAll();

	///-------�p�[�e�B�N���`�悱���܂�-------///
}

void TitleScene::Draw3DLightView()
{
	//Object3d���ʃR�}���h
	ObjObject3d::DrawLightViewPrev();
	///-------Object3d�`�悱������-------///

	//�w�i�}�b�v���x���f�[�^
	backgroundMapData->DrawLightCameraView();

	///-------Object3d�`�悱���܂�-------///
}

void TitleScene::Draw3DTopLightView()
{
	//Object3d���ʃR�}���h
	ObjObject3d::DrawLightViewPrev();
	///-------Object3d�`�悱������-------///

	//���@
	player->DrawTopLightCameraView();

	///-------Object3d�`�悱���܂�-------///
}

void TitleScene::DrawFrontSprite()
{
	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->DrawPrev();
	///-------�X�v���C�g�`�悱������-------///

	//�^�C�g��UI
	if (titleUI) {
		titleUI->Draw();
	}

	//�V�[���ύX���o�`��
	SceneChangeEffect::Draw();

	///-------�X�v���C�g�`�悱���܂�-------///
}

void TitleScene::SortieStart()
{
	//���ɏo�����Ă����甲����
	if (player->GetIsSortie()) { return; }
	//�X�y�[�X�L�[�������Ă��Ȃ���Δ�����
	if (!(Input::GetInstance()->TriggerKey(DIK_SPACE) || Input::GetInstance()->TriggerGamePadButton(Input::PAD_B))) { return; }

	//���@�ƃJ�������o����Ԃɂ���
	player->SetIsSortie(true);
	titleCamera->SortieStart();

	//�^�C�g��UI�̕\�����I����
	titleUI->SetShowEnd();
}

void TitleScene::MovieSkip()
{
	//�o�����Ă��Ȃ���Δ�����
	if (!player->GetIsSortie()) { return; }
	//�X�L�b�v���͂��Ȃ���Δ�����
	if (!(Input::GetInstance()->TriggerKey(DIK_RETURN) || Input::GetInstance()->TriggerGamePadButton(Input::PAD_START))) { return; }

	//�X�e�[�W�I���փV�[���ύX���J�n����
	SceneChangeStart({ 0,0,0,0 }, 40, 60, 60, "STAGESELECT");
}
