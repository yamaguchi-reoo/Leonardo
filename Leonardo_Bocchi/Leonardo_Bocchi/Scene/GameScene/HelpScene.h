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

	std::vector<int> sounds_data;
	int select_se;
	int decision_se;
; 
public:
	HelpScene();
	~HelpScene();

	void Initialize() override;		//初期化処理
	eSceneType Update() override;	//更新処理
	void Draw() override;		//描画処理
	void Finalize() override;		//終了時処理

	eSceneType GetNowSceneType()const override;

	// ヘルプの描画関数
	void DrawPlayerControls();
	void DrawObjectPage();
	void DrawTeleport(int x, int y);

	// 楕円を描画する関数
	void DrawEllipseAA(float cx, float cy, float rx, float ry, int num_segments, int color, bool fill, int line_thickness);

	// ヘルプオブジェクトの描画
	void DrawAButton(int x, int y);
	void DrawStick(int x, int y);

	//リソースの読み込み
	void LoadResource();
	//効果音の再生
	void PlaySoundSe(int _handle, int volume);
};

