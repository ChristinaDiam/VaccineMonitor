#ifndef BLOOMFILTER
#define BLOOMFILTER

#include<iostream>

#include "hash.h"

using namespace std;


class bloomfilter
{    
private:
    int bloomsize;
    unsigned char *str;

public:
    bloomfilter(int bloomsize);
    ~bloomfilter();
    void insert(unsigned char *item);
    bool search(unsigned char *item);
};


class bloomvirus
{
private:
    bloomfilter *bloom;
    string virusname;
    
public:
    bloomvirus(string name, bloomfilter *bloom);
    ~bloomvirus();
    string getvname();
    bloomfilter* getbloom();
};


#endif //BLOOMFILTER