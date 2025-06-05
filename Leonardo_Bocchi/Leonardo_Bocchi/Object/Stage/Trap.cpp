#include "Trap.h"
#include <DxLib.h>

void Trap::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);
	object_type = TRAP;

}

void Trap::Update()
{
}

void Trap::Draw(Vector2D offset, double rate) const
{
	DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(0, 0, 255), FALSE);
}

void Trap::Finalize()
{
}

void Trap::OnHitCollision(GameObject* hit_object)
{
}
