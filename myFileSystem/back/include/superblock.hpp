#include <fstream>
#include <list>
#include <map>
#include <string>
#include <vector>


#define BLOCK_SIZE 512    // 扇区大小
#define INODE_NUM 1024  // inode数量
#define BLOCK_NUM 1024  // 数据扇区数量
#define SUPER_BEGIN 0     // 超级块起始地址
#define INODE_BEGIN  sizeof(superblock)
#define BLOCK_BEGIN (sizeof(superblock) + sizeof(Inode) * INODE_NUM)
#define MAX_SEC   ((BLOCK_BEGIN + BLOCK_NUM * SEC_SIZE) / SEC_SIZE )

class superblock
{
    private:
        bool inode_bitmap[INODE_NUM];
        bool block_bitmap[BLOCK_NUM];

    public:
        // can be seen as superblock
        const std::string filename;     //disk file name
        std::fstream disk_file;
        static uint node_num;          // now inode num
        const uint block_size;
        const uint direct_blocks;
        const uint blocks_num;
       
//
        superblock();
//        bool init();
};
