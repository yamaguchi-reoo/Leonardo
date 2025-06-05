#include "Block.h"
#include <DxLib.h>

void Block::Initialize(Vector2D _location, Vector2D _box_size)
{
	__super::Initialize(_location, _box_size);

	object_type = BLOCK;

    image = LoadGraph("Resource/Images/Block/block.png");
}

void Block::Update()
{
}

void Block::Draw(Vector2D offset, double rate) const
{
	__super::Draw(offset, 1.5);
	//DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(255, 0, 0), FALSE);
	//DrawBoxAA(offset.x, offset.y, offset.x + box_size.x, offset.y + box_size.y, GetColor(255, 0, 0), TRUE);
	//DrawBoxAA(location.x, location.y, location.x + box_size.x, location.y + box_size.y, GetColor(0, 0, 0), TRUE);
	//DrawBoxAA(location.x, location.y, location.x + box_size.x, location.y + box_size.y, GetColor(255, 255, 255), FALSE);
	//Draw3DBlock(offset);
}

void Block::Finalize()
{
}

void Block::Draw3DBlock(Vector2D offset) const
{
    Vector2D size = box_size;
    int shift = 20;

    // 色設定
    int frontColor = GetColor(255, 0, 0);
    int topColor = GetColor(255, 100, 100);
    int sideColor = GetColor(150, 0, 0);
    int edgeColor = GetColor(0, 0, 0);  // オプション：輪郭用

    // 各頂点座標
    Vector2D A = offset;                             // 左上
    Vector2D B = offset + Vector2D(size.x, 0);       // 右上
    Vector2D C = offset + Vector2D(size.x, size.y);  // 右下
    Vector2D D = offset + Vector2D(0, size.y);       // 左下

    Vector2D A2 = A + Vector2D(shift, -shift);       // 左上（奥）
    Vector2D B2 = B + Vector2D(shift, -shift);       // 右上（奥）
    Vector2D C2 = C + Vector2D(shift, -shift);       // 右下（奥）
    Vector2D D2 = D + Vector2D(shift, -shift);       // 左下（奥）

    // ① 上面（A, B, B2, A2）
    DrawTriangle(A.x, A.y, B.x, B.y, B2.x, B2.y, topColor, TRUE);
    DrawTriangle(A.x, A.y, B2.x, B2.y, A2.x, A2.y, topColor, TRUE);

    // ② 側面（B, C, C2, B2）
    DrawTriangle(B.x, B.y, C.x, C.y, C2.x, C2.y, sideColor, TRUE);
    DrawTriangle(B.x, B.y, C2.x, C2.y, B2.x, B2.y, sideColor, TRUE);

    // ③ 正面（A, B, C, D）
    DrawBoxAA(A.x, A.y, C.x, C.y, frontColor, TRUE);
}
