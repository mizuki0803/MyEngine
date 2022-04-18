#include "AndosGame.h"
#include "fbxsdk.h"

//Windowsアプリでのエントリーポイント（main関数）
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	FbxManager* fbxManager = FbxManager::Create();

	FrameWork *game = new AndosGame();

	game->Run();

	delete game;

	return 0;
}