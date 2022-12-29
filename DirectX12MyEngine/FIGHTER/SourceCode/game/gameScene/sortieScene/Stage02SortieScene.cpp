#include "Stage02SortieScene.h"
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

void Stage02SortieScene::Initialize()
{
	//���C�g����
	lightGroup.reset(LightGroup::Create());
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, false);

	//obj���烂�f���f�[�^��ǂݍ���
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydomeStage02"));
	modelFighter.reset(ObjModel::LoadFromOBJ("fighter"));

	//�|�X�g�G�t�F�N�g�̃u���[���������Ă���
	GamePostEffect::GetPostEffect()->SetRadialBlur(false);

	//���@����
	player.reset(Stage02SortiePlayer::Create(modelFighter.get(), { 0, 20, -1000 }));

	//�J����������
	sortieCamera.reset(new Stage02SortieCamera());
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

void Stage02SortieScene::Update()
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

	//�p�[�e�B�N���X�V
	ParticleEmitter::GetInstance()->Update();


	//���@�̏o���s��������������
	if (player->GetIsSortieEnd()) {
		//�X�e�[�W02�փV�[���ύX���J�n����
		SceneChangeStart({ 1,1,1,0 }, 10, 0, 20, "STAGE02");
	}
	//���[�r�[�X�L�b�v�@�\
	MovieSkip();

	//�V�[���ύX���
	SceneChangeMode();

	//�V�[���ύX���o�X�V
	SceneChangeEffect::Update();
}

void Stage02SortieScene::DrawBackSprite()
{
}

void Stage02SortieScene::Draw3D()
{
	//Object3d���ʃR�}���h
	ObjObject3d::DrawPrev();
	///-------Object3d�`�悱������-------///

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

void Stage02SortieScene::Draw3DLightView()
{
}

void Stage02SortieScene::Draw3DTopLightView()
{
	//Object3d���ʃR�}���h
	ObjObject3d::DrawLightViewPrev();
	///-------Object3d�`�悱������-------///

	//���@
	player->DrawTopLightCameraView();

	///-------Object3d�`�悱���܂�-------///
}

void Stage02SortieScene::DrawFrontSprite()
{
	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->DrawPrev();
	///-------�X�v���C�g�`�悱������-------///

	//�V�[���ύX���o�`��
	SceneChangeEffect::Draw();


	///-------�X�v���C�g�`�悱���܂�-------///
}

void Stage02SortieScene::LightCameraUpdate()
{
	//�^�[�Q�b�g�ɂȂ���W
	const Vector3 targetPos = sortieCamera->GetEye();
	//�^�[�Q�b�g�Ǝ��_�̋���
	const Vector3 targetDistance = { -300, 200, -150 };
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

void Stage02SortieScene::SortieAction()
{
	//���Ƀu�[�X�g��ԂȂ甲����
	if (player->GetSortieActionPhase() == Stage02SortiePlayer::Stage02SortieActionPhase::Boost) { return; }
	//�J�����̎��@�Y�[�����I�����Ă��Ȃ���Δ�����
	if (!sortieCamera->GetIsZoomEnd()) { return; }

	//���@���u�[�X�g��Ԃɂ���
	player->BoostStart();
}

void Stage02SortieScene::MovieSkip()
{
	//�X�L�b�v���͂��Ȃ���Δ�����
	if (!(Input::GetInstance()->TriggerKey(DIK_RETURN) || Input::GetInstance()->TriggerGamePadButton(Input::PAD_START))) { return; }

	//�X�e�[�W02�փV�[���ύX���J�n����
	SceneChangeStart({ 0,0,0,0 }, 15, 0, 15, "STAGE02");
}
