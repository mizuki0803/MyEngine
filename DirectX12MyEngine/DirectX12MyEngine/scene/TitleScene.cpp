#include "TitleScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Easing.h"
#include "ParticleEmitter.h"
#include <cassert>
#include <fstream>
#include <iomanip>

void TitleScene::Initialize()
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

	//�X�v���C�g���ʕ����̃C���X�^���X���擾
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	//�X�v���C�g�p�e�N�X�`���ǂݍ���
	spriteCommon->LoadTexture(1, "reticle.png");
	spriteCommon->LoadTexture(2, "HPGaugeIn.png");
	spriteCommon->LoadTexture(3, "HPGaugeOut.png");
	spriteCommon->LoadTexture(4, "bossHPGaugeIn.png");
	spriteCommon->LoadTexture(5, "bossHPGaugeOut.png");


	//obj���烂�f���f�[�^��ǂݍ���
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydome"));
	modelGround.reset(ObjModel::LoadFromOBJ("ground"));
	modelMountain.reset(ObjModel::LoadFromOBJ("mountain"));
	modelSphere.reset(ObjModel::LoadFromOBJ("sphere", true));
	modelFighter.reset(ObjModel::LoadFromOBJ("fighter", true));

	//���@����
	player.reset(TitlePlayer::Create(modelFighter.get(), { 0, 5, 0 }));

	//�J����������
	titleCamera.reset(new TitleCamera());
	titleCamera->SetTitlePlayer(player.get());
	titleCamera->Initialize();

	//�V������
	skydome.reset(Skydome::Create(modelSkydome.get()));

	//�n�ʐ���
	ground.reset(Ground::Create(modelGround.get()));

	//�w�i�p(�R)����
	for (int i = 0; i < 20; i++) {
		std::unique_ptr<Mountain> newMountain;
		newMountain.reset(Mountain::Create(modelMountain.get(), { -70, 0, 0 + (float)i * 40 }));
		mountains.push_back(std::move(newMountain));
	}
	for (int i = 0; i < 20; i++) {
		std::unique_ptr<Mountain> newMountain;
		newMountain.reset(Mountain::Create(modelMountain.get(), { 70, 0, 0 + (float)i * 40 }));
		mountains.push_back(std::move(newMountain));
	}

	//obj�I�u�W�F�N�g�ɃJ�������Z�b�g
	ObjObject3d::SetCamera(titleCamera.get());
	//obj�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	ObjObject3d::SetLightGroup(lightGroup.get());

	//�p�[�e�B�N���ɃJ�������Z�b�g
	ParticleManager::SetCamera(titleCamera.get());
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

	//���@����܂ōs���Č����Ȃ��Ȃ�����
	const bool isPlayerSky = (player->GetPosition().y >= 500);
	if (isPlayerSky) {
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("GAME");
	}

	if (input->TriggerKey(DIK_RETURN)) {
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}

void TitleScene::Draw()
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


	//�p�[�e�B�N�����ʃR�}���h
	ParticleManager::DrawPrev();
	///-------�p�[�e�B�N���`�悱������-------///

	//�p�[�e�B�N���`��
	ParticleEmitter::GetInstance()->DrawAll();

	///-------�p�[�e�B�N���`�悱���܂�-------///
}

void TitleScene::SortieStart()
{
	//���ɏo�����Ă����甲����
	if (player->GetIsSortie()) { return; }
	//�X�y�[�X�L�[�������Ă��Ȃ���Δ�����
	if (!Input::GetInstance()->TriggerKey(DIK_SPACE)) { return; }

	//���@�ƃJ�������o����Ԃɂ���
	player->SetIsSortie(true);
	titleCamera->SortieStart();
}
