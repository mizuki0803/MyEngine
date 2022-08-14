#pragma once
#include <d3dx12.h>
#include <vector>

/// <summary>
/// �Z���N�^�[
/// </summary>
class Selector
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�Z���N�^�[</returns>
	static Selector* Create();

	/// <summary>
	/// �Z���N�g
	/// </summary>
	bool Select();

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