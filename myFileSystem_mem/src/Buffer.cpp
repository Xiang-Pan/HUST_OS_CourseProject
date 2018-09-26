//
// Created by wings on 8/15/18.
//

#include "Buffer.hpp"

// open disk
Buffer::Buffer()
{
    disk_file.open(DISK, std::fstream::in | std::fstream::out | std::fstream::binary);
    if(disk_file.is_open())
        cout << "File have opened" << endl;
    else
    {
        // cerr << "FILE NOT OPEN" << endl;
    }  
}

Buffer::~Buffer()
{
    disk_file.close();
}

bool Buffer::write_disk(const BufferNode& node)
{
    assert(node.block_num >= 0 && node.block_num < MAX_SEC);
    int i;
    i = has_block(node.block_num);
    if(i >= 0)
    {
        cout << "write disk: buffer have this sec" << endl;
        cache[i].update(node);
        return true;
    }

    i = is_full(); // get the lowest pirority
    if(i >= 0)
    {
        cout << "write disk: buffer full，replace " << i << " buffer " << endl;
        real_disk_write(cache[i]);
        cache.erase(cache.begin() + i);
    }
    cache.push_back(node);
    cout << "write disk: already writes" << endl;
    return true;
}



bool Buffer::read_disk(int block_num, BufferNode& node)
{
    assert(block_num >= 0 && block_num < MAX_SEC);

    int i;
    i = has_block(block_num);

    if(i >= 0)
    {
        cout << "read disk: buffer existed" << endl;
        node.update(cache[i]);
        return true;
    }

    i = is_full();
    if(i >= 0)
    {
        cout << "write disk: buffer full，replace " << i << " buffer " << endl;
        // 写回
        real_disk_write(cache[i]);
        cache.erase(cache.begin() + i);
        real_disk_read(block_num, node);
        node.init(block_num);
        cache.push_back(node);
    }
    else
    {
        real_disk_read(block_num, node);
        node.init(block_num);
        cache.push_back(node);
        cout << "read disk: buffer not full ，load " << endl;
    }
    return true;
}


bool Buffer::real_disk_write(const BufferNode& node)
{
    assert(node.block_num >= 0 && node.block_num < MAX_SEC);
    cout << "read disk write write back " << node.block_num << " sec " << endl;
    disk_file.seekg(node.block_num * SEC_SIZE, disk_file.beg);
    disk_file.write(node.buffer, SEC_SIZE);
    return true;
}

bool Buffer::real_disk_read(int block_num, BufferNode& node){
    assert(block_num >= 0 && block_num < MAX_SEC);
    cout << "real disk read read " << block_num << " sec" << endl;
    disk_file.seekg(block_num * SEC_SIZE, disk_file.beg);
    disk_file.read(node.buffer, SEC_SIZE);
    node.buffer[SEC_SIZE] = '\0';
    node.block_num = block_num;
    return true;
}


//D: travel to find if block existe
int Buffer::has_block(int block_number)
{
    for(int i = 0; i < cache.size(); i++)
    {
        if(cache[i].block_num == block_number)
            return i;
    }
    return -1;
}


int Buffer::is_full()
{
    if(cache.size() == 15){
        int min = 9999, min_i = 0;
        for(int i = 0; i < cache.size(); i++)
        {
            if(cache[i].pri < min)
            {
                min = cache[i].pri;
                min_i = i;
            }
        }
        return min_i;
    }
    else
        {
        return -1;
    }
}
