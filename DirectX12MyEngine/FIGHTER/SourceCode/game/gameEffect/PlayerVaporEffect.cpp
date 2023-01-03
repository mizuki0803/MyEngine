#include "PlayerVaporEffect.h"
#include "ParticleEmitter.h"
#include "Spline.h"

void PlayerVaporEffect::Update(const Vector3& leftWingPos, const Vector3& rightWingPos)
{
	//左翼の座標を記録しておく
	leftWingPosList.push_back(leftWingPos);
	//右翼の座標を記録しておく
	rightWingPosList.push_back(rightWingPos);

	if (leftWingPosList.size() > 5) {
		leftWingPosList.erase(leftWingPosList.begin());
	}

	if (rightWingPosList.size() > 5) {
		rightWingPosList.erase(rightWingPosList.begin());
	}

	//飛行機雲生成
	const int segmentCount = 25;
	VaporCreate(leftWingPosList, segmentCount); //左翼
	VaporCreate(rightWingPosList, segmentCount); //右翼
}

void PlayerVaporEffect::VaporStart()
{
}

void PlayerVaporEffect::VaporEnd()
{
}

void PlayerVaporEffect::VaporCreate(std::list<Vector3>& wingPosList, const int segmentCount)
{
	//スプライン補間用にvectorに要素を入れておく
	std::vector<Vector3> points = {};
	std::list<Vector3>::iterator itr = wingPosList.begin();
	int listSize = (int)wingPosList.size();
	for (int i = 0; i < listSize; i++) {
		points.push_back(*itr);
		itr++;
	}

	//スプライン補間での直近の2点を繋いだ線を細かく刻んで飛行機雲を生成
	for (int i = 0; i < segmentCount + 1; i++) {
		float t = 1.0f / segmentCount * i; //0～1の数字を算出
		//生成座標を取得
		Vector3 pos = Spline::splinePosition(points, listSize - 3, t);

		//パーティクル生成
		ParticleEmitter::GetInstance()->PlayerVapor(pos);
	}
}
