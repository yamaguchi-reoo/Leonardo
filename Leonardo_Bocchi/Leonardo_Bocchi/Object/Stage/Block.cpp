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

    // �F�ݒ�
    int frontColor = GetColor(255, 0, 0);
    int topColor = GetColor(255, 100, 100);
    int sideColor = GetColor(150, 0, 0);
    int edgeColor = GetColor(0, 0, 0);  // �I�v�V�����F�֊s�p

    // �e���_���W
    Vector2D A = offset;                             // ����
    Vector2D B = offset + Vector2D(size.x, 0);       // �E��
    Vector2D C = offset + Vector2D(size.x, size.y);  // �E��
    Vector2D D = offset + Vector2D(0, size.y);       // ����

    Vector2D A2 = A + Vector2D(shift, -shift);       // ����i���j
    Vector2D B2 = B + Vector2D(shift, -shift);       // �E��i���j
    Vector2D C2 = C + Vector2D(shift, -shift);       // �E���i���j
    Vector2D D2 = D + Vector2D(shift, -shift);       // �����i���j

    // �@ ��ʁiA, B, B2, A2�j
    DrawTriangle(A.x, A.y, B.x, B.y, B2.x, B2.y, topColor, TRUE);
    DrawTriangle(A.x, A.y, B2.x, B2.y, A2.x, A2.y, topColor, TRUE);

    // �A ���ʁiB, C, C2, B2�j
    DrawTriangle(B.x, B.y, C.x, C.y, C2.x, C2.y, sideColor, TRUE);
    DrawTriangle(B.x, B.y, C2.x, C2.y, B2.x, B2.y, sideColor, TRUE);

    // �B ���ʁiA, B, C, D�j
    DrawBoxAA(A.x, A.y, C.x, C.y, frontColor, TRUE);
}
