#include "../include/inode.hpp"
#include <algorithm>
#include <list>
#include <vector>

using std::list;
using std::shared_ptr;
using std::sort;
using std::vector;

uint Inode::block_size = 0;
list<FreeNode> * Inode::free_list = nullptr;

Inode::Inode(): size(0), blocks_used(0), i_blocks(new vector<vector<uint>>())
{
    inode_num= static_cast<uint>(inode_total);
    inode_total++;
}

Inode::~Inode()
{
    if (blocks_used == 0)
    {
        return;
    }
    else
    {
        if(blocks_used == 1) 
        {
            free_list->emplace_front(block_size, d_blocks[0]);
        }
    }

    vector<uint> blocks;

    for (uint block : d_blocks)
    {
        blocks.push_back(block);
    }

    for (auto vec: *i_blocks)
    {
        for (uint block: vec)
        {
            blocks.push_back(block);
        }
    }

    sort(begin(blocks), end(blocks));

    uint start = blocks.front();
    uint last = start;
    uint size = block_size;
    blocks.erase(begin(blocks));
    for (uint block : blocks)
    {
        if (block - last != block_size)
        {
            free_list->emplace_back(size, start);
            start= block;
            last = start;
            size = 0;
        }
        else
        {
            last = block;
            size += block_size;
        }
    }
    free_list->emplace_front(size, start);
}



//
// bool Inode::read_inode_from_disk(int inode_num) 
// {
//     assert(inode_num >= 0 && inode_num < INODE_NUM);
//     set_inode_num(inode_num);
//     int sec_num = get_inode_sec_num();
//     int num_in_sec = inode_num % 16;
//     BufferNode buffer_node;

//     buffer.read_disk(sec_num, buffer_node);
//     memcpy(this, buffer_node.buffer + num_in_sec * sizeof(Inode), sizeof(Inode));

//     return true;
// }

// // D: inode is used and modefied
// bool Inode::write_inode_back_to_disk(int inode_num)
// {
//    block_size
//     int sec_num = get_inode_sec_num();
//     int num_in_sec = _inode_num % 16;
//     BufferNode buffer_node;
//
//     buffer.read_disk(sec_num, buffer_node);
//     memcpy(buffer_node.buffer + num_in_sec * sizeof(Inode), this, sizeof(Inode));
//     cout << "将inode写回磁盘, inode号码" << _inode_num << ", 扇区号：" << sec_num << endl;
//     buffer.write_disk(buffer_node);
//     return true;
// }


