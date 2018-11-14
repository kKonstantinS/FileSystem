// FileSystem.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdio>

namespace fs = std::experimental::filesystem;

std::string inputRootDirectory();
void showElements();
void showChoose();
void createFile(std::string name);
void deleteFile(const std::string name);
void createDirectory(const std::string name);

int main()
{
	std::cout << "Input initial folder to create file system\n";
	fs::path::path(inputRootDirectory());
}

void showElements() {
	system("cls");
	int number = 1;
	for (auto & p : fs::directory_iterator(fs::current_path()))
		std::cout << number++ << ": " << p << std::endl;
	showChoose();
}

void showChoose() {
	int c;
	std::string nameOfFile;
	std::cout << "Type 1: create element; 2: move element; 3: change name of element;\n 4: change element; 5: delete element; 6: move to folder; 0: move to root directory\n";
	std::cin >> c;
	switch (c)
	{
	case 1: {
		std::cout << "Choose action: \n1 - create file;\n2 - create folder.\n";
		std::cin >> c;
		if (c==1){
		std::cout << "Type name of file with extension: ";
		std::cin >> nameOfFile;
		createFile(nameOfFile);
		}else{
			if (c==2){
				std::cout << "Type name of directory: ";
				std::cin >> nameOfFile;
				createDirectory(nameOfFile);
			}
		}
		showElements();
		std::cout << "Element created succesfully!";
	}
	case 2: {

	}
	case 3: {

	}
	case 4: {

	}
	case 5: {
		std::cout << "Type name of file with extension to delete: ";

	}
	default:
		break;
	}
}

void createFile(const std::string name) {
	std::ofstream file(name);
	file.close();
}

void createDirectory(const std::string name) {
	if (fs::exists(name)) {
		system("cls");
		std::cout << "Directory exists! Type anything to return.";
		std::string a;
		std::cin >> a;
	}
	fs::create_directory(name);
}

void deleteFile(const std::string name) {
	/*if (fs::exists(name)){
	if (std::remove(name) != 0)
		perror("Error deleting file");
	else
		puts("File successfully deleted");
	}*/
}

std::string inputRootDirectory() {
	std::string path;
	try{
		std::cin >> path;
		fs::current_path(path);
		showElements();
		return path;
	}
	catch (const std::exception&) {
		std::cerr << "Wrong path! Type path again:\n";
		inputRootDirectory();
	}
}

void openFolder(std::string path, int var) {
	int k = 0;
	std::cout << "Please input number of folder you want to open or type '0' to type base directory\n";
	if (var==0)
	{
		inputRootDirectory();
	}
	else
	{
		try {
			for (auto & p : fs::directory_iterator(path)){
				if (var==k)
				{
					for (auto & p : fs::directory_iterator(path))
						std::cout << p << std::endl;
					std::cout << fs::current_path();
				}
				k++;
			}
		}
		catch (const std::exception&) {
			std::cout << "Wrong input";
		}
		
	}
	

}

class ElementEntity
{
public:
	ElementEntity();
	~ElementEntity();

private:

};

ElementEntity::ElementEntity()
{
}

ElementEntity::~ElementEntity()
{
}