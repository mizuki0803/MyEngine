#include "PlayerVaporEffect.h"
#include "ParticleEmitter.h"
#include "Spline.h"

const float PlayerVaporEffect::scrollSpeed = 0.5f;

void PlayerVaporEffect::Update(const Vector3& leftWingPos, const Vector3& rightWingPos)
{
	//�����̍��W���L�^���Ă���
	leftWingPosList.push_back(leftWingPos);
	//�E���̍��W���L�^���Ă���
	rightWingPosList.push_back(rightWingPos);
	//����5�t���[�����̗v�f�Ōv�Z������̂ŁA������ߋ��̗v�f���폜
	if (leftWingPosList.size() > 5) {
		leftWingPosList.erase(leftWingPosList.begin());
	}
	if (rightWingPosList.size() > 5) {
		rightWingPosList.erase(rightWingPosList.begin());
	}

	//��s�@�_��������ԂłȂ���Δ�����
	if (!isVaporCreate) { return; }

	//��s�@�_�̑傫����ύX
	ChangeVaporSize();

	//��s�@�_����
	const int segmentCount = 25;
	VaporCreate(leftWingPosList, segmentCount); //����
	VaporCreate(rightWingPosList, segmentCount); //�E��
}

void PlayerVaporEffect::VaporStart()
{
	//��s�@�_�̑傫����傫�������ԂɕύX
	vaporSizeChangeMode = VaporSizeChangeMode::Increase;

	//�T�C�Y���J�n�̏�Ԃɂ��Ă���
	const float startSize = 0.01f;
	vaporSize = startSize;

	//��s�@�_�𐶐���Ԃɂ���
	isVaporCreate = true;
}

void PlayerVaporEffect::VaporEnd()
{
	//��s�@�_�̑傫���������������ԂɕύX
	vaporSizeChangeMode = VaporSizeChangeMode::Smaller;
}

void PlayerVaporEffect::VaporCreate(std::list<Vector3>& wingPosList, const int segmentCount)
{
	//�X�v���C����ԗp��vector�ɗv�f�����Ă���
	std::vector<Vector3> points = {};
	std::list<Vector3>::iterator itr = wingPosList.begin();
	int listSize = (int)wingPosList.size();
	for (int i = 0; i < listSize; i++) {
		//�X�N���[����ԂȂ�ۑ����Ă��������W�����ɂ��炷
		if (isScrollMode) { (*itr).z -= scrollSpeed; }
		points.push_back(*itr);
		itr++;
	}

	//�X�N���[����ԂȂ瑬�x��t����
	Vector3 vel = {};
	if (isScrollMode) { vel.z -= scrollSpeed; }

	//�X�v���C����Ԃł̒��߂�2�_���q���������ׂ�������Ŕ�s�@�_�𐶐�
	for (int i = 0; i < segmentCount + 1; i++) {
		float t = 1.0f / segmentCount * i; //0�`1�̐������Z�o
		//�������W���擾
		Vector3 pos = Spline::splinePosition(points, listSize - 3, t);

		//�p�[�e�B�N������
		ParticleEmitter::GetInstance()->PlayerVapor(pos, vaporSize, vel);
	}
}

void PlayerVaporEffect::ChangeVaporSize()
{
	//��s�@�_�̃T�C�Y��傫������ꍇ
	if (vaporSizeChangeMode == VaporSizeChangeMode::Increase) {
		//�T�C�Y��傫������
		const float sizeChangeNum = 0.02f; //�T�C�Y�ύX�̗�
		vaporSize += sizeChangeNum;
		//�ő�T�C�Y���z���Ȃ��悤�ɂ���
		const float maxSize = 0.5f;
		vaporSize = min(vaporSize, maxSize);
	}
	//��s�@�_�̃T�C�Y������������ꍇ
	else if (vaporSizeChangeMode == VaporSizeChangeMode::Smaller) {
		//�T�C�Y������������
		const float sizeChangeNum = -0.007f; //�T�C�Y�ύX�̗�
		vaporSize += sizeChangeNum;

		//�T�C�Y��0�ȉ��ɂȂ����琶����Ԃ���������
		if (vaporSize <= 0) {
			isVaporCreate = false;
		}
	}
}
