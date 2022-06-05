#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include<sys/stat.h>
#include<string.h>
#include<time.h>

#include "util.h"
#include "logger.h"
#include "elist.h"
/* Forward declarations: */
void print_usage(char *argv[]);
int sum_elist(struct elist *list);
int comparator(const void *a, const void *b);
int comparator_access(const void *a, const void *b);
int comparator_string(const void* a, const void* b);
void printer(struct elist *list, size_t limit);
void dirread(char *fn, struct elist *list);
void print_usage(char *argv[]) {
    fprintf(stderr, "Disk Analyzer (da): analyzes disk space usage\n");
    fprintf(stderr, "Usage: %s [-ahs] [-l limit] [directory]\n\n", argv[0]);

    fprintf(stderr, "If no directory is specified, the current working directory is used.\n\n");

    fprintf(stderr, "Options:\n"
            "    * -a              Sort the files by time of last access (descending)\n"
            "    * -h              Display help/usage information\n"
            "    * -l limit        Limit the output to top N files (default=unlimited)\n"
            "    * -s              Sort the files by size (default, ascending)\n\n"
           );
}

int main(int argc, char *argv[])
{
    struct da_options {
        bool sort_by_time;
        unsigned int limit;
        char *directory;
    } options
    = { false, 0, "." };
    int c;
    opterr = 0;
    while ((c = getopt(argc, argv, "ahl:s")) != -1) {
        switch (c) {
            case 'a':
                options.sort_by_time = true;
                break;
            case 'h':
                print_usage(argv);
                return 0;
            case 's':
                options.sort_by_time = false;
                break;
            case 'l': {
                          char *endptr;
                          long llimit = strtol(optarg, &endptr, 10);
                          if (llimit < 0 || llimit > INT_MAX || endptr == optarg) {
                              fprintf(stderr, "Invalid limit: %s\n", optarg);
                              print_usage(argv);
                              return 1;
                          }
                          options.limit = (int) llimit;
                          break;
                      }
            case '?':
                      if (optopt == 'l') {
                          fprintf(stderr,
                                  "Option -%c requires an argument.\n", optopt);
                      } else if (isprint(optopt)) {
                          fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                      } else {
                          fprintf(stderr,
                                  "Unknown option character `\\x%x'.\n", optopt);
                      }
                      print_usage(argv);
                      return 1;
            default:
                      abort();
        }
    }

    if (optind < argc) {
        options.directory = argv[optind];
    }

    LOGP("Done parsing arguments.\n");
    LOG("Sorting by: [%s], limit: [%u]\n",
            options.sort_by_time == true ? "time" : "size",
            options.limit);
    LOG("Directory to analyze: [%s]\n", options.directory);
    if (argc <= 1) {
        print_usage(argv);
        return EXIT_FAILURE;
    }
    char *dir = options.directory;
    DIR *directory;
    if ((directory = opendir(dir)) == NULL) {
        perror("opendir");
        return EXIT_FAILURE;
    }
    closedir(directory);

    struct elist *list = elist_create(128);


    dirread(dir, list);

    if(options.sort_by_time == false){
        elist_sort(list,comparator);
    }else{
        elist_sort(list,comparator_access);
    }

    printer(list, (size_t) options.limit);
    elist_destroy(list);
    list = NULL;

    return 0;
}

void printer(struct elist *list, size_t limit)
{  size_t size;
    if(limit == 0){
        size = elist_size(list);
    }else{
        size = limit;
    } 
    for(int i =elist_size(list) - size; i < elist_size(list); i++){
        char size[11];
        char access[11];
        simple_time_format(access ,80 ,*(time_t*)(elist_get(list, i) + 8));
        human_readable_size(size, 11, *(double*)(elist_get(list, i) + 0), 1);
        printf("%10s | %11s | %s\n",size , access, (char*)(elist_get(list, i) + 16));
    }
}
int comparator(const void *a, const void *b)
{
    const double **ai = (const double **)a;
    const double **bi = (const double **)b;
    if( **ai  != **bi){
        return **ai > **bi;
    }else{
        int result = comparator_string(a, b);
        return result;
    }
}
int comparator_string(const void* a, const void* b)
{
    const char** aa = (const char**)a;
    const char** bb = (const char**)b;
    if(strcmp((*aa + 16),(*bb + 16)) > 0){
        return 0;
    }else{
        return 1;
    }

}
int comparator_access(const void *a, const void *b)
{
    const time_t **ai = (const time_t **)a;
    const time_t **bi = (const time_t **)b;

    if( *(*ai + 1) != *(*bi + 1)){
        return *(*ai + 1) < *(*bi + 1);
    }else{
        int result = comparator_string(a, b);
        return result;
    }
}

int j =0;
struct file_info{
    double  file_size;
    time_t access_time;
    char file_name[500]; 
}a[PATH_MAX];

int ap = 1;
void dirread(char *fn, struct elist *list){
    ap++;
    DIR *directory;
    struct dirent *de;
    struct stat buf;

    int size;
    char *dir_fn;
    int dir_size;
    int sz;
    time_t tm;

    directory = opendir(fn);
    if(directory != NULL){
    }

    if (directory == NULL)
    {
        return;
        closedir(directory);
        perror(fn);
        exit(1);
    }

    size = strlen(fn);
    dir_size = size + 10;
    dir_fn = (char *)malloc(sizeof(char) * dir_size);

    if (dir_fn == NULL)
    {
        closedir(directory);
        perror("malloc dir_fn");
        exit(1);
    }
    strcpy(dir_fn, fn);
    fn = NULL;
    strcat(dir_fn + size, "/");

    for (de = readdir(directory); de != NULL; de = readdir(directory))
    {

        sz = strlen(de->d_name);
        char * re = NULL;
        if (dir_size < size + sz + 2)
        {
            dir_size = size + sz + 10;
            re = realloc(dir_fn, dir_size);

        }
        if(re != NULL){
            dir_fn = re;
        }else{
            free(re);
            re =NULL;
        }
        strcpy(dir_fn + size + 1, de->d_name);

        if (stat(dir_fn, &buf) < 0)
        {
            closedir(directory);
            fprintf(stderr, "Couldn't stat %s\n", dir_fn);
            exit(1);
        }
        stat(dir_fn, &buf);
        if (S_ISREG(buf.st_mode) && strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
        {  tm = buf.st_atime; 
            a[j].file_size = buf.st_size;
            a[j].access_time = tm;
            strcpy(a[j].file_name, dir_fn);

            elist_add(list, &a[j++]);

        }


        if (S_ISDIR(buf.st_mode) && strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
        {
            dirread(dir_fn, list);
        }
    }
    closedir(directory);
    directory = NULL;
    free(dir_fn);
    dir_fn = NULL;
    free(de);
    de = NULL;   
}
