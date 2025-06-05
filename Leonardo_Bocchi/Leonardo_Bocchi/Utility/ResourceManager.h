#pragma once

#include <map>
#include <string>
#include <vector>

// �摜�p�p�����[�^
struct MaterialParam
{
	std::string file_path;
	int all_num;
	int num_x;
	int num_y;
	int size_x;
	int size_y;
};

// �����p�p�����[�^
struct SoundParam
{
	std::string file_path;
};

class ResourceManager
{
private:
	//���N���X�̃|�C���^�i�A�h���X�̐�ɃC���X�^���X�𐶐�����j
	static ResourceManager* instance;

	//�摜�i�[�R���e�i
	std::map<std::string, std::vector<int>>images_container;

	//�T�E���h�i�[�R���e�i
	std::map<std::string, std::vector<int>>sounds_container;

private:
	//�R���X�g���N�^��private�ɂ��邱�ƂŁA
	//���N���X�̃����o�֐��ł����C���X�^���X�𐶐��ł��Ȃ��悤�ɂ���
	ResourceManager() = default;

	//�R�s�[�K�[�h
	//�N���X�O�ŃC���X�^���X�𐶐����ēn�����Ƃ��ł��Ȃ��悤�ɂ���
	ResourceManager(ResourceManager& v) = delete;
	ResourceManager& operator = (const ResourceManager& v) = delete;
	//~�R�s�[�K�[�h

public:
	//�f�X�g���N�^
	~ResourceManager() = default;

public:
	/// <summary>
	/// ���\�[�X�Ǘ��N���X�̃C���X�^���X�擾����
	/// </summary>
	/// <returns>�N���X�̃|�C���^</returns>
	static ResourceManager* GetInstance();

	/// <summary>
	/// ���\�[�X�Ǘ��N���X�̃C���X�^���X�폜����
	/// </summary>
	static void DeleteInstance();

public:
	/// <summary>
	/// �摜���擾
	/// </summary>
	/// <param name="file_name">�t�@�C���p�X</param>
	/// <param name="all_num">�摜�̑���</param>
	/// <param name="num_x">���̑���</param>
	/// <param name="num_y">�c�̗p��</param>
	/// <param name="size_x">���̃T�C�Y�i�����j</param>
	/// <param name="size_y">�c�̃T�C�Y�i�����j</param>
	/// <returns>�ǂݍ��񂾉摜�n���h����std::vector�z��</returns>
	const std::vector<int>& GetImages(std::string file_name, int all_num = 1, int num_x = 1, int num_y = 1, int size_x = 0, int size_y = 0);
	const std::vector<int>& GetImages(const char* file_name, int all_num = 1, int num_x = 1, int num_y = 1, int size_x = 0, int size_y = 0);
	const std::vector<int>& GetImages(MaterialParam element);

	/// <summary>
	/// �����̎擾
	/// </summary>
	/// <param name="file_name">�t�@�C���p�X</param>
	/// <param name="sound_type">�T�E���h�^�C�v</param>
	/// <returns>�ǂݍ��񂾉�����std::vector�z��</returns>
	const std::vector<int>& GetSound(std::string file_name);
	const std::vector<int>& GetSound(const char* file_name);
	const std::vector<int>& GetSound(SoundParam element);

	/// <summary>
	/// material_handle �폜�������摜�n���h����std::vector�z��
	/// </summary>
	void UnloadResourcesAll();

private:
	/// <summary>
	/// �摜�n���h����ǂݍ��݃��\�[�X���쐬����
	/// </summary>
	/// <param name="file_name">�t�@�C���p�X</param>
	void CreateImagesResource(std::string file_name);

	/// <summary>
	/// �摜�n���h����ǂݍ��݃��\�[�X���쐬����
	/// </summary>
	/// <param name="file_name">�t�@�C���p�X</param>
	/// <param name="all_num">�摜�̑���</param>
	/// <param name="num_x">���̑���</param>
	/// <param name="num_y">�c�̑���</param>
	/// <param name="size_x">���̃T�C�Y�i�����j</param>
	/// <param name="size_y">�c�̃T�C�Y�i�����j</param>
	void CreateImagesResource(std::string file_name, int all_num, int num_x, int num_y, int size_x, int size_y);


	void CreateImagesResourceSingle(std::string file_name, int all_num);

	/// <summary>
	/// �����ǂݍ��݃��\�[�X���쐬����
	/// </summary>
	/// <param name="file_name">�t�@�C���p�X</param>
	/// <param name="sound_type">�T�E���h�^�C�v</param>
	void CreateSoundResource(std::string file_name);
};


