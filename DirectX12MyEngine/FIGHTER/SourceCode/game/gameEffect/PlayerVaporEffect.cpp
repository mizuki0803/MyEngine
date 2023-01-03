#include "PlayerVaporEffect.h"
#include "ParticleEmitter.h"
#include "Spline.h"

void PlayerVaporEffect::Update(const Vector3& leftWingPos, const Vector3& rightWingPos)
{
	//�����̍��W���L�^���Ă���
	leftWingPosList.push_back(leftWingPos);
	//�E���̍��W���L�^���Ă���
	rightWingPosList.push_back(rightWingPos);

	if (leftWingPosList.size() > 5) {
		leftWingPosList.erase(leftWingPosList.begin());
	}

	if (rightWingPosList.size() > 5) {
		rightWingPosList.erase(rightWingPosList.begin());
	}

	//��s�@�_����
	const int segmentCount = 25;
	VaporCreate(leftWingPosList, segmentCount); //����
	VaporCreate(rightWingPosList, segmentCount); //�E��
}

void PlayerVaporEffect::VaporStart()
{
}

void PlayerVaporEffect::VaporEnd()
{
}

void PlayerVaporEffect::VaporCreate(std::list<Vector3>& wingPosList, const int segmentCount)
{
	//�X�v���C����ԗp��vector�ɗv�f�����Ă���
	std::vector<Vector3> points = {};
	std::list<Vector3>::iterator itr = wingPosList.begin();
	int listSize = (int)wingPosList.size();
	for (int i = 0; i < listSize; i++) {
		points.push_back(*itr);
		itr++;
	}

	//�X�v���C����Ԃł̒��߂�2�_���q���������ׂ�������Ŕ�s�@�_�𐶐�
	for (int i = 0; i < segmentCount + 1; i++) {
		float t = 1.0f / segmentCount * i; //0�`1�̐������Z�o
		//�������W���擾
		Vector3 pos = Spline::splinePosition(points, listSize - 3, t);

		//�p�[�e�B�N������
		ParticleEmitter::GetInstance()->PlayerVapor(pos);
	}
}
