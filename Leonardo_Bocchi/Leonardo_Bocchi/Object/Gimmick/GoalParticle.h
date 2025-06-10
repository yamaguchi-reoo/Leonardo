#pragma once
#include "../GameObject.h"
#include <DxLib.h>
#include <cmath>

struct GoalParticle
{
	Vector2D position; // �p�[�e�B�N���̈ʒu
	Vector2D velocity; // �p�[�e�B�N���̑��x

	float scale; // �p�[�e�B�N���̃X�P�[��
	int timer; // �p�[�e�B�N���̃��C�t�^�C��
	int duration; // �p�[�e�B�N���̎�������
	bool is_active; // �p�[�e�B�N�����A�N�e�B�u���ǂ���

	GoalParticle(Vector2D pos)
		: position(pos), scale(1.0f), timer(0), duration(30 + rand() % 20), is_active(true)
	{
		// �����_���ȑ��x��ݒ�
		float angle = (rand() % 360) * (DX_PI / 180.0f); // �����_���Ȋp�x
		float speed = 1.0f + (rand() % 20) / 10.0f; // �����_���ȑ��x
		velocity = Vector2D(cosf(angle) * speed, sinf(angle) * speed);
	}

	void Update()
	{
		position += velocity; // �ʒu���X�V
		timer++;
		if (timer >= duration) {
			is_active = false; 
		}
	}

};
