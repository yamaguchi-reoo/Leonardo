#pragma once
#include "../../GameObject.h"
#include <Dxlib.h>
#include <cmath>

struct HealParticle
{
	Vector2D position; // パーティクルの位置
	Vector2D velocity; // パーティクルの速度

	float scale; // パーティクルのスケール
	int timer; // パーティクルのライフタイム
	int duration; // パーティクルの持続時間
	bool is_active; // パーティクルがアクティブかどうか

	HealParticle(Vector2D pos)
		: position(pos), velocity(RandomVelocity()), scale(1.0f), timer(0), duration(30 + rand() % 20) , is_active(true)
	{}

	Vector2D RandomVelocity()
	{
		float angle = (rand() % 360) * (DX_PI / 180.0f); // ランダムな角度
		float speed = 1.0f + (rand() % 20) / 10.0f; // ランダムな速度

		return Vector2D(cosf(angle) * speed, sinf(angle) * speed);
	}
};