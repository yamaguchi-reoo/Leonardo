#include "HealItem.h"
#include <DxLib.h>

void HealItem::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);
	object_type = HEAL;

	image = LoadGraph("Resource/Images/Items/heal01.png");
	//image = LoadGraph("Resource/Images/Items/heart.png");
}

void HealItem::Update()
{
}

void HealItem::Draw(Vector2D offset, double rate) const
{
	__super::Draw(offset, 2.0f);
	//DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(255, 255, 255), TRUE);
}

void HealItem::Finalize()
{
}

void HealItem::OnHitCollision(GameObject* hit_object)
{
	if (hit_object->GetObjectType() == PLAYER)
	{
		this->SetDelete();
	}
}
