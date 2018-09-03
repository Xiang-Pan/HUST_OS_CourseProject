#ifndef _MYFS_H_
#define _MYFS_H_


#include<boost/serialization/vector.hpp>
#include <fstream>
#include <list>
#include <map>
#include <string>
#include <vector>
#include "inode.hpp"
#include "direntry.hpp"
#include "freenode.hpp"



class myFS 
{
    enum Mode {R, W, RW}; // open file mode

    // Mode 
    struct Descriptor 
    {
        Mode mode;
        uint byte_pos;
        std::weak_ptr<Inode> inode;
        std::weak_ptr<DirEntry> from;
        uint fd;
    };
    bool getMode(Mode *mode, std::string mode_s);

    //return path
    struct PathRet 
    {
        bool invalid_path = false;
        std::string final_name;
        std::shared_ptr<DirEntry> parent_node;
        std::shared_ptr<DirEntry> final_node;
    };

    // can be seen as superblock
    const std::string filename;
    std::fstream disk_file;
    const uint block_size;
    const uint direct_blocks;
    const uint num_blocks;

    // DirEntry root;
    std::list<FreeNode>free_list;
    std::shared_ptr<DirEntry> root_dir;
    std::shared_ptr<DirEntry> pwd;
    std::map<uint, Descriptor> open_files;  //save open file uint and desp
    uint next_descriptor = 0;

    void init_disk(const std::string& filename);
    std::unique_ptr<PathRet> parse_path(std::string path_str) const;
    bool basic_open(Descriptor *d, std::vector <std::string> args);
    std::unique_ptr<std::string> basic_read(Descriptor &desc, const uint size);
    uint basic_write(Descriptor &desc, const std::string data);
    bool basic_close(uint fd);

    public:
    myFS(const std::string& filename,
        const uint fs_size,
        const uint block_size,
        const uint direct_blocks);
    ~myFS();
    void open(std::vector<std::string> args);
    void read(std::vector<std::string> args);
    void write(std::vector<std::string> args);
    void seek(std::vector<std::string> args);
    void close(std::vector<std::string> args);
    void mkdir(std::vector<std::string> args);
    void rmdir(std::vector<std::string> args);
    void cd(std::vector<std::string> args);
    void link(std::vector<std::string> args);
    void unlink(std::vector<std::string> args);
    void stat(std::vector<std::string> args);
    void ls(std::vector<std::string> args);
    void cat(std::vector<std::string> args);
    void cp(std::vector<std::string> args);
    void tree(std::vector<std::string> args);
    void import(std::vector<std::string> args);
    void printwd(std::vector<std::string> args);

    std::string getpwd(std::vector<std::string> args);

    void FS_export(std::vector<std::string> args);


    friend class boost::serialization::access;
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
        ar &filename;
        ar &num_blocks;
        ar &block_size;
        ar &direct_blocks;
	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar &filename;
        ar &num_blocks;
        ar &block_size;
        ar &direct_blocks;
	}

};

#endif /* _MYFS_H_ */
