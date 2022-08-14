#pragma once
#include <d3dx12.h>
#include <vector>

/// <summary>
/// �V�[�P���T�[
/// </summary>
class Sequencer
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�V�[�P���T�[</returns>
	static Sequencer* Create();

	/// <summary>
	/// �V�[�P���X
	/// </summary>
	bool Sequence();

	/// <summary>
	/// �q�m�[�h��ǉ�����
	/// </summary>
	/// <param name="func()">�֐�</param>
	void AddNode(bool func());

private:
	typedef bool (*Func)();
	//�q�m�[�h
	std::vector<Func> nodes;
};