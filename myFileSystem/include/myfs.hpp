#include "myfs_macro.h"
#include "superblock.hpp"
#include "inode.hpp"
#include "Buffer.hpp"
#include "direntry.hpp"
#include <sstream>
#include <cstring>
#include <vector>
class myFS
{
public:
    void myshell();

    void run();

    // construct
    myFS();

    void vim(vector<string> args);
//    void open(vector<string> args);
//    void read(vector<string> args);
//    void write(vector<string> args);
//    void seek(vector<string> args);
//    void close(vector<string> args);
    void mkdir(vector<string> args);
    void rmdir(vector<string> args);
    void cd(vector<string> args);
//    void link(vector<string> args);
//    void unlink(vector<string> args);
//    void stat(vector<string> args);
    void ls(vector<string> args);
    void touch(vector<string> args);
    void cat(vector<string> args);
//    void cp(vector<string> args);
//    void tree(vector<string> args);
//    void import(vector<string> args);
    void printpwd(vector<string> args);

    string getpwd(vector<string> args);

//    void FS_export(vector<string> args);
    string PRMPT = "sh> ";


    superblock sp;
    Inode cur_dir_node;
    sector_dir cur_dir;
private:

    string pwd;

    Buffer my_cache;

    bool format_file_system();
    bool del_inode(Inode& node, sector_dir& del_dir);

    bool move_in(int ionde_num,string file_name);
//    bool move_out(string name);
    bool move_out(int inode_num);

    bool init_file_system();

    int is_existed_file(string filename);



};
