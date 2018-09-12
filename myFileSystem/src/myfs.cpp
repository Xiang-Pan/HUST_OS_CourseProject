#include "myfs.hpp"
using  namespace std;

// less than opt required
#define ops_at_least(x)                                 \
  if (static_cast<int>(args.size()) < x+1) {            \
    cerr << args[0] << ": missing operand" << endl;     \
    return;                                             \
  }

// more than opt required
#define ops_less_than(x)                                \
  if (static_cast<int>(args.size()) > x+1) {            \
    cerr << args[0] << ": too many operands" << endl;   \
    return;                                             \
  }\


// construct
myFS::myFS()
{
    cout << endl << "****************** Hover's FileSystem ******************" << endl;
    this->sp.myfs=this;


//    cout<<sp.cur_dir_num<<" "<<sp.cur_dir_node_num;
//
    cur_dir_node.read_inode_from_disk(sp.cur_dir_node_num,my_cache);
//    cout<<cur_dir_node.get_sec_beg()<<" "<<cur_dir_node.get_inode_num();

    sector_dir root_sec_dir;
    root_sec_dir.read_dir_from_disk(my_cache,cur_dir_node.get_sec_beg());
    cur_dir=root_sec_dir;
    cur_dir.write_back_to_disk(my_cache,sp.cur_dir_num);

//    format_file_system();

}

void myFS::myshell()
{
    string cmd;
    myFS * fs=this;
    vector<string> args;
    string token;
    PRMPT=getpwd(args);
//    cout<<fs->getpwd(args);
    PRMPT+=">";
    cout << PRMPT;
    while (getline(cin, cmd))
    {
//        PRMPT=fs->getpwd(args);
//        PRMPT+=">";
        args.clear();
        istringstream iss(cmd);
        while (iss >> token) { args.push_back(token); }
        if (args.size() == 0)
        {
            cout << PRMPT;
            continue;
        }
        if (args[0] == "ls")
        {
            ls(args);
        }
        else if (args[0] == "touch")
        {
            touch(args);
        }
        else if (args[0] == "cd")
        {
            cur_dir_node.write_inode_back_to_disk(my_cache);
            cd(args);
        }
        else if (args[0] == "mkdir")
        {
            mkdir(args);
        }
        else if (args[0] == "rmdir")
        {
            rmdir(args);
        }
        else if (args[0] == "print")
        {
            sp.print_block_bitmap();
            cout<<endl;
            sp.print_inode_bitmap();
        }
        else if (args[0] == "cat")
        {
            cat(args);
        }
        else if (args[0] == "format")
        {
            format_file_system();
        }
        else if (args[0] == "pwd")
        {
            cout<<"pwd:";
            printpwd(args);
        }
        else if (args[0] == "move_in")
        {
            string file_name=args[1];
            const char *name = file_name.c_str();
            int inode_num=is_existed_file(file_name);
            move_in(inode_num,file_name);
        }
        else if (args[0] == "move_out")
        {
            string file_name=args[1];
            const char *name = file_name.c_str();
            int inode_num=is_existed_file(file_name);
            move_out(inode_num);
        }
        else if (args[0] == "vim")
        {
            vim(args);
        }
        else if (args[0] == "exit")
        {
            cerr<<cur_dir_node.get_sec_beg()<<" "<<cur_dir_node.get_inode_num()<<endl;
            cur_dir_node.read_inode_from_disk(cur_dir_node.get_inode_num(), my_cache);
            cur_dir_node.write_inode_back_to_disk(my_cache);
            fs->sp.write_to_disk();
            fs->my_cache.all_write_to_disk();
            return;
        }
        else
        {
            cerr<< "comman not found"<<endl;
        }
        PRMPT=fs->getpwd(args);
        PRMPT+=">";
        cout<<PRMPT;
    }

}


namespace strtool
{
    string trim(const string& str)
    {
        string::size_type pos = str.find_first_not_of(' ');
        if (pos == string::npos)
        {
            return str;
        }
        string::size_type pos2 = str.find_last_not_of(' ');
        if (pos2 != string::npos)
        {
            return str.substr(pos, pos2 - pos + 1);
        }
        return str.substr(pos);
    }

    int split(const string& str, vector<string>& ret_, string sep = ",")
    {
        if (str.empty())
        {
            return 0;
        }

        string tmp;
        string::size_type pos_begin = str.find_first_not_of(sep);
        string::size_type comma_pos = 0;

        while (pos_begin != string::npos)
        {
            comma_pos = str.find(sep, pos_begin);
            if (comma_pos != string::npos)
            {
                tmp = str.substr(pos_begin, comma_pos - pos_begin);
                pos_begin = comma_pos + sep.length();
            }
            else
            {
                tmp = str.substr(pos_begin);
                pos_begin = comma_pos;
            }

            if (!tmp.empty())
            {
                ret_.push_back(tmp);
                tmp.clear();
            }
        }
        return 0;
    }

    string replace(const string& str, const string& src, const string& dest)
    {
        string ret;

        string::size_type pos_begin = 0;
        string::size_type pos       = str.find(src);
        while (pos != string::npos)
        {
            cout <<"replacexxx:" << pos_begin <<" " << pos <<"\n";
            ret.append(str.data() + pos_begin, pos - pos_begin);
            ret += dest;
            pos_begin = pos + 1;
            pos       = str.find(src, pos_begin);
        }
        if (pos_begin < str.length())
        {
            ret.append(str.begin() + pos_begin, str.end());
        }
        return ret;
    }
}


void myFS::ls(vector<string> args)
{
    for(int i = 2; i < 15; i++)
    {
        cout << cur_dir.dirs[i].name << " ";
    }
    cout << endl;
}


//Begin
void myFS::run()
{
    myshell();
    return;
}


bool myFS::del_inode(Inode& node, sector_dir& del_dir)
{
    cout << "delete inode, inode num为" << node.get_inode_num() << endl;
    if(node.get_type())
    {
        for(int i = 2; i < 15; i++)
        {
            if(del_dir.dirs[i].inode_num == node.get_inode_num())
            {
                cout << "delate inode，删除sector中对文件的记录" << endl;
                memset(&del_dir.dirs[i], 0, sizeof(sector_dir_entry));
                del_dir.write_back_to_disk(my_cache, node.get_sec_beg());
                break;
            }
        }

        // 2. 释放inode和对应的扇区
        sp.recv_sec(node.get_sec_beg() - BLOCK_BEGIN / 512);
        sp.recv_inode(node.get_inode_num());
    }
    else {
        // dir
        // 1.先删除当前目录对这个目录的记录
        for(int i = 0; i < 15; i++) {
            if(node.get_inode_num() == del_dir.dirs[i].inode_num) {
                cout << "delate inode，删除sector中对文件的记录" << endl;
                memset(&del_dir.dirs[i], 0, sizeof(sector_dir_entry));
                del_dir.write_back_to_disk(my_cache, node.get_sec_beg());
                break;
            }
        }
        // 2. 释放这个目录的inode和扇区
        sp.recv_sec(node.get_sec_beg() - BLOCK_BEGIN / 512);
        sp.recv_inode(node.get_inode_num());
        // 3. 切换到要删除的目录下
        Inode new_node;
        new_node = node;
        sector_dir new_dir;
        new_dir = del_dir;

        new_dir.read_dir_from_disk(my_cache, new_node.get_sec_beg());
        // 4. delete every files and directories recursively
        for(int i = 2; i < 15; i++) {
            if(new_dir.dirs[i].inode_num != 0) {
                new_node.read_inode_from_disk(new_dir.dirs[i].inode_num, my_cache);
                del_inode(new_node, new_dir);
            }
        }
    }
}


bool myFS::move_in(int ionde_num,string file_name)
{
    const char *name = file_name.c_str();
    // get file size, compute needed block number, allocate block
    ifstream is(IMG);
    if(is)
    {
        is.seekg(0, is.end);
        int length = is.tellg();
        cout << "size of the file:" << length << " bytes" << endl;

        // 2. compute needed blocks
        int needed_block = length / VALID_DATA_LENGTH;
        if(length % VALID_DATA_LENGTH != 0)
            needed_block++;
        int left = length % VALID_DATA_LENGTH;
        cout << endl << "last node contain " << ((left == 0) ? VALID_DATA_LENGTH : left) << "bytes of data" << endl;
        cout << "need " << needed_block << " blocks to store data" << endl;
        int flag = false;
        Inode now_file_inode;
        if(ionde_num==-1)
        {
            Inode new_file_inode(sp.get_new_inode(), true, length, sp.get_new_sec());
            new_file_inode._is_file= true;
            new_file_inode.write_inode_back_to_disk(my_cache);
            ionde_num=new_file_inode.get_inode_num();

            cout << "img inode info: #inode: " << new_file_inode.get_inode_num() << endl;
            cout << "file length " << new_file_inode.get_file_size() << endl;
            cout << " #sector begin: " << new_file_inode.get_sec_beg() << endl;
            now_file_inode.read_inode_from_disk(ionde_num,my_cache);
            // 3. add new entry in current directory

            for(int i = 2; i < 15; i++)
            {
                if(cur_dir.dirs[i].inode_num == -1)
                {
                    cur_dir.dirs[i].init(name, ionde_num);
                    flag = true;
                    break;
                }
            }
        }
        else
        {
            now_file_inode.read_inode_from_disk(ionde_num,my_cache);
            now_file_inode._file_size=length;
        }
        if(flag)
        {
            cur_dir.write_back_to_disk(my_cache, cur_dir_node.get_sec_beg());// write back now
        }
        cerr<<now_file_inode._file_size<<"!!!!!!!file size"<<endl;
        // 4. store data into file system
        is.seekg(0, is.beg);
        char buffer[VALID_DATA_LENGTH];
        sector_file img_sectors[needed_block];
        int sec_numbers[needed_block];
        sec_numbers[0] = now_file_inode.get_sec_beg();
        for(int i = 0; i < needed_block - 1; i++)
        {
            is.read(buffer, VALID_DATA_LENGTH);
            sec_numbers[i+1] = sp.get_new_sec();
            memcpy(img_sectors[i].data, buffer, VALID_DATA_LENGTH);
            img_sectors[i].next = sec_numbers[i+1];
            cout << "#next data sector:" << img_sectors[i].next << endl;
        }
        if(left == 0)
        {
            is.read(buffer, VALID_DATA_LENGTH);
            memcpy(img_sectors[needed_block - 1].data, buffer, VALID_DATA_LENGTH);
            img_sectors[needed_block - 1].next = -1;
        }
        else
        {
            is.read(buffer, left);
            memcpy(img_sectors[needed_block - 1].data, buffer, left);
            img_sectors[needed_block - 1].next = -1;
        }

        cout << "File pointer location" << is.tellg() << endl;
        cout << "file sectors info" << endl;
        cout << now_file_inode.get_sec_beg();
        for(int i = 0; i <=needed_block; i++)
        {
//            cout << " -> " << img_sectors[i];
        }
        cout << endl;
        for(int i = 0; i <needed_block; i++)
        {
            img_sectors[i].write_back_to_disk(my_cache, sec_numbers[i]);
        }
        now_file_inode.write_inode_back_to_disk(my_cache);
        is.close();
    }

}

bool myFS::move_out(int inode_num)
{

    if(inode_num == -1)
    {
        cerr << "file do not exist" << endl;
        return false;
    }
    Inode file_node;
    file_node.read_inode_from_disk(inode_num, my_cache);

    cout << "file info: #inode " << file_node.get_inode_num() << endl;
    cout << "file length: " << file_node.get_file_size() << endl;
    cout << "sec number: " << file_node.get_sec_num() << endl;
    cout << "sec_begin: " << file_node.get_sec_beg() << endl << endl;

    // get data from my file system
    sector_file data_sec;
    data_sec.read_dir_from_disk(my_cache, file_node.get_sec_beg());


    fstream os(IMG,   fstream::in | fstream::out |ios::trunc);
    char buffer[VALID_DATA_LENGTH];
    int next_sec = -1, left = file_node.get_file_size() % VALID_DATA_LENGTH;
    if(os)
    {
        for(int i = 0; i < file_node.get_sec_num() ; i++)
        {
            if(i != file_node.get_sec_num() - 1 || left == 0)
            {
                next_sec = data_sec.next;
                memcpy(buffer, data_sec.data, VALID_DATA_LENGTH);
                os.write(buffer, VALID_DATA_LENGTH);
                data_sec.read_dir_from_disk(my_cache, next_sec);
            }
            else
            {
                memcpy(buffer, data_sec.data, left);
                os.write(buffer, left);
            }
            cout << "size of new file:" << os.tellg() << endl;
        }
        os.close();
    }
    return true;
}

void myFS::cat(vector<string> args)
{
    ops_at_least(1);
    string file_name=args[1];
    const char *name = file_name.c_str();
    int inode_num=is_existed_file(file_name);
    if(inode_num == -1)
    {
        cerr << "file do not exist" << endl;
        return;
    }
    Inode file_node;
    file_node.read_inode_from_disk(inode_num, my_cache);
    if((bool)file_node._is_file== false)
    {
        cerr << "can not cat dir" << endl;
        return;
    }

    cout << "file info: #inode " << file_node.get_inode_num() << endl;
    cout << "file length: " << file_node.get_file_size() << endl;
    cout << "sec number: " << file_node.get_sec_num() << endl;
    cout << "sec_begin: " << file_node.get_sec_beg() << endl << endl;

    sector_file data_sec;
    data_sec.read_dir_from_disk(my_cache, file_node.get_sec_beg());

    char buffer[VALID_DATA_LENGTH];
    int next_sec = -1, left = file_node.get_file_size() % VALID_DATA_LENGTH;
    string out_str;
    for(int i = 0; i < file_node.get_sec_num() ; i++)
    {
        if(i != file_node.get_sec_num() - 1 || left == 0)
        {
            next_sec = data_sec.next;
            memcpy(buffer, data_sec.data, VALID_DATA_LENGTH);
            out_str+=buffer;
            data_sec.read_dir_from_disk(my_cache, next_sec);
        }
        else
        {
            memcpy(buffer, data_sec.data, left);
            out_str+=buffer;
        }

    }
    cout<<out_str.substr(0,file_node.get_file_size())<<endl;
    return;
}


string myFS::getpwd(vector<string> args)
{

    sector_dir back_dir=cur_dir;
    Inode back_inode=cur_dir_node;
    string path;
    string dir_name;
    if(cur_dir.isroot())
    {
        path="/";
//        cout<<path;
        return path;
    }
    path=cur_dir.dir_name;
    dir_name=cur_dir.dir_name;
    sector_dir* now=&cur_dir;
    while(!now->isroot())
    {
        int dir_inode_num=cur_dir.dirs[1].inode_num;
        cur_dir_node.read_inode_from_disk(dir_inode_num, my_cache);
        cur_dir.read_dir_from_disk(my_cache, cur_dir_node.get_sec_beg());
        dir_name=cur_dir.dir_name;
        path=dir_name+"/"+path;
        now=&cur_dir;
    }
//    cout<<path<<endl;
    cur_dir_node=back_inode;
    cur_dir=back_dir;
    return path;
}

void myFS::printpwd(vector<string> args)
{
    ops_less_than(0);
    string pwd;
    pwd=getpwd(args);
    cout<<"pwd!!!!"<<pwd<<endl;
}

// format
bool myFS::format_file_system()
{
    sp.init();
    sp.format_disk();
    Inode root_node(sp.get_new_inode(), false, 0, sp.get_new_sec());
    Inode bin_node(sp.get_new_inode(), false, 0, sp.get_new_sec());
    Inode etc_node(sp.get_new_inode(), false, 0, sp.get_new_sec());
    Inode home_node(sp.get_new_inode(), false, 0, sp.get_new_sec());
    Inode dev_node(sp.get_new_inode(), false, 0, sp.get_new_sec());
    Inode tangrui_node(sp.get_new_inode(), false, 0, sp.get_new_sec());


    root_node.write_inode_back_to_disk(my_cache);
    bin_node.write_inode_back_to_disk(my_cache);
    etc_node.write_inode_back_to_disk(my_cache);
    home_node.write_inode_back_to_disk(my_cache);
    dev_node.write_inode_back_to_disk(my_cache);
    tangrui_node.write_inode_back_to_disk(my_cache);


    sector_dir root_sec_dir;
    strcpy(root_sec_dir.dir_name,"root");
    root_sec_dir.dirs[0].init(".", 0);
    root_sec_dir.dirs[1].init("..", 0);
    root_sec_dir.dirs[2].init("bin", bin_node.get_inode_num());
    root_sec_dir.dirs[3].init("etc", etc_node.get_inode_num());
    root_sec_dir.dirs[4].init("home", home_node.get_inode_num());
    root_sec_dir.dirs[5].init("dev", dev_node.get_inode_num());

    sector_dir bin_sec_dir;
    strcpy(bin_sec_dir.dir_name,"bin");
    bin_sec_dir.dirs[0].init(".", bin_node.get_inode_num());
    bin_sec_dir.dirs[1].init("..", root_node.get_inode_num());

    sector_dir etc_sec_dir;
    strcpy(etc_sec_dir.dir_name,"etc");
    etc_sec_dir.dirs[0].init(".", etc_node.get_inode_num());
    etc_sec_dir.dirs[1].init("..", root_node.get_inode_num());


    sector_dir dev_sec_dir;
    strcpy(dev_sec_dir.dir_name,"dev");
    dev_sec_dir.dirs[0].init(".",  dev_node.get_inode_num());
    dev_sec_dir.dirs[1].init("..", root_node.get_inode_num());


    root_sec_dir.write_back_to_disk(my_cache, root_node.get_sec_beg());
    bin_sec_dir.write_back_to_disk(my_cache, bin_node.get_sec_beg());
    etc_sec_dir.write_back_to_disk(my_cache, etc_node.get_sec_beg());
//    home_sec_dir.write_back_to_disk(my_cache, home_node.get_sec_beg());
    dev_sec_dir.write_back_to_disk(my_cache, dev_node.get_sec_beg());

    cur_dir.read_dir_from_disk(my_cache,root_node.get_sec_beg());
    cur_dir_node.read_inode_from_disk(0,my_cache);
//    cur_dir = root_sec_dir;
//    cur_dir_node = root_node;
    return true;
}

// D:mkdir dir
void myFS::mkdir(vector<string> args)
{
    string file_name=args[1];
    const char *name = file_name.c_str();
    // create inode
    Inode new_dir_inode(sp.get_new_inode(), false, 0, sp.get_new_sec());
    cout<<"mkdir inode num num"<<new_dir_inode.get_inode_num();
//    cout<<"mkdir inode num num"<<new_dir_inode.get_inode_num();
    // write back to disk
    new_dir_inode.write_inode_back_to_disk(my_cache);

    // mkdir entry
    sector_dir new_sec_dir;
    strcpy(new_sec_dir.dir_name,name);
    new_sec_dir.dirs[0].init(".", new_dir_inode.get_inode_num());
    new_sec_dir.dirs[1].init("..", cur_dir_node.get_inode_num());
    new_sec_dir.write_back_to_disk(my_cache, new_dir_inode.get_sec_beg());

    // add dir to parent dir
    int flag = false;
    for(int i = 2; i < 15; i++)
    {
        if(cur_dir.dirs[i].inode_num == -1)
        {
            cur_dir.dirs[i].init(name, new_dir_inode.get_inode_num());
            flag = true;
            break;
        }
    }
    if(flag)
    {
        cur_dir.write_back_to_disk(my_cache, cur_dir_node.get_sec_beg());
    }
    return;
}

// D: touch file
void myFS::touch(vector<string> args)
{
    string file_name=args[1];
    const char *name = file_name.c_str();
    cout << "touch file" << endl;
    // create inode
    Inode new_file_inode(sp.get_new_inode(), true, 1, sp.get_new_sec());
    new_file_inode._is_file= true;
    new_file_inode.write_inode_back_to_disk(my_cache);

//    sector_file new_sec_file;
//    new_sec_file.write_back_to_disk(my_cache, new_file_inode.get_sec_beg());

    // add inode to dir
    int flag = false;
    for(int i = 2; i < 15; i++)
    {
        if(cur_dir.dirs[i].inode_num == -1)
        {
            cur_dir.dirs[i].init(name, new_file_inode.get_inode_num());

            flag = true;
            break;
        }
    }
    if(flag)
    {
        cur_dir.write_back_to_disk(my_cache, cur_dir_node.get_sec_beg());
    }


    return;
}


void myFS::rmdir(vector<string> args)
{
    string file_name=args[1];
    const char *name = file_name.c_str();
    int del_inode_num = -1;
    for(int i = 0; i < 15; i++)
    {
        if(strncmp(name, cur_dir.dirs[i].name, strlen(name)) == 0)
        {
            del_inode_num = cur_dir.dirs[i].inode_num;
            cout << "inode num of the dir is ：" << del_inode_num << endl;
            break;
        }
    }
    if(del_inode_num == -1)
    {
        cerr << "dir not exist" << endl;
    }


    Inode del_node;
    del_node.read_inode_from_disk(del_inode_num, my_cache);


    del_inode(del_node, cur_dir);
    cur_dir.write_back_to_disk(my_cache, cur_dir_node.get_sec_beg());

}

int myFS::is_existed_file(string file_name)
{
    const char *name = file_name.c_str();
    for(int i = 0; i < 15; i++)
    {
        if(strncmp(name, cur_dir.dirs[i].name, strlen(name)) == 0)
        {
            int inode_num = cur_dir.dirs[i].inode_num;
            return inode_num;

        }
    }
    return -1;
}

int myFS::get_dir_index(int inode_num)
{
    for(int i = 0; i < 15; i++)
    {
        if(inode_num==cur_dir.dirs[i].inode_num)
        {
            return i;
        }
    }
    return -1;
}


void myFS::vim(vector<string> args)
{
    string file_name=args[1];
    const char *name = file_name.c_str();
    int inode_num= is_existed_file(file_name);
    if(inode_num!=-1)
    {
        Inode file_node;
        file_node.read_inode_from_disk(inode_num, my_cache);
        if(!file_node._is_file)
        {
            cerr << "can not cat dir" << endl;
            return;
        }
        move_out(inode_num);
    }

//    string tstr="vim /tmp/myfs_temp";
    const char *vim_cmd="vim /tmp/myfs_temp";
    int return_val=system(vim_cmd);
    cout<<return_val<<"return_val"<<endl;
    if(return_val==0)
    {
        cout<<"move_in";
        move_in(inode_num,file_name);
    }
    return;
}


void myFS::cp(vector<string> args)
{
    string src_str=args[1];
    const char *src = src_str.c_str();
    string dest_str=args[1];
    const char *dest = src_str.c_str();
    int src_inode_num=is_existed_file(src_str);
    int dest_inode_num=is_existed_file(src_str);
    if(src_inode_num=-1||dest_inode_num==-1)
    {
        cerr<<"file not exist!";
    }
    Inode src_inode;
    Inode dest_inode;
    src_inode.read_inode_from_disk(src_inode_num,my_cache);
    dest_inode.read_inode_from_disk(dest_inode_num,my_cache);
    int src_index=get_dir_index(src_inode_num);

}


void myFS::cd(vector<string> args)
{
    string file_name=args[1];
    const char *name = file_name.c_str();
    // get subdir inode
    int dir_inode_num = -1;
    for(int i = 0; i < 15; i++)
    {
        if(strncmp(name, cur_dir.dirs[i].name, strlen(name)) == 0)
        {
            dir_inode_num = cur_dir.dirs[i].inode_num;
            cout << "inode num:" << dir_inode_num << endl;
            break;
        }
    }
    if(dir_inode_num == -1)
    {
        cerr << "cd: no such file or directory:" <<file_name << endl;
        return;
    }

    // find inode by inode num
    cur_dir_node.read_inode_from_disk(dir_inode_num, my_cache);

    // read info by inode info
    cur_dir.read_dir_from_disk(my_cache, cur_dir_node.get_sec_beg());
}
