/* FileName:    superblock.hpp
 * Author:      Hover
 * E-Mail:      hover@hust.edu.cn
 * GitHub:      HoverWings
 * Description: superblock
 */
#ifndef _SUPERBLOCK_H_
#define _SUPERBLOCK_H_
#include "inode.hpp"
#include <fstream>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <cstring>
#include "macro.h"
#include "assert.h"
#include "myfs.hpp"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>


class myFS;
class SuperBlock
{
    private:
        bool inode_bitmap[INODE_NUM];
        bool block_bitmap[BLOCK_NUM];

    public:
//        uint node_num;          // now inode num
//        uint direct_blocks;
//        uint blocks_num;
//        uint block_size;
        int q;
        class myFS* myfs;
        SuperBlock();
        bool write_back_to_disk();

        bool read_from_disk();

        // template<class Archive>
        // void serialize(Archive & ar, const unsigned int version)
        // {
        //     ar& inode_bitmap;
        //     ar& block_bitmap;
        // }
};

#endif
