#pragma once
#include <Windows.h>
#include "DirectXBase.h"
#include <DirectXMath.h>
#include "PipelineSet.h"
#include "ObjModel.h"
#include "LightGroup.h"
#include "Camera.h"

/// <summary>
/// obj3Dオブジェクト
/// </summary>
class ObjObject3d
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: //サブクラス
	//定数バッファ用データ構造体B0
	struct ConstBufferDataB0
	{
		XMMATRIX viewproj;	//ビュープロジェクション行列
		XMMATRIX world;		//ワールド行列
		XMFLOAT3 cameraPos;	//カメラ座標(ワールド座標)
	};


public: //静的メンバ関数
	/// <summary>
	/// Object3d共通部分の初期化
	/// </summary>
	/// <param name="dev">デバイス</param>
	/// <param name="cmdList">コマンドリスト</param>
	static void Object3dCommon(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// パイプライン生成
	/// </summary>
	static void CreatePipeline();

	/// <summary>
	/// 描画前処理
	/// </summary>
	static void DrawPrev();

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>object3d</returns>
	static ObjObject3d* Create(ObjModel* model);


public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	virtual bool Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	//getter
	const XMFLOAT3& GetPosition() { return position; }
	const XMFLOAT3& GetRotation() { return rotation; }
	const XMFLOAT3& GetScale() { return scale; }

	//setter
	void SetPosition(const XMFLOAT3& position) { this->position = position; }
	void SetRotation(const XMFLOAT3& rotation) { this->rotation = rotation; }
	void SetScale(const XMFLOAT3& scale) { this->scale = scale; }
	void SetModel(ObjModel* model) { this->model = model; }
	static void SetLightGroup(LightGroup* lightGroup) { ObjObject3d::lightGroup = lightGroup; }
	static void SetCamera(Camera* camera) { ObjObject3d::camera = camera; }

protected: //静的メンバ変数
	//デバイス
	static ID3D12Device* dev;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//パイプラインセット
	static PipelineSet pipelineSet;
	//ライト
	static LightGroup* lightGroup;
	//カメラ
	static Camera* camera;

protected: //メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource> constBuffB0;
	//アフィン変換情報
	XMFLOAT3 scale = { 1, 1, 1 };
	XMFLOAT3 rotation = { 0, 0, 0 };
	XMFLOAT3 position = { 0, 0, 0 };
	XMFLOAT4 color = { 1, 1, 1, 1 };
	//ワールド変換行列
	XMMATRIX matWorld;
	//親子構造
	ObjObject3d* parent = nullptr;
	//モデル
	ObjModel* model = nullptr;
};