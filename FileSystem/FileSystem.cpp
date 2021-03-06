// FileSystem.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdio>
#include <time.h>
#include <inttypes.h>

namespace fs = std::experimental::filesystem;

class FileEntity
{
public:

	uintmax_t getSize() {
		return size;
	}

	void setSize(uintmax_t size) {
		this->size = size;
	}

	time_t getLastModifiedTime() {
		return this->lastModifiedTime;
	}

	void setLastModifiedTime(time_t lastModifiedTime) {
		this->lastModifiedTime = lastModifiedTime;
	}

	time_t getCreationTime() {
		return this->creationTime;
	}

	void setCreationTime(time_t creationTime) {
		this->creationTime = creationTime;
	}

	std::string getExtension() {
		return extension;
	}

	void setExtension(std::string extension) {
		this->extension = extension;
	}
private:
	uintmax_t size;
	time_t lastModifiedTime;
	time_t creationTime;
	std::string extension;
};

class FolderEntity
{
public:

	fs::file_time_type getLastModifiedDate() {
		return lastModifiedDate;
	}

	void setLastModifiedDate(fs::file_time_type lastModifiedDate) {
		this->lastModifiedDate = lastModifiedDate;
	}

	fs::file_time_type getCreationDate() {
		return creationDate;
	}

	void setCreationDate(fs::file_time_type creationDate) {
		this->creationDate = creationDate;
	}
private:
	fs::file_time_type lastModifiedDate;
	fs::file_time_type creationDate;
};

__interface IFileService
{
	void createFile(std::string name);
	void renameFile(const std::string name, const std::string newname);
	void initFile(fs::path path);
	void updateFile(fs::path path);
	void removeFile(fs::path path);
};

class FileServiceImpl : public IFileService
{
public:

	void createFile(const std::string name) {
		std::ofstream file(name);
		file.close();
	}

	void renameFile(const std::string oldname, const std::string newname) {
		fs::rename(oldname, newname);
	}

	void initFile(fs::path path) {
		FileEntity file;
		system("cls");

		std::cout << "File name: " << path.u8string() << "\n";


		file.setSize(fs::file_size(path));
		std::cout << "File size: ";
		printf("%ju", file.getSize());
		std::cout << " bytes\n";
		
		std::string path1 = path.u8string();
		file.setExtension(path1.substr(path1.find_last_of(".")));
		std::cout << "File extension: " << file.getExtension() << "\n";

		struct stat fileInfo;
		const char *str = path1.c_str();
		if (stat(str, &fileInfo) != 0) {  
			std::cerr << "Error: " << strerror(errno) << '\n';
		}

		file.setLastModifiedTime(fileInfo.st_mtime);
		const time_t time = file.getLastModifiedTime();
		std::cout << "Last modified time: " << asctime(std::localtime(&time));

		file.setCreationTime(fileInfo.st_ctime);
		const time_t time1 = file.getCreationTime();
		std::cout << "Creation time: " << asctime(std::localtime(&time1));

		system("pause");
	}

	void updateFile(fs::path path) {
		fs::resize_file(path, fs::file_size(path));
	}

	void removeFile(fs::path path) {
		fs::remove(path);
	}

private:

};

__interface ISymLinkService
{
	void createSymLink();
	void renameSymLink();
	void deleteSymLink();
	void moveSymLink();
};

class SymLinkServiceImpl : public ISymLinkService {
public:

	void createSymLink() {
		std::cout << "Type name of link and name of target: \n";
		std::cin >> path2;
		std::cin >> path1;
		fs::create_symlink(path2, path1);
	}

	void renameSymLink() {
		std::cout << "Type old name and new name of link:";
		std::cin >> path1;
		std::cin >> path2;
		fs::rename(path1, path2);
	}

	void deleteSymLink() {
		std::cout << "Type name of link:";
		std::cin >> path1;
		fs::remove(path1);
	}

	void moveSymLink() {
		std::cout << "Type name of link: ";
		std::cin >> path1;
		std::cout << "Type full path of destination: ";
		std::cin >> path2;
		fs::rename(path1, path2);
	}
private:
	std::string path1, path2;
};

__interface IHardLinkService
{
	void createHardLink();
	void renameHardLink();
	void deleteHardLink();
	void moveHardLink();
};

class HardLinkServiceImpl : public IHardLinkService
{
public:

	void createHardLink(){
		std::cout << "Type name of hard link and name of target: \n";
		std::cin >> path2;
		std::cin >> path1;
		fs::create_hard_link(path1, path2);
	}

	void renameHardLink(){
		std::cout << "Type old name and new name of link:";
		std::cin >> path1;
		std::cin >> path2;
		fs::rename(path1, path2);
	}

	void deleteHardLink(){
		std::cout << "Type name of link:";
		std::cin >> path1;
		fs::remove(path1);
	}

	void moveHardLink(){
		std::cout << "Type name of link: ";
		std::cin >> path1;
		std::cout << "Type full path of destination: ";
		std::cin >> path2;
		fs::rename(path1, path2);
	}

private:
	std::string path1, path2;
};

__interface IFolderService {
	void showElements();
	void showChoose();
	void createDirectory(const std::string name);
	void openFolder();
	void renameFolder(const std::string oldname, const std::string newname);
	void initFolder(fs::path path);
	void removeFolder(fs::path path);
};

class FolderServiceImpl : public IFolderService {
public:
	FileServiceImpl fileServiceImpl;
	SymLinkServiceImpl symLinkServiceImpl;
	HardLinkServiceImpl hardLinkServiceImpl;
	fs::path path;

	void showChoose() {
		int c;
		std::string nameOfFile;
		do
		{
		showElements();
		std::cout << "Type 1: create element; 2: move element; 3: change name of element;\n 4: update element; 5: delete element; 6: open folder; 7: show info;\n 8: manage links; 0: move to root directory -1: to stop program\n";
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
			if (c == 2) {
				std::cout << "Type name of directory: ";
				std::cin >> nameOfFile;
				createDirectory(nameOfFile);
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
			try
			{
				if (fs::is_directory(path1)) {
					renameFolder(s1, s2);
				}
				if (fs::is_regular_file(path1)) {
					fileServiceImpl.renameFile(s1, s2);
				}
			}
			catch (const std::exception&)
			{
				std::cout << "Something went wrong!\n";
				system("pause");
			}
			
			break;
		}
		case 3: {
			std::string s1, s2;
			std::cout << "Type old name of element: ";
			std::cin >> s1;
			std::cout << "Type new name of element: ";
			std::cin >> s2;
			std::string path = fs::current_path().u8string();
			path.append("\\");
			path.append(s1);
			fs::path path1(path);
			if (fs::is_directory(path1)) {
				renameFolder(s1, s2);
			}
			if (fs::is_regular_file(path1)) {
				fileServiceImpl.renameFile(s1, s2);
			}
			break;
			}
		case 4: {
			std::cout << "Type name of file to update(not full path) or -1 to return: ";
			std::cin >> nameOfFile;
			if (nameOfFile == "-1") break;
			std::string path = fs::current_path().string();
			fs::path path1(path.append(nameOfFile));
			try
			{
			fileServiceImpl.updateFile(path1);
			}
			catch (const std::exception&)
			{
				std::cout << "Wrong path!\n";
				system("pause");
			}
			break;
		}
		case 5: {
			std::cout << "Type name of file with extension or folder to delete: ";
			std::cin >> nameOfFile;
			std::string path = fs::current_path().string();
			path.append("\\");
			fs::path path1(path.append(nameOfFile));
			try
			{
				if (fs::is_directory(path1)) {
					removeFolder(path1);
				}
				if (fs::is_regular_file(path1)) {
					fileServiceImpl.removeFile(path1);
				}
			}
			catch (const std::exception&)
			{
				std::cout << "Something went wrong\n";
				system("pause");
			}
			break;
		}
		case 6: {
			openFolder();
			break;
		}
		case 7: {
			std::cout << "Type name of element you want to check info(not full path) or -1 to return: ";
			std::cin >> nameOfFile;
			if (nameOfFile == "-1") break;

			std::string path = fs::current_path().string();
			fs::path path1(path.append(nameOfFile));
			if (fs::is_directory(path1)) {
				initFolder(path1);
			}
			if(fs::is_regular_file(path1)) {
				fileServiceImpl.initFile(path1);
			}
			break;
		}
		case 8: {
			std::cout << "1: Create sym link; 2: Rename sym link; 3: Delete sym link; 4: Move sym link;\n 5: Create hard link; 6: Rename hard link; 7: Delete hard link; 8: Move hard link;\n ";
			std::cin >> c;
			switch (c)
			{
			case 1: {
				symLinkServiceImpl.createSymLink();
				break;
			}
			case 2: {
				symLinkServiceImpl.renameSymLink();
				break;
			}
			case 3: {
				symLinkServiceImpl.deleteSymLink();
				break;
			}
			case 4: {
				symLinkServiceImpl.moveSymLink();
				break;
			}
			case 5: {
				hardLinkServiceImpl.createHardLink();
				break;
			}
			case 6: {
				hardLinkServiceImpl.renameHardLink();
				break;
			}
			case 7: {
				hardLinkServiceImpl.deleteHardLink();
				break;
			}
			case 8: {
				hardLinkServiceImpl.moveHardLink();
				break;
			}
			default:
				break;
			}
		}
		case 0: {
			fs::current_path(path);
			break;
		}
		default:
			break;
		}
		} while (c!=-1);
		
	}

	void removeFolder(fs::path path) {
		fs::remove(path);
	}

	void initFolder(fs::path path) {
				FolderEntity folder;
				folder.setLastModifiedDate(fs::last_write_time(path));
				std::time_t cftime = decltype(fs::last_write_time(path))::clock::to_time_t(fs::last_write_time(path));
				std::cout << "Last modified time: " <<  asctime(std::localtime(&cftime));
				std::cout << "\nCreation date: ";
				system("pause");
	}

	void showElements() {
		system("cls");
		for (auto & p : fs::directory_iterator(fs::current_path()))
			std::cout << p << std::endl;
	}

	void openFolder() {
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
			std::cin >> path;
			fs::current_path(path);
			this->path = path;
			showChoose();
			return path;
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

int main()
{
	std::cout << "Input initial folder to create file system\n";
	FolderServiceImpl folderService;
	folderService.inputRootDirectory();
}