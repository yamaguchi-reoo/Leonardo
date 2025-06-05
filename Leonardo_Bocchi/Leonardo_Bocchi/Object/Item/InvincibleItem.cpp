#include "InvincibleItem.h"
#include <DxLib.h>

void InvincibleItem::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);
	object_type = INVINCIBLE;

	image = LoadGraph("Resource/Images/Items/adamas.png");
}

void InvincibleItem::Update()
{
}

void InvincibleItem::Draw(Vector2D offset, double rate) const
{
	__super::Draw(offset, rate);
	//DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(0, 255, 0), FALSE);
}

void InvincibleItem::Finalize()
{
}

void InvincibleItem::OnHitCollision(GameObject* hit_object)
{
	if (hit_object->GetObjectType() == PLAYER)
	{
		this->SetDelete();
	}
}
