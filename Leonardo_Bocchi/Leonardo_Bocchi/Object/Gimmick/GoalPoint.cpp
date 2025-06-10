#include "GoalPoint.h"
#include <DxLib.h>
#include "GoalParticle.h"


void GoalPoint::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);

	object_type = GOAL;
}

void GoalPoint::Update()
{
	if (is_active)
	{
		effect_timer++;

		for (auto& particle : particles)
		{
			particle.Update();
		}

	}
	__super::Update();
}

void GoalPoint::Draw(Vector2D offset, double rate)
{
	Vector2D center = offset + box_size / 2;

	// 光のリング
	if (is_active)
	{
		float radius = 40.0f + 8.0f * sinf(effect_timer * 0.2f);
		int alpha = static_cast<int>(255 * (1.0f - effect_timer / 90.0f));
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawCircleAA(center.x, center.y, radius, 32, GetColor(0, 255, 255), FALSE);

		// 光の柱
		DrawBoxAA(center.x - 2, center.y - 48, center.x + 2, center.y + 48, GetColor(0, 200, 255), TRUE);

		// パーティクル
		for (const auto& p : particles)
		{
			if (!p.is_active) continue;

			float t = static_cast<float>(p.timer) / p.duration;
			int particle_alpha = static_cast<int>(255 * (1.0f - t));

			SetDrawBlendMode(DX_BLENDMODE_ALPHA, particle_alpha);
			DrawCircleAA(p.position.x, p.position.y, 3.0f * p.scale, 12, GetColor(0, 255, 255), TRUE);
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else
	{
		// 通常状態（GoalPoint 表示）
		DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(255, 255, 0), TRUE);
	}
}

void GoalPoint::Finalize()
{
	particles.clear();
}

void GoalPoint::OnHitCollision(GameObject* hit_object)
{
	if (!is_active && hit_object->GetObjectType() == PLAYER)
	{
		is_active = true;
		effect_timer = 0;
		CreateParticles();
	}
}

void GoalPoint::CreateParticles()
{
	Vector2D center = location + box_size / 2;
	for (int i = 0; i < 20; ++i)
	{
		Vector2D pos = center + Vector2D(rand() % 64 - 32, rand() % 64 - 32);
		particles.emplace_back(pos);
	}
}
