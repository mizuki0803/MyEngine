#include "PlayerVaporEffect.h"
#include "ParticleEmitter.h"
#include "Spline.h"

const float PlayerVaporEffect::scrollSpeed = 0.5f;

void PlayerVaporEffect::Update(const Vector3& leftWingPos, const Vector3& rightWingPos)
{
	//左翼の座標を記録しておく
	leftWingPosList.push_back(leftWingPos);
	//右翼の座標を記録しておく
	rightWingPosList.push_back(rightWingPos);
	//直近5フレーム分の要素で計算をするので、それより過去の要素を削除
	if (leftWingPosList.size() > 5) {
		leftWingPosList.erase(leftWingPosList.begin());
	}
	if (rightWingPosList.size() > 5) {
		rightWingPosList.erase(rightWingPosList.begin());
	}

	//飛行機雲が生成状態でなければ抜ける
	if (!isVaporCreate) { return; }

	//飛行機雲の大きさを変更
	ChangeVaporSize();

	//飛行機雲生成
	const int segmentCount = 25;
	VaporCreate(leftWingPosList, segmentCount); //左翼
	VaporCreate(rightWingPosList, segmentCount); //右翼
}

void PlayerVaporEffect::VaporStart()
{
	//飛行機雲の大きさを大きくする状態に変更
	vaporSizeChangeMode = VaporSizeChangeMode::Increase;

	//サイズを開始の状態にしておく
	const float startSize = 0.01f;
	vaporSize = startSize;

	//飛行機雲を生成状態にする
	isVaporCreate = true;
}

void PlayerVaporEffect::VaporEnd()
{
	//飛行機雲の大きさを小さくする状態に変更
	vaporSizeChangeMode = VaporSizeChangeMode::Smaller;
}

void PlayerVaporEffect::VaporCreate(std::list<Vector3>& wingPosList, const int segmentCount)
{
	//スプライン補間用にvectorに要素を入れておく
	std::vector<Vector3> points = {};
	std::list<Vector3>::iterator itr = wingPosList.begin();
	int listSize = (int)wingPosList.size();
	for (int i = 0; i < listSize; i++) {
		//スクロール状態なら保存しておいた座標を後ろにずらす
		if (isScrollMode) { (*itr).z -= scrollSpeed; }
		points.push_back(*itr);
		itr++;
	}

	//スクロール状態なら速度を付ける
	Vector3 vel = {};
	if (isScrollMode) { vel.z -= scrollSpeed; }

	//スプライン補間での直近の2点を繋いだ線を細かく刻んで飛行機雲を生成
	for (int i = 0; i < segmentCount + 1; i++) {
		float t = 1.0f / segmentCount * i; //0～1の数字を算出
		//生成座標を取得
		Vector3 pos = Spline::splinePosition(points, listSize - 3, t);

		//パーティクル生成
		ParticleEmitter::GetInstance()->PlayerVapor(pos, vaporSize, vel);
	}
}

void PlayerVaporEffect::ChangeVaporSize()
{
	//飛行機雲のサイズを大きくする場合
	if (vaporSizeChangeMode == VaporSizeChangeMode::Increase) {
		//サイズを大きくする
		const float sizeChangeNum = 0.02f; //サイズ変更の量
		vaporSize += sizeChangeNum;
		//最大サイズを越えないようにする
		const float maxSize = 0.5f;
		vaporSize = min(vaporSize, maxSize);
	}
	//飛行機雲のサイズを小さくする場合
	else if (vaporSizeChangeMode == VaporSizeChangeMode::Smaller) {
		//サイズを小さくする
		const float sizeChangeNum = -0.007f; //サイズ変更の量
		vaporSize += sizeChangeNum;

		//サイズが0以下になったら生成状態を解除する
		if (vaporSize <= 0) {
			isVaporCreate = false;
		}
	}
}
