#include "GameScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Easing.h"
#include <cassert>
#include <sstream>
#include <iomanip>

using namespace DirectX;


void GameScene::Initialize()
{
	//オーディオのインスタンスを取得
	Audio* audio = Audio::GetInstance();
	//サウンドデータ読み込み
	audio->LoadWave("BGM.wav");
	audio->LoadWave("BossDamage.wav");
	//audio->LoadWave("Shot.wav");

	//音全体のボリューム変更
	audio->ChangeVolume(soundVol);
	audio->PlayWave("BGM.wav", true);


	//カメラ初期化
	camera = new Camera();
	camera->Initialize();


	//ライト生成
	lightGroup = LightGroup::Create();

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


	//パーティクル生成
	ParticleManager::SetCamera(camera);
	particleMan = ParticleManager::Create();

	//スプライト共通部分のインスタンスを取得
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	//スプライト用テクスチャ読み込み
	spriteCommon->LoadTexture(1, "pad1.png");
	spriteCommon->LoadTexture(2, "mario.jpg");

	//スプライト生成
	sprite = Sprite::Create(2, { 0, 0 });
	sprite2 = Sprite::Create(1);

	//スプライト座標移動
	sprite->SetPosition({ 100, 100 });
	sprite2->SetPosition({ 800, 300 });
	//スプライト回転
	sprite2->SetRotation(90);
	//スプライトサイズ変更
	sprite->SetSize({ 100, 100 });
	sprite2->SetSize({ 428, 150 });
	//スプライトテクスチャサイズ変更
	sprite->SetTexSize({ 64, 478 });
	sprite2->SetTexSize({ 428, 150 });
	//スプライト色変更
	sprite2->SetColor({ 1, 0, 0, 0.8f });


	//objからモデルデータを読み込む
	modelMan = ObjModel::LoadFromOBJ("man");
	modelSkydome = ObjModel::LoadFromOBJ("skydome");
	modelGround = ObjModel::LoadFromOBJ("ground");
	modelSphere = ObjModel::LoadFromOBJ("sphere", true);
	modelFighter = ObjModel::LoadFromOBJ("fighter", true);

	//objオブジェクト生成
	objMan = ObjObject3d::Create(modelMan);
	objGround = ObjObject3d::Create(modelGround);
	objSkydome = ObjObject3d::Create(modelSkydome);
	objSphere = ObjObject3d::Create(modelSphere);

	//モデル割り当て
	/*objMan->SetModel(modelMan);
	objGround->SetModel(modelGround);
	objSkydome->SetModel(modelSkydome);
	objSphere->SetModel(modelSphere);*/

	//初期座標
	objMan->SetPosition({ 0, 1, 0 });
	objGround->SetPosition({ 0, -1, 0 });
	objSphere->SetPosition({ -1, 0, 0 });

	//角度初期値
	objMan->SetRotation({ 0, 90, 0 });

	//objオブジェクトにカメラをセット
	ObjObject3d::SetCamera(camera);
	//objオブジェクトにライトをセット
	ObjObject3d::SetLightGroup(lightGroup);


	//当たり判定 球 の初期値を設定
	sphere.center = XMVectorSet(0, 2, 0, 1); //中心点座標
	sphere.radius = 1.0f; //半径

	//当たり判定 平面 の初期値を設定
	plane.normal = XMVectorSet(0, 1, 0, 0); //法線ベクトル
	plane.distance = 0.0f; //原点(0, 0, 0)からの距離

	//当たり判定 三角形 の初期値を設定
	triangle.p0 = XMVectorSet(-1.0f, +1.0, -1.0f, 1);	//左手前
	triangle.p1 = XMVectorSet(-1.0f, +1.0, +1.0f, 1);	//左奥
	triangle.p2 = XMVectorSet(+1.0f, +1.0, -1.0f, 1);	//右手前
	triangle.normal = XMVectorSet(0.0f, 1.0f, 0.0f, 0);	//上向き

	//当たり判定 レイ の初期値を設定
	ray.start = XMVectorSet(0, 5, 0, 1);	//原点やや上
	ray.dir = XMVectorSet(0, -1, 0, 0);		//下向き
}

void GameScene::Finalize()
{
	//カメラ解放
	delete camera;

	//ライト解放
	delete lightGroup;

	//パーティクル解放
	delete particleMan;

	//スプライト解放
	delete sprite;
	delete sprite2;

	//モデル解放
	delete modelMan;
	delete modelSkydome;
	delete modelGround;
	delete modelSphere;
	delete modelFighter;

	//objオブジェクト解放
	delete objMan;
	delete objGround;
	delete objSkydome;
	delete objSphere;
}

void GameScene::Update()
{
	//入力のインスタンスを取得
	Input* input = Input::GetInstance();
	//デバッグテキストのインスタンスを取得
	DebugText* debugText = DebugText::GetInstance();

	if (input->TriggerKey(DIK_I))
	{
		Audio::GetInstance()->PlayWave("Shot.wav", false);
	}
	if (input->TriggerKey(DIK_0))
	{
		Audio::GetInstance()->StopWave("BGM.wav");
	}

	//モデル変更とスプライト変化
	if (input->PushKey(DIK_SPACE))
	{
		objMan->SetModel(modelFighter);

		sprite2->SetTexNumber(2);
		sprite->SetIsFlipX(false);
	}
	else
	{
		objMan->SetModel(modelMan);

		sprite2->SetTexNumber(1);
		sprite->SetIsFlipX(true);
	}

	//キー入力でプレイヤーの位置を変更
	if (input->PushKey(DIK_1) || input->PushKey(DIK_2) || input->PushKey(DIK_3) || input->PushKey(DIK_4) || input->PushKey(DIK_5) || input->PushKey(DIK_6))
	{
		XMFLOAT3 move = { 0, 0, 0 };
		if (input->PushKey(DIK_1)) { move.x += 0.1f; }
		if (input->PushKey(DIK_2)) { move.x -= 0.1f; }
		if (input->PushKey(DIK_3)) { move.y += 0.1f; }
		if (input->PushKey(DIK_4)) { move.y -= 0.1f; }
		if (input->PushKey(DIK_5)) { move.z += 0.1f; }
		if (input->PushKey(DIK_6)) { move.z -= 0.1f; }
		XMFLOAT3 playerPos = objMan->GetPosition();
		playerPos.x += move.x;
		playerPos.y += move.y;
		playerPos.z += move.z;
		objMan->SetPosition(playerPos);

		//カメラも同じ値を渡す。追従する
		camera->MoveVector(move);
	}

	//スプライトの座標や読み込み位置をキー入力でずらす
	if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT))
	{
		XMFLOAT2 pos = sprite->GetPosition();
		XMFLOAT2 leftTop = sprite->GetTexLeftTop();

		if (input->PushKey(DIK_RIGHT))
		{
			pos.x += 1.0f;
			leftTop.x += 1.0f;
		}

		if (input->PushKey(DIK_LEFT))
		{
			pos.x -= 1.0f;
			leftTop.x -= 1.0f;
		}

		sprite->SetPosition(pos);
		sprite->SetTexLeftTop(leftTop);
	}


	//レイ移動
	{
		XMVECTOR moveZ = XMVectorSet(0, 0, 0.01f, 0);
		if (input->PushKey(DIK_W))
		{
			ray.start += moveZ;
		}
		else if (input->PushKey(DIK_S))
		{
			ray.start -= moveZ;
		}

		XMVECTOR moveX = XMVectorSet(0.01f, 0, 0, 0);
		if (input->PushKey(DIK_D))
		{
			ray.start += moveX;
		}
		else if (input->PushKey(DIK_A))
		{
			ray.start -= moveX;
		}
	}
	//stringstreamで変数の値を埋め込んで整形する
	std::ostringstream raystr;
	raystr << "ray.start("
		<< std::fixed << std::setprecision(2)//小数点以下2桁まで
		<< ray.start.m128_f32[0] << ","	//x
		<< ray.start.m128_f32[1] << ","	//y
		<< ray.start.m128_f32[2] << ")",//z

		debugText->Print(raystr.str(), 50, 180, 1.0f);

	//レイと球の当たり判定
	XMVECTOR inter;
	float distance;
	bool hit = Collision::CheckRay2Sphere(ray, sphere, &distance, &inter);
	if (hit) {
		debugText->Print("HIT", 50, 260, 1.0f);

		//stringstreamで変数の値を埋め込んで整形する
		raystr.str("");
		raystr.clear();
		raystr << "inter:("
			<< std::fixed << std::setprecision(2)//小数点以下2桁まで
			<< inter.m128_f32[0] << ","	//x
			<< inter.m128_f32[1] << ","	//y
			<< inter.m128_f32[2] << ")",//z

			debugText->Print(raystr.str(), 50, 280, 1.0f);

		raystr.str("");
		raystr.clear();
		raystr << "distance:(" << std::fixed << std::setprecision(2) << distance << ")";

		debugText->Print(raystr.str(), 50, 300, 1.0f);
	}

	//オブジェクトを回転させる
	XMFLOAT3 sphereRot = objSphere->GetRotation();
	const float rotSpeed = 1.0f;
	sphereRot.y += rotSpeed;
	objSphere->SetRotation(sphereRot);
	

	//ライト更新
	lightGroup->Update();


	// カメラ移動
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A) || input->PushKey(DIK_E) || input->PushKey(DIK_C))
	{
		XMFLOAT3 move = { 0, 0, 0 };
		if (input->PushKey(DIK_W)) { move.y += 0.1f; }
		else if (input->PushKey(DIK_S)) { move.y -= 0.1f; }
		if (input->PushKey(DIK_D)) { move.x += 0.1f; }
		else if (input->PushKey(DIK_A)) { move.x -= 0.1f; }
		if (input->PushKey(DIK_E)) { move.z += 0.1f; }
		else if (input->PushKey(DIK_C)) { move.z -= 0.1f; }
		camera->MoveVector(move);
	}
	/*if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
	{
		XMFLOAT3 move = { 0, 0, 0 };
		if (input->PushKey(DIK_UP)) { move.y += 0.1f; }
		else if (input->PushKey(DIK_DOWN)) { move.y -= 0.1f; }
		if (input->PushKey(DIK_RIGHT)) { move.x += 0.1f; }
		else if (input->PushKey(DIK_LEFT)) { move.x -= 0.1f; }
		camera->MoveVector(move);
	}*/


	//色
	const XMFLOAT4 purple = { 1.0f, 0.0f, 1.0f, 0.8f }; //紫
	const XMFLOAT4 lightBlue = { 0.0f, 1.0f, 1.0f, 0.8f }; //水色
	//パーティクル更新
	for (int i = 0; i < 10; i++)
	{
		//X,Y,Z全て[-5.0f, +5.0f]でランダムに分布
		const float md_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		//X,Y,Z全て[-0.05f, +0.05f]でランダムに分布
		const float md_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//重力に見立ててYのみ[-0.001f, 0]でランダムに分布
		XMFLOAT3 acc{};
		const float md_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * md_acc;

		//追加
		particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f, purple, lightBlue);
	}


	//カメラのアングルを変更する
	if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
	{
		XMFLOAT3 angle = camera->GetAngle();
		if (input->PushKey(DIK_LEFT)) { angle.y += 1.0f; }
		if (input->PushKey(DIK_RIGHT)) { angle.y -= 1.0f; }
		if (input->PushKey(DIK_UP)) { angle.x += 1.0f; }
		if (input->PushKey(DIK_DOWN)) { angle.x -= 1.0f; }
		camera->SetAngle(angle);
	}

	//カメラの距離を変更する
	if (input->PushKey(DIK_M) || input->PushKey(DIK_N))
	{
		float dis = camera->GetDistance();
		if (input->PushKey(DIK_M)) { dis += 0.1f; }
		if (input->PushKey(DIK_N)) { dis -= 0.1f; }
		camera->SetDistance(dis);
	}

	//カメラ更新
	camera->Update();

	//Object3d更新
	objMan->Update();
	objGround->Update();
	objSkydome->Update();
	objSphere->Update();


	//スプライト更新
	sprite->Update();
	sprite2->Update();

	//パーティクル更新
	particleMan->Update();

	//デバックテキスト
	//X座標,Y座標,縮尺を指定して表示
	debugText->Print("GAME SCENE", 1000, 50);

	//デバッグ出力
	//sprintf_s(str, "%f\n", soundVol);
	//OutputDebugStringA(str);

	if (input->TriggerKey(DIK_RETURN))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
		Audio::GetInstance()->StopWave("BGM.wav");
	}
}

void GameScene::Draw()
{
	//Object3d共通コマンド
	ObjObject3d::DrawPrev();
	///-------Object3d描画ここから-------///


	objMan->Draw();
	objGround->Draw();
	objSkydome->Draw();
	objSphere->Draw();


	///-------Object3d描画ここまで-------///


	//スプライト共通コマンド
	SpriteCommon::GetInstance()->DrawPrev();
	///-------スプライト描画ここから-------///


	sprite->Draw();
	sprite2->Draw();


	///-------スプライト描画ここまで-------///


	//パーティクル共通コマンド
	ParticleManager::DrawPrev();
	///-------パーティクル描画ここから-------///


	//パーティクル描画
	particleMan->Draw();


	///-------パーティクル描画ここまで-------///
}
