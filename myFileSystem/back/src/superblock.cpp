#include "superblock.hpp"


superblock::superblock()
{
    static uint node_num;          // now inode num
    block_size=BLOCK_SIZE;
//    direct_blocks;
    blocks_num=BLOCK_NUM;
    memset(inode_bitmap, 0, sizeof(inode_bitmap));
    memset(block_bitmap, 0, sizeof(block_bitmap));
}
