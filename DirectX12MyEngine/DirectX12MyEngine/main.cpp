#include "AndosGame.h"
#include "fbxsdk.h"

//Windows�A�v���ł̃G���g���[�|�C���g�imain�֐��j
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	FbxManager* fbxManager = FbxManager::Create();

	FrameWork *game = new AndosGame();

	game->Run();

	delete game;

	return 0;
}