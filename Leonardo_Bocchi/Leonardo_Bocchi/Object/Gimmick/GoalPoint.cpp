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
	DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(255, 0, 0), FALSE);

	Vector2D center = offset + box_size / 2;
	float radius_x = 30.0f + 6.0f * sinf(effect_timer * 0.1f);
	float radius_y = 40.0f + 10.0f * cosf(effect_timer * 0.1f);

    if (is_active)
    {
        float ring_angle = effect_timer * 0.05f;

        // 半透明の中心グラデーション楕円
        for (int i = 0; i < 5; ++i)
        {
            int alpha = 80 - i * 15;
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
            DrawEllipseAA(center.x, center.y, radius_x * (1.0f - i * 0.15f), radius_y * (1.0f - i * 0.15f), 64, GetColor(100, 200, 255), true, 1);
        }

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180); // 元に戻す
        // 外周の回転楕円リング
        for (int i = 0; i < 3; ++i)
        {
            float ring_rx = radius_x + i * 2.0f;
            float ring_ry = radius_y + i * 2.0f;
            DrawEllipseAA(center.x, center.y, ring_rx, ring_ry, 64, GetColor(0, 255, 255), false, 2);
        }

        // パーティクル吸い込み
        for (auto& p : particles)
        {
            if (!p.is_active) continue;

            Vector2D dir = center - p.position;
            dir = dir.Normalize() * 0.5f; // 吸い込み速度
            p.position += dir;

            float t = static_cast<float>(p.timer) / p.duration;
            int particle_alpha = static_cast<int>(255 * (1.0f - t));

            SetDrawBlendMode(DX_BLENDMODE_ALPHA, particle_alpha);
            DrawCircleAA(p.position.x, p.position.y, 3.0f * p.scale, 12, GetColor(0, 255, 255), TRUE);
        }

        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
    else
    {
        // 非アクティブ時の楕円 (同じ大きさ)
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
        DrawEllipseAA(center.x, center.y, radius_x, radius_y, 64, GetColor(200, 200, 200), TRUE);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
	__super::Draw(offset, rate);
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

bool GoalPoint::IsActive() const
{
	if (!is_active) return false;

	for (const auto& p : particles)
	{
		if (p.is_active) return true;
	}
	return false;
}
