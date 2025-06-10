#pragma once
#include "../GameObject.h"
#include <map>
#include <vector>

class Trap :
    public GameObject
{
private:
    std::vector<int> animation_data;
    int animation_frame = 0; //現在のアニメーションフレーム

	int trap_image = NULL;  //トラップの画像

	bool is_active = false; // トラップがアクティブかどうか
	int effect_timer = 0;   // エフェクトのタイマー

public:
    //初期化処理
    void Initialize(Vector2D _location, Vector2D _box_size) override;
    //更新処理
    void Update()override;
    //描画処理
    void Draw(Vector2D offset, double rate) override;
    //終了時処理
    void Finalize()override;

    //当たった時の挙動
    void OnHitCollision(GameObject* hit_object)override;

    void LoadTrapImage();
};

