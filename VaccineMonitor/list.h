#ifndef LIST
#define LIST


class listnode
{
private:
    void *value;
    listnode *next;
public:
    listnode(void *data);
    ~listnode();
    void setvalue(void *newvalue);
    void *getvalue();
    void setnext(listnode *next);
    listnode *getnext();
    void printnodelist(void (*visit)(void*));
};



class list{
    private:
    listnode *head;
    int counter;

    public:
    list(){
        this->head=NULL;
        this->counter=0;
    };

    ~list();
    void listinsert(void *value);
    void printlist(void (*visit)(void*));
    listnode *gethead();
    int getcounter();
    void *search(void *item,int (*compare)(void*,void*));
    void sethead(listnode *head);
};

#endif //LIST
