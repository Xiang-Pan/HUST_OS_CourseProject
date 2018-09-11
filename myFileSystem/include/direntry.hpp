#ifndef DIRENTRY_H
#define DIRENTRY_H
#include "Buffer.hpp"
#include "assert.h"

// 32 Bytes
struct sector_dir_entry
{
    char name[28];
    int inode_num;
    void init(const char* _name, int _num);
    sector_dir_entry();
    sector_dir_entry operator = (const sector_dir_entry& dir);

    bool operator == (const sector_dir_entry& dir);
    void clone(const sector_dir_entry& dir);
};

// 512 Bytes. the final link to the next
class sector_dir
{
public:
    sector_dir();
    char dir_name[28];
    bool write_back_to_disk(Buffer& buffer, int sec_num);

    sector_dir operator = (const sector_dir& sec_dir);

    sector_dir_entry dirs[16];

    bool read_dir_from_disk(Buffer& buffer, int sec_num);
    bool isroot();
};

// 512 Bytes ！
class sector_file
{
public:
    char data[VALID_DATA_LENGTH];
    int next;

    sector_file();
    sector_file operator = (const sector_file& sec_file);
    bool read_dir_from_disk(Buffer& buffer, int sec_num);
    bool write_back_to_disk(Buffer& buffer, int sec_num);
};

#endif