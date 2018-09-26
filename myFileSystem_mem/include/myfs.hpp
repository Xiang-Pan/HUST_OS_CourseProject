/* FileName:    myfs.hpp
 * Author:      Hover
 * E-Mail:      hover@hust.edu.cn
 * GitHub:      HoverWings
 * Description: myfs
 */
#ifndef _MYFS_H_
#define _MYFS_H_


//#include<boost/serialization/vector.hpp>
#include <fstream>
#include <list>
#include <map>
#include <string>
#include <vector>
#include "macro.h"
#include "inode.hpp"
#include "direntry.hpp"
#include "freenode.hpp"
#include "superblock.hpp"
#include "Buffer.hpp"


using namespace std;


class myFS 
{

    enum Mode {R, W, RW}; // open file mode

    // Mode 
    struct Descriptor 
    {
        Mode mode;
        uint byte_pos;
        weak_ptr<Inode> inode;
        weak_ptr<DirEntry> from;
        uint fd;
    };
    bool getMode(Mode *mode, string mode_s);

    //return path
    struct PathRet 
    {
        bool invalid_path = false;
        string final_name;
        shared_ptr<DirEntry> parent_node;
        shared_ptr<DirEntry> final_node;
    };

    // can be seen as superblock
    string filename;     //disk file name

    uint block_size;
    uint direct_blocks;
    uint block_num;

    // DirEntry root;
    list<FreeNode>free_list;
    shared_ptr<DirEntry> root_dir;
    shared_ptr<DirEntry> pwd;
    map<uint, Descriptor> open_files;  //save open file uint and desp
    uint next_descriptor = 0;

    void init_disk(const string& filename);
    unique_ptr<PathRet> parse_path(string path_str) const;
    bool basic_open(Descriptor *d, vector <string> args);
    unique_ptr<string> basic_read(Descriptor &desc, const uint size);
    uint basic_write(Descriptor &desc, const string data);
    bool basic_close(uint fd);

    public:
    myFS(string& filename);
    ~myFS();


    fstream disk_file;


    class SuperBlock* p_sp;

    // current dir
    Inode cur_dir_node; // current dir inode
    DirEntry cur_dir;   //current dir

    // cache
    Buffer fs_cache;

    void open(vector<string> args);
    void read(vector<string> args);
    void write(vector<string> args);
    void seek(vector<string> args);
    void close(vector<string> args);
    void mkdir(vector<string> args);
    void rmdir(vector<string> args);
    void cd(vector<string> args);
    void link(vector<string> args);
    void unlink(vector<string> args);
    void stat(vector<string> args);
    void ls(vector<string> args);
    void cat(vector<string> args);
    void cp(vector<string> args);
    void tree(vector<string> args);
    void import(vector<string> args);
    void printwd(vector<string> args);

    string getpwd(vector<string> args);

    void FS_export(vector<string> args);


   friend class boost::serialization::access;
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
       ar &filename;
       ar &blocks_num;
       ar &block_size;
       ar &direct_blocks;
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar &filename;
       ar &blocks_num;
       ar &block_size;
       ar &direct_blocks;
	}

};

#endif /* _MYFS_H_ */
