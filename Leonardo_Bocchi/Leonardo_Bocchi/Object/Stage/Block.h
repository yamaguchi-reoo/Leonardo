#pragma once
#include "../GameObject.h"
#include "../../common.h"
class Block :
    public GameObject
{
private:

public:
    //����������
    void Initialize(Vector2D _location, Vector2D _box_size) override;
    //�X�V����
    void Update()override;
    //�`�揈��
    void Draw(Vector2D offset, double rate) const override;
    //�I��������
    void Finalize()override;

    void Draw3DBlock(Vector2D offset)const;
};

