
#ifndef INODE_H
#define INODE_H
#include "myfs_macro.h"
#include "Buffer.hpp"
#include <iostream>
#include "assert.h"
#include "superblock.hpp"


// compensate to 32 Bytes
class Inode
{
friend class Buffer;
private:



    int _sec_beg;   //  link by ptr
    int _sec_num;   // total sec num
    char _compensate[12];

public:
    bool _is_file;
    int _file_size; // Byte
    int _inode_num;
  Inode();
  class Buffer *buffer;
  int mode;
  time_t creat_time;
  time_t modify_time;
  Inode(int node_num, bool _is_file, int file_size, int sec_begin);

  int get_inode_num();

  // true->file; false->dir
  bool get_type();

  int get_file_size();

  int get_sec_beg();

  int get_sec_num();

  void set_inode_num(int num);

  int get_inode_sec_num();

  bool read_inode_from_disk(int inode_num,Buffer &buffer);

  bool write_inode_back_to_disk(Buffer &buffer);

  Inode operator = (const Inode& b)
  {

  }
};

#endif