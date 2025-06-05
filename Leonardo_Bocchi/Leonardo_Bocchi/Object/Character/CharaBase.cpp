#include "CharaBase.h"
#include <iostream>
#include <algorithm>

#define GRAVITY (9.087f)

void CharaBase::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);
    on_ground = false;
}

void CharaBase::Update()
{
    on_ground = false;

	//共通の重力適用
	ApplyGravity();

}

void CharaBase::Draw(Vector2D offset, double rate) const
{
	//親クラスに書かれた描画処理の内容を実行する
	__super::Draw(offset, rate);

}

void CharaBase::Finalize()
{
}

void CharaBase::OnHitCollision(GameObject* hit_object)
{
    if (hit_object->GetObjectType() != BLOCK && hit_object->GetObjectType() != MOVE_BLOCK)
        return;

    const float delta_time = 1.0f / 60.0f;

    Vector2D target_pos = hit_object->GetLocation();
    Vector2D char_pos = location;

    Vector2D char_center = location + hit_box / 2.0f;
    Vector2D target_center = target_pos + hit_object->GetBoxSize() / 2.0f;

    Vector2D char_half = hit_box / 2.0f;
    Vector2D target_half = hit_object->GetBoxSize() / 2.0f;

    Vector2D diff = char_center - target_center;

    bool is_overlapping_now =
        fabsf(diff.x) < (char_half.x + target_half.x) &&
        fabsf(diff.y) < (char_half.y + target_half.y);

    if (is_overlapping_now) {
        float depth_x = (char_half.x + target_half.x) - fabsf(diff.x);
        float depth_y = (char_half.y + target_half.y) - fabsf(diff.y);

        if (depth_y < depth_x) {
            if (diff.y < 0) {
                on_ground = true;
                location.y -= depth_y;
                velocity.y = 0.0f;
                g_velocity = 0.0f;
                jump_count = 0;
            }
            else {
                location.y += depth_y;
                if (velocity.y < 0.0f) velocity.y = 0.0f;
            }
        }
        else {
            if (diff.x < 0) {
                location.x -= depth_x;
            }
            else {
                location.x += depth_x;
            }
            velocity.x = 0.0f;
        }
    }

}


void CharaBase::ApplyGravity()
{
	if (!on_ground)
	{
		// 重力加速度を加算
		g_velocity += GRAVITY / 444.0f;

		// 速度に加算
		velocity.y += g_velocity;

		// 落下速度の上限を制限
		if (velocity.y > max_fall_speed)
		{
			velocity.y = max_fall_speed;
		}
	}
	else
	{
		// 地面にいる場合は重力加速度をリセット
		g_velocity = 0.0f;
	}

	// 位置の更新（重力による移動）
	location.y += velocity.y;
}

