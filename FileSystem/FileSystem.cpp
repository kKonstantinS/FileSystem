// FileSystem.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdio>

namespace fs = std::experimental::filesystem;


__interface IFileService
{
	void createFile(std::string name);
	void showInfo(const std::string name);
	void renameFile(const std::string name, const std::string newname);
	void move(const std::string name);
};

class FileServiceImpl : public IFileService
{
public:

	void createFile(const std::string name) {
		std::ofstream file(name);
		file.close();
	}

	void showInfo(const std::string name) {

	}

	void renameFile(const std::string oldname, const std::string newname) {
		fs::rename(oldname, newname);
	}

	void move(const std::string name) {
		
	}
private:

};

__interface IFolderService {
	void showElements();
	void showChoose();
	void createDirectory(const std::string name);
	void openElement();
	void renameFolder(const std::string oldname, const std::string newname);
};

class FolderServiceImpl : public IFolderService {
public:
	FileServiceImpl fileServiceImpl;
	FolderServiceImpl();
	~FolderServiceImpl();

	void showChoose() {
		int c;
		std::string nameOfFile;
		do
		{
		showElements();
		std::cout << "Type 1: create element; 2: move element; 3: change name of element;\n 4: change element; 5: delete element; 6: choose element; 7: show info; 0: move to root directory\n-1: to stop program\n";
		std::cin >> c;
		switch (c)
		{
		case 1: {
			std::cout << "Choose action: \n1 - create file;\n2 - create folder.\n";
			std::cin >> c;
			if (c == 1) {
				std::cout << "Type name of file with extension: ";
				std::cin >> nameOfFile;
				fileServiceImpl.createFile(nameOfFile);
			}
			else {
				if (c == 2) {
					std::cout << "Type name of directory: ";
					std::cin >> nameOfFile;
					createDirectory(nameOfFile);
				}
			}
			break;
		}
		case 2: {
			std::string s1, s2;
			std::cout << "Type name of file or folder: ";
			std::cin >> s1;
			std::cout << "Type full path of destination: ";
			std::cin >> s2;
			std::string path = fs::current_path().string();
			fs::path path1(path.append(s1));
			if (fs::is_directory(path1)) {
				renameFolder(s1, s2);
			}
			if (fs::is_regular_file(path1)) {
				fileServiceImpl.renameFile(s1, s2);
			}
			break;
		}
		case 3: {
			std::string s1, s2;
			std::cout << "Type old name of element: ";
			std::cin >> s1;
			std::cout << "Type new name of element: ";
			std::cin >> s2;
			std::string path = fs::current_path().string();
			fs::path path1(path.append(s1));
			if (fs::is_directory(path1)) {
				renameFolder(s1, s2);
			}
			if (fs::is_regular_file(path1)) {
				fileServiceImpl.renameFile(s1, s2);
			}
			break;
			}
		case 4: {
			
			break;
		}
		case 5: {
			std::cout << "Type name of file with extension or folder to delete: ";
			std::cin >> nameOfFile;
			fs::remove(nameOfFile);
			break;
		}
		case 6: {
			openAndInitFolder();
			break;
		}
		case 7: {
			
			break;
		}
		case 0: {
			fs::path path;
			fs::current_path(path);
			break;
		}
		default:
			break;
		}
		} while (c!=-1);
		
	}

	void showElements() {
		system("cls");
		int number = 1;
		for (auto & p : fs::directory_iterator(fs::current_path()))
			std::cout << number++ << ": " << p << std::endl;
		//showChoose();
	}

	void openAndInitFolder() {
		int k = 0, n = 0;
		int var;
		system("cls");
		for (auto & p : fs::directory_iterator(fs::current_path())) {
			std::cout << n++ << ": " << p << std::endl;
		}
		std::cout << "Please input number of element you want to open or type '0' to return: ";
		std::cin >> var;
		if (var == 0)
		{
			return;
		}
		else
		{
			try {
				for (auto & p : fs::directory_iterator(fs::current_path())) {
					if (var == k)
					{
						fs::current_path(p);
						showElements();
						std::cout << fs::current_path();
					};
					k++;
				}
			}
			catch (const std::exception&) {
				std::cout << "Wrong input";
			}
		}
	}

	std::string inputRootDirectory() {
		std::string path;
		try {
			std::cin >> path;
			fs::current_path(path);
			showChoose();
			return path;
		}
		catch (const std::exception&) {
			std::cerr << "Wrong path! Type path again:\n";
			inputRootDirectory();
		}
	}

	void createDirectory(const std::string name){
		if (fs::exists(name)) {
			system("cls");
			std::cout << "Directory exists! Type anything to return.";
			std::string a;
			std::cin >> a;
		}
		fs::create_directory(name);
	}

	void renameFolder(const std::string oldname, const std::string newname) {
		fs::rename(oldname, newname);
	}

private:

};

FolderServiceImpl::FolderServiceImpl()
{
}

FolderServiceImpl::~FolderServiceImpl()
{
}

class FileEntity
{
public:

private:
	std::string size;
	std::string lastModifiedDate;
	std::string creationDate;
	std::string extension;
};

class FolderEntity
{
public:

private:
	std::string lastModifiedDate;
	std::string creationDate;
};

int main()
{
	std::cout << "Input initial folder to create file system\n";
	FolderServiceImpl folderService;
	folderService.inputRootDirectory();
}