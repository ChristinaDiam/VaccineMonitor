#include <iostream>

#include"list.h"

using namespace std;



//LISTNODE FUNCTIONS


//listnode constructor
listnode::listnode(void *data)
{
    this->value=data;
    this->next=NULL;
}


//listnode destructor
listnode::~listnode()
{
}



//list set next (sets the pointer to a next node)

void listnode::setnext(listnode *next){

    this->next=next;
}


//list get next

listnode* listnode::getnext(){

    return this->next;
}


//list set value (sets a value to the listnode)

void listnode::setvalue(void *newvalue){

    this->value=newvalue;
}


//list get value

void* listnode::getvalue(){

    return this->value;
}


//listnode print (prints the listnode)

void listnode::printnodelist(void (*visit)(void*)){

    visit(this->value); 
}



//LIST FUNCTIONS


//list instert (inserts a new node)

void list::listinsert(void *value){

    listnode *lnode=new listnode(value);

    lnode->setnext(head);
    head=lnode;

    this->counter++;

}


//print list

void list::printlist(void (*visit)(void*)){
    
    listnode *temp=head;

    while(temp!=NULL){
        temp->printnodelist(visit);
        temp=temp->getnext();
    }

}


//get head of the list

listnode* list::gethead(){

    return this->head;
}


void* list::search(void *item,int (*compare)(void*,void*)){

    listnode *tmp=head;

    while(tmp!=NULL){

        void *value=tmp->getvalue();

        if(compare(value,item)==0){

            return value;
        }

        tmp=tmp->getnext();
    }

    return NULL;
}


int list::getcounter(){

    return this->counter;
}


void list::sethead(listnode *head){

    this->head=head;
}


list::~list(){

    listnode *lnode;
    lnode=this->head;

    while(lnode!=NULL){

        this->head=lnode->getnext();

        delete lnode;
        lnode=this->head;
    }

    this->counter=0;
}

