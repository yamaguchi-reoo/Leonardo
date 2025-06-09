#include "Enemy.h"
#include <DxLib.h>
#include <vector>
#include "../../../Utility/UtilityList.h"

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

	//image = LoadGraph("Resource/Images/Character/Enemy/Enemy-idle/enemy-idle1.png");
	LoadEnemyImage();

	animation_count = 0;
}

void Enemy::Update()
{
	if (replay_history.empty()) return;

	if (current_frame + 1 < replay_history.size())
	{
		// 補間率
		float t = static_cast<float>(frame_timer) / frame_duration;
		t = Clamp(t, 0.0f, 1.0f);

		// 線形補間
		Vector2D interpolated = start_position * (1.0f - t) + end_position * t;
		SetLocation(interpolated);

		// 向き反映
		flip_flg = replay_history[current_frame].flip;

		// アニメーション再生
		AnimationControl(replay_history[current_frame].action_state);

		++frame_timer;
		if (frame_timer >= frame_duration)
		{
			++current_frame;
			frame_timer = 0;

			// 位置更新
			if (current_frame + 1 < replay_history.size())
			{
				start_position = replay_history[current_frame].position;
				end_position = replay_history[current_frame + 1].position;
			}
			else if (replay_history.size() >= 2)
			{
				// 最後の速度保存
				velocity = replay_history.back().position - replay_history[replay_history.size() - 2].position;
			}
		}
	}

	__super::Update();
}

void Enemy::Draw(Vector2D offset, double rate) 
{
	offset.y -= 4.5f;
	__super::Draw(offset, 2.0);

	switch (action_state)
	{
	case ActionState::IDLE:
		DrawFormatString(80, 140, GetColor(255, 255, 255), "State: IDLE");
		break;
	case ActionState::WALK:
		DrawFormatString(80, 140, GetColor(255, 255, 255), "State: WALK");
		break;
	case ActionState::JUMP:
		DrawFormatString(80, 140, GetColor(255, 255, 255), "State: JUMP");
		break;
	case ActionState::DAMAGE:
		DrawFormatString(80, 140, GetColor(255, 255, 255), "State: DAMAGE");
		break;
	}
	DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(0, 255, 0), FALSE);
	//DrawFormatString(offset.x, offset.y, GetColor(255, 0, 0), "Enemy");
	//DrawFormatString(10, 60, GetColor(255, 0, 0), "%f",location.x);
}

void Enemy::Finalize()
{
}

void Enemy::Movement()
{
}

void Enemy::AnimationControl(ActionState state)
{
	if (action_state != state)
	{
		animation_frame = 0;
		action_state = state;
	}

	animation_frame++;

	const auto& frames = animation_data[state];
	if (frames.empty()) return;

	int index = (animation_frame / 10) % frames.size();
	image = frames[index];
}


void Enemy::OnHitCollision(GameObject* hit_object)
{
	__super::OnHitCollision(hit_object);


	if (hit_object->GetObjectType() == GOAL)
	{
		SetDelete();
	}

}

void Enemy::SetReplayHistory(const std::vector<PlayerMoveRecord>& history)
{
	replay_history = history;
	current_frame = 0;
	frame_timer = 0;
	velocity = Vector2D(0.0f, 0.0f);

	if (replay_history.size() >= 2)
	{
		start_position = replay_history[0].position;
		end_position = replay_history[1].position;
		SetLocation(start_position);
	}
	else if (replay_history.size() == 1)
	{
		start_position = end_position = replay_history[0].position;
		SetLocation(start_position);
	}
}

void Enemy::LoadEnemyImage()
{
	// Enemy 用のアニメーションデータ読み込み
	ResourceManager* rm = ResourceManager::GetInstance();

	// IDLE
	auto idle_imgs = rm->GetImages("Resource/Images/Character/Enemy/Enemy-idle/enemy-idle", 6);
	animation_data[ActionState::IDLE] = idle_imgs;

	// WALK
	auto walk_imgs = rm->GetImages("Resource/Images/Character/Enemy/Enemy-run/enemy-run", 6);
	animation_data[ActionState::WALK] = walk_imgs;

	// JUMP
	auto jump_imgs = rm->GetImages("Resource/Images/Character/Enemy/Enemy-jump/enemy-jump", 2);
	animation_data[ActionState::JUMP] = jump_imgs;
;
	image = animation_data[ActionState::IDLE][0];

}