#ifndef _DIRENTRY_H_
#define _DIRENTRY_H_

#include <list>
#include <memory>
#include <string>
#include <sys/types.h>
#include "freenode.hpp"
#include "inode.hpp"

using namespace std;

enum EntryType { file, dir };

class DirEntry: public enable_shared_from_this<DirEntry> 
{
    public:
    DirEntry();
    static shared_ptr<DirEntry> make_de_dir(const string name,
                                                const shared_ptr<DirEntry> parent);
    static shared_ptr<DirEntry> make_de_file(const string name,
                                                const shared_ptr<DirEntry> parent,
                                                const shared_ptr<Inode> &inode=nullptr);
    uint block_size;
    EntryType type;                 //file or dir
    string name;   
    weak_ptr<DirEntry> parent; // .
    weak_ptr<DirEntry> self;   // ..
    shared_ptr<Inode> inode;   // file
    list<shared_ptr<DirEntry>> contents; // dir entry
    bool is_locked;                      // lock

    shared_ptr<DirEntry> find_child(const string name) const;
    shared_ptr<DirEntry> add_dir(const string name);
    shared_ptr<DirEntry> add_file(const string name);
};

#endif /* _DIRENTRY_H_ */
