#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


struct list
{
	char name[64];
	struct list* next;
	time_t t_changed;
};


time_t 				 get_time_changed(const char *path);
int            is_changed(const char* path, time_t t_changed);
void 	         compile();
struct list*   get_files(DIR* d, const char* src);


int main()
{
	DIR* d;
	char src[] = "./src/";

	struct list* files;
	struct list* file;

	d = opendir(src);
	if (!d)
	{
		printf("Can't open directory %s", src);
		exit(1);
	}

	files = get_files(d, src);

	while (1)
	{
		file = files;
		sleep(1);
		while (file->next)
		{
			if (is_changed(file->name, file->t_changed))
			{
				compile();
				file->t_changed = get_time_changed(file->name);
			}
			file = file->next;
		}
	}

	return 0;
}


time_t get_time_changed(const char* path) {
	struct stat file_stat;
	
	int err = stat(path, &file_stat);

	return file_stat.st_mtim.tv_sec;
}


int is_changed(const char* path, time_t t_changed)
{
		time_t now=0;

		now = get_time_changed(path);

		return now != t_changed;
}


void compile()
{
	system("clear");
	int status = system("cmake --build build/ 1>/dev/null");

	if (status == 0)
	{
		printf("\e[1;32mAll Good!\e[0m\n");
	}
}


struct list* get_files(DIR* d, const char* src)
{
	char file_name[64] = {0};
	struct dirent* dir;	
	struct list* files = malloc(sizeof(struct list));
	struct list* now = files;

	while ((dir = readdir(d)) != NULL)
	{
		if (dir->d_name[0] == '.') continue;

		sprintf(file_name, "%s%s", src, dir->d_name);
		strcpy(now->name, file_name);
		now->t_changed = get_time_changed(file_name);

		now->next = malloc(sizeof(struct list));
		now = now->next;
	}
	now->name[0] = '\0';
	now->next = NULL;

	return files;
}


