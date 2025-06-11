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

	//�t�H���g�i�[�R���e�i
	std::map<std::string, int>fonts_container;

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
	/// ���\�[�X�Ǘ��N���X�̃C���X�^���X�擾����
	static ResourceManager* GetInstance();

	/// ���\�[�X�Ǘ��N���X�̃C���X�^���X�폜����
	static void DeleteInstance();

public:
	/// �摜���擾
	const std::vector<int>& GetImages(std::string file_name, int all_num = 1, int num_x = 1, int num_y = 1, int size_x = 0, int size_y = 0);
	const std::vector<int>& GetImages(const char* file_name, int all_num = 1, int num_x = 1, int num_y = 1, int size_x = 0, int size_y = 0);
	const std::vector<int>& GetImages(MaterialParam element);


	/// �����̎擾
	const std::vector<int>& GetSound(std::string file_name);
	const std::vector<int>& GetSound(const char* file_name);
	const std::vector<int>& GetSound(SoundParam element);

	void LoadFont(std::string file_name, std::string font_name);
	int GetFontHandle(const std::string& font_name, int size);

	/// material_handle �폜�������摜�n���h����std::vector�z��
	void UnloadResourcesAll();

private:

	/// �摜�n���h����ǂݍ��݃��\�[�X���쐬����
	void CreateImagesResource(std::string file_name);


	/// �摜�n���h����ǂݍ��݃��\�[�X���쐬����
	void CreateImagesResource(std::string file_name, int all_num, int num_x, int num_y, int size_x, int size_y);

	void CreateImagesResourceSingle(std::string file_name, int all_num);

	
	/// �����ǂݍ��݃��\�[�X���쐬����
	void CreateSoundResource(std::string file_name);


};


