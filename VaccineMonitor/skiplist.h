#ifndef SKIPLIST
#define SKIPLIST

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

#include "list.h"
#include "citizen.h"

using namespace std;

class skiplist
{
private:
    
    list *levels;
    string virusname;   //virus name
    int maxlevel;       //number of levels in list
    int counter;        //elements in list

    void printlevel(int l);

public:
    skiplist(string vname);
    ~skiplist();

    string getvname();
    int getmaxlevel();
    int getcounter();

    void insert(citizen *c);
    citizen *search(string ID);
    void remove(citizen *c);

    void print();
    void printfirstlevel();
};





#endif //SKIPLIST