#include "MoveBlock.h"
#include <DxLib.h>

void MoveBlock::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);
	object_type = MOVE_BLOCK;

	velocity.x = 0.7f;
}

void MoveBlock::Update()
{
}

void MoveBlock::Draw(Vector2D offset, double rate) const
{
	DrawBoxAA(offset.x, offset.y, offset.x + hit_box.x, offset.y + hit_box.y, GetColor(255, 0, 255), TRUE);
	//DrawFormatString(10, 200, GetColor(255, 255, 255), "%f", move_range_right);
	//DrawFormatString(10, 220, GetColor(255, 255, 255), "%f", location.x);
}

void MoveBlock::Finalize()
{
}

void MoveBlock::OnHitCollision(GameObject* hit_object)
{
}
