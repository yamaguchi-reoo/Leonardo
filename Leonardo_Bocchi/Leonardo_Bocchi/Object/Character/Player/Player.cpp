#include <DxLib.h>
#include "Player.h"
#include "../../../Utility/UtilityList.h"


Player::Player() : animation_data(), damage_timer(), is_invincible(false), invincible_timer(0),is_goal(false)
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

	animation_frame = 0;
	animation_count = 0;
	jump_count = 1;

	is_goal = false;
	is_invincible = false;

	sound_manager.LoadSounds();
	LoadPlayerImage();
	PlayerTeleport();

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

	UpdateTeleport();	

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
	//DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(255, 0, 0), FALSE);

	if (is_teleport)
	{
		DrawTeleport(offset);
	}

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

	if (!is_goal) {
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

			// �W�����vSE��炷
			sound_manager.PlaySoundSE(SoundType::JUMP, 25, true);
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
	}
	// �_���[�W���͏�ԑJ�ڂ��Ȃ�
	if (action_state != ActionState::DAMAGE)
		action_state = next_state;

	if (action_state != next_state) {
		action_state = next_state;
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

	int frame_index = (animation_frame / 10) % frames.size();

	static int prev_frame_index = -1;

	if (action_state == ActionState::WALK && on_ground) {
		// ������炷�ׂ��摜�̃C���f�b�N�X
		const std::vector<int> footstep_frames = { 1, 4 };

		if (frame_index != prev_frame_index) {
			for (int footstep_frame : footstep_frames) {
				if (frame_index == footstep_frame) {
					sound_manager.PlaySoundSE(SoundType::WALK, 50, true);
					break;
				}
			}
		}
	}
	prev_frame_index = frame_index;

}

void Player::OnHitCollision(GameObject* hit_object)
{
	__super::OnHitCollision(hit_object);

	// �G�l�~�[�q�b�g�� OR �g���b�v�q�b�g��
	if (hit_object->GetObjectType() == ENEMY || hit_object->GetObjectType() == TRAP)
	{
		if (!damage_flg && !is_invincible && !is_goal)
		{
			ApplyDamage();
		}
	}

	// �񕜃A�C�e���q�b�g��
	if (hit_object->GetObjectType() == HEAL)
	{
		sound_manager.PlaySoundSE(SoundType::HEAL, 50, true); // �񕜉�
		hp += 1;
		for (int i = 0; i < 10; ++i)
		{
			heal_particles.push_back(HealParticle(Vector2D{ box_size.x / 2, box_size.y / 2 }));
		}
	}

	// ���G�A�C�e���q�b�g��
	if (hit_object->GetObjectType() == INVINCIBLE)
	{
		sound_manager.PlaySoundSE(SoundType::INVINCIBLE, 50, true); //�o���A��
		if (!is_invincible)
		{
			is_invincible = true;
		}
	}

	// GOAL�q�b�g��
	if (hit_object->GetObjectType() == GOAL)
	{
		PlayerToGoal();
		is_goal = true;
	}
}


void Player::SaveMoveHistory()
{
	PlayerMoveRecord record;
	record.position = this->location;
	record.flip = this->flip_flg;
	record.action_state = this->action_state;

	move_history.push_back(record);
}

void Player::ApplyDamage()
{
	sound_manager.PlaySoundSE(SoundType::DAMAGE, 70, true); // �_���[�W��
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
		float radius_y = box_size.y * 0.6f;  // Y�����̔��a 

		Vector2D center = { offset.x + box_size.x / 2, offset.y + box_size.y / 2 + 4.5f };

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

		// �ւ����̑ȉ~��`��
		DrawEllipseAA(center.x, center.y, radius_x, radius_y, 64, GetColor(0, 200, 255), false, 2);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 70);
		DrawEllipseAA(center.x, center.y, radius_x, radius_y, 64, GetColor(0, 200, 255), true, 2);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		

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

void Player::PlayerToGoal()
{
	// �Ō�̈ʒu�E��Ԃ�1frame�L�^
	SaveMoveHistory();

	//���S��~������Ԃ�1frame�ǉ�����
	PlayerMoveRecord stop_record;
	stop_record.position = this->location;     // �S�[���n�_
	stop_record.flip = this->flip_flg;
	stop_record.action_state = ActionState::IDLE; // �~�܂��Ă�����

	move_history.push_back(stop_record);

	//sound_manager.PlaySoundSE(SoundType::GOAL, 50, true); // �S�[����
	velocity = Vector2D(0.0f, 0.0f);
	g_velocity = 0.0f;
	//is_invincible = true;
}

void Player::PlayerTeleport()
{
	is_goal = true;
	is_teleport = true;
	teleport_timer = 0;
	teleport_particles.clear();

	Vector2D center = location + box_size / 2;
	// �p�[�e�B�N������
	for (int i = 0; i < 30; ++i)
	{
		float angle = (float)i / 30.0f * 2.0f * DX_PI;
		float radius = 50.0f + rand() % 30;
		Vector2D spawn_pos = location + Vector2D(cosf(angle), sinf(angle)) * radius;
		teleport_particles.emplace_back(spawn_pos);
	}
	sound_manager.PlaySoundSE(SoundType::TELEPORT, 50, true);
}

void Player::UpdateTeleport()
{
	if (is_teleport)
	{
		velocity = Vector2D(0.0f, 0.0f);
		g_velocity = 0.0f;
		//is_invincible = true;

		teleport_timer++;
		if (teleport_timer >= 30) // 30�t���[����Ƀe���|�[�g�I��
		{
			is_goal = false;
			is_teleport = false;
			teleport_timer = 0;
		}
		for (auto& p : teleport_particles)
		{
			if (!p.is_active) continue;

			// �z�����ݕ����ɕ␳�i�p�[�e�B�N���N���X�ɂ͌��X�Ȃ��̂ŋ����㏑���j
			Vector2D dir = location - p.position;
			dir = dir.Normalize() * 0.5f; // �z�����ݑ��x
			p.velocity = dir;
			//p.position += dir;

			p.Update();
		}
	}
}

void Player::DrawTeleport(Vector2D offset)
{
	Vector2D center = location + box_size / 2;
	float radius_x = 30.0f + 6.0f * sinf(teleport_timer * 0.1f);
	float radius_y = 50.0f + 10.0f * cosf(teleport_timer * 0.1f);

	// ���S�O���f�[�V�����ȉ~
	for (int i = 0; i < 5; ++i)
	{
		int alpha = 80 - i * 15;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawEllipseAA(center.x, center.y,
			radius_x * (1.0f - i * 0.15f),
			radius_y * (1.0f - i * 0.15f),
			64, GetColor(100, 200, 255), TRUE, 1);
	}

	// �O�������O
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	for (int i = 0; i < 3; ++i)
	{
		float ring_rx = radius_x + i * 2.0f;
		float ring_ry = radius_y + i * 2.0f;
		DrawEllipseAA(center.x, center.y, ring_rx, ring_ry, 64, GetColor(0, 255, 255), FALSE, 2);
	}

	// �p�[�e�B�N���`��
	for (auto& p : teleport_particles)
	{
		if (!p.is_active) continue;

		float t = static_cast<float>(p.timer) / p.duration;
		int alpha = static_cast<int>(255 * (1.0f - t));
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

		DrawCircleAA(p.position.x + 16, p.position.y + 5, 3.0f * p.scale, 12, GetColor(0, 255, 255), TRUE);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
