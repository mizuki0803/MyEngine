#include "SceneManager.h"
#include <cassert>

SceneManager::~SceneManager()
{
	//�Ō�̃V�[���̏I���Ɖ��
	delete scene;
}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;

	return &instance;
}

void SceneManager::Update()
{
	//�V�[���؂�ւ�������Ȃ�
	if (nextScene)
	{
		//���V�[��������Ȃ�
		if (scene)
		{
			//���V�[���̏I��
			delete scene;
		}
		//�V�[����؂�ւ�
		scene = nextScene;
		//���̃V�[���͋�ɂ��Ă���
		nextScene = nullptr;

		//�V�����V�[��������
		scene->Initialize();
	}

	//���t���[���X�V
	scene->Update();
}

void SceneManager::Draw()
{
	//�`��
	scene->Draw();
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory);
	assert(nextScene == nullptr);

	//���̃V�[�����Z�b�g
	nextScene = sceneFactory->CreateScene(sceneName);
}
