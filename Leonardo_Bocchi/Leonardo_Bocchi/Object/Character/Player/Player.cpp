#include <DxLib.h>
#include "Player.h"
#include "../../../Utility/UtilityList.h"

Player::Player() : animation_data(), damage_timer(), is_invincible(false), invincible_timer(0)
{
}

Player::~Player()
{
}

void Player::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);

	object_type = PLAYER;
	hp = 2;
	velocity = { 0.0f };
	g_velocity = 0.35f;
	max_fall_speed = 10.0f;
	on_ground = false;

	damage_timer = 0;

	//image = LoadGraph("Resource/Images/Character/Player/player.png");

	//ResourceManager* rm = ResourceManager::GetInstance();

	//IDLE���
	//animation_data.push_back(rm->GetImages("Resource/Images/Character/Player/Player.png")[0]);

	//MOVE���
	//animation_data.push_back(rm->GetImages("Resource/Images/Character/Player/PlayerMove01.png")[0]);
	//animation_data.push_back(rm->GetImages("Resource/Images/Character/Player/PlayerMove02.png")[0]);

	//image = animation_data[0];
	animation_frame = 0;
	animation_count = 0;
	jump_count = 1;

	LoadPlayerImage();
}

void Player::Update()
{
	//�ړ�����
	HandleInput();

	//�v���C���[�̓�����ۑ�
	SaveMoveHistory();

	//�_���[�W���󂯂Ă���̖��G����
	if (damage_flg)
	{
		damage_timer++;
		if (damage_timer >= 180)
		{
			damage_timer = 0;
			damage_flg = false;
		}
	}

	if (is_invincible)
	{
		invincible_timer++;
		if (invincible_timer >= 240)
		{
			invincible_timer = 0;
			is_invincible = false;
		}
	}
	// �ő呬�x�̐���
	ConstrainVelocity();

	//�ʒu���X�V
	location += velocity;

	//�A�j���[�V�����Ǘ�
	AnimationControl();


	for (auto& particle : heal_particles)
	{
		UpdateHealParticle(particle);
	}

	heal_particles.erase(
		std::remove_if(heal_particles.begin(), heal_particles.end(),
			[](const HealParticle& p) { return !p.is_active; }),
		heal_particles.end()
	);

	__super::Update();
}

void Player::Draw(Vector2D offset, double rate) 
{
	//__super::Draw(offset, 1.5);
	DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(255, 0, 0), FALSE);

	// ���G���Ԓ��͓_�ł�����
	bool is_draw = true;
	if (damage_timer)
	{
		// 10�t���[�������ɕ\��/��\����؂�ւ���i�_�Łj
		is_draw = (damage_timer / 10) % 2 == 0;
	}

	if (is_draw)
	{
		offset.y -= 4.5f;
		__super::Draw(offset, 2.0);
	}
	DrawFormatString(10, 120, GetColor(255, 255, 255), "HP �~ %d", hp);

	InvincibleEffect(offset);

	for (const auto& particle : heal_particles)
	{
		DrawHealParticle(particle, offset);
	}

#ifdef _DEBUG
	//DrawFormatString(10, 120, GetColor(255, 255, 255), "HP �~ %d", hp);
	DrawFormatString(10, 100, GetColor(255, 255, 255), "%f     %f", location.x + (box_size.x / 2), location.y + (box_size.y / 2));
	DrawFormatString(10, 80, GetColor(255, 255, 255), "%d", jump_time);
	DrawFormatString(10, 60, GetColor(255, 255, 255), "invicible :%d", is_invincible);
	DrawFormatString(10, 40, GetColor(255, 255, 255), "invicible_timer :%d", invincible_timer);
	switch (action_state)
	{
	case ActionState::IDLE:
		DrawFormatString(10, 140, GetColor(255, 255, 255), "State: IDLE");
		break;
	case ActionState::WALK:
		DrawFormatString(10, 140, GetColor(255, 255, 255), "State: WALK");
		break;
	case ActionState::JUMP:
		DrawFormatString(10, 140, GetColor(255, 255, 255), "State: JUMP");
		break;
	case ActionState::DAMAGE:
		DrawFormatString(10, 140, GetColor(255, 255, 255), "State: DAMAGE");
		break;
	}

	DrawFormatString(10, 160, GetColor(255, 255, 255), "%d", on_ground);
#endif // DEBUG


}
void Player::Finalize()
{
	animation_data.clear();
}

void Player::ApplyDeceleration()
{
	if (velocity.x < -1e-6f) {
		velocity.x = Min<float>(velocity.x + 0.5f, 0.0f);
	}
	else if (velocity.x > 1e-6f) {
		velocity.x = Max<float>(velocity.x - 0.5f, 0.0f);
	}
}

void Player::ConstrainVelocity()
{
	// �ő呬�x�̐���
	const float max_speed = 7.0f;
	velocity.x = Min<float>(Max<float>(velocity.x, -max_speed), max_speed);
}

void Player::HandleInput()
{
	InputControl* input = InputControl::GetInstance();

	ActionState next_state = action_state;

	// ���E�ړ�����
	if (input->GetButton(XINPUT_BUTTON_DPAD_LEFT))
	{
		move = MoveDirection::LEFT;
		velocity.x -= 0.5f;
		flip_flg = true;
		if (on_ground) next_state = ActionState::WALK;
	}
	else if (input->GetButton(XINPUT_BUTTON_DPAD_RIGHT))
	{
		move = MoveDirection::RIGHT;
		velocity.x += 0.5f;
		flip_flg = false;
		if (on_ground) next_state = ActionState::WALK;
	}
	else
	{
		move = MoveDirection::NONE;
		ApplyDeceleration();
		if (on_ground) next_state = ActionState::IDLE;
	}

	// �W�����v�J�n�i�W�����v�L�[������ & �n�ʂɂ���Ƃ��j
	if (input->GetButtonDown(XINPUT_BUTTON_A) && on_ground)
	{
		velocity.y = -4.0f;  // ����
		jump_time = 0;
		on_ground = false;  // �W�����v����͋󒆈���
		next_state = ActionState::JUMP;
	}

	// �������ŃW�����v����
	if (input->GetButton(XINPUT_BUTTON_A) &&
		action_state == ActionState::JUMP &&
		jump_time < 20)
	{
		jump_time++;
		velocity.y -= 0.25f;  // �㏸��ǉ�
		velocity.y = Max(velocity.y, -9.0f); // �㏸����
	}

	// ���n�`�F�b�N �� Update�̌�� on_ground = true �ɂ����O��
	if (on_ground && action_state == ActionState::JUMP)
	{
		next_state = (Abs(velocity.x) > 0.1f) ? ActionState::WALK : ActionState::IDLE;
	}

	// �_���[�W���͏�ԑJ�ڂ��Ȃ�
	if (action_state != ActionState::DAMAGE)
		action_state = next_state;

	if (action_state != next_state)
	{
		animation_frame = 0;
	}

}

void Player::AnimationControl()
{

	animation_frame++;

	const auto& frames = animation_data[action_state];
	if (frames.empty()) return;

	int index = (animation_frame / 10) % frames.size();
	image = frames[index];
}


void Player::OnHitCollision(GameObject* hit_object)
{
	__super::OnHitCollision(hit_object);

	// �G�l�~�[�q�b�g�� OR �g���b�v�q�b�g��
	if (hit_object->GetObjectType() == ENEMY || hit_object->GetObjectType() == TRAP)
	{
		if (!damage_flg && !is_invincible)
		{
			ApplyDamage();
		}
	}

	// �񕜃A�C�e���q�b�g��
	if (hit_object->GetObjectType() == HEAL)
	{
		hp += 1;
		for (int i = 0; i < 10; ++i)
		{
			heal_particles.push_back(HealParticle(Vector2D{ box_size.x / 2, box_size.y / 2 }));
		}
	}

	// ���G�A�C�e���q�b�g��
	if (hit_object->GetObjectType() == INVINCIBLE)
	{
		if (!is_invincible)
		{
			is_invincible = true;
		}
	}

	// GOAL�q�b�g��
	if (hit_object->GetObjectType() == GOAL)
	{
		// �Ō�̈ʒu�E��Ԃ�1frame�����I�ɋL�^
		SaveMoveHistory();

		// �C�ӁF���S��~������Ԃ�1frame�ǉ�����iIDLE��ԁj
		PlayerMoveRecord stop_record;
		stop_record.position = this->location;     // �S�[���n�_
		stop_record.flip = this->flip_flg;
		stop_record.action_state = ActionState::IDLE; // �~�܂��Ă�����

		move_history.push_back(stop_record);
	}
}


void Player::SaveMoveHistory()
{
	PlayerMoveRecord record;
	record.position = this->location;
	record.flip = this->flip_flg;
	record.action_state = this->action_state; // �� ������Y�ꂸ�ɁI

	move_history.push_back(record);
}

void Player::ApplyDamage()
{
	damage_flg = true;
	hp--;

}

void Player::LoadPlayerImage()
{
	ResourceManager* rm = ResourceManager::GetInstance();

	// IDLE
	auto idle_imgs = rm->GetImages("Resource/Images/Character/Player/Player-idle/player-idle", 6);
	animation_data[ActionState::IDLE] = idle_imgs;

	// WALK
	auto walk_imgs = rm->GetImages("Resource/Images/Character/Player/Player-run/player-run", 6);
	animation_data[ActionState::WALK] = walk_imgs;

	// JUMP
	auto jump_imgs = rm->GetImages("Resource/Images/Character/Player/Player-jump/player-jump", 2);
	animation_data[ActionState::JUMP] = jump_imgs;

	image = animation_data[ActionState::IDLE][0];
}

void Player::InvincibleEffect(Vector2D offset) 
{
	if (is_invincible)
	{
		int alpha = 255;

		if (invincible_timer >= 180)
		{
			int blink_cycle = 10;
			bool visible = ((invincible_timer / blink_cycle) % 2 == 0);
			alpha = visible ? 255 : 50;
		}

		float radius_x = box_size.x * 0.5f;  // X�����̔��a
		float radius_y = box_size.y * 0.6f;  // Y�����̔��a (�ȉ~���o���Ȃ班���傫��)

		Vector2D center = { offset.x + box_size.x / 2, offset.y + box_size.y / 2 + 4.5f };

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

		// �ւ����̑ȉ~��`��
		DrawEllipseAA(center.x, center.y, radius_x, radius_y, 64, GetColor(0, 200, 255), false, 2);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 70);
		DrawEllipseAA(center.x, center.y, radius_x, radius_y, 64, GetColor(0, 200, 255), true, 2);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		

	}


}

void Player::DrawEllipseAA(float cx, float cy, float rx, float ry, int num_segments, int color, bool fill, int line_thickness)
{
	float angle_step = 2.0f * DX_PI_F / num_segments;

	if (fill)
	{
		// ������h��ꍇ�͑��p�`�Ƃ��ēh��
		for (int i = 0; i < num_segments; ++i)
		{
			float theta1 = angle_step * i;
			float theta2 = angle_step * (i + 1);

			float x1 = cx + rx * cosf(theta1);
			float y1 = cy + ry * sinf(theta1);

			float x2 = cx + rx * cosf(theta2);
			float y2 = cy + ry * sinf(theta2);

			DrawTriangle(cx, cy, x1, y1, x2, y2, color, TRUE);
		}
	}
	else
	{
		// �������`���i�ւ����j
		float prev_x = cx + rx * cosf(0);
		float prev_y = cy + ry * sinf(0);

		for (int i = 1; i <= num_segments; ++i)
		{
			float theta = angle_step * i;

			float x = cx + rx * cosf(theta);
			float y = cy + ry * sinf(theta);

			DrawLine(prev_x, prev_y, x, y, color, line_thickness);

			prev_x = x;
			prev_y = y;
		}
	}
}

void Player::UpdateHealParticle(HealParticle& particle)
{
	if (!particle.is_active) return;

	particle.position += particle.velocity;
	particle.velocity *= 0.95f;
	particle.scale *= 0.98f; // ���X�ɏ������Ȃ�

	particle.timer++;
	if (particle.timer >= particle.duration)
	{
		// ���C�t�^�C���I���Ŕ�A�N�e�B�u��
		particle.is_active = false; 
	}


}

void Player::DrawHealParticle(const HealParticle& particle, Vector2D offset)
{
	if (!particle.is_active) return;

	float t = static_cast<float>(particle.timer) / particle.duration;
	float alpha = static_cast<int>(255 * (1.0f - t)); // ���X�ɓ����ɂȂ�

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawCircleAA(particle.position.x + offset.x, particle.position.y + offset.y, 3.0f * particle.scale, 12, GetColor(100, 255, 100), true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
