/* FileName:    superblock.cpp
 * Author:      Hover
 * E-Mail:      hover@hust.edu.cn
 * GitHub:      HoverWings
 * Description: superblock
 */
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

