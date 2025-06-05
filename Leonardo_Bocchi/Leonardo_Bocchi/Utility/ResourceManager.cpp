#include "ResourceManager.h"
#include "DxLib.h"

//�ÓI�����o�ϐ���`
ResourceManager* ResourceManager::instance = nullptr;

/// <summary>
/// ���\�[�X�Ǘ��N���X�̃C���X�^���X�擾����
/// </summary>
/// <returns>�N���X�̃|�C���^</returns>
ResourceManager* ResourceManager::GetInstance()
{
	//�C���X�^���X����������Ă��Ȃ��ꍇ�A��������
	if (instance == nullptr)
	{
		instance = new ResourceManager();
	}
	//�C���X�^���X�̃|�C���^��ԋp����
	return instance;
}

/// <summary>
/// ���\�[�X�Ǘ��N���X�̃C���X�^���X�폜����
/// </summary>
void ResourceManager::DeleteInstance()
{
	//�C���X�^���X�����݂��Ă���΁A�폜����
	if (instance != nullptr)
	{
		instance->UnloadResourcesAll();
		delete instance;
		instance = nullptr;
	}
}

/// <summary>
/// �摜���擾����
/// </summary>
/// <param name="file_name">�t�@�C���p�X</param>
/// <param name="all_num">�摜�̑���</param>
/// <param name="num_x">���̑���</param>
/// <param name="num_y">�c�̑���</param>
/// <param name="size_x">���̃T�C�Y�i�����j</param>
/// <param name="size_y">�c�̃T�C�Y�i�����j</param>
/// <returns>�ǂݍ��񂾉摜�n���h����std::vector�z��</returns>
const std::vector<int>& ResourceManager::GetImages(std::string file_name, int all_num, int num_x, int num_y, int size_x, int size_y)
{
	//�R���e�i���Ɏw��t�@�C�����Ȃ���΁A��������
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
	//�R���e�i���Ɏw��t�@�C�����Ȃ���΁A��������
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
	//�R���e�i���ɉ摜���Ȃ���΁A�������I������
	if (images_container.size() == NULL)
	{
		return;
	}

	//�S�Ẳ摜���폜
	for (std::pair<std::string, std::vector<int>> value : images_container)
	{
		DeleteSharingGraph(value.second[0]);
		value.second.clear();
	}

	//�S�Ă�Sound���폜
	for (std::pair<std::string, std::vector<int>> value : sounds_container)
	{
		DeleteSoundMem(value.second[0]);
		value.second.clear();
	}

	//�R���e�i���J��
	images_container.clear();
	sounds_container.clear();

}

void ResourceManager::CreateImagesResource(std::string file_name)
{
	//�w�肳�ꂽ�t�@�C����ǂݍ���
	int handle = LoadGraph(file_name.c_str());

	//�G���[�`�F�b�N
	if (handle == -1)
	{
		throw(file_name + "������܂���\n");
	}

	//�R���e�i�ɓǂݍ��񂾉摜��ǉ�����
	images_container[file_name].push_back(handle);
}

void ResourceManager::CreateImagesResource(std::string file_name, int all_num, int num_x, int num_y, int size_x, int size_y)
{
	//�w�肳�ꂽ�t�@�C����ǂݍ���
	int* handle = new int[all_num];
	int err = LoadDivGraph(file_name.c_str(), all_num, num_x, num_y, size_x, size_y, handle);

	//�G���[�`�F�b�N
	if (err == -1) {
		throw(file_name + "������܂���\n");
	}

	//�R���e�i�ɓǂݍ��񂾉摜��ǉ�����
	for (int i = 0; i < all_num; i++)
	{
		images_container[file_name].push_back(handle[i]);
	}

	//���I�������J��
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
			throw(std::string(buf) + " ������܂���\n");
		}
		handles.push_back(handle);
	}

	images_container[file_name] = handles;
}

void ResourceManager::CreateSoundResource(std::string file_name)
{
	int handle = LoadSoundMem(file_name.c_str());;

	//�G���[�`�F�b�N
	if (handle == -1) {
		throw(file_name + "������܂���\n");
	}

	// �R���e�i�ɓǂݍ��񂾉�����ǉ�����
	sounds_container[file_name].push_back(handle);
}
