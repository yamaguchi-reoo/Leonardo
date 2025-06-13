#include "Trap.h"
#include <DxLib.h>
#include "../../Utility/UtilityList.h"

void Trap::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);
	object_type = TRAP;

	image = LoadGraph("Resource/Images/Gimmick/trap.png");
	LoadTrapImage();
}

void Trap::Update()
{
	if (is_active)
	{
		effect_timer++;
		if (effect_timer >= 60)
		{
			is_active = false;
		}
	}
}

void Trap::Draw(Vector2D offset, double rate) 
{
	offset.y += 12.5f; 
	__super::Draw(offset, 0.45);
	//DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(0, 0, 255), FALSE);

	if (is_active)
	{
		int frame_index = (effect_timer / 5) % 5;

		DrawGraph(offset.x + box_size.x / 2 - 25,offset.y + box_size.y / 2 - 32,animation_data[frame_index],TRUE);
		DrawGraph(offset.x + box_size.x / 2 - 5,offset.y + box_size.y / 2 - 32,animation_data[frame_index],TRUE);
		DrawGraph(offset.x + box_size.x / 2 + 15,offset.y + box_size.y / 2 - 32,animation_data[frame_index],TRUE);
		
	}
}

void Trap::Finalize()
{
}

void Trap::OnHitCollision(GameObject* hit_object)
{
	if (hit_object->GetObjectType() == PLAYER)
	{
		// 既にアクティブでないときのみ再生開始
		if (!is_active)
		{
			is_active = true;
			effect_timer = 0;
		}
	}
}


void Trap::LoadTrapImage()
{
	ResourceManager* rm = ResourceManager::GetInstance();

	animation_data = rm->GetImages("Resource/Images/Gimmick/force-field", 5);

	trap_image = animation_data[0]; // 最初の画像を設定
}