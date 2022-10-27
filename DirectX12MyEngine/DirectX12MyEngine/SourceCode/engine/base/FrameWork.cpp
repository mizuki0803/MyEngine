#include "FrameWork.h"
#include "SpriteTextureLoader.h"
#include "ObjObject3d.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "LightGroup.h"

void FrameWork::Run()
{
	//�Q�[��������
	Initialize();

	while (true)	//�Q�[�����[�v
	{
		//���t���[���X�V
		Update();

		//�I�����N�G�X�g�������烋�[�v�𔲂���
		if (GetIsEndRequest())
		{
			break;
		}

		//�`��
		Draw();
	}

	//�Q�[���I��
	Finalize();
}

void FrameWork::Initialize()
{
	//�E�C���h�E�쐬
	win = new WindowApp();
	win->WindowCreate(L"FIGHTER");

	//DirectX������
	dxbase = new DirectXBase();
	dxbase->Initialize(win);;

	//���͂̏�����
	input = Input::GetInstance();
	input->Initialize(win);

	//����������
	audio = Audio::GetInstance();
	audio->Initialize();

	//�X�v���C�g���ʕ���������
	spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->Initialize(dxbase->GetDevice(), dxbase->GetCmdList(), win->window_width, win->window_height);
	//�X�v���C�g�e�N�X�`���S�ǂݍ���
	SpriteTextureLoader::TextureLoad();

	//�f�o�b�O�e�L�X�g������
	debugText = DebugText::GetInstance();
	debugText->Initialize(SpriteTextureLoader::DebugFont);

	//Object3d���ʏ���������
	PostEffect::PostEffectCommon(dxbase->GetDevice(), dxbase->GetCmdList());
	//�|�X�g�G�t�F�N�g�̏�����
	postEffect = PostEffect::Create();

	//obj�I�u�W�F�N�g3d���ʏ���������
	ObjObject3d::Object3dCommon(dxbase->GetDevice(), dxbase->GetCmdList());

	//FBXLoader������
	FbxLoader::GetInstance()->Initialize(dxbase->GetDevice());

	//FBX�I�u�W�F�N�g3d����
	FbxObject3d::SetDevice(dxbase->GetDevice());
	FbxObject3d::SetCommandList(dxbase->GetCmdList());
	//�O���t�B�b�N�X�p�C�v���C������
	FbxObject3d::CreateGraphicsPipeline();

	//���C�g���ʏ���������
	LightGroup::StaticInitialize(dxbase->GetDevice());

	//�p�[�e�B�N�����ʏ���������
	ParticleManager::ParticleManagerCommon(dxbase->GetDevice(), dxbase->GetCmdList());
	//�p�[�e�B�N���G�~�b�^�[������
	ParticleEmitter::GetInstance()->Initialize();
}

void FrameWork::Finalize()
{
	//�V�[���H����
	delete sceneFactory;

	//�|�X�g�G�t�F�N�g�̉��
	delete postEffect;

	//FBXLoader���
	FbxLoader::GetInstance()->Finalize();

	//audio���
	audio->Finalize();

	//DirectX���
	delete dxbase;

	//�E�C���h�E���
	win->WindowRelease();
	delete win;
	win = nullptr;
}

void FrameWork::Update()
{
	//���b�Z�[�W
	//�~�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
	if (win->MessageProc())
	{
		isEndRequest = true;
		return;
	}
	//�G�X�P�[�v�L�[�ŃQ�[�����[�v�I��
	if (input->PushKey(DIK_ESCAPE))
	{
		isEndRequest = true;
		return;
	}

	//���͂̍X�V
	input->Update();

	//�V�[���X�V
	SceneManager::GetInstance()->Update();
}

void FrameWork::Draw()
{
	//�����_�[�e�N�X�`���ւ̕`��
	//postEffect->DrawScenePrev();
	//SceneManager::GetInstance()->Draw();
	//postEffect->DrawSceneRear();

	//�O���t�B�b�N�X�R�}���h(�O)
	dxbase->GraphicsCommandPrev();

	//�|�X�g�G�t�F�N�g�̕`��
	//postEffect->Draw();

	//�V�[���`��
	SceneManager::GetInstance()->Draw();

	//�X�v���C�g���ʃR�}���h
	//spriteCommon->DrawPrev();
	////�f�o�b�N�e�L�X�g�`��
	//debugText->DrawAll();

	//�O���t�B�b�N�X�R�}���h(��)
	dxbase->GraphicsCommandRear();
}