#include "GameMainScene.h"
#include "DxLib.h"
#include "../../Object/ObjectList.h"
#include "../../Utility/UtilityList.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <random>

GameMainScene::GameMainScene() :stage_width_num(0), stage_height_num(0), stage_data{ 0 }, player(nullptr), back_ground_image(0), clone_spawn_timer(0.0f), is_create(false), is_game_over(false), fade_alpha(0), font_48(0), font_24(0), trap_num(0), decision_se(-1),main_bgm(-1),game_over_se(-1)
{
}

GameMainScene::~GameMainScene()
{
}

void GameMainScene::Initialize()
{
	//ステージを読み込む
	LoadStage();
	//カメラの初期位置を設定
	camera_location = Vector2D(0.0f, 0.0f); //カメラの初期位置を設定
	//back_ground_image = LoadGraph("Resource/Images/back_ground.png"); // 背景画像を読み込む
	back_ground_image = LoadGraph("Resource/Images/BackGround/Base_Color.png"); // 背景画像を読み込む


	LoadGameMainResource();
	PlaySoundBgm(main_bgm, 60);

	trap_num = 3;
}

eSceneType GameMainScene::Update()
{
	//ステージリロード	
	if (IsStageReload() && goal_point && !goal_point->IsActive())
	{
		//ステージクリア時の処理
		StageClear();
		//ステージの再読み込み
		ReLoadStage();
	}
	//カメラ更新
	UpdateCamera();

	// プレイヤーがいない場合のみ探す
	if (!player)
	{
		FindPlayer();
	}


	if (!is_create)
	{
		clone_spawn_timer++;
		if (clone_spawn_timer >= 50)
		{
			CreateClone();
			clone_spawn_timer = 0;
			is_create = true;
		}
	}

	//死亡処理
	if (player->GetHp() <= 0 || player->GetLocation().y > 850.0f)
	{
		player->SetDelete();
		is_game_over = true;
	}


	if (is_game_over)
	{
		if (fade_alpha < 200)
		{
			fade_alpha += 5;
		}
		
		InputControl* input = InputControl::GetInstance();

		if (input->GetButtonDown(XINPUT_BUTTON_A))
		{
			PlaySoundSe(decision_se, 80);	
			is_decided = true;
			game_over_timer = 0;
			//return eSceneType::RESULT;
			is_game_over = false;
		}
	}

	//効果音再生中は何もしないで、再生完了を待つ
	if (is_decided)
	{
		game_over_timer++;
		if (game_over_timer >= 30)
		{
			return eSceneType::RESULT; // 効果音が終わったら遷移
		}
	}

	return __super::Update();
}

void GameMainScene::Draw()
{
	ResourceManager* rm = ResourceManager::GetInstance();

	//DrawGraph(0, 0, back_ground_image, TRUE); // 背景画像を読み込む
	DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(10, 10, 30), TRUE); // 背景色を描画

	__super::Draw();

	InputControl* input = InputControl::GetInstance();
	if (input->GetKey(KEY_INPUT_0)) {
		for (int i = 0; i < stage_height_num; ++i)
		{
			for (int j = 0; j < stage_width_num; ++j)
			{
				DrawFormatString(j * 48, i * 48, GetColor(255, 255, 255), "%d", stage_data[i][j]);
			}
		}
	}

	DrawUI();

	// ゲームオーバー中なら文字を描画
	if (is_game_over)
	{
		// 半透明の黒い背景
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade_alpha);
		DrawBox(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// ゲームオーバー文字
		const char* msg = "GAME OVER";
		int text_width = GetDrawStringWidthToHandle(msg, strlen(msg), font_48);
		DrawStringToHandle((SCREEN_WIDTH - text_width) / 2, SCREEN_HEIGHT / 2 - 50, msg, GetColor(255, 0, 0), font_48);


		const char* hint = "Press A to continue";
		int hint_width = GetDrawStringWidthToHandle(hint, strlen(hint), font_24);
		DrawStringToHandle((SCREEN_WIDTH - hint_width) / 2, SCREEN_HEIGHT / 2 + 90, hint, GetColor(255, 255, 255), font_24);
	}

	//DrawFormatString(10, 500, GetColor(255, 255, 255), "%d", trap_num);
}

void GameMainScene::Finalize()
{
	DeleteGraph(back_ground_image);
	back_ground_image = 0;

	StopGameMainSound();   // シーン終了時に停止
	ResourceManager::GetInstance()->UnloadResourcesAll();
	ResourceManager::DeleteInstance();
}

eSceneType GameMainScene::GetNowSceneType() const
{
	return eSceneType::GAME_MAIN;
}

void GameMainScene::LoadStage()
{
	std::ifstream file("Resource/file/stage.csv");

	if (!file) {
		std::cerr << "ファイルを開けませんでした: " << std::endl;
		return;
	}
	// 1行目を読み込んでステージ幅と高さを取得
	std::string line;

	//fileから1行読み込んで、line に格納。
	if (std::getline(file, line)) {
		//文字列を解析するためのストリームを作成。
		std::stringstream ss(line);
		std::string width, height;

		// カンマで分割して幅と高さを取得
		std::getline(ss, width, ',');
		std::getline(ss, height, ',');

		//文字列を整数に変換
		stage_width_num = std::stoi(width);   // ステージ幅
		stage_height_num = std::stoi(height); // ステージ高さ
	}

	//ステージデータの読み込み（CSVの2行目以降）
	for (int i = 0; i < stage_height_num; i++) {
		//1行ずつ読み込む
		if (std::getline(file, line)) {
			std::stringstream ss(line);
			for (int j = 0; j < stage_width_num; j++) {
				//カンマ区切りでデータを取得
				std::string value;
				if (std::getline(ss, value, ',')) {
					//文字列を整数に変換してステージデータに格納
					stage_data[i][j] = std::stoi(value);
				}
			}
		}
	}

	file.close();

	SetStage();
}

void GameMainScene::SetStage()
{
	for (int i = 0; i < stage_height_num; ++i) {
		for (int j = 0; j < stage_width_num; ++j) {
			int x = j * BOX_SIZE;
			int y = 720 - ((stage_height_num - i) * BOX_SIZE);
			Vector2D pos(x, y);

			switch (stage_data[i][j]) {
			case EMPTY: break;
			case BLOCK: CreateObject<Block>(pos, Vector2D((float)BOX_SIZE)); break;
			case PLAYER: CreateObject<Player>(pos, Vector2D(48.0f, 64.0f)); break;
			case MOVE_BLOCK: CreateObject<MoveBlock>(pos, Vector2D((float)BOX_SIZE, 24.0f)); break;
			case GOAL: CreateObject<GoalPoint>(pos, Vector2D((float)BOX_SIZE / 2, (float)BOX_SIZE / 1.5));
				goal_pos = pos;
				break;
			default: break;
			}
		}
	}

	CreateItem();
	CreateGimmick();

}

void GameMainScene::UpdateCamera()
{
	//プレイヤーが存在するならカメラを追従させる
	if (player)
	{
		float screen_half_width = SCREEN_WIDTH / 2;				//画面の半分の幅
		float stage_limit_left = 0.0f;							//ステージの左端
		float stage_limit_right = stage_width_num * BOX_SIZE - SCREEN_WIDTH; //ステージの右端 

		//カメラ位置 ＝ プレイヤーの位置 - 画面の半分の幅 
		camera_location.x = player->GetLocation().x - screen_half_width;

		//画面端ではスクロールしないよう制限
		if (camera_location.x < stage_limit_left) camera_location.x = stage_limit_left;
		if (camera_location.x > stage_limit_right) camera_location.x = stage_limit_right;
	}
}

void GameMainScene::StageClear()
{
	clear_count++;
	//プレイヤーを取得
	Player* p = static_cast<Player*>(player);

	if (p){
		// プレイヤーの移動履歴を保存
		stage_clear_history.push_back(p->GetMoveHistory());
	}
	trap_num += 1;
}

void GameMainScene::ReLoadStage()
{
	//オブジェクトの削除
	for (auto obj : objects)
	{
		delete obj;
		obj = nullptr;
	}
	player = nullptr;
	objects.clear();
	stage_reload = false;
	is_create = false;
	LoadStage();

}

void GameMainScene::FindPlayer()
{
	//プレイヤーオブジェクトを探して取得
	for (auto obj : objects)
	{
		if (obj->GetObjectType() == PLAYER)
		{
			player = obj;
			break;
		}
	}
}

void GameMainScene::CreateClone()
{
	//ステージクリア時のプレイヤーの履歴を基にエネミーを生成
	for (const auto& history : stage_clear_history)
	{
		//新しいエネミー（過去のプレイヤー）を生成
		Enemy* enemy = CreateObject<Enemy>(Vector2D(0.0f, 0.0f) , Vector2D(48.0f, 64.0f));

		// 履歴をエネミーにセット
		enemy->SetReplayHistory(history);
	}
}

void GameMainScene::CreateItem()
{
	std::vector<Vector2D> item_positions;

	// アイテムを置く候補位置を探す（空＋その下がブロック）
	for (int i = 0; i < stage_height_num - 1; ++i) {
		for (int j = 0; j < stage_width_num; ++j) {
			if (stage_data[i][j] != EMPTY) continue;
			if (stage_data[i + 1][j] != BLOCK) continue;

			int x = j * BOX_SIZE;
			int y = 720 - ((stage_height_num - i) * BOX_SIZE);

			// プレイヤーがすぐ見えない位置かつゴール前
			if (x > (SCREEN_WIDTH - 280) && x < goal_pos.x) {
				item_positions.emplace_back(x, y);
			}
		}
	}

	// 候補をシャッフル
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(item_positions.begin(), item_positions.end(), gen);

	// 最大3個までアイテムを生成
	int item_count = Min(3, static_cast<int>(item_positions.size()));
	for (int i = 0; i < item_count; ++i) {
		const Vector2D& pos = item_positions[i];

		int grid_x = pos.x / BOX_SIZE;
		int grid_y = stage_height_num - ((720 - pos.y) / BOX_SIZE);

		if (i % 2 == 0) {
			CreateObject<HealItem>(pos, Vector2D((float)BOX_SIZE));
			stage_data[grid_y][grid_x] = HEAL;
		}
		else {
			CreateObject<InvincibleItem>(pos, Vector2D((float)BOX_SIZE));
			stage_data[grid_y][grid_x] = INVINCIBLE;
		}
	}
}


void GameMainScene::CreateGimmick()
{
	std::vector<Vector2D> trap_positions;

	// トラップを置く候補位置を探す（空＋その下がブロック）
	for (int i = 0; i < stage_height_num - 1; ++i) {
		for (int j = 0; j < stage_width_num; ++j) {
			if (stage_data[i][j] != EMPTY) continue;
			if (stage_data[i + 1][j] != BLOCK) continue;

			// このマスにアイテムがあればスキップ（かぶり防止）
			if (stage_data[i][j] == HEAL || stage_data[i][j] == INVINCIBLE) continue;

			int x = j * BOX_SIZE;
			int y = 720 - ((stage_height_num - i) * BOX_SIZE); // ← CreateItemと同じ

			// ゴール前の画面右端エリア
			if (x > (SCREEN_WIDTH - 280) && x < goal_pos.x) {
				trap_positions.emplace_back(x, y);
			}
		}
	}

	// 候補をシャッフル
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(trap_positions.begin(), trap_positions.end(), gen);

	// 最大5個までトラップを生成
	const int trap_count = Min(trap_num, static_cast<int>(trap_positions.size()));
	for (int i = 0; i < trap_count; ++i) {
		const Vector2D& pos = trap_positions[i];

		int grid_x = pos.x / BOX_SIZE;
		int grid_y = stage_height_num - ((720 - pos.y) / BOX_SIZE);

		CreateObject<Trap>(pos, Vector2D((float)BOX_SIZE));
		stage_data[grid_y][grid_x] = TRAP;
	}
}

void GameMainScene::LoadGameMainResource()
{
	ResourceManager* rm = ResourceManager::GetInstance();

	//サウンド読込
	//rm->GetSound("Resource/Sounds/BGM/AS_259735_ストイックなサイバー感4つ打ち.mp3");
	sounds_data = rm->GetSound("Resource/Sounds/BGM/AS_259735_ストイックなサイバー感4つ打ち.mp3");
	main_bgm = sounds_data[0];

	sounds_data = rm->GetSound("Resource/Sounds/SE/AS_1296213_サイバーな感じの決定音.mp3");
	decision_se = sounds_data[0];

	sounds_data = rm->GetSound("Resource/Sounds/SE/AS_1593387_がっくり、ダメな感じの音.mp3");
	game_over_se = sounds_data[0];

	//フォント読込
	rm->LoadFont("Resource/Font/TepidTerminal.ttf", "Tepid Terminal");
	font_48 = rm->GetFontHandle("Tepid Terminal", 48);
	font_24 = rm->GetFontHandle("Tepid Terminal", 24);

	//UI画像読込
	heart_img = LoadGraph("Resource/Images/UI/Hp.png"); // ハート画像を読み込む
}


void GameMainScene::PlaySoundBgm(int _handle, int volume)
{
	ChangeVolumeSoundMem(volume, _handle);
	PlaySoundMem(_handle, DX_PLAYTYPE_LOOP); // BGMをループ再生
}

void GameMainScene::PlaySoundSe(int _handle, int volume)
{
	ChangeVolumeSoundMem(volume, _handle);
	PlaySoundMem(_handle, DX_PLAYTYPE_BACK); // SEは1回のみ再生
}

void GameMainScene::StopGameMainSound()
{
	if (!sounds_data.empty()) {
		StopSoundMem(sounds_data[0]);
	}
}

void GameMainScene::DrawUI() const
{
	if (player) 
	{
		// ハート描画
		for (int i = 0; i < player->GetHp(); ++i)
		{
			DrawGraph(20 + i * 50, 20, heart_img, TRUE);
		}

	}

	// LOOP テキスト生成
	std::string loop_text = "LOOP : " + std::to_string(clear_count);
	int text_width = GetDrawStringWidthToHandle(loop_text.c_str(), -1, font_48);
	int text_height = GetFontSizeToHandle(font_48); // 高さも取得

	// 表示位置（右上）
	int padding = 10;
	int box_x = SCREEN_WIDTH - text_width - padding * 2 - 50;
	int box_y = 20;
	int box_w = text_width + padding * 2;
	int box_h = text_height + padding * 2;

	// 背景描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(box_x, box_y, box_x + box_w, box_y + box_h, GetColor(30, 30, 60), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 枠線（白）
	DrawBox(box_x, box_y, box_x + box_w, box_y + box_h, GetColor(255, 255, 255), FALSE);

	// テキスト描画
	DrawStringToHandle(box_x + padding, box_y + padding, loop_text.c_str(), GetColor(255, 255, 455), font_48);

}

