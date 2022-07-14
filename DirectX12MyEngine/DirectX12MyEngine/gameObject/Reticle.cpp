#include "Reticle.h"

Reticle* Reticle::Create(ObjModel* model)
{
	//レティクルのインスタンスを生成
	Reticle* reticle = new Reticle();
	if (reticle == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!reticle->Initialize()) {
		delete reticle;
		assert(0);
		return nullptr;
	}

	//モデルをセット
	assert(model);
	reticle->model = model;

	return reticle;
}

void Reticle::Update()
{
	//自機からレティクルまでの距離
	const float distancePlayerToReticle = 25.0f;
	//自機から3DレティクルへのオフセットZ向き
	Vector3 offset = { 0, 0, 1.0f };
	//自機のワールド行列の回転をレティクルに反映
	offset = MatrixTransformDirection(offset, followMatWorld);
	//ベクトルの長さを整える
	offset = offset.normalize() * distancePlayerToReticle;
	position = {
		followMatWorld.r[3].m128_f32[0] + offset.x,
	followMatWorld.r[3].m128_f32[1] + offset.y,
	followMatWorld.r[3].m128_f32[2] + offset.z, };

	//3Dレティクルの座標を設定
	XMMATRIX matScale, matRot, matTrans;
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//ワールド行列の合成
	matWorld = XMMatrixIdentity();	//変形をリセット
	matWorld *= matScale;	//ワールド行列にスケーリングを反映
	matWorld *= matRot;		//ワールド行列に回転を反映
	matWorld *= matTrans;	//ワールド行列に平行移動を反映


	const XMMATRIX matViewProjection = camera->GetMatView() * camera->GetMatProjection();
	const Vector3 cameraPos = camera->GetEye();

	//定数バッファへのデータ転送
	ConstBufferDataB0* constMap = nullptr;
	if (SUCCEEDED(constBuffB0->Map(0, nullptr, (void**)&constMap)))
	{
		constMap->viewproj = matViewProjection;
		constMap->world = matWorld;
		constMap->cameraPos = cameraPos;
		constBuffB0->Unmap(0, nullptr);
	}
}

Vector3 Reticle::GetWorldPos()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//平行移動成分を取得
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
}
