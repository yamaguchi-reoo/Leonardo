#pragma once
#include "../GameObject.h"
#include <DxLib.h>
#include <cmath>

struct GoalParticle
{
	Vector2D position; // パーティクルの位置
	Vector2D velocity; // パーティクルの速度

	float scale; // パーティクルのスケール
	int timer; // パーティクルのライフタイム
	int duration; // パーティクルの持続時間
	bool is_active; // パーティクルがアクティブかどうか

	GoalParticle(Vector2D pos)
		: position(pos), scale(1.0f), timer(0), duration(30 + rand() % 20), is_active(true)
	{
		// ランダムな速度を設定
		float angle = (rand() % 360) * (DX_PI / 180.0f); // ランダムな角度
		float speed = 1.0f + (rand() % 20) / 10.0f; // ランダムな速度
		velocity = Vector2D(cosf(angle) * speed, sinf(angle) * speed);
	}

	void Update()
	{
		position += velocity; // 位置を更新
		timer++;
		if (timer >= duration) {
			is_active = false; 
		}
	}

};
