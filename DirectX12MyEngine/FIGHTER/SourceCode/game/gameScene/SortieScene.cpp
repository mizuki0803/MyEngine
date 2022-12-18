#include "SortieScene.h"
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

void SortieScene::Initialize()
{
	//���C�g����
	lightGroup.reset(LightGroup::Create());
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);

	//obj���烂�f���f�[�^��ǂݍ���
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydome"));
	modelGround.reset(ObjModel::LoadFromOBJ("ground"));
	modelBuilding[0].reset(ObjModel::LoadFromOBJ("building01"));
	modelBuilding[1].reset(ObjModel::LoadFromOBJ("building02"));
	modelFighter.reset(ObjModel::LoadFromOBJ("fighter"));

	//�|�X�g�G�t�F�N�g�̃u���[���������Ă���
	GamePostEffect::GetPostEffect()->SetRadialBlur(false);

	//���@����
	player.reset(SortiePlayer::Create(modelFighter.get(), { 0, 20, -1000 }));

	//�J����������
	sortieCamera.reset(new SortieCamera());
	sortieCamera->SetSortiePlayer(player.get());
	sortieCamera->Initialize();
	//�e�p�����J����������
	lightCamera.reset(new LightCamera());
	lightCamera->Initialize({ 0, 500, 0 });
	lightCamera->SetProjectionNum({ 500, 500 }, { -500, -500 });
	//���ォ��̉e�p�����J����������
	topLightCamera.reset(new LightCamera());
	topLightCamera->Initialize({ 0, 500, 0 });
	topLightCamera->SetProjectionNum({ 100, 100 }, { -100, -100 });

	//�V������
	skydome.reset(Skydome::Create(modelSkydome.get()));
	//���h���������p�x�ɕύX���Ă���
	skydome->SetRotation({ 0, 180, 0 });

	//�n�ʐ���
	ground.reset(Ground::Create(modelGround.get()));

	//�w�i�p(�r��)����
	int createLeftModelNum = 0;
	for (int i = 0; i < 40; i++) {
		//�V�����r������
		std::unique_ptr<Building> newBuilding;
		newBuilding.reset(Building::Create(modelBuilding[createLeftModelNum].get(), { -85, 0, -1100 + (float)i * 25 }));
		//�����̓I�u�W�F�N�g�𔽓]������
		newBuilding->SetRotation({ 0, 180, 0 });
		buildings.push_back(std::move(newBuilding));

		//���̐ݒu�̂��߂Ƀ��f���ԍ���ύX����
		createLeftModelNum++;
		if (createLeftModelNum >= modelBuilding.size()) { createLeftModelNum = 0; }
	}
	int createRightModelNum = 1;
	for (int i = 0; i < 40; i++) {
		//�V�����r������
		std::unique_ptr<Building> newBuilding;
		newBuilding.reset(Building::Create(modelBuilding[createRightModelNum].get(), { 85, 0, -1100 + (float)i * 25 }));
		buildings.push_back(std::move(newBuilding));

		//���̐ݒu�̂��߂Ƀ��f���ԍ���ύX����
		createRightModelNum++;
		if (createRightModelNum >= modelBuilding.size()) { createRightModelNum = 0; }
	}

	//obj�I�u�W�F�N�g�ɃJ�������Z�b�g
	ObjObject3d::SetCamera(sortieCamera.get());
	ObjObject3d::SetLightCamera(lightCamera.get());
	ObjObject3d::SetTopLightCamera(topLightCamera.get());
	//obj�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	ObjObject3d::SetLightGroup(lightGroup.get());

	//�p�[�e�B�N���ɃJ�������Z�b�g
	ParticleManager::SetCamera(sortieCamera.get());
	//��ʂɃp�[�e�B�N�����c�邱�Ƃ�����̂őS�č폜���Ă���
	ParticleEmitter::GetInstance()->AllDelete();
}

void SortieScene::Update()
{
	//�f�o�b�O�e�L�X�g�̃C���X�^���X���擾
	DebugText* debugText = DebugText::GetInstance();

	//�o�������Ǘ�
	SortieAction();

	//�J�����X�V
	sortieCamera->Update();
	//�e�����p���C�g�J����
	LightCameraUpdate();

	//���C�g�X�V
	lightGroup->SetAmbientColor(XMFLOAT3(ambientColor0));
	lightGroup->SetDirLightDir(0, XMVECTOR({ lightDir0[0], lightDir0[1], lightDir0[2], 0 }));
	lightGroup->SetDirLightColor(0, XMFLOAT3(lightColor0));
	lightGroup->SetDirLightDir(1, XMVECTOR({ lightDir1[0], lightDir1[1], lightDir1[2], 0 }));
	lightGroup->SetDirLightColor(1, XMFLOAT3(lightColor1));
	lightGroup->Update();

	//�I�u�W�F�N�g�X�V
	//���@
	player->Update();
	//�V��
	skydome->Update();
	//�n��
	ground->Update();
	//�w�i�p(�r��)
	for (const std::unique_ptr<Building>& building : buildings) {
		building->Update();
	}

	//�p�[�e�B�N���X�V
	ParticleEmitter::GetInstance()->Update();


	//���@�̏o���s��������������
	if (player->GetIsSortieEnd()) {
		//�Q�[���V�[���փV�[���ύX���J�n����
		SceneChangeStart({ 1,1,1,0 }, 10, 0, 20, "GAME");
	}
	//���[�r�[�X�L�b�v�@�\
	MovieSkip();

	//�V�[���ύX���
	SceneChangeMode();

	//�V�[���ύX���o�X�V
	SceneChangeEffect::Update();
}

void SortieScene::DrawBackSprite()
{
}

void SortieScene::Draw3D()
{
	//Object3d���ʃR�}���h
	ObjObject3d::DrawPrev();
	///-------Object3d�`�悱������-------///

	//���@
	player->Draw();
	//�V��
	skydome->Draw();
	//�n��
	ground->Draw();
	//�w�i�p(�r��)
	for (const std::unique_ptr<Building>& building : buildings) {
		building->Draw();
	}

	///-------Object3d�`�悱���܂�-------///

	///-------�p�[�e�B�N���`�悱������-------///

	//�p�[�e�B�N���`��
	ParticleEmitter::GetInstance()->DrawAll();

	///-------�p�[�e�B�N���`�悱���܂�-------///
}

void SortieScene::Draw3DLightView()
{
	//Object3d���ʃR�}���h
	ObjObject3d::DrawLightViewPrev();
	///-------Object3d�`�悱������-------///

	//�w�i�p(�r��)
	for (const std::unique_ptr<Building>& building : buildings) {
		building->DrawLightCameraView();
	}

	///-------Object3d�`�悱���܂�-------///
}

void SortieScene::Draw3DTopLightView()
{
	//Object3d���ʃR�}���h
	ObjObject3d::DrawLightViewPrev();
	///-------Object3d�`�悱������-------///

	//���@
	player->DrawTopLightCameraView();

	///-------Object3d�`�悱���܂�-------///
}

void SortieScene::DrawFrontSprite()
{
	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->DrawPrev();
	///-------�X�v���C�g�`�悱������-------///

	//�V�[���ύX���o�`��
	SceneChangeEffect::Draw();


	///-------�X�v���C�g�`�悱���܂�-------///
}

void SortieScene::LightCameraUpdate()
{
	//�^�[�Q�b�g�ɂȂ���W
	const Vector3 targetPos = sortieCamera->GetEye();
	//�^�[�Q�b�g�Ǝ��_�̋���
	const Vector3 targetDistance = { -300, 200, -300 };
	//���C�g�J�����p�̎��_
	const Vector3 lightEye = targetPos + targetDistance;
	lightCamera->SetEyeTarget(lightEye, targetPos);
	lightCamera->Update();


	//���ォ��̃��C�g�J�����p�^�[�Q�b�g�Ǝ��_�̋���
	const Vector3 topCameraTargetDistance = { 0, 500, 350 };
	//���ォ��̃��C�g�J�����p�̎��_
	const Vector3 topLightEye = targetPos + topCameraTargetDistance;
	topLightCamera->SetEyeTarget(topLightEye, targetPos);
	topLightCamera->Update();
}

void SortieScene::SortieAction()
{
	//���Ƀu�[�X�g��ԂȂ甲����
	if (player->GetSortieActionPhase() == SortiePlayer::SortieActionPhase::Boost) { return; }
	//�J�����̎��@�Y�[�����I�����Ă��Ȃ���Δ�����
	if (!sortieCamera->GetIsZoomEnd()) { return; }

	//���@���u�[�X�g��Ԃɂ���
	player->BoostStart();
}

void SortieScene::MovieSkip()
{
	//�X�L�b�v���͂��Ȃ���Δ�����
	if (!(Input::GetInstance()->TriggerKey(DIK_RETURN) || Input::GetInstance()->TriggerGamePadButton(Input::PAD_START))) { return; }

	//�Q�[���V�[���փV�[���ύX���J�n����
	SceneChangeStart({ 0,0,0,0 }, 15, 0, 15, "GAME");
}
