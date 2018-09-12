
#include <string.h>
#include <unistd.h> //unix std API
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>

#define BUFFERSIZE 1024
#define COPYMORE 0644   // the owner can w/r others can only r


int copyD2D(char *src, char *dest);
int copyF2F(char *src, char *dest);
bool isdir(char *filename);
char *strrev(char *str);

int main(int argc, char **argv)
{
    bool opt_r = false; // -r/-R:copy the dir recursively 
    bool opt_l = false; // create the hard link
    bool opt_s = false; // create the soft link

    char *src = NULL;
    char *dest = NULL;

    char opt;

    while ((opt = getopt(argc, argv, "rRls")) != -1)
    {
        switch (opt)
        {
            case 'R':
            case 'r':
                opt_r = true;
                break;

            case 'l':
                opt_l = true;
                break;

            case 's':
                opt_s = true;
                break;
        }
    }

    // check the arg nums
    if (optind >= argc - 1)
    {
        printf("missing Operator \n");
        exit(1);
    }

    // get src and dest
    src = argv[optind];
    dest = argv[optind + 1];

    if (opt_l)
    {
        if (isdir(src))
        {
            printf(" dir can not create the hard link \n");
            exit(1);
        }
        if ((link(src, dest)) == 0)
        {
            return 0;
        }
        else
        {
            printf("create hard link fail\n");
            exit(1);
        }
    }

    if (opt_s)
    {
        if (isdir(src)) // 
        {
            printf("dir can not create the symbol link\n");
            exit(1);
        }

        if ((symlink(src, dest)) == 0) //symlink to create sybmol link
            return 0;
        else
        {
            printf("fail to create the symbol link\n");
            exit(1);
        }
    }


    if (!isdir(src))
    {
        // if not dir, process as file
        if ((copyF2F(src, dest)) == 0)
            return 0;
        else
        {
            printf("copy file fail\n");
            exit(1);
        }
    }
    else if (isdir(src))
    {
        if (!isdir(dest))
        {
            printf("can not copy a dir to a file\n");
            exit(1);
        }
        else if (isdir(dest) && opt_r)
        {
            if (copyD2D(src, dest) != 0)
            {
                printf("copy dir fail\n");
                exit(1);
            }
            else
                return 0;
        }
        else
        {
            printf("you may need -R/-r opeartor to copy a dir\n");
            exit(1);
        }
    }
    else
    {
        printf("illegal operation");
        exit(1);
    }

    return 0;
}




int copyF2F(char *src_file, char *dest_file)
{
    int in_fd, out_fd, n_chars;
    char buf[BUFFERSIZE];

    // if dest is dir, then create the same name file in the dir defautly
    if (isdir(dest_file))
    {
        char c;
        char temp[10] = { '\0' };
        char *r_temp;
        int n = strlen(src_file);
        int m = 0;

        // the final level name as the dest name
        while ((c = src_file[n - 1]) != '/')
        {
            temp[m] = c;
            m++;
            n--;
        }
        r_temp = strrev(temp);
        strcat(dest_file, r_temp);
    }

    if ((in_fd = open(src_file, O_RDONLY)) == -1)
    {
        printf("%s read file fail！",src_file);
        return 1;
    }

    if ((out_fd = open(dest_file, O_WRONLY | O_CREAT, COPYMORE)) == -1)
        return 1;

    while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0)
    {
        if (write(out_fd, buf, n_chars) != n_chars)
        {
            printf("%s write file fail！", dest_file);
            return 1;
        }
        if (n_chars == -1)
        {
            printf("%s read file fail！", src_file);
            return 1;
        }
    }

    //close file
    if (close(in_fd) == -1 || close(out_fd) == -1)
    {
        printf("close file fail");
        return 1;
    }
    return 0;
}

//typedef struct __dirstream DIR;
//struct __dirstream
//{
//    void                   *__fd;              /* `struct hurd_fd' pointer for descriptor.   */
//    char                   *__data;            /* Directory block.   */
//    int                       __entry_data;    /* Entry number `__data' corresponds to.   */
//    char                   *__ptr;             /* Current pointer into the block.   */
//    int                       __entry_ptr;     /* Entry number `__ptr' corresponds to.   */
//    size_t                  __allocation;      /* Space allocated for the block.   */
//    size_t                  __size;            /* Total valid data in the block.   */
//    __libc_lock_define (, __lock)              /* Mutex lock for this structure.   */
//};


//Struct dirent
//{
//    ino_t                  d_ino;              //此目录进入点的inode
//    ff_t                   d_off;              //目录文件开头到此目录进入点的位移
//    signed  short int      d_reclem;           //_name的长度，不包含NULL字符
//    unsigned  char         d_type;             // d_name所指的文件类型
//    char                   d_name[256];        //文件名
//}


/*
D:is dir check
*/
bool isdir(char *filename)
{
    struct stat fileInfo;
    if (stat(filename, &fileInfo) >= 0)
    {
        if (S_ISDIR(fileInfo.st_mode))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

/*
D:copy dir
*/
int copyD2D(char *src_dir, char *dest_dir)
{
    DIR *dp = NULL;
    struct dirent *dirp;
    char tempDest[256];
    char tempSrc[256];
    strcpy(tempDest, dest_dir);
    strcpy(tempSrc, src_dir);
    

    //open dir
    if ((dp = opendir(src_dir)) == NULL)
        return 1;
    else
    {
        //get dirent
        while ((dirp = readdir(dp)))
        {
            struct stat file_stat;
            if (!isdir(dirp->d_name))
            {
                //link name
                strcat(tempDest, "/");
                strcat(tempSrc, "/");
                strcat(tempDest, dirp->d_name);
                strcat(tempSrc, dirp->d_name);
                // printf("%s\n",tempDest);
                // printf("%s\n",tempSrc);
                //copy file
                copyF2F(tempSrc, tempDest);

                //recover name
                strcpy(tempDest, dest_dir);
                strcpy(tempSrc, src_dir);
            }
        }
        //close dir
        closedir(dp);
        return 0;
    }
}

/*
D:to convert the string 
*/
char * strrev(char *str)
{
    int i = strlen(str) - 1, j = 0;
    char ch;
    while (i>j)
    {
        ch = str[i];
        str[i] = str[j];
        str[j] = ch;
        i--;
        j++;
    }
    return str;
}

