
#ifndef BUFFER_H
#define BUFFER_H

#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include "myfs_macro.h"
#include "assert.h"
#include "inode.hpp"


using namespace std;

struct BufferNode
{
    char buffer[SEC_SIZE + 1];  //read in 1 sec
    int pri;
    int sec_num;
    BufferNode operator = (const BufferNode& b) 
    {
        memcpy(buffer, b.buffer, SEC_SIZE + 1);
        pri = b.pri;
        sec_num = b.sec_num;
    }
    BufferNode()
    {
        memset(buffer, 0, SEC_SIZE);
        pri = 0;
        sec_num = 0;
    }
    void init(int _sec_num) 
    {
        pri = 1;
        sec_num = _sec_num;
    }
    void update(const BufferNode& b) 
    {
        memcpy(buffer, b.buffer, SEC_SIZE + 1);
        pri = b.pri + 1;
        sec_num = b.sec_num;
    }
};

class Buffer
{
    public:
    int buffer_size;
    Buffer();
    ~Buffer();
    bool write_disk(const BufferNode& node);
    bool read_disk(int sec_num, BufferNode& node);
    void all_write_to_disk();

    private:

    bool real_disk_write(const BufferNode& node);
    bool real_disk_read(int sec_num, BufferNode& node);
    int has_sec(int sec_number);
    int is_full();


    vector<BufferNode> cache;
    fstream disk;
};

#endif