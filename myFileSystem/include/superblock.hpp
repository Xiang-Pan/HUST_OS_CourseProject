#include <fstream>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <cstring>

#define BLOCK_SIZE 512      // 扇区大小
#define INODE_NUM 1024      // inode数量
#define BLOCK_NUM 1024      // 数据扇区数量
#define SUPER_BEGIN 0       // 超级块起始地址
#define INODE_BEGIN  sizeof(superblock)
#define BLOCK_BEGIN (sizeof(superblock) + sizeof(Inode) * INODE_NUM)
#define MAX_SEC   ((BLOCK_BEGIN + BLOCK_NUM * SEC_SIZE) / SEC_SIZE )

class SuperBlock
{
    private:
        bool inode_bitmap[INODE_NUM];
        bool block_bitmap[BLOCK_NUM];

    public:
        // can be seen as superblock
        std::string filename;     //disk file name
        std::fstream disk_file;
        uint node_num;          // now inode num
        uint direct_blocks;
        uint blocks_num;
        uint block_size;
       
//
        SuperBlock();
//        bool init();
};
