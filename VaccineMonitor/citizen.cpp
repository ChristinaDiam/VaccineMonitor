
#include"citizen.h"

using namespace std;


//VIRUS FUNCTIONS


void visitvirus(void *item){

    cvirus *v=(cvirus*)item;
    v->printvirus();
}


//virus constructor

cvirus::cvirus(string vname, string yn, string date){

    this->virusname=vname;

    if(yn=="YES"){
        this->yesno=true;
    }else{
        this->yesno=false;
    }

    this->date=date;
}

//virus destructor

cvirus::~cvirus(){

}


//print virus

void cvirus::printvirus(){

    cout<<endl<<"Virus name: "<<this->virusname<<endl;

    cout<<"Vaccinated: ";
    if(this->yesno==true){
        cout<<"YES"<<endl;
    }else{
        cout<<"NO"<<endl;
    }

    if(this->date!=""){
        cout<<"Date: "<<this->date<<endl;
    }
}


//get virus name

string cvirus::getvname(){

    return this->virusname;
}


//get virus yes/no

bool cvirus::getyesno(){

   return this->yesno;
}


//get date

string cvirus::getdate(){

    return this->date;
}


//set yes/no

void cvirus::setyesno(bool yn){

    this->yesno=yn;
}


//set date

void cvirus::setdate(string date){

    this->date=date;
}



//CITIZEN FUNCTIONS


//citizen constructor

citizen::citizen(string ID, string fname, string lname, string country, string age, string vname, string yn, string date)
{
    this->ID=ID;
    this->f_name=fname;
    this->l_name=lname;
    this->country=country;
    this->age=atoi(age.c_str());

    cvirus *v=new cvirus(vname,yn,date);

    this->virus=new list();
    this->virus->listinsert(v);
}


//citizen destructor

citizen::~citizen()
{
    listnode *lnode;
    lnode=this->virus->gethead();

    while(lnode!=NULL){

        cvirus *c;
        c=(cvirus*)lnode->getvalue();

        delete c;

        lnode=lnode->getnext();
    }

    delete this->virus;
}


//citizen print all

void citizen::print(){
 
    cout<<endl<<"ID: "<<this->ID<<endl;
    cout<<"First Name: "<<this->f_name<<endl;
    cout<<"Last Name: "<<this->l_name<<endl;
    cout<<"Country: "<<this->country<<endl;
    cout<<"Age: "<<this->age<<endl;

    this->virus->printlist(visitvirus);

    cout<<endl;
}


//get ID

string citizen::getID(){

    return this->ID;
}


//get first name

string citizen::getf_name(){

    return this->f_name;
}


//get last name

string citizen::getl_name(){

    return this->l_name;
}


//get age

int citizen::getage(){

    return this->age;
}


//get country

string citizen::getcountry(){

    return this->country;
}


//get virus information (virus name, yes/no, date) 
//returns a pointer to the virus list

list* citizen::getvirusinfo(){

    return this->virus;
}

