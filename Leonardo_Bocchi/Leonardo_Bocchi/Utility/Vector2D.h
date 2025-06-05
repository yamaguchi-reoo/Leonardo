#pragma once

class Vector2D
{
public:
	float x;
	float y;

public:
	//�R���X�g���N�^
	Vector2D();
	Vector2D(float scalar);
	Vector2D(float mx, float my);
	//�f�X�g���N�^
	~Vector2D();

public:
	//���
	Vector2D& operator = (const Vector2D& location);

	//���Z
	const Vector2D operator + (const Vector2D& location)const;
	//�������Z
	Vector2D& operator += (const Vector2D& location);


	//���Z
	const Vector2D operator - (const Vector2D& location)const;
	//�����Ɍ��Z
	Vector2D& operator -= (const Vector2D& location);


	//��Z(�X�J���[)
	const Vector2D operator * (const float& scalar)const;
	//��Z
	const Vector2D operator * (const Vector2D& location)const;
	//�����ɏ�Z(�X�J���[)
	Vector2D& operator *= (const float& scalor);
	//�����ɏ�Z
	Vector2D& operator *= (const Vector2D& location);


	//���Z(�X�J���[)
	const Vector2D operator / (const float& scalar)const;
	//���Z
	const Vector2D operator / (const Vector2D& location)const;
	//�����ɏ��Z(�X�J���[)
	Vector2D& operator /= (const float& scalor);
	//�����ɏ��Z
	Vector2D& operator /= (const Vector2D& location);

	//�����^�ɕϊ�
	void ToInt(int* x, int* y)const;

	float Length()const;

	Vector2D Normalize()const;
};

