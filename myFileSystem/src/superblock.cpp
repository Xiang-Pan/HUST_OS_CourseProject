#include "superblock.hpp"



using namespace boost;
//Inode::Inode():
SuperBlock::SuperBlock()
{

    static uint node_num;          // now inode num
    memset(inode_bitmap, 0, sizeof(inode_bitmap));
    memset(block_bitmap, 0, sizeof(block_bitmap));
}


bool SuperBlock::write_back_to_disk()
{
    stringstream binary_sstream;
    boost::archive::binary_oarchive binary_oa(binary_sstream);
    binary_oa<<block_bitmap;
    binary_oa<<inode_bitmap;
    cout<<binary_sstream.str();
//    binary_sstream<<binary_sstream;



}

//int SuperBlock::get_new_sec()
//{
//    for(int i = 0; i < BLOCK_NUM; i++)
//        if(!block_bitmap[i])
//        {
//            block_bitmap[i] = true;
//            return i + INODE_BEGIN / 512 + (INODE_NUM * sizeof(Inode)) / 512;
//        }
//    return -1;
//}


//// 回收i节点
//bool SuperBlock::recv_inode(int inode_num)
//{
//    assert(inode_num >= 0 && inode_num < INODE_NUM);
//
//    inode_bitmap[inode_num] = false;
//    return true;
//}
//
//// 回收扇区
//bool SuperBlock::recv_sec(int sec_num)
//{
//    assert(sec_num >= 0 && sec_num < BLOCK_NUM);
//
//    block_bitmap[sec_num] = false;
//    return true;
//}

