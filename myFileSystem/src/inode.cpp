#include "inode.hpp"

using namespace std;
extern Buffer buffer;

// 构造函数初始化
Inode::Inode()
{
    _inode_num = 0;
    _is_file = false;
    _file_size = 0;
    _sec_beg = 0;
    _sec_num = 0;
    memset(_compensate, 0, 12);
}

Inode::Inode(int node_num, bool _is_file, int file_size, int sec_begin)
{
    _inode_num = node_num;
    _is_file = _is_file;
    _file_size = file_size;
    _sec_beg = sec_begin;
    _sec_num = (file_size) / sizeof(VALID_DATA_LENGTH) + 1;
    cout << "创建新的i节点，inode号" << node_num ;
    cout << " ，开始扇区：" << sec_begin << endl;
}

int Inode::get_inode_num()
{
    return _inode_num;
}

// true->file; false->dir
bool Inode::get_type() {
    return _is_file;
}

int Inode::get_file_size() {
    return _file_size;
}

int Inode::get_sec_beg() {
    return _sec_beg;
}

int Inode::get_sec_num() {
    return _sec_num;
}

// 返回Inode对应的扇区号
int Inode::get_inode_sec_num()
{
//    return INODE_BEGIN / SEC_SIZE + _inode_num / sizeof(Inode);
    return INODE_BEGIN / SEC_SIZE + _inode_num;
}

void Inode::set_inode_num(int num)
{
    _inode_num = num;
}


bool Inode::read_inode_from_disk(int inode_num,Buffer &buffer)
{
    assert(inode_num >= 0 && inode_num < INODE_NUM);
    set_inode_num(inode_num);
    int sec_num = get_inode_sec_num();
    int num_in_sec = inode_num % 16;
    BufferNode buffer_node;

    buffer.read_disk(sec_num, buffer_node);
    memcpy(this, buffer_node.buffer + num_in_sec * sizeof(Inode), sizeof(Inode));

    return true;
}


bool Inode::write_inode_back_to_disk(Buffer &buffer)
{
    int sec_num = get_inode_sec_num();
    int num_in_sec = _inode_num % 16;
    BufferNode buffer_node;

    buffer.read_disk(sec_num, buffer_node);
    memcpy(buffer_node.buffer + num_in_sec * sizeof(Inode), this, sizeof(Inode));
    cout << "将inode写回磁盘, inode号码" << _inode_num << ", 扇区号：" << sec_num << endl;
    buffer.write_disk(buffer_node);
    return true;
}
