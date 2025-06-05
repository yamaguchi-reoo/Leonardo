#include "GoalPoint.h"
#include <DxLib.h>

void GoalPoint::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);

	object_type = GOAL;
}

void GoalPoint::Update()
{
	__super::Update();
}

void GoalPoint::Draw(Vector2D offset, double rate) const
{
	//__super::Draw(offset, 1.0f);
	DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(255, 255, 0), TRUE);
	DrawString(offset.x + 2, location.y, "Goal", GetColor(255, 255, 255));
}

void GoalPoint::Finalize()
{
}

void GoalPoint::OnHitCollision(GameObject* hit_object)
{

}
