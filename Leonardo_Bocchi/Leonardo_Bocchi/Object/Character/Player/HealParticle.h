#pragma once
#include "../../GameObject.h"
#include <Dxlib.h>
#include <cmath>

struct HealParticle
{
	Vector2D position; // �p�[�e�B�N���̈ʒu
	Vector2D velocity; // �p�[�e�B�N���̑��x

	float scale; // �p�[�e�B�N���̃X�P�[��
	int timer; // �p�[�e�B�N���̃��C�t�^�C��
	int duration; // �p�[�e�B�N���̎�������
	bool is_active; // �p�[�e�B�N�����A�N�e�B�u���ǂ���

	HealParticle(Vector2D pos)
		: position(pos), velocity(RandomVelocity()), scale(1.0f), timer(0), duration(30 + rand() % 20) , is_active(true)
	{}

	Vector2D RandomVelocity()
	{
		float angle = (rand() % 360) * (DX_PI / 180.0f); // �����_���Ȋp�x
		float speed = 1.0f + (rand() % 20) / 10.0f; // �����_���ȑ��x

		return Vector2D(cosf(angle) * speed, sinf(angle) * speed);
	}
};