#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "myfs.hpp"
//
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
//
//#include <boost/archive/binary_iarchive.hpp>
//#include <boost/archive/binary_oarchive.hpp>

//using namespace boost;

using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::istringstream;
using std::make_shared;
using std::shared_ptr;
using std::string;
using std::vector;

string PRMPT = "sh> ";
const uint DISKSIZE = 100000000;
const uint BLOCKSIZE = 1024;
const uint DIRECTBLOCKS = 100;

//int test_fs(const string filename)
//{
//    myFS myfs(filename, DISKSIZE, BLOCKSIZE, DIRECTBLOCKS);
//
//    myfs.mkdir({"mkdir", "dir-2"});
//    myfs.mkdir({"mkdir", "dir-2/dir-b"});
//    myfs.mkdir({"mkdir", "dir-2/dir-b/dir-deep"});
//    myfs.open({"open", "somefile", "w"});
//    myfs.open({"open", "somefile2", "w"});
//    myfs.write({"write", "0", "hi there buddy"});
//    myfs.close({"close", "0"});
//    myfs.open({"open", "somefile", "r"});
//    myfs.seek({"seek", "2", "3"});
//    myfs.read({"read", "2", "5"});
//    myfs.ls({"ls"});
//    myfs.close({"close", "1"});
//    myfs.close({"close", "2"});
//    myfs.cat({"cat", "somefile"});
//    myfs.tree({"tree"});
//    myfs.import({"import", "exampleFile.txt", "ex.txt"});
//    myfs.cat({"cat", "somefile"});
//    myfs.cat({"cat", "ex.txt"});
//    myfs.link({"link", "ex.txt", "/dir-2/dir-b/linked"});
//    myfs.cat({"cat", "/dir-2/dir-b/linked"});
//    myfs.cp({"cp", "ex.txt", "newEx.txt"});
//    //myfs.unlink({"unlink", "ex.txt"});
//    //myfs.FS_export({"export", "dir-2/dir-b/linked", "newExFile.txt"});
//    myfs.tree({"tree"});
//    myfs.stat({"stat", "somefile", "somefile2", "dir-2/dir-b/linked"});
//    //myfs.unlink({"unlink", "dir-2/dir-b/linked"});
//    //myfs.rmdir({"rmdir", "dir-2/dir-b/dir-deep", "dir-2/dir-b", "dir-2"});
//    myfs.tree({"tree"});
//    myfs.mkdir({"mkdir", "ant"});
//    myfs.cd({"cd", "ant"});
//    myfs.printwd({"pwd"});
//    myfs.tree({"tree"});
//    return 0;
//}

void repl(string filename)
{
    myFS *fs = new myFS(filename);
    // test
    fs->mkdir({"mkdir", "dir-2/dir-b"});
    fs->mkdir({"mkdir", "dir-2/dir-b/dir-deep"});
    fs->open({"open", "somefile", "w"});
    fs->open({"open", "somefile2", "w"});
    fs->write({"write", "0", "hi there buddy"});
    fs->close({"close", "0"});
    fs->open({"open", "somefile", "r"});
    fs->seek({"seek", "2", "3"});
    fs->read({"read", "2", "5"});
    fs->ls({"ls"});
    fs->close({"close", "1"});
    fs->close({"close", "2"});
    fs->cat({"cat", "somefile"});
    fs->tree({"tree"});
    fs->import({"import", "exampleFile.txt", "ex.txt"});
    fs->cat({"cat", "somefile"});
    fs->cat({"cat", "ex.txt"});
    fs->link({"link", "ex.txt", "/dir-2/dir-b/linked"});
    fs->cat({"cat", "/dir-2/dir-b/linked"});
    fs->cp({"cp", "ex.txt", "newEx.txt"});

    string cmd;
    vector<string> args;
    string token;
    PRMPT=fs->getpwd(args);
    // cout<<fs->getpwd(args);
    PRMPT+=">";
    cout << PRMPT;
    while (getline(cin, cmd)) 
    {
        PRMPT=fs->getpwd(args);
        PRMPT+=">";
        args.clear();
        istringstream iss(cmd);
        while (iss >> token) { args.push_back(token); }
        if (args.size() == 0) 
        {
            cout << PRMPT;
            continue;
        }

        if (args[0] == "mkfs") 
        {
            if (args.size() == 1) 
            {
                delete(fs);
                fs = new myFS(filename);
            } else 
            {
                cerr << "mkfs: too many operands" << endl;
            }
        } 
        else if (args[0] == "open") 
        {
            fs->open(args);
        } 
        else if (args[0] == "read") 
        {
            fs->read(args);
        } 
        else if (args[0] == "write") 
        {
            if(args.size() >= 3) 
            {
                auto start = cmd.find("\"");
                auto end = cmd.find("\"", start+1);
                if (start != string::npos && end != string::npos) 
                {
                    string w_str = cmd.substr(start+1, end-start-1);
                    auto rn = cmd.find_first_not_of(" \t",end+1);
                    if (rn != string::npos) 
                    {
                        args = {args[0], args[1], w_str, cmd.substr(rn)};
                    } 
                    else 
                    {
                        args = {args[0], args[1], w_str};
                    }
                }
            } 
            else 
            {
              args = {"write"};
            }
            fs->write(args);
        } 
        else if (args[0] == "seek")
        {
            fs->seek(args);
        } 
        else if (args[0] == "close")
        {
            fs->close(args);
        } 
        else if (args[0] == "mkdir")
        {
            fs->mkdir(args);
        } 
        else if (args[0] == "rmdir") 
        {
            fs->rmdir(args);
        } 
        else if (args[0] == "cd") 
        {
            fs->cd(args);
        } 
        else if (args[0] == "link") 
        {
            fs->link(args);
        } 
        else if (args[0] == "unlink") 
        {
            fs->unlink(args);
        } 
        else if (args[0] == "stat") 
        {
            fs->stat(args);
        } 
        else if (args[0] == "ls") 
        {
            fs->ls(args);
        } 
        else if (args[0] == "cat") 
        {
            fs->cat(args);
        } 
        else if (args[0] == "cp") 
        {
            fs->cp(args);
        } 
        else if (args[0] == "tree")
        {
            fs->tree(args);
        } 
        else if (args[0] == "import")
        {
            fs->import(args);
        } 
        else if (args[0] == "export") 
        {
            fs->FS_export(args);
        } 
        else if (args[0] == "exit") 
        {
            break;
        } 
        else if (args[0] == "pwd") 
        {
            fs->printwd(args);
        } 
        else 
        {
            cout << "unknown command: " << args[0] << endl;
        }
        PRMPT=fs->getpwd(args);
        PRMPT+=">";     
        cout << PRMPT;
    }
    delete(fs);
    return;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cerr << "usage: " << argv[0] << " filename" << endl;
        return 1;
    }
#ifdef DEBUG
    test_fs(string(argv[1]));
#else
    repl(argv[1]);
#endif
    return 0;
}


