#include "skiplist.h"

using namespace std;




skiplist::skiplist(string vname)
{
    srand((unsigned int) time(NULL));

    this->levels=new list();
    this->virusname=vname;
    this->maxlevel=0;
    this->counter=0;
}


skiplist::~skiplist()
{
    delete levels;
}


string skiplist::getvname(){

    return this->virusname;
}


int skiplist::getmaxlevel(){

    return this->maxlevel;
}


int skiplist::getcounter(){

    return this->counter;
}


//prints the level given
void skiplist::printlevel(int l){

    listnode *tmp=levels->gethead();

    int i;
    for(i=0; i<l-1; i++){

        tmp=tmp->getnext();
    }

    list *level=(list*)tmp->getvalue();

    tmp=level->gethead()->getnext();

    while(tmp){

        listnode *lnode;
        lnode=(listnode*)tmp->getvalue();

        citizen *c;
        c=(citizen*)lnode->getvalue();

        cout<<c->getID();

        if(tmp->getnext()!=NULL){

            cout<<" -> ";
        }

        tmp=tmp->getnext();
    }

    cout<<endl;
}


//prints the list
void skiplist::print(){

    int i;
    for(i=this->maxlevel; i>=1; i--){

        cout<<"L"<<i<<": ";
        printlevel(i);        
    }
}


//prints only the first level of the skiplist
void skiplist::printfirstlevel(){

    listnode *tmp=levels->gethead();

    list *level=(list*)tmp->getvalue();

    tmp=level->gethead()->getnext();

    while(tmp){

        listnode *lnode;
        lnode=(listnode*)tmp->getvalue();

        citizen *c;
        c=(citizen*)lnode->getvalue();

        cout<<c->getID()<<" ";
        cout<<c->getf_name()<<" ";
        cout<<c->getl_name()<<" ";
        cout<<c->getcountry()<<" ";
        cout<<c->getage()<<endl;

        tmp=tmp->getnext();
    }

    cout<<endl;
}


//inserts an item (citizen) in list 
void skiplist::insert(citizen *c){

    //we insert the first item in skiplist
    if(this->counter==0){

        list *firstlevel=new list();
        levels->listinsert(firstlevel);

        firstlevel->listinsert(new listnode(c));

        //dummy node
        firstlevel->listinsert(new listnode(NULL));

        this->counter++;
        this->maxlevel=1;

        return;
    }


    int level=this->maxlevel;

    //array to mark nodes
    listnode *records[level];

    int i;
    for(i=0; i<level; i++){

        records[i]=NULL;
    }


    //we mark the path
    while(level>0){

        listnode *node;
        node=levels->gethead();

        //we reach the current level
        for(i=0; i<level-1; i++){

            node=node->getnext();
        }

        list *levellist;
        levellist=(list*)node->getvalue();

        //we are on top of the list
        if(level==this->maxlevel){

            node=levellist->gethead();

        }else{

            node=records[level];

            //we need the node below the marked node
            node=(listnode*)node->getvalue();
            node=node->getnext();
            
        }
        

        listnode *next;
        next=node->getnext();

        while(next!=NULL)
        {
            citizen *nodecitizen;

            //we take the inside node to compare the value(ID) with citizen's ID
            listnode *inside;
            inside=(listnode*)next->getvalue();

            nodecitizen=(citizen*)inside->getvalue();

            if(nodecitizen->getID()>=c->getID()){

                break;
            }

            node=next;
            next=next->getnext();
        }

        records[level-1]=node;   //marked
        level--;
    }


    //we insert the node at the first level (level 0)

    //we create a new node with the citizen
    listnode *node=new listnode(c);         //inside node
    listnode *lnode=new listnode(node);     //exterior node


    //we have to put the new node after the marked one
    
    lnode->setnext(records[0]->getnext());
    records[0]->setnext(lnode);
    
    this->counter++;


    //now we will start growing the node

    //the number of copies of the node
    int maxcopies=(int)log2(this->counter)-1;

    int currentcopy=1;
    
    while(maxcopies>0){

        //flip a coin
        int experiment=rand()%2;

        //experiment is successfull, we need to copy the node
        if(experiment==1){

            //if there's no list above (we need to create a new level)
            if(currentcopy==this->maxlevel){

                this->maxlevel++;

                list *newlist=new list();

                listnode *temp;
                temp=this->levels->gethead();

                //we reach at the end of the list
                while(temp->getnext()!=NULL){

                    temp=temp->getnext();
                }

                listnode *li=new listnode(newlist);
                temp->setnext(li);

                newlist->listinsert(new listnode(c));

                //dummy node
                newlist->listinsert(new listnode(NULL));

                node=newlist->gethead();
                listnode *inside=(listnode*)node->getvalue();

                list *l=(list*)temp->getvalue();
                listnode *ln=l->gethead();
                inside->setnext(ln);

                node=newlist->gethead()->getnext();
                inside=(listnode*)node->getvalue();
                inside->setnext(lnode);

                currentcopy++;

                lnode=node;


            //if there's already a list above (we don't need to create a new level)
            }else{

                listnode *ll=new listnode(c);       //inside node
                listnode *ell=new listnode(ll);     //exterior node

                ll->setnext(lnode);
                ell->setnext(records[currentcopy]->getnext());
                records[currentcopy]->setnext(ell);

                currentcopy++;

                lnode=ell;
            }

        
        //experiment unsuccessfull, we don't copy the node
        }else{

            break;
        }

        maxcopies--;
    }
}


//search for an item (citizen) with "ID" in list
citizen* skiplist::search(string ID){

    //if counter=0 there's no item in list
    if(this->counter==0){

        return NULL;
    }


    int level=this->maxlevel;
    int i;

    listnode *n;

    //we search in list
    while(level>0){

        listnode *node;
        node=levels->gethead();

        //we reach the current level
        for(i=0; i<level-1; i++){

            node=node->getnext();
        }

        list *levellist;
        levellist=(list*)node->getvalue();

        //we are on top of the list
        if(level==this->maxlevel){

            node=levellist->gethead();

        }else{

            node=n;

            //we need the node below
            node=(listnode*)node->getvalue();
            node=node->getnext();
            
        }


        listnode *next;
        next=node->getnext();

        while(next!=NULL)
        {
            citizen *nodecitizen;

            //we take the inside node to compare the value(ID) with citizen's ID
            listnode *inside;
            inside=(listnode*)next->getvalue();

            nodecitizen=(citizen*)inside->getvalue();

            //item found, return citizen info
            if(nodecitizen->getID()==ID){

                return nodecitizen;
            }

            if(nodecitizen->getID()>ID){

                break;
            }

            node=next;
            next=next->getnext();
        }

        n=node;
        level--;
    }

    return NULL;
}


//remove an item (citizen) from list
void skiplist::remove(citizen *c){

    //if counter=0 list has no items
    if(this->counter==0){

        cout<<endl<<"Skiplist is empty."<<endl;

        return;
    }


    int level=this->maxlevel;

    //array to mark nodes
    listnode *records[level];

    int i;
    for(i=0; i<level; i++){

        records[i]=NULL;
    }

    int flag=0;

    //we mark the path
    while(level>0){

        listnode *node;
        node=levels->gethead();

        //we reach the current level
        for(i=0; i<level-1; i++){

            node=node->getnext();
        }

        list *levellist;
        levellist=(list*)node->getvalue();

        //we are on top of the list
        if(level==this->maxlevel){

            node=levellist->gethead();

        }else{

            node=records[level];

            //we need the node below the marked node
            node=(listnode*)node->getvalue();
            node=node->getnext();
            
        }
        

        listnode *next;
        next=node->getnext();

        while(next!=NULL)
        {
            citizen *nodecitizen;

            //we take the inside node to compare the value(ID) with citizen's ID
            listnode *inside;
            inside=(listnode*)next->getvalue();

            nodecitizen=(citizen*)inside->getvalue();

            //we found the node to remove
            if(nodecitizen->getID()==c->getID()){

                flag=level;
                break;
            }

            if(nodecitizen->getID()>c->getID()){

                break;
            }

            node=next;
            next=next->getnext();
        }

        records[level-1]=node;   //marked
        level--;
    }


    //we remove the higher node first
    while(flag!=0){

        listnode *removed=records[flag-1]->getnext();

        //first we remove the inside node
        listnode *inside;
        inside=(listnode*)removed->getvalue();

        delete inside;

        listnode *next=removed->getnext();

        delete removed;

        //we connect the nodes after the removal
        records[flag-1]->setnext(next);


        listnode *node,*prev;
        node=levels->gethead();
        prev=NULL;

        //we reach the current level
        for(i=0; i<level-1; i++){

            prev=node;
            node=node->getnext();
        }

        list *levellist;
        levellist=(list*)node->getvalue();

        //if marked is the dummy node and list is empty
        if((records[flag-1]==levellist->gethead()) && (next==NULL)){

            //first we remove the inside node
            inside=(listnode*)records[flag-1]->getvalue();

            delete inside;
            delete levellist;

            //we remove the first level
            if(prev==NULL){

                levels->sethead(NULL);

            }else{

                prev->setnext(NULL);
            }

            delete node;

            this->maxlevel--;
        }

        flag--;
    }
    
    this->counter--;
}