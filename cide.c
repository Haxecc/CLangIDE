#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

time_t get_time_changed(const char *path);
void 	 compile(const char* path);


int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("Usage: %s <file_name.c>\n", argv[0]);
		exit(1);
	}

	char* file_name = argv[1];

	time_t now, then;
	while (1)
	{
		now = get_time_changed(file_name);
		sleep(1);
		then = get_time_changed(file_name);

		if (now != then)
			compile(file_name);
	}

	return 0;
}


time_t get_time_changed(const char* path) {
	struct stat file_stat;
	int err = stat(path, &file_stat);

	return file_stat.st_mtim.tv_sec;
}

void compile(const char* path)
{
	char compile_cmd[256];
	sprintf(compile_cmd, "gcc %s -o garbage", path);

	system("clear");
	int status = system(compile_cmd);
	system("rm garbage");

	if (status == 0)
	{
		printf("\e[1;32mAll Good!\e[0m\n");
	}
}
