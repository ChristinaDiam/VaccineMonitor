#include<iostream>
#include<cstdio>
#include<string>
#include<cstring>

#include "citizen.h"
#include "helpmain.h"


using namespace std;


void visitcitizen(void *item){

    citizen *c=(citizen*)item;
    c->print();
    cout<<"_____________________"<<endl;
}


void visitskip(void *item){

    skiplist *s=(skiplist*)item;
    s->print();
    cout<<"_____________________"<<endl<<endl;
}


void printInt(void *i)
{
  cout << *((int *) i) << " ";
}



int main(int argc, char *argv[]){


    int bloomsize;
    char fname[20];

    if(argc==5){

        if((!strcmp(argv[1],"-c")) && (!strcmp(argv[3],"-b"))){

            bloomsize=atoi(argv[4]);

            strcpy(fname,argv[2]);

        }else if((!strcmp(argv[1],"-b")) && (!strcmp(argv[3],"-c"))){

            bloomsize=atoi(argv[2]);

            strcpy(fname,argv[4]);

        }else{
            cout<<endl<<"Wrong input."<<endl;
            return 0;
        }

    }else{
        cout<<endl<<"Wrong input."<<endl;
        return 0;
    }


    string temp[8];                         //template array for every record

    list *blooms=new list();                //list of bloom filters

    list *citizens=new list();              //list of citizens

    list *skipvaccinated=new list();        //list of skiplists (vaccinated citizens)

    list *skipnotvaccinated=new list();     //list of skiplists (not vaccinated citizens)


    // we open the file and read it

    FILE *fp;

    fp = fopen(fname, "r");        //open file
    if(fp == NULL)
    {
        printf("File could not be opened.\n");
        exit(EXIT_FAILURE);
    }  

    char id[20],fn[20],ln[20],country[20],age[4],virName[20],yes_no[4],date[15];

    while(fscanf(fp, "%s%s%s%s%s%s%s", id, fn, ln, country, age, virName, yes_no)!=EOF){


        temp[0]=string(id);
        temp[1]=string(fn);
        temp[2]=string(ln);
        temp[3]=string(country);
        temp[4]=string(age);
        temp[5]=string(virName);
        temp[6]=string(yes_no);

        char ch=fgetc(fp);

        if(ch==' '){
            fscanf(fp, "%s",date);
            temp[7]=string(date);
        }else{
            temp[7]="";
        }

        if((temp[6]=="NO" && temp[7]=="") || (temp[6]=="YES" && temp[7]!="")){

            if(temp[7]==""){

                temp[7]="-";
            }

            insertCitizenRecord(temp[0],temp[1],temp[2],temp[3],temp[4],temp[5],temp[6],
            temp[7],skipvaccinated,skipnotvaccinated,citizens,blooms,bloomsize);

        }else{

            //wrong citizen record (wrong information about date and yes/no)
            cout<<"ERROR IN RECORD "<<temp[0]<<endl;
        }
    }

    //print list of citizens, skip-vaccinated, skip-not vaccinated

    //citizens->printlist(visitcitizen);  


    //cout<<endl<<"SKIPLIST"<<endl;

    //cout<<endl<<"Vaccinated citizens"<<endl<<endl;

    //skipvaccinated->printlist(visitskip);

    //cout<<endl<<"Not vaccinated citizens"<<endl<<endl;

    //skipnotvaccinated->printlist(visitskip);


    fclose(fp);

    printoptions();

    int input=0;

    //reads user's input until "/exit" given
    while (input!=1){

        input=readinput(citizens,blooms,skipvaccinated,skipnotvaccinated,bloomsize);
        cout<<endl;
    }

    return 0;
}