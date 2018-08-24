#ifndef MYFS_H
#define MYFS_H

#define BLOCK_SIZE 1024
#define BLOCK_NUM 102400
#define SYSTEM_NAME "FileSystem.tzh"


// the define of command 
#define SUDO     0
#define HELP     1
#define LS       2
#define CD       3
#define MKDIR    4
#define TOUCH    5
#define CAT      6
#define WRITE    7
#define RM       8
#define MV       9
#define CP      10
#define CHMOD   11
#define LOGOUT  12
#define EXIT    13
#define SYSINFO 14
#define CLEAR   15
#define ACCOUNT 16

using namespace std;

/*
Descraption:Super Block
Scale:      14B
*/
typedef struct{
    unsigned short blockSize;   //2B
    unsigned int blockNum;      //4B 
    unsigned int inodeNum;      //4B the num of file
    unsigned int blockFree;     //4B the num of free block
}SuperBlock;


/**
Descraption:inode Block
Scale:      100B
*/
typedef struct
{
    unsigned int id;      //4B,i节点索引
    char name[30];        //30B,文件名，最大长度29
    unsigned char isDir;  //1B,文件类型 0-file 1-dir
    unsigned int parent;  //4B,父目录i节点索引
    unsigned int length;  //4B,文件长度,unsigned int最大2^32-1(4GB-1B),目录文件则为子文件项数量
    unsigned char type;   //1B,文件权限,0-read-only,1-read-write
    time_t time;          //8B,文件最后修改时间，从1970年1月1日00时00分00秒至今所经过的秒数
    unsigned int addr[12];//12*4B,文件内容索引,文件前10项为直接索引，目录前11项为直接索引
    unsigned int blockId; //文件项所在的目录数据块的id，便于删除时定位
}Inode,*PInode;


/**
文件部分信息节点，用于简要显示文件信息
文件名 文件／目录　文件长度　文件权限　修改时间
*/
typedef struct 
{
    unsigned int id;     //4B
    char name[30];       //30B
    unsigned char isDir; //1B
    unsigned int blockId; //4B,在目录数据块中的位置0-255
}Fcb,*PFcb;

typedef struct FCB_LINK_NODE
{
    Fcb fcb;
    struct FCB_LINK_NODE* next;
}FcbLinkNode, *FcbLink;

/*
D:user struct 
*/
typedef struct
{
    char username[10];  //username
    char password[10];  //password
}User;

class FileSystem
{
    public:
        FileSystem(char* name);
        virtual ~FileSystem();

        int init();
        //system init
        void createFileSystem();
        void openFileSystem();

        //command
        void help();
        void ls();
        void ls_l();
        int cd(char* name);
        //file operation
        int createFile(char* name, unsigned char isDir);
        int read(char* name);
        int write(char* name);
        int del(char* name);
        int mv(char* name, char* newName);
        int cp(char* src, char* dst);
        int chmod(char* name, unsigned char type);
        //system

        void login();
        void logout();
        void exit();
        void systemInfo();
        void clr();
        int account();

        void command(void);


    protected:
    private:
        const int blockSize;        //文件块大小
        const int blockNum;         //文件块数量
        unsigned char* blockBitmap; //文件块使用图
        unsigned char* inodeBitmap; //i节点使用图
        //大小
        unsigned short userSize;
        unsigned short superBlockSize;
        unsigned short blockBitmapSize;
        unsigned short inodeBitmapSize;
        unsigned short inodeSize;
        unsigned short fcbSize;
        unsigned short itemSize;
        //偏移
        unsigned long sOffset;     //superBlock偏移
        unsigned long bbOffset;    //blockBitmap偏移
        unsigned long ibOffset;    //inodeBitmap偏移
        unsigned long iOffset;     //i节点区偏移
        unsigned long bOffset;     //数据区偏移
        unsigned char isAlive;     //系统是否在运行

        char name[30];//文件系统名
        FILE* fp;//文件系统文件指针
        SuperBlock superBlock;//文件系统块信息
        string curPath;//当前目录字符串
        Inode curInode;//当前目录i节点
        FcbLink curLink;//当前目录子文件链指针


        User user;//用户
        char cmd[5][20];//命令行输入
        char* SYS_CMD[17];//系统支持的命令

        //user
        void getUser(User* pUser);
        void setUser(User user);

        //superBlock
        void getSuperBlock(SuperBlock* pSuper);
        void updateSuperBlock(SuperBlock super);

        //blockBitmap
        unsigned int getAvailableBlockId();
        void getBlockBitmap(unsigned char* bitmap);
        void updateBlockBitmap(unsigned char* bitmap, unsigned int index);
        void updateBlockBitmap(unsigned char* bitmap, unsigned int start, unsigned int count);

        //inodeBitmap
        unsigned int getAvailableInodeId();
        void getInodeBitmap(unsigned char* bitmap);
        void updateInodeBitmap(unsigned char* bitmap, unsigned int index);
        void updateInodeBitmap(unsigned char* bitmap, unsigned int start, unsigned int count);

        //inode block
        void getInode(PInode pInode, unsigned int id);
        void updateInode(Inode inode);
        void releaseInode(unsigned int id);

        //data block
        //地址或文件项
        unsigned int getAvailableFileItem(Inode& inode, unsigned int* availableIndex);
        unsigned int getItem(unsigned int blockId, unsigned int index);
        void updateItem(unsigned int blockId, unsigned int index, unsigned int value);
        void releaseItem(unsigned int blockId, unsigned int id);

        //文件内容
        int getData(unsigned int blockId, char* buff, unsigned int size, unsigned int offset);
        int writeData(unsigned int blockId, char* buff, unsigned int size, unsigned int offset);
        void releaseBlock(unsigned int blockId);

        //定位操作
        unsigned int findChildInode(FcbLink curLink, char* name);

        //目录信息链操作
        void getFcbLinkNode(FcbLink pNode, Inode inode);
        void getFcbLink(FcbLink& curLink, Inode inode);
        void appendFcbLinkNode(FcbLink curLink, Inode inode);
        void removeFcbLinkNode(FcbLink curLink, Inode inode);
        void removeFcbLinkNode(FcbLink curLink, char* name);
        void releaseFcbLink(FcbLink& curLink);

        //system command
        int analyse(char* str);
        void stopHandle(int sig);
        void updateResource();
        void showPath();

        //util
        void showFileDigest(FcbLink pNode);
        void showFileDetail(PInode pInode);
        unsigned int waitForInput(char* buff, unsigned int limit);

};
#endif