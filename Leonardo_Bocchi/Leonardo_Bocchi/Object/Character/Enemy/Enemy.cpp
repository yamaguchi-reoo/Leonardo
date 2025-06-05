#include "Enemy.h"
#include <DxLib.h>
#include <vector>

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);

	object_type = ENEMY;
	velocity = { 0.0f };
	g_velocity = 0.0f;

	image = LoadGraph("Resource/Images/Character/Enemy/enemy.png");

	animation_count = 0;
}

void Enemy::Update()
{
	__super::Update();
	//移動処理
	//Movement();
	//アニメーション管理
	//AnimationControl();

	/*if (current_frame < replay_history.size())
	{
		const auto& record = replay_history[current_frame];
		SetLocation(record.position);
		flip_flg = record.flip;
		++current_frame;
	}*/


	if (current_frame + 1 < replay_history.size())
	{
		// 補間率
		float t = static_cast<float>(frame_timer) / frame_duration;
		// 線形補間
		Vector2D interpolated = start_position * (1.0f - t) + end_position * t;
		SetLocation(interpolated);

		flip_flg = replay_history[current_frame].flip;

		++frame_timer;
		if (frame_timer >= frame_duration) {
			++current_frame;
			frame_timer = 0;

			// 位置保存：最後の移動量を記録しておく
			if (current_frame + 1 < replay_history.size()) {
				start_position = replay_history[current_frame].position;
				end_position = replay_history[current_frame + 1].position;
			}
			else if (replay_history.size() >= 2) {
				// 補間完了 → 最後の速度を保存しておく
				velocity = replay_history.back().position - replay_history[replay_history.size() - 2].position;
			}
		}
	}
	else {
		// 補間終了 → 最後の速度でそのまま動かす
		location += velocity;
		SetLocation(location);
	}
}

void Enemy::Draw(Vector2D offset, double rate) const
{
	__super::Draw(offset, 1.5);
	//DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(0, 255, 0), TRUE);
	//DrawFormatString(offset.x, offset.y, GetColor(255, 0, 0), "Enemy");
	//DrawFormatString(10, 60, GetColor(255, 0, 0), "%f",location.x);
}

void Enemy::Finalize()
{
}

void Enemy::Movement()
{
}

void Enemy::AnimationControl()
{
}

void Enemy::OnHitCollision(GameObject* hit_object)
{
	__super::OnHitCollision(hit_object);


	if (hit_object->GetObjectType() == GOAL)
	{
		SetDelete();
	}

}

void Enemy::SetReplayHistory(const std::vector<MoveRecord>& history)
{
	replay_history = history;
	current_frame = 0;
	//replay_history.clear();

	if(!replay_history.empty()) {
		start_position = replay_history[0].position;
		if (replay_history.size() > 1)
			end_position = replay_history[1].position;
		else
			end_position = start_position;
		frame_timer = 0;
	}
}


