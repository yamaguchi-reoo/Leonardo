#pragma once

#include <map>
#include <string>
#include <vector>

// 画像用パラメータ
struct MaterialParam
{
	std::string file_path;
	int all_num;
	int num_x;
	int num_y;
	int size_x;
	int size_y;
};

// 音源用パラメータ
struct SoundParam
{
	std::string file_path;
};

class ResourceManager
{
private:
	//自クラスのポインタ（アドレスの先にインスタンスを生成する）
	static ResourceManager* instance;

	//画像格納コンテナ
	std::map<std::string, std::vector<int>>images_container;

	//サウンド格納コンテナ
	std::map<std::string, std::vector<int>>sounds_container;

	//フォント格納コンテナ
	std::map<std::string, int>fonts_container;

private:
	//コンストラクタをprivateにすることで、
	//自クラスのメンバ関数でしかインスタンスを生成できないようにする
	ResourceManager() = default;

	//コピーガード
	//クラス外でインスタンスを生成して渡すことができないようにする
	ResourceManager(ResourceManager& v) = delete;
	ResourceManager& operator = (const ResourceManager& v) = delete;
	//~コピーガード

public:
	//デストラクタ
	~ResourceManager() = default;

public:
	/// リソース管理クラスのインスタンス取得処理
	static ResourceManager* GetInstance();

	/// リソース管理クラスのインスタンス削除処理
	static void DeleteInstance();

public:
	/// 画像を取得
	const std::vector<int>& GetImages(std::string file_name, int all_num = 1, int num_x = 1, int num_y = 1, int size_x = 0, int size_y = 0);
	const std::vector<int>& GetImages(const char* file_name, int all_num = 1, int num_x = 1, int num_y = 1, int size_x = 0, int size_y = 0);
	const std::vector<int>& GetImages(MaterialParam element);


	/// 音源の取得
	const std::vector<int>& GetSound(std::string file_name);
	const std::vector<int>& GetSound(const char* file_name);
	const std::vector<int>& GetSound(SoundParam element);

	void LoadFont(std::string file_name, std::string font_name);
	int GetFontHandle(const std::string& font_name, int size);

	/// material_handle 削除したい画像ハンドルのstd::vector配列
	void UnloadResourcesAll();

private:

	/// 画像ハンドルを読み込みリソースを作成する
	void CreateImagesResource(std::string file_name);


	/// 画像ハンドルを読み込みリソースを作成する
	void CreateImagesResource(std::string file_name, int all_num, int num_x, int num_y, int size_x, int size_y);

	void CreateImagesResourceSingle(std::string file_name, int all_num);

	
	/// 音源読み込みリソースを作成する
	void CreateSoundResource(std::string file_name);


};


