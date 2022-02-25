#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include "DirectXBase.h"
#include <unordered_map>

/// <summary>
/// 3Dモデル
/// </summary>
class Model
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos;	//xyz座標
		XMFLOAT3 normal;//法線ベクトル
		XMFLOAT2 uv;	//uv座標
	};

	//定数バッファ用データ構造体B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;	//アンビエント係数
		float pad1;			//パディング
		XMFLOAT3 diffuse;	//ディフューズ係数
		float pad2;			//パディング
		XMFLOAT3 specular;	//スペキュラー係数
		float alpha;		//アルファ
	};

	//マテリアル
	struct Material
	{
		std::string name;	//マテリアル名
		XMFLOAT3 ambient;	//アンビエント影響度
		XMFLOAT3 diffuse;	//ディフューズ影響度
		XMFLOAT3 specular;	//スペキュラー影響度
		float alpha;	//アルファ
		std::string textureFilename;	//テクスチャファイル名
		//コンストラクタ
		Material() {
			ambient = { 0.3f, 0.3f, 0.3f };
			diffuse = { 0.0f, 0.0f, 0.0f };
			specular = { 0.0f, 0.0f, 0.0f };
			alpha = 1.0f;
		}
	};

public: //静的メンバ関数
	/// <summary>
	/// objファイルから3Dモデルを読み込む
	/// </summary>
	/// <param name="modelname">モデル名</param>
	/// <param name="smoothing">スムージングをかけるか</param>
	/// <returns></returns>
	static Model *LoadFromOBJ(const std::string &modelname, bool smoothing = false);

	//setter
	static void SetDevice(ID3D12Device *device) { Model::dev = device; };

private: //静的メンバ変数
	//デバイス
	static ID3D12Device *dev;


private: //非公開のメンバ関数
	//objファイルから3Dモデルを読み込む(非公開)

	/// <summary>
	/// OBJファイル読み込み
	/// </summary>
	/// <param name="modelname">モデル名</param>
	/// <param name="smoothing">スムージングをかけるか</param>
	void LoadFromOBJInternal(const std::string &modelname, bool smoothing);

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	/// <param name="directoryPath">パス</param>
	/// <param name="filename">ファイル名</param>
	void LoadMaterial(const std::string &directoryPath, const std::string &filename);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="directoryPath">パス</param>
	/// <param name="filename">ファイル名</param>
	void LoadTexture(const std::string &directoryPath, const std::string &filename);

	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	void InitializeDescHeap();

	/// <summary>
	/// 各種バッファ生成
	/// </summary>
	void CreateBuffers();

	/// <summary>
	/// 頂点データの数を取得
	/// </summary>
	/// <returns>頂点データの数</returns>
	inline size_t GetVertexCount() { return vertices.size(); }

	/// <summary>
	/// エッジ平滑化データの追加
	/// </summary>
	/// <param name="indexPosition">座標インデックス</param>
	/// <param name="indexVertex">頂点インデックス</param>
	void AddSmootgData(unsigned short indexPosition, unsigned short indexVertex);

	/// <summary>
	/// 平滑化された頂点法線の計算
	/// </summary>
	void CalculateSmoothedVertexNormals();


public: //メンバ関数

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	/// <param name="rootOaramIndexMaterial">ルートパラメータの数字</param>
	void Draw(ID3D12GraphicsCommandList *cmdList, UINT rootParamIndexMaterial);

private: //メンバ変数
	//テクスチャ用デスクリプタヒープの生成
	ComPtr<ID3D12DescriptorHeap> descHeap;
	//テクスチャリソース(テクスチャバッファ)
	ComPtr<ID3D12Resource> texBuff;
	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//頂点データ配列
	std::vector<VertexPosNormalUv> vertices;
	//インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};
	//頂点インデックス配列
	std::vector<unsigned short> indices;
	//定数バッファ(マテリアル)
	ComPtr<ID3D12Resource> constBuffB1;
	//マテリアル
	Material material;
	//頂点法線スムージング用データ
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;
};