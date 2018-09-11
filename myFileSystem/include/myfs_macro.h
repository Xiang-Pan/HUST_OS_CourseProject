#ifndef MYFS_MACRO_H
#define MYFS_MACRO_H

#define SEC_SIZE 1024
#define INODE_NUM 1024
#define BLOCK_NUM 1024
#define DISK  "disk.img"
#define IMG "/tmp/myfs_temp"
#define SUPER_BEGIN 0
#define INODE_BEGIN sizeof(superblock)
#define VALID_DATA_LENGTH (SEC_SIZE-sizeof(int))
#define BLOCK_BEGIN (sizeof(superblock) + sizeof(Inode) * INODE_NUM)
#define MAX_SEC   ((BLOCK_BEGIN + BLOCK_NUM * SEC_SIZE) / SEC_SIZE )


#endif //MYFS_MACRO_H