#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "myfs.hpp"
#include "myfs_macro.h"
#include "direntry.hpp"

using namespace std;


using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::istringstream;
// using std::make_shared;
// using std::shared_ptr;
using std::string;
using std::vector;

string PRMPT = "sh> ";


int main(int argc, char **argv)
{
    myFS *fs = new myFS();
    fs->run();
    return 0;
}


