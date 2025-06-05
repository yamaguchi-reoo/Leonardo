#pragma once
#include "../GameObject.h"
class MoveBlock :
    public GameObject
{
private:
    float move_range_right = 0.0f;
    float move_range_left = 0.0f;
public:
    //����������
    void Initialize(Vector2D _location, Vector2D _box_size) override;
    //�X�V����
    void Update()override;
    //�`�揈��
    void Draw(Vector2D offset, double rate) const override;
    //�I��������
    void Finalize()override;

    //�����������̋���
    void OnHitCollision(GameObject* hit_object)override;
};

