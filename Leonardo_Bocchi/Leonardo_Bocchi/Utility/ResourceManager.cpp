#include "ResourceManager.h"
#include "DxLib.h"

//静的メンバ変数定義
ResourceManager* ResourceManager::instance = nullptr;

/// <summary>
/// リソース管理クラスのインスタンス取得処理
/// </summary>
/// <returns>クラスのポインタ</returns>
ResourceManager* ResourceManager::GetInstance()
{
	//インスタンスが生成されていない場合、生成する
	if (instance == nullptr)
	{
		instance = new ResourceManager();
	}
	//インスタンスのポインタを返却する
	return instance;
}

/// <summary>
/// リソース管理クラスのインスタンス削除処理
/// </summary>
void ResourceManager::DeleteInstance()
{
	//インスタンスが存在していれば、削除する
	if (instance != nullptr)
	{
		instance->UnloadResourcesAll();
		delete instance;
		instance = nullptr;
	}
}

/// <summary>
/// 画像を取得する
/// </summary>
/// <param name="file_name">ファイルパス</param>
/// <param name="all_num">画像の総数</param>
/// <param name="num_x">横の総数</param>
/// <param name="num_y">縦の総数</param>
/// <param name="size_x">横のサイズ（ｐｘ）</param>
/// <param name="size_y">縦のサイズ（ｐｘ）</param>
/// <returns>読み込んだ画像ハンドルのstd::vector配列</returns>
const std::vector<int>& ResourceManager::GetImages(std::string file_name, int all_num, int num_x, int num_y, int size_x, int size_y)
{
	//コンテナ内に指定ファイルがなければ、生成する
	if (images_container.count(file_name) == NULL)
	{
		if (num_x == 0 || num_y == 0)//if (all_num == 1)
		{
			//CreateImagesResource(file_name);
			CreateImagesResourceSingle(file_name, all_num);
		}
		else
		{
			CreateImagesResource(file_name, all_num, num_x, num_y, size_x, size_y);
		}
	}
	return images_container[file_name];
}

const std::vector<int>& ResourceManager::GetImages(const char* file_name, int all_num, int num_x, int num_y, int size_x, int size_y)
{
	return GetImages(std::string(file_name), all_num, num_x, num_y, size_x, size_y);
}

const std::vector<int>& ResourceManager::GetImages(MaterialParam element)
{
	return GetImages(element.file_path, element.all_num, element.num_x, element.num_y, element.size_x, element.size_y);
}

const std::vector<int>& ResourceManager::GetSound(std::string file_name)
{
	//コンテナ内に指定ファイルがなければ、生成する
	if (sounds_container.count(file_name) == NULL)
	{
		CreateSoundResource(file_name);
	}
	return sounds_container[file_name];
}

const std::vector<int>& ResourceManager::GetSound(const char* file_name)
{
	return GetSound(std::string(file_name));
}

const std::vector<int>& ResourceManager::GetSound(SoundParam element)
{
	return GetSound(element.file_path);
}

void ResourceManager::UnloadResourcesAll()
{
	//コンテナ内に画像がなければ、処理を終了する
	if (images_container.size() == NULL)
	{
		return;
	}

	//全ての画像を削除
	for (std::pair<std::string, std::vector<int>> value : images_container)
	{
		DeleteSharingGraph(value.second[0]);
		value.second.clear();
	}

	//全てのSoundを削除
	for (std::pair<std::string, std::vector<int>> value : sounds_container)
	{
		DeleteSoundMem(value.second[0]);
		value.second.clear();
	}

	//コンテナを開放
	images_container.clear();
	sounds_container.clear();

}

void ResourceManager::CreateImagesResource(std::string file_name)
{
	//指定されたファイルを読み込む
	int handle = LoadGraph(file_name.c_str());

	//エラーチェック
	if (handle == -1)
	{
		throw(file_name + "がありません\n");
	}

	//コンテナに読み込んだ画像を追加する
	images_container[file_name].push_back(handle);
}

void ResourceManager::CreateImagesResource(std::string file_name, int all_num, int num_x, int num_y, int size_x, int size_y)
{
	//指定されたファイルを読み込む
	int* handle = new int[all_num];
	int err = LoadDivGraph(file_name.c_str(), all_num, num_x, num_y, size_x, size_y, handle);

	//エラーチェック
	if (err == -1) {
		throw(file_name + "がありません\n");
	}

	//コンテナに読み込んだ画像を追加する
	for (int i = 0; i < all_num; i++)
	{
		images_container[file_name].push_back(handle[i]);
	}

	//動的メモリ開放
	delete[] handle;
}

void ResourceManager::CreateImagesResourceSingle(std::string file_name, int all_num)
{
	std::vector<int> handles;

	for (int i = 1; i <= all_num; ++i)
	{
		char buf[256];
		sprintf_s(buf, sizeof(buf), "%s%d.png", file_name.c_str(), i);


		int handle = LoadGraph(buf);
		if (handle == -1)
		{
			throw(std::string(buf) + " がありません\n");
		}
		handles.push_back(handle);
	}

	images_container[file_name] = handles;
}

void ResourceManager::CreateSoundResource(std::string file_name)
{
	int handle = LoadSoundMem(file_name.c_str());;

	//エラーチェック
	if (handle == -1) {
		throw(file_name + "がありません\n");
	}

	// コンテナに読み込んだ音源を追加する
	sounds_container[file_name].push_back(handle);
}
