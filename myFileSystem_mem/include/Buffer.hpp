//
// Created by wings on 8/15/18.
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

    bool write_disk(const BufferNode& node);

    bool read_disk(int block_num, BufferNode& node);

private:
    fstream disk_file;

    bool real_disk_write(const BufferNode& node);

    bool real_disk_read(int block_num, BufferNode& node);

    int has_block(int block_number);

    int is_full();



    vector<BufferNode> cache;

};

#endif //MYFS_BUFFER_H
