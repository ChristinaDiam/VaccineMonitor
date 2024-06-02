#include<cstdio>
#include<string>
#include<cstring>

#include "bloomfilter.h"


//BLOOMFILTER FUNCTIONS

bloomfilter::bloomfilter(int bloomsize)
{
    this->bloomsize=bloomsize;
    this->str=new unsigned char[bloomsize];

    int i;
    for(i=0; i<bloomsize; i++){

        str[i]=0;
    }
}


bloomfilter::~bloomfilter()
{
    delete[] str;
}


void bloomfilter::insert(unsigned char *item){

    unsigned long h1;
    unsigned long h2;
    unsigned long h3;

    h1=djb2(item);
    h2=sdbm(item);
    h3=hash_i(item,3);

    unsigned long h[3];
    h[0]=h1;
    h[1]=h2;
    h[2]=h3;

    int i;
    for(i=0; i<3; i++){

        h[i]=h[i]%(this->bloomsize*8);

        int character;
        int bit;

        character=h[i]/8;   //str index
        bit=h[i]%8;         //bit of str index

        char mask=1;
        int j;

        for(j=0; j<bit; j++){

            mask<<=1;
        }
        
        this->str[character]=this->str[character] | mask;
    }
}


bool bloomfilter::search(unsigned char *item){

    unsigned long h1;
    unsigned long h2;
    unsigned long h3;

    h1=djb2(item);
    h2=sdbm(item);
    h3=hash_i(item,3);

    unsigned long h[3];
    h[0]=h1;
    h[1]=h2;
    h[2]=h3;

    int i;
    for(i=0; i<3; i++){

        h[i]=h[i]%(this->bloomsize*8);

        int character;
        int bit;

        character=h[i]/8;   //str index
        bit=h[i]%8;         //bit of str index

        char mask=1;
        int j;

        for(j=0; j<bit; j++){

            mask<<=1;
        }

        if((this->str[character] & mask) == 0){
        
            return false; 
        }
    }

    return true;
}



//BLOOM VIRUS FUNCTIONS

bloomvirus::bloomvirus(string name, bloomfilter* bloom){

    this->virusname=name;
    this->bloom=bloom;
}


bloomvirus::~bloomvirus(){

    delete[] bloom;
}


string bloomvirus::getvname(){

    return this->virusname;
}


bloomfilter* bloomvirus::getbloom(){
    
    return this->bloom;
}