#pragma once

//��Βl�����߂�֐��e���v���[�g
template<typename T>
T Abs(T value)
{
	T result;
	//���̒l�̏ꍇ�A���̂܂ܕԂ�
	if (value > 0)
	{
		result = value;
	}
	// ���̒l�̏ꍇ�A���̐�Βl��Ԃ�
	else
	{
		result = -value;
	}

	return result;
}

//�ő�l�����߂�֐��e���v���[�g
template <typename T>
T Max(T a, T b)
{
	T result;
	//b��a���傫���ꍇ��b��Ԃ�
	if (a < b)
	{
		result = b;
	}
	//a��b�ȏ�̏ꍇ��a��Ԃ�
	else
	{
		result = a;
	}

	return result;
}

//�ŏ��l�����߂�֐��e���v���[�g
template<typename T>
T Min(T a, T b)
{
	T result;
	//b��a��菬�����ꍇ��b��Ԃ�
	if (a > b)
	{
		result = b;
	}
	//a��b�ȉ��̏ꍇ��a��Ԃ�
	else
	{
		result = a;
	}

	return result;
}

