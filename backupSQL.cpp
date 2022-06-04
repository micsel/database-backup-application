#include <windows.h>
#include <stdio.h>
#include <Lmcons.h>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <conio.h>
#include <vector>
#include <new>
#include <exception>
#include <dirent.h>
#include <cstring>
#include <ctime>

/*********************** INSTRUCTIONS OF USAGE ***********************/
/*
*
*
This function assumes you are using mysql
And your mysql functions are located into C:/xampp/mysql/bin/ Directory
Note: this program has to be runned from a command line
Before running the program you need to compile it first
We suggest to using "Gcc compiler"
If you do not have gcc compiler downloaded already, please refer -> https://sourceforge.net/projects/gcc-win64/
to Compile open cmd, go to the program directory and type: g++ backupSQL.cpp -o sqlBK.exe

To backup databases run:

-> C:/path/to/program/sqlBK.exe

*
*
**********************************************************************/


std::string setPath(std::string dbname)
{
	std::string _path1_ = "C:/xampp/mysql/bin/mysqldump --default-character-set=utf8mb4 -uroot -h localhost --databases ";
	std::string _path2_ = _path1_.append(dbname);
	_path2_.append(" > ");
	_path2_.append(dbname);
	_path2_.append(".sql");
	return (std::string)_path2_;
}


std::string __GetSizeOfBackups(std::string database_name)
{
	std::string pathTo = "";
	static __int64 FileSize;
	std::string __title = "Size Of Database Backup ";
	__title.append(database_name);
	__title.append(" is ");
	std::fstream ReadSize;
	ReadSize.open(database_name,std::fstream::in);
	if(!ReadSize.is_open())
	{
		return (std::string)"no databases found!";
	}else{
		ReadSize.tellg();
		ReadSize.seekg(0, std::fstream::end);
		FileSize = ReadSize.tellg();
	}
	ReadSize.close();
	__title.append(std::to_string(FileSize));
	__title.append(" Bytes");
	return (std::string) __title;
}


void backup(std::vector <std::string> dbnames)
{
	static __int32 __counter = 0;
	std::string __ext = ".sql";
	std::vector <std::string> __Sizes(0);
	std::string __CP = "COPY ";
	for(__counter = 0; __counter < dbnames.size();__counter++)
	{
		std::cout << "database backup: " << __counter << " = "<< dbnames[__counter] << std::endl;
        system(setPath(dbnames[__counter]).c_str());
        std::cout << "database backed up successfuly" << std::endl;
        __Sizes.push_back(dbnames[__counter].append(__ext));
	}
	std::string __Result = "Total Databases Backed Up: ";
	std::string __GetResult = __Result.append(std::to_string(__counter));
	std::cout << __GetResult << std::endl;
	__counter = 0;
	for(__counter = 0; __counter < dbnames.size(); __counter++)
	{
		std::cout << __GetSizeOfBackups(__Sizes[__counter]) << std::endl;
	}
	Sleep(1000);
}

static int __Moved = 0;

void __MoveFile(std::string *fileName)
{
	std::string command = "MOVE ";
	command.append(*fileName);
	command.append(" SQL");
	system(command.c_str());
}

void __SetDatabaseName(std::string *dbname)
{
std::cout << "Enter database name: ";
std::cin >> *dbname;
}

time_t timer = (time(0));
tm *GetTime = (localtime(&timer));
__int64 seconds = GetTime->tm_sec;

int main()
{
std::vector <std::string> dbnames;
std::string dbName;
__int32 numberOfDb;
std::cout << "how many databases do you have for backup ? ";
std::cin >> numberOfDb;
for(__int32 counter = 0; counter < numberOfDb; counter++)
{
__SetDatabaseName(&dbName);
dbnames.push_back(dbName);
}
	backup(dbnames);
	DIR *directory;
	struct dirent *__Explore;
	std::string vec, nameFile;
	static int COUNTER__ = 0;

	while(COUNTER__ <= dbnames.size()-1)
	{
		dbnames[COUNTER__].append(".sql");
		COUNTER__++;
	}

	COUNTER__ = 0;
	if((directory = opendir("SQL_BACKUP/")) != NULL)
	{
		while((__Explore = readdir(directory)) != NULL)
		{
					nameFile =__Explore->d_name;

            for(static __int32 iterator = 0; iterator < dbnames.size(); iterator++)
						{
							if(nameFile.compare(dbnames[iterator]) == 0 )
							{
								Sleep(1000);
								std::cout << "Found Sql File: " << nameFile << std::endl;
								std::cout.flush();

							}
					}
				Sleep(10);
				std::cout.flush();
				std::cout << "FileName: " << nameFile << std::endl;

			}
		closedir(directory);
	}

	__int64 secondsNow = GetTime->tm_sec;
	std::cout << "Process Took: " << secondsNow + seconds << " seconds" << std::endl;

	return EXIT_SUCCESS;
}
