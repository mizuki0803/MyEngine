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
	pointLightPos[0] = 0.5f;
	pointLightPos[1] = 1.0f;
	pointLightPos[2] = 0.0f;
	lightGroup->SetPointLightActive(0, false);
	lightGroup->SetPointLightActive(1, false);
	lightGroup->SetPointLightActive(2, false);
	//lightGroup->SetSpotLightActive(0, true);
	lightGroup->SetCircleShadowActive(0, true);

	//obj���烂�f���f�[�^��ǂݍ���
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydome"));
	modelGround.reset(ObjModel::LoadFromOBJ("ground"));
	modelMountain.reset(ObjModel::LoadFromOBJ("mountain"));
	modelSphere.reset(ObjModel::LoadFromOBJ("sphere", true));
	modelFighter.reset(ObjModel::LoadFromOBJ("fighter"));

	//���@����
	player.reset(SortiePlayer::Create(modelFighter.get(), { 0, 20, -1000 }));

	//�J����������
	sortieCamera.reset(new SortieCamera());
	sortieCamera->SetSortiePlayer(player.get());
	sortieCamera->Initialize();
	//�e�p�����J����������
	lightCamera.reset(new LightCamera());
	lightCamera->Initialize({ 0, 500, 0 });

	//�V������
	skydome.reset(Skydome::Create(modelSkydome.get()));

	//�n�ʐ���
	ground.reset(Ground::Create(modelGround.get()));

	//�w�i�p(�R)����
	for (int i = 0; i < 40; i++) {
		std::unique_ptr<Mountain> newMountain;
		newMountain.reset(Mountain::Create(modelMountain.get(), { -75, 0, -1000 + (float)i * 40 }));
		mountains.push_back(std::move(newMountain));
	}
	for (int i = 0; i < 40; i++) {
		std::unique_ptr<Mountain> newMountain;
		newMountain.reset(Mountain::Create(modelMountain.get(), { 75, 0, -1000 + (float)i * 40 }));
		mountains.push_back(std::move(newMountain));
	}

	//obj�I�u�W�F�N�g�ɃJ�������Z�b�g
	ObjObject3d::SetCamera(sortieCamera.get());
	ObjObject3d::SetLightCamera(lightCamera.get());
	//obj�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	ObjObject3d::SetLightGroup(lightGroup.get());

	//�p�[�e�B�N���ɃJ�������Z�b�g
	ParticleManager::SetCamera(sortieCamera.get());
	//��ʂɃp�[�e�B�N�����c�邱�Ƃ�����̂őS�č폜���Ă���
	ParticleEmitter::GetInstance()->AllDelete();

	//�|�X�g�G�t�F�N�g�̃u���[��������
	GamePostEffect::GetPostEffect()->SetRadialBlur(true);
	const float blurStrength = 0.2f;
	GamePostEffect::GetPostEffect()->SetRadialBlurStrength(blurStrength);
}

void SortieScene::Update()
{
	//�f�o�b�O�e�L�X�g�̃C���X�^���X���擾
	DebugText* debugText = DebugText::GetInstance();

	//�o�������Ǘ�
	SortieAction();

	//�J�����X�V
	sortieCamera->Update();
	lightCamera->Follow(player->GetPosition());
	lightCamera->Update();

	//�I�u�W�F�N�g�X�V
	//���@
	player->Update();
	//�V��
	skydome->Update();
	//�n��
	ground->Update();
	//�w�i�p(�R)
	for (const std::unique_ptr<Mountain>& mountain : mountains) {
		mountain->Update();
	}

	//�p�[�e�B�N���X�V
	ParticleEmitter::GetInstance()->Update();

	//�f�o�b�N�e�L�X�g
	//X���W,Y���W,�k�ڂ��w�肵�ĕ\��
	//debugText->Print("GAME SCENE", 1000, 50);

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
	//�w�i�p(�R)
	for (const std::unique_ptr<Mountain>& mountain : mountains) {
		mountain->Draw();
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

	//���@
	player->DrawLightCameraView();
	//�V��
	skydome->DrawLightCameraView();
	//�n��
	ground->DrawLightCameraView();
	//�w�i�p(�R)
	for (const std::unique_ptr<Mountain>& mountain : mountains) {
		mountain->DrawLightCameraView();
	}

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
