#include "FrameWork.h"
#include "Object3d.h"
#include "ParticleManager.h"
#include "LightGroup.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"

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
	win->WindowCreate(L"�N���X�����ē����˂�...");

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

	//�f�o�b�O�e�L�X�g�p�̃e�N�X�`���ԍ����w��
	const int debugTextNumber = 0;
	//�f�o�b�O�e�L�X�g������
	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextNumber);
	//�f�o�b�O�e�L�X�g�p�̃e�N�X�`���ǂݍ���
	spriteCommon->LoadTexture(debugTextNumber, "debugFont.png");

	//Object3d���ʏ���������
	Object3d::Object3dCommon(dxbase->GetDevice(), dxbase->GetCmdList());

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
}

void FrameWork::Finalize()
{
	//�V�[���H����
	delete sceneFactory;

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
	//�O���t�B�b�N�X�R�}���h(�O)
	dxbase->GraphicsCommandPrev();

	//�V�[���`��
	SceneManager::GetInstance()->Draw();

	//�X�v���C�g���ʃR�}���h
	spriteCommon->DrawPrev();
	//�f�o�b�N�e�L�X�g�`��
	debugText->DrawAll();

	//�O���t�B�b�N�X�R�}���h(��)
	dxbase->GraphicsCommandRear();
}
