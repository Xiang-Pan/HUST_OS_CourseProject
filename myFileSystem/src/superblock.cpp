#include "superblock.hpp"
#include "myfs.hpp"
superblock::superblock()
{
    disk.open(DISK, std::fstream::in | std::fstream::out | std::fstream::binary);
    read_from_disk();
//    memset(inode_bitmap, 0, sizeof(inode_bitmap));
//    memset(block_bitmap, 0, sizeof(block_bitmap));
}

superblock::~superblock()
{
    write_to_disk();
}


void superblock::print_inode_bitmap()
{
    for(int i = 0; i < INODE_NUM; i++)
    {
        printf("%d",inode_bitmap[i]);
    }
};

void superblock::print_block_bitmap()
{
    for(int i = 0; i < INODE_NUM; i++)
    {
        printf("%d",block_bitmap[i]);
    }
};


// 剩余的inode数量
int superblock::remain_inode()
{
    int count = 0;
    for(int i = 0; i < INODE_NUM; i++)
        if(!inode_bitmap[i])
            count++;
    return count;
}

// 剩余的扇区数量
int superblock::remain_sec()
{
    int count = 0;
    for(int i = 0; i < INODE_NUM; i++)
        if(!block_bitmap[i])
            count++;
    return count;
}

// 返回未使用的i节点, 如果满了就返回-1
int superblock::get_new_inode()
{
    for(int i = 0; i < INODE_NUM; i++)
    {
        if(!inode_bitmap[i])
        {
            inode_bitmap[i] = true;
            return i;
        }
    }
    return -1;
}

// 返回未使用的扇区,如果扇区已经用完就返回-1
int superblock::get_new_sec()
{
    for(int i = 0; i < BLOCK_NUM; i++)
        if(!block_bitmap[i])
        {
            block_bitmap[i] = true;
            return i + INODE_BEGIN / SEC_SIZE + (INODE_NUM * sizeof(Inode)) / SEC_SIZE;
        }
    return -1;
}

// 回收i节点
bool superblock::recv_inode(int inode_num) {
    assert(inode_num >= 0 && inode_num < INODE_NUM);

    inode_bitmap[inode_num] = false;
    return true;
}

// 回收扇区
bool superblock::recv_sec(int sec_num) {
    assert(sec_num >= 0 && sec_num < BLOCK_NUM);

    block_bitmap[sec_num] = false;
    return true;
}

// 格式化
bool superblock::init()
{
    memset(inode_bitmap, 0, INODE_NUM);
    memset(block_bitmap, 0, sizeof(block_bitmap));
    return true;
}

void superblock::read_from_disk()
{
    disk.seekg(SUPER_BEGIN);
//    if(disk.is_open())
//    {
//        cout<<"read open sus!";
//    }f
//    int i=123;
//    int j=789;
//    disk.seekg(SUPER_BEGIN);
//    disk.write((const char*)&i, sizeof(int) * 1);
//    disk.write((const char*)&j, sizeof(int) * 1);
    disk.seekg(SUPER_BEGIN);
//    disk>>cur_dir_node_num>>cur_dir_num;
    disk.read((char*)&cur_dir_node_num, sizeof(int));
    disk.read((char*)&cur_dir_num, sizeof(int));
    disk.read((char*)inode_bitmap, sizeof(bool) * INODE_NUM);
    disk.read((char*)block_bitmap, sizeof(bool) * BLOCK_NUM);
//    cout<<cur_dir_node_num<<cur_dir_num;
}

void superblock::write_to_disk()
{
    cur_dir_node_num=myfs->cur_dir_node.get_inode_num();
    cur_dir_num=myfs->cur_dir_node.get_sec_beg();
//    cout<<cur_dir_node_num<<cur_dir_num;
    if(disk.is_open())
    {
        cout<<"wriet open sus!";
    }
    disk.seekg(SUPER_BEGIN);
//    disk<<cur_dir_node_num<<cur_dir_num;
    disk.write((const char*)&cur_dir_node_num, sizeof(int));
    disk.write((const char*)&cur_dir_num, sizeof(int));
    disk.write((const char*)inode_bitmap, sizeof(bool) * INODE_NUM);
    disk.write((const char*)block_bitmap, sizeof(bool) * BLOCK_NUM);
    disk.close();

}
