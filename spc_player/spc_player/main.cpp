#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

int main(int argc,char * argv[]) {
	if (argc != 2) {
		printf("-----------------------------------------\n");
		printf("Simple SPC Player using spc_player.dll\n\n");
		printf("Usage: %s infile.spc\n",argv[0]);
		printf("-----------------------------------------");
		return 0;
	}

	HMODULE hd = LoadLibraryA("spc_player.dll");
	if (!hd) {
		puts("load library fail!");
		FreeLibrary(hd);
		exit(1);
	}
	typedef int(*type_play)(char *, int);
	typedef void(*type_stop)(void);

	type_play play_music = (type_play)GetProcAddress(hd, "play_music");
	type_stop stop_music = (type_stop)GetProcAddress(hd, "stop_music");

	if (!play_music || !stop_music) {
		puts("load function fail!");
		FreeLibrary(hd);
		exit(1);
	}

	FILE * fp = fopen(argv[1], "rb");
	if (!fp) {
		puts("load music fail!");
		exit(1);
	}

	fseek(fp, 0, SEEK_END);
	size_t file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char * music = (char *)malloc(file_size * sizeof(char));

	fread(music, 1, file_size, fp);
	fclose(fp);

	printf("-----------------------------------------\n");

	if (play_music(music, file_size)!=1) {
		printf("Not spc music file!");
		return 0;
	}
	printf("Playing music.....\n");
	printf("Press any key to exit.");
	getchar();
	stop_music();
	return 0;
}