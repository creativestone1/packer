#pragma once
#include "additional.h" 

#define BUFF_SIZE 1024
//FILE: [uint8_t type, mode_t mode, uint32_t sizeof_path, uint8_t path[], uint64_t sizeof_file, uint8_t file[]]
//DIR:  [uint8_t type, mode_t mode, uint32_t sizeof_path, uint8_t path[]]
//
typedef struct {
	char* path;
	uint32_t separator;
	uint32_t len;
} path;// path to pivot dir/file + relative path from pivot dir to file
//example:
//         012345678
//  path = path/fold
//			    ^
//	separator = 5
//	len = 9
//	
//	fold is pivot folder(folder need to be compressed)

int serealizeFile(path pathIn, FILE* out){	
	//сереализация типа файла
	fputc(DT_REG, out);
	//Получаю инфу о правах папки
	mode_t mode = getFilePermissions(pathIn.path);
	//Сереализую инфу о правах, длину относительного пути, относительный путь 
	fwrite(&mode, sizeof(mode), 1, out);
	uint32_t relativePathInLen = pathIn.len-pathIn.separator;
	fwrite(&relativePathInLen, sizeof(uint32_t), 1, out);
	fwrite(pathIn.path+pathIn.separator, sizeof(uint8_t), relativePathInLen, out);
	
	//открываю файл
	FILE* in = fopen(pathIn.path, "rb");
	if(in == NULL)
		return -1;
	fseek(in, 0, SEEK_END);
	uint64_t fileSize = ftell(in);

	fwrite(&fileSize, sizeof(uint64_t), 1, out);

	fseek(in, 0, SEEK_SET);
	while(1){
		char buff[BUFF_SIZE];
		size_t bytesRead = fread(buff, sizeof(uint8_t), BUFF_SIZE, in);
		fwrite(buff, sizeof(uint8_t), bytesRead, out);
		if(bytesRead<BUFF_SIZE)
			break;
	}
	fclose(in);
	return 0;
}

int serealizeDir(path pathIn, FILE* out){	
	//Сереализую тип папка	
	fputc(DT_DIR, out);
	//Получаю инфу о правах папки
	mode_t mode = getFilePermissions(pathIn.path);
	uint32_t relativePathInLen = pathIn.len-pathIn.separator;
	//Сереализую инфу о правах, длину относительного пути, относительный путь 
	fwrite(&mode, sizeof(mode), 1, out);
	fwrite(&relativePathInLen, sizeof(uint32_t), 1, out);
	fwrite(pathIn.path+pathIn.separator, sizeof(uint8_t), relativePathInLen, out);
		
	//открываю папку	
	DIR* dir = opendir(pathIn.path);
	if(dir == NULL){
		perror("opendir");
		fclose(out);
		return -1;
	}
	//Созаю структуру для анализа имён содержимого в папке
	struct dirent *entry;
	while((entry = readdir(dir))&& entry!= NULL){
		if(strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name,"..") == 0)
			continue;
		
		//создаём новый путь
		uint32_t nameLen = strlen(entry->d_name);
		path newPathIn;
		newPathIn.len = pathIn.len+1+nameLen;
		newPathIn.path = (char*)malloc(newPathIn.len+1);
		memcpy(newPathIn.path, pathIn.path, pathIn.len);
		newPathIn.path[pathIn.len] = '/';
		memcpy(newPathIn.path + pathIn.len + 1, entry->d_name, nameLen);
		newPathIn.path[newPathIn.len] = '\0';
		newPathIn.separator = pathIn.separator;

		//Сереализую файл если выбранный объект это файл
		if(entry->d_type == DT_REG){
			if(serealizeFile(newPathIn,out)==-1){
				printf("serealization error occured!\n");
				fclose(out);
				closedir(dir);
				return -1;
			}
		}
		//Сереализую папку если выбранный объект это папка
		else if(entry->d_type == DT_DIR){
			if(serealizeDir(newPathIn, out)==-1)
				return -1;
		}
		free(newPathIn.path);
	}	
	closedir(dir);
	return 0;
}
int serealize(char* pathInStr, char* pathOutStr){
	//--filling path to dir/file that needs to be serealized--
	path pathIn;	
	pathIn.len = strlen(pathInStr);
	pathIn.path = pathInStr;
	//remove last / if exitst
	if(pathInStr[pathIn.len-1]=='/')
		pathInStr[--pathIn.len]='\0';
	//find separator
	char* fileInName = strrchr(pathIn.path, '/');
	if(fileInName)
		fileInName++;
	else
		fileInName = pathIn.path;	
	size_t fileInNameLen = strlen(fileInName);
	pathIn.separator = pathIn.len - strlen(fileInName);
	//----
		
	//--make out file path+name--
	char* newPathOut;	
	if(pathOutStr == NULL){	
		size_t newPathOutLen = strlen("./") + fileInNameLen + strlen(".at");
		newPathOut = (char*)malloc(newPathOutLen+1);
		memcpy(newPathOut, "./",strlen("./"));
		memcpy(newPathOut+strlen("./"), fileInName,fileInNameLen);
		memcpy(newPathOut+strlen("./")+fileInNameLen, ".at",strlen(".at"));
		newPathOut[newPathOutLen] = '\0';
	}
	else {	

		//remove last / if exists
		size_t oldPathOutLen = strlen(pathOutStr);
		if(pathOutStr[oldPathOutLen-1]=='/')
			pathOutStr[--oldPathOutLen]='\0';
		
		size_t newPathOutLen = oldPathOutLen+strlen("/")+fileInNameLen+strlen(".at");
		newPathOut = (char*)malloc(newPathOutLen+1);
		memcpy(newPathOut, pathOutStr, oldPathOutLen);
		memcpy(newPathOut+oldPathOutLen, "/", strlen("/"));
		memcpy(newPathOut+oldPathOutLen+strlen("/"), fileInName, fileInNameLen);
		memcpy(newPathOut+oldPathOutLen+strlen("/")+fileInNameLen, ".at",strlen(".at"));
		newPathOut[newPathOutLen] = '\0';
	}
	//-- --
	
	//file with serealized info
	FILE* out = fopen(newPathOut, "wb");
	if(out == NULL)	
		return -1;

	int serealizeReturn;
	if(getFileType(pathIn.path) == DT_DIR)
		serealizeReturn = serealizeDir(pathIn, out);
	else if(getFileType(pathIn.path) == DT_REG)
		serealizeReturn = serealizeFile(pathIn, out);

	fclose(out);
	if(serealizeReturn == -1)
		return -1;
}
int deserealize(char* pathInStr, char* pathOutStr){
	//remove last / if exitst
	size_t pathInLen = strlen(pathInStr);
	if(pathInStr[pathInLen-1]=='/')
		pathInStr[--pathInLen]='\0';
	
	//remove last / if exitst
	size_t pathOutLen;
	if(pathOutStr!=NULL){
		pathOutLen = strlen(pathOutStr);
		if(pathOutStr[pathOutLen-1]=='/')
			pathOutStr[--pathOutLen]='\0';
	}
	else{
		pathOutLen = 2;
		pathOutStr = "./";
	}

	FILE* in = fopen(pathInStr, "rb");
	if(in == NULL)
		return -1;
	while(1){
		uint8_t type;
		if(fread(&type, sizeof(uint8_t), 1, in)!=1)
			break;

		mode_t mode;
		fread(&mode, sizeof(mode), 1, in);

		uint32_t relativePathOutLen;
		fread(&relativePathOutLen, sizeof(uint32_t), 1, in);

		uint32_t newPathOutLen = pathOutLen+1+relativePathOutLen;
		char* newPathOut = (char*)malloc(newPathOutLen+1);
		memcpy(newPathOut, pathOutStr, pathOutLen);
		newPathOut[pathOutLen]='/';
		fread(newPathOut+pathOutLen+1, sizeof(uint8_t), relativePathOutLen, in);
		newPathOut[newPathOutLen]='\0';
		printf("%s",newPathOut);

		if(type == DT_DIR){
			mkdir(newPathOut,mode);
			printf(" | directory\n");
		}
		if(type == DT_REG){
			printf(" | file \n");
			uint64_t fileSize;	
			fread(&fileSize, sizeof(uint64_t), 1, in);

			FILE* out = fopen(newPathOut, "wb");
			if(out == NULL)	{
				printf("ERROR: can't open the file!\n");
				return -1;
			}
			uint64_t lastBytesToRead = fileSize;
			while(lastBytesToRead!=0){
				size_t bytesToRead = (lastBytesToRead <= BUFF_SIZE) ? lastBytesToRead : BUFF_SIZE;
				uint8_t buff[bytesToRead];
				fread(buff, sizeof(uint8_t), bytesToRead, in);
				fwrite(buff, sizeof(uint8_t), bytesToRead, out);
				lastBytesToRead -= bytesToRead;
			}
			fclose(out);
			setFilePermissions(newPathOut, mode);	
		}
		free(newPathOut);
	}
	fclose(in);
	return 0;
}
