#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H
#include <fstream>
#include <string.h>
#include <vector>
#include <assert.h>
#include <iostream>
#include <cstring>
#include "myfs_macro.h"
#include "inode.hpp"
//#include "myfs.hpp"


using namespace std;
class superblock
{
private:
    bool inode_bitmap[INODE_NUM];
    bool block_bitmap[BLOCK_NUM];
    fstream disk;



public:
    int cur_dir_node_num; // inode num
    int cur_dir_num;      // dir block num
    class myFS* myfs;

    int remain_inode();

    int remain_sec();

    int get_new_inode();

    int get_new_sec();

    bool recv_inode(int inode_num);

    bool recv_sec(int sec_num);

    superblock();
    ~superblock();

    bool init();

    void print_inode_bitmap();
    void print_block_bitmap();

    void read_from_disk();
    void write_to_disk();
//    fstream disk;

};

#endif