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
	/// <summary>
	/// リソース管理クラスのインスタンス取得処理
	/// </summary>
	/// <returns>クラスのポインタ</returns>
	static ResourceManager* GetInstance();

	/// <summary>
	/// リソース管理クラスのインスタンス削除処理
	/// </summary>
	static void DeleteInstance();

public:
	/// <summary>
	/// 画像を取得
	/// </summary>
	/// <param name="file_name">ファイルパス</param>
	/// <param name="all_num">画像の総数</param>
	/// <param name="num_x">横の総数</param>
	/// <param name="num_y">縦の用数</param>
	/// <param name="size_x">横のサイズ（ｐｘ）</param>
	/// <param name="size_y">縦のサイズ（ｐｘ）</param>
	/// <returns>読み込んだ画像ハンドルのstd::vector配列</returns>
	const std::vector<int>& GetImages(std::string file_name, int all_num = 1, int num_x = 1, int num_y = 1, int size_x = 0, int size_y = 0);
	const std::vector<int>& GetImages(const char* file_name, int all_num = 1, int num_x = 1, int num_y = 1, int size_x = 0, int size_y = 0);
	const std::vector<int>& GetImages(MaterialParam element);

	/// <summary>
	/// 音源の取得
	/// </summary>
	/// <param name="file_name">ファイルパス</param>
	/// <param name="sound_type">サウンドタイプ</param>
	/// <returns>読み込んだ音源のstd::vector配列</returns>
	const std::vector<int>& GetSound(std::string file_name);
	const std::vector<int>& GetSound(const char* file_name);
	const std::vector<int>& GetSound(SoundParam element);

	/// <summary>
	/// material_handle 削除したい画像ハンドルのstd::vector配列
	/// </summary>
	void UnloadResourcesAll();

private:
	/// <summary>
	/// 画像ハンドルを読み込みリソースを作成する
	/// </summary>
	/// <param name="file_name">ファイルパス</param>
	void CreateImagesResource(std::string file_name);

	/// <summary>
	/// 画像ハンドルを読み込みリソースを作成する
	/// </summary>
	/// <param name="file_name">ファイルパス</param>
	/// <param name="all_num">画像の総数</param>
	/// <param name="num_x">横の総数</param>
	/// <param name="num_y">縦の総数</param>
	/// <param name="size_x">横のサイズ（ｐｘ）</param>
	/// <param name="size_y">縦のサイズ（ｐｘ）</param>
	void CreateImagesResource(std::string file_name, int all_num, int num_x, int num_y, int size_x, int size_y);


	void CreateImagesResourceSingle(std::string file_name, int all_num);

	/// <summary>
	/// 音源読み込みリソースを作成する
	/// </summary>
	/// <param name="file_name">ファイルパス</param>
	/// <param name="sound_type">サウンドタイプ</param>
	void CreateSoundResource(std::string file_name);
};


