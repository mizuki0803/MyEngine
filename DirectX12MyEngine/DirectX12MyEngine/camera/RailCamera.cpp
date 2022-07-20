#include "RailCamera.h"
#include "Input.h"

void RailCamera::Initialize()
{
	//初期座標を設定
	position = { 0, 0, -30 };
	//初期回転角を設定
	rotation = { 0, 0, 0 };

	//ビュー行列と射影行列の初期化
	UpdateMatView();
	UpdateMatProjection();
}

void RailCamera::Update()
{
	Input* input = Input::GetInstance();

	//移動
	Vector3 velocity(0, 0, 0.1f);
	//XYを入力で移動させる
	float moveSpeed = 0.3f;
	if (input->PushKey(DIK_RIGHT)) { velocity.x += moveSpeed; }
	if (input->PushKey(DIK_LEFT)) { velocity.x -= moveSpeed; }
	if (input->PushKey(DIK_UP)) { velocity.y += moveSpeed; }
	if (input->PushKey(DIK_DOWN)) { velocity.y -= moveSpeed; }

	//スティック傾きの判定を取る
	const float stickNum = 500;
	if (input->TiltGamePadLStickX(stickNum) || input->TiltGamePadLStickX(-stickNum) || input->TiltGamePadLStickY(stickNum) || input->TiltGamePadLStickY(-stickNum)) {
		//プレイヤーはスティックを倒した方向に動く
		float padRota = input->GetPadLStickAngle();
		float moveAngle = XMConvertToRadians(padRota);
		velocity.x = moveSpeed * cosf(moveAngle);
		velocity.y = moveSpeed * -sinf(moveAngle);
	}

	position += velocity;

	//XYの移動範囲を設定
	const XMFLOAT2 moveLimit = { 20.0f, 10.0f };

	position.x = max(position.x, -moveLimit.x);
	position.x = min(position.x, +moveLimit.x);
	position.y = max(position.y, -moveLimit.y);
	position.y = min(position.y, +moveLimit.y);
	//回転
	//Vector3 rot(0.1f, 0, 0);
	//rotation += rot;

	//回転　平行移動行列の計算
	DirectX::XMMATRIX matRot, matTrans;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//ワールド行列の合成
	matWorld = XMMatrixIdentity();	//変形をリセット
	matWorld *= matRot;		//ワールド行列に回転を反映
	matWorld *= matTrans;	//ワールド行列に平行移動を反映

	//視点をワールド座標に設定
	eye = { matWorld.r[3].m128_f32[0], matWorld.r[3].m128_f32[1], matWorld.r[3].m128_f32[2] };
	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	//カメラの回転を反映させる
	forward = MatrixTransformDirection(forward, matWorld);
	//視点から前方に進んだ位置を注視点に設定
	target = eye + forward;
	//天地が反転してもいいように上方向ベクトルも回転させる
	Vector3 baseUp(0, 1, 0);
	up = MatrixTransformDirection(baseUp, matWorld);

	//ビュー行列と射影行列の更新
	UpdateMatView();
	UpdateMatProjection();
}
