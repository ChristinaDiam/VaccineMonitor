#ifndef CITIZEN
#define CITIZEN

#include <iostream>
#include <string>
#include<cstdlib>

#include"list.h"

using namespace std;


class cvirus
{
private:
    string virusname;
    bool yesno;
    string date;
public:
    cvirus(string vname, string yn, string date);
    ~cvirus();

    void printvirus();

    string getvname();
    bool getyesno();
    string getdate();

    void setyesno(bool yn);
    void setdate(string date);
};


class citizen
{
private:
    string ID;
    string f_name;
    string l_name;
    string country;
    int age;
    list *virus;

public:
    citizen(string ID, string fname, string lname, string country, string age, string vname, string yn, string date);
    ~citizen();
    void print();
    string getID();
    string getf_name();
    string getl_name();
    string getcountry();
    int getage();
    list* getvirusinfo();
};


#endif //CITIZEN

