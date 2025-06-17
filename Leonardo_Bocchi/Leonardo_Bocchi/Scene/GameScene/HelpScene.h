#pragma once
#include "../SceneBase.h"
#include <vector>
#include <string>
#include <map>

enum class HelpObjectType {
	Image,
	Shapes,
};

struct HelpInfo
{
	std::string image_path; //画像のパス
	std::string name;		//名前
	std::string text;		//テキスト
	HelpObjectType type;	//オブジェクトの種類
};

class HelpScene :
    public SceneBase
{
private:
	int font_small;
	int font_large;

	int current_page; // 現在のページ番号
	int total_pages; // 総ページ数

	std::vector < std::vector<std::string>> help_texts; // ページごとのヘルプテキスト

	enum class PlayerAnimationType { IDLE, WALK, JUMP };

	int frame_width = 64;
	int frame_height = 64;

	std::map<PlayerAnimationType, int> animation_frame;
	std::map<PlayerAnimationType, int> animation_timer;

	std::map<PlayerAnimationType, std::vector<int>> animation_data; // {開始フレーム, フレーム数}
public:
	HelpScene();
	~HelpScene();

	void Initialize() override;		//初期化処理
	eSceneType Update() override;	//更新処理
	void Draw() override;		//描画処理
	void Finalize() override;		//終了時処理

	eSceneType GetNowSceneType()const override;

	void DrawPlayerControls();
	void DrawObjectPage();

	void DrawTeleport(int x, int y);

	void DrawEllipseAA(float cx, float cy, float rx, float ry, int num_segments, int color, bool fill, int line_thickness);
};

