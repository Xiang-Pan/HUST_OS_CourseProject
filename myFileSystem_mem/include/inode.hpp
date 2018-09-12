#ifndef _INODE_H_
#define _INODE_H_

#include <sys/types.h>
#include <list>
#include <memory>
#include <vector>
#include <string>
#include "freenode.hpp"
#include "macro.h"
using namespace std;



class Inode
{
    public:
    long create_time;
    uint inode_num;
    uint sec_num;

    uint size;
    uint blocks_used;
    static uint block_size;
    static list<FreeNode> *free_list; // freenode list


    // use unique_ptr to ensure the alloc err , if false then recollect
    vector<uint> d_blocks;
    unique_ptr<std::vector<std::vector<uint>>> i_blocks; // i_blocks 

    Inode();
    ~Inode();
};

#endif /* _INODE_H_ */
