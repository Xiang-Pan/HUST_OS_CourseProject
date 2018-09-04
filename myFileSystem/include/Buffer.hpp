//
// Created by wings on 9/3/18.
//
#ifndef MYFS_BUFFER_H
#define MYFS_BUFFER_H




#include <cstring>
#include "iostream"
#include "macro.h"
//#include "superblock.hpp"
#include "assert.h"
#include <iostream>
#include <vector>
//#include <fstream>

using  namespace std;


struct BufferNode
{
    char buffer[SEC_SIZE + 1];
    int pri;
    int block_num;

    BufferNode operator = (const BufferNode& b) //redefine =
    {
        memcpy(buffer, b.buffer, SEC_SIZE + 1);
        pri = b.pri;
        block_num = b.block_num;
    }

    BufferNode()
    {
        memset(buffer, 0, SEC_SIZE);
        pri = 0;
        block_num = 0;
    }

    // priority
    void init(int _block_num)
    {
        pri = 5;
        block_num = _block_num;
    }

    void update(const BufferNode& b)
    {
        memcpy(buffer, b.buffer, SEC_SIZE + 1);
        pri = b.pri + 1;
        block_num = b.block_num;
    }
};

class Buffer
{
public:
    Buffer();

    ~Buffer();

    // 将 buffer 里面的内容写入扇区中.单位为512KB。放入缓存队尾部。
    // 如果扇区已经存在于缓存中，则刷新扇区
    bool write_disk(const BufferNode& node);

    // 将扇区中的内容读入buffer中,首先会从缓存里找有没有这个节点。
    // 新读入缓存的节点优先级为5，如果存在于缓存中，则优先级加 1
    bool read_disk(int block_num, BufferNode& node);

private:
    fstream disk_file;
    // 真正操作文件
    bool real_disk_write(const BufferNode& node);

    // 真正操作文件
    bool real_disk_read(int block_num, BufferNode& node);

    // 检查缓存中有没有给定扇区号的缓存
    int has_block(int block_number);

    // 返回优先级最低的缓存号码,没满返回0
    int is_full();


    // 磁盘缓存，共15个节点，满了之后会将优先级最低的节点写回磁盘
    vector<BufferNode> cache;
    // 静态的文件指针
//    fstream disk; // real disk file
};

#endif //MYFS_BUFFER_H
