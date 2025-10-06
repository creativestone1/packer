#include "packer.h"

int main(int argc, char *argv[]){
	if(argc == 1){
		printf("I dont understand you, use -h flag pls\n");
		return -1;
	}
	if(strcmp(argv[1],"-p") == 0){
		if(argc == 3)
			return serealize(argv[2],NULL);
		if(argc == 4)
			return serealize(argv[2], argv[3]);
	}
	else if(strcmp(argv[1],"-u") == 0){
		if(argc == 3)
			return deserealize(argv[2],NULL);
		if(argc == 4)
			return deserealize(argv[2], argv[3]);
	}
	else if(strcmp(argv[1],"-h") == 0){
		printf(
			"Usage: packer [OPTION] [FILE]\n"
			"Simple packer program.\n\n"
			"Options:\n"
			"  -p <fileIn> [<pathOut>]   Pack (archive) the specified file or directory.\n"
			"  -u <fileIn> [<pathOut>]   Unpack (extract) the specified archive.\n"
			"  -h           Display this help message and exit.\n\n"
			"Examples:\n"
			"  packer -p myfolder\t\t\t# Pack the folder 'myfolder' in current directory\n"
			"  packer -p path/myfolder pathOut\t# Pack the folder 'myfolder' in current directory\n"
			"  packer -u archive.arc\t\t# Unpack 'archive.arc' archive in current directory\n"
			"  packer -h\t\t\t\t# Show this help message\n"

		);
		return 0;
	}
	return 0;
}
