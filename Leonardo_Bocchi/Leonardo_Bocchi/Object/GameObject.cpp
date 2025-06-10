#include <DxLib.h>
#include "GameObject.h"
#include <math.h>
#include "../common.h"

GameObject::GameObject() : location(), box_size(),hit_box(), image(NULL), object_type(), flip_flg(FALSE)
{
}

GameObject::~GameObject()
{
}

void GameObject::Initialize(Vector2D _location, Vector2D _box_size)
{
	//位置を設定
	location = _location;
	//サイズを設定
	box_size = _box_size;

	hit_box = Vector2D(box_size.x, box_size.y);

	//画像を設定
	//image = NULL;
}

void GameObject::Update()
{
}

void GameObject::Draw(Vector2D offset, double rate) 
{
	// 描画範囲の判定
	if (offset.x + box_size.x >= 0 && offset.x < SCREEN_WIDTH)
	{
		//メンバの情報を基に画像を描画する
		DrawRotaGraphF(offset.x + (box_size.x / 2), offset.y + (box_size.y / 2), rate, 0.0, image, TRUE, flip_flg);
		// デバッグ用の当たり判定ボックス描画

		//DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(255, 0, 0), FALSE);
	}

	//描画範囲の可視化
	DrawBoxAA(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(255, 0, 0), FALSE);
}

void GameObject::Finalize()
{
}

void GameObject::SetLocation(Vector2D _location)
{
	this->location = _location;
}

Vector2D GameObject::GetLocation() const
{
	return this->location;
}

Vector2D GameObject::GetBoxSize() const
{
	return this->box_size;
}

Vector2D GameObject::GetVelocity() const
{
	return this->velocity;
}


eObjectType GameObject::GetObjectType()
{
	return this->object_type;
}

void GameObject::OnHitCollision(GameObject* hit_object)
{

}

bool GameObject::CheckBoxCollision(GameObject* obj)
{
	//自分の左上座標
	Vector2D my_pos = location;
	//自分の幅と高さの半分
	Vector2D my_size = hit_box / 2.0f;

	//相手の左上座標
	Vector2D sub_pos = obj->GetLocation();
	//相手の幅と高さの半分
	Vector2D sub_size = obj->GetBoxSize() / 2.0f;

	//中心座標の差分
	Vector2D diff = (my_pos + my_size) - (sub_pos + sub_size);

	//当たり判定の演算
	return (fabsf(diff.x) <= my_size.x + sub_size.x &&
		fabsf(diff.y) <= my_size.y + sub_size.y);

}

void GameObject::DrawEllipseAA(float cx, float cy, float rx, float ry, int num_segments, int color, bool fill, int line_thickness)
{
	float angle_step = 2.0f * DX_PI_F / num_segments;

	if (fill)
	{
		// 内側を塗る場合は多角形として塗る
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
		// 線だけ描く（輪っか）
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

