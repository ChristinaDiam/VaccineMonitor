#include"helpmain.h"

using namespace std;


int compareblooms(void* item1, void* item2){

    bloomvirus *bv1=(bloomvirus*) item1;
    bloomvirus *bv2=(bloomvirus*) item2;

    return (bv1->getvname()).compare(bv2->getvname());
}

int comparebloom(void* item1, void* item2){

    bloomvirus *bv1=(bloomvirus*) item1;
    bloomvirus *bv2=(bloomvirus*) item2;

    return (bv1->getvname()).compare(bv2->getvname());
}


int comparestrings(void* item1, void* item2){

    string s1=*((string *) item1);
    string s2=*((string *) item2);

    return (s1.compare(s2));
}



//checks if the record is valid (for duplicate records) 
int checkrecord(listnode *tmp,int flag,string ID, string fname, string lname, string country, string age, 
string vname, string yn, string date, list *blooms, int bloomsize, list *skipvaccinated, list *skipnotvaccinated){


    while(tmp!=NULL){

        citizen *cit=(citizen*)tmp->getvalue();

        //citizens with different ID are saved as a new citizen
        //citizens with same ID must have the same info but different virus info
        if(ID==cit->getID()){

            flag=1;

            //same citizen must have same first name
            if(fname==cit->getf_name()){

                //same citizen must have same last name
                if(lname==cit->getl_name()){

                    //same citizen must have same country
                    if(country==cit->getcountry()){

                        //same citizen must have same age
                        if(atoi(age.c_str())==cit->getage()){
                        
                            list *cv=cit->getvirusinfo();
                            listnode *cvir=cv->gethead();

                            int flag2=0;

                            while(cvir!=NULL){

                                cvirus *vir=(cvirus*)cvir->getvalue();

                                //if it's the same person we must have different viruses
                                if(vname==vir->getvname()){
                                    flag2=1;
                                    break;
                                }

                                cvir=cvir->getnext();
                            }

                            //we found a new virus
                            if(flag2==0){

                                cvirus *newvirus=new cvirus(vname, yn, date);
                                cv->listinsert(newvirus);

                                //if citizen is vaccinated we add the virus in bloomfilter list
                                if(yn=="YES"){
                                    
                                    bloomvirus *bv=new bloomvirus(vname,NULL);

                                    bloomvirus *search=(bloomvirus*)blooms->search(bv,compareblooms);

                                    if(search==NULL){

                                        bloomfilter *filter=new bloomfilter(bloomsize);

                                        char* s = new char [ID.length() + 1];
                                        strcpy(s, ID.c_str());

                                        filter->insert((unsigned char*)s);

                                        delete[] s;

                                        bloomvirus *virus=new bloomvirus(vname,filter);

                                        blooms->listinsert(virus);

                                    }else{

                                        char* s = new char [ID.length() + 1];
                                        strcpy(s, ID.c_str());

                                        search->getbloom()->insert((unsigned char*)s);

                                        delete[] s;
                                    } 

                                    delete bv;  

                                    
                                    //skilpist (vaccinated) insertion

                                    //checks if the virus already exists in list
                                    listnode *skiptmp=skipvaccinated->gethead();

                                    skiplist *found=NULL;

                                    while(skiptmp!=NULL){

                                        skiplist *sk=(skiplist*)skiptmp->getvalue();

                                        if(sk->getvname()==vname){

                                            found=sk;
                                            break;

                                        }else{

                                            skiptmp=skiptmp->getnext();
                                        }
                                    }

                                    //we found a new virus
                                    if(found==NULL){

                                        skiplist *skip=new skiplist(vname);

                                        skip->insert(cit);

                                        skipvaccinated->listinsert(skip);

                                    }else{

                                        found->insert(cit);
                                    }
                                    
                                //if citizen is not vaccinated we add the virus in skiplist for not vaccinated citizens
                                }else if(yn=="NO"){

                                    //skilpist (not vaccinated) insertion

                                    //checks if the virus already exists in list
                                    listnode *skipnottmp=skipnotvaccinated->gethead();
                                    
                                    skiplist *foundnot=NULL;

                                    while(skipnottmp!=NULL){

                                        skiplist *sknot=(skiplist*)skipnottmp->getvalue();

                                        if(sknot->getvname()==vname){

                                            foundnot=sknot;
                                            break;

                                        }else{

                                            skipnottmp=skipnottmp->getnext();
                                        }
                                    }

                                    //we found a new virus
                                    if(foundnot==NULL){

                                        skiplist *skipnot=new skiplist(vname);

                                        skipnot->insert(cit);

                                        skipnotvaccinated->listinsert(skipnot);

                                    }else{

                                        foundnot->insert(cit);
                                    }
                                }

                                break;

                            }else{

                                //wrong citizen record (duplicate record)
                                cout<<"ERROR IN RECORD "<<ID<<endl;
                                break;
                            }

                        }else{

                            //wrong citizen record (same person with different age)
                            cout<<"ERROR IN RECORD "<<ID<<endl;
                            break;
                        }

                    }else{

                        //wrong citizen record (same person with different country)
                        cout<<"ERROR IN RECORD "<<ID<<endl;
                        break;
                    }

                }else{

                    //wrong citizen record (same person with different last name)
                    cout<<"ERROR IN RECORD "<<ID<<endl;
                    break;
                }

            }else{

                //wrong citizen record (same person with different first name)
                cout<<"ERROR IN RECORD "<<ID<<endl;
                break;
            }

        }else{

            tmp=tmp->getnext();
        }
    }

    return flag;
}



void printoptions(){

    cout<<endl<<endl<<"Give input: "<<endl;

    cout<<"/vaccineStatusBloom citizenID virusName"<<endl;
    cout<<"/vaccineStatus citizenID virusName"<<endl;
    cout<<"/vaccineStatus citizenID"<<endl;
    cout<<"/populationStatus [country] virusName date1 date2"<<endl;
    cout<<"/popStatusByAge [country] virusName date1 date2"<<endl;
    cout<<"/insertCitizenRecord citizenID firstName lastName country age virusName YES/NO [date]"<<endl;
    cout<<"/vaccinateNow citizenID firstName lastName country age virusName"<<endl;
    cout<<"/list-nonVaccinated-Persons virusName"<<endl;
    cout<<"/exit"<<endl<<endl;
}



void vaccineStatusBloom(list *blooms, string ID, string vname){

    bloomfilter *bf;
    bf=NULL;

    listnode *lnode;
    lnode=blooms->gethead();

    while(lnode!=NULL){

        bloomvirus *bv;
        bv=(bloomvirus*)lnode->getvalue();

        if(bv->getvname()==vname){

            bf=bv->getbloom();
        }

        lnode=lnode->getnext();
    }

    bool tf;

    if(bf!=NULL){

        char* s = new char [ID.length() + 1];
        strcpy(s, ID.c_str());

        tf=bf->search((unsigned char*)s);

        if(tf==true){

            cout<<endl<<"MAYBE"<<endl;
            return;

        }else if(tf==false){

            cout<<endl<<"NOT VACCINATED"<<endl;
            return;

        }
    }

    cout<<endl<<"NOT FOUND"<<endl;
}



void vaccineStatus(list *skipvaccinated, list *skipnotvaccinated, string ID, string vname){

    //first we check for vaccinated citizens
    listnode *lnode;
    lnode=skipvaccinated->gethead();

    while(lnode!=NULL){

        skiplist *sl;
        sl=(skiplist*)lnode->getvalue();

        if(sl->getvname()==vname){

            //search in skiplist (vaccinated)
            citizen *cit;
            cit=sl->search(ID);

            //if citizen found then print message and return
            if(cit!=NULL){

                cout<<endl<<"VACCINATED ON ";

                cvirus *cv;
                cv=findvirus(cit,vname);

                if(cv!=NULL){

                    cout<<cv->getdate()<<endl;
                }

                return;
            } 
        }

        lnode=lnode->getnext();
    }


    //we check for not vaccinated citizens
    listnode *lnot;
    lnot=skipnotvaccinated->gethead();

    while(lnot!=NULL){

        skiplist *sl;
        sl=(skiplist*)lnot->getvalue();

        if(sl->getvname()==vname){

            //search in skiplist (not vaccinated)
            citizen *cit;
            cit=sl->search(ID);

            //if citizen found then print message and return
            if(cit!=NULL){

                cout<<endl<<"NOT VACCINATED"<<endl;

                return;
            }
        }

        lnot=lnot->getnext();
    }

    cout<<endl<<"Unkown information"<<endl;
}



void vaccineStatus(list *skipvaccinated,list *skipnotvaccinated, string ID){

    //first we check for vaccinated citizens
    listnode *lnode;
    lnode=skipvaccinated->gethead();

    while(lnode!=NULL){

        skiplist *sl;
        sl=(skiplist*)lnode->getvalue();

        //search in skiplist for ID
        citizen *cit;
        cit=sl->search(ID);

        //we found a vaccinated citizen, print message
        if(cit!=NULL){

            cout<<sl->getvname()<<" YES ";

            cvirus *cv;
            cv=findvirus(cit,sl->getvname());

            if(cv!=NULL){

                cout<<cv->getdate()<<endl;
            }
        }

        lnode=lnode->getnext();
    }


    //we check for not vaccinated citizens
    listnode *lnot;
    lnot=skipnotvaccinated->gethead();

    while(lnot!=NULL){

        skiplist *sl;
        sl=(skiplist*)lnot->getvalue();

        //search in list for ID
        citizen *cit;
        cit=sl->search(ID);

        //we found a not vaccinated citizen, print messsage
        if(cit!=NULL){

            cout<<sl->getvname()<<" NO"<<endl;
        }
        
        lnot=lnot->getnext();
    }
}



void insertCitizenRecord(string ID, string fname,string lname,string country,string age,string vname,string yn,
string date, list *skipvaccinated, list *skipnotvaccinated, list *citizens, list *blooms, int bloomsize){

    listnode *tmp=citizens->gethead();

    //ages must be 0<age<120
    if((atoi(age.c_str())>0) && (atoi(age.c_str())<120)){

        int flag1=0;

        flag1=checkrecord(tmp, flag1, ID, fname, lname, country, age, vname, yn, date,
        blooms, bloomsize,skipvaccinated,skipnotvaccinated);

        //we found a new citizen
        if(flag1==0){

            citizen *c=new citizen(ID, fname, lname, country, age, vname, yn, date);
            citizens->listinsert(c);

            char* s = new char [c->getID().length() + 1];
            strcpy(s, c->getID().c_str());

            delete[] s;

            //if citizen is vaccinated we add the virus in bloomfilter list
            //if citizen is vaccinated we add the virus in skiplist for vaccinated citizens
            if(yn=="YES"){
                

                //bloomfilter list insertion

                bloomvirus *bv=new bloomvirus(vname,NULL);

                bloomvirus *search=(bloomvirus*)blooms->search(bv,comparebloom);

                if(search==NULL){

                    bloomfilter *filter=new bloomfilter(bloomsize);

                    char* s = new char [ID.length() + 1];
                    strcpy(s, ID.c_str());

                    filter->insert((unsigned char*)s);

                    delete[] s;

                    bloomvirus *virus=new bloomvirus(vname,filter);

                    blooms->listinsert(virus);

                }else{

                    char* s = new char [ID.length() + 1];
                    strcpy(s, ID.c_str());

                    search->getbloom()->insert((unsigned char*)s);

                    delete[] s;
                } 

                delete bv; 


                //skilpist (vaccinated) insertion

                //checks if the virus already exists in list
                listnode *skiptmp=skipvaccinated->gethead();

                skiplist *found=NULL;

                while(skiptmp!=NULL){

                    skiplist *sk=(skiplist*)skiptmp->getvalue();

                    if(sk->getvname()==vname){

                        found=sk;
                        break;

                    }else{

                        skiptmp=skiptmp->getnext();
                    }
                }

                //we found a new virus
                if(found==NULL){

                    skiplist *skip=new skiplist(vname);

                    skip->insert(c);

                    skipvaccinated->listinsert(skip);

                }else{

                    found->insert(c);
                }

            }else{

                //skilpist (not vaccinated) insertion

                //checks if the virus already exists in list
                listnode *skipnottmp=skipnotvaccinated->gethead();
                
                skiplist *foundnot=NULL;

                while(skipnottmp!=NULL){

                    skiplist *sknot=(skiplist*)skipnottmp->getvalue();

                    if(sknot->getvname()==vname){

                        foundnot=sknot;
                        break;

                    }else{

                        skipnottmp=skipnottmp->getnext();
                    }
                }

                //we found a new virus
                if(foundnot==NULL){

                    skiplist *skipnot=new skiplist(vname);

                    skipnot->insert(c);

                    skipnotvaccinated->listinsert(skipnot);

                }else{

                    foundnot->insert(c);
                }
            }
        }
    }else{

        //wrong citizen record (wrong age input)
        cout<<"ERROR IN RECORD "<<ID<<endl;
    }
}



//checks if citizen is vaccinated
int checkcitizen(string ID, string vname, list *citizens){

    listnode *lnode;
    lnode=citizens->gethead();

    while(lnode!=NULL){

        citizen *c;
        c=(citizen*)lnode->getvalue();

        if(c->getID()==ID){

            list *virus;
            virus=c->getvirusinfo();

            listnode *vnode;
            vnode=virus->gethead();

            while(vnode!=NULL){

                cvirus *cv;
                cv=(cvirus*)vnode->getvalue();

                if(cv->getvname()==vname){

                    if(cv->getyesno()==true){

                        cout<<"ERROR. CITIZEN "<<ID<<" ALREADY VACCINATED ON ";
                        cout<<cv->getdate();

                        return 1;

                    }else{

                        return 0;
                    }

                }

                vnode=vnode->getnext();
            }
        }

        lnode=lnode->getnext();
    }

    return 0;
}



//making today's date
string get_current_date(void)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char todays_date[11];

    sprintf(todays_date, "%d-%d-%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    string date=todays_date;

    return date;
}



void vaccinateNow(string ID,string fname, string lname,string country,string age ,string vname,
list *skipvaccinated, list *skipnotvaccinated, list *citizens, list *blooms, int bloomsize){
  

    //first we check if the citizen is vaccinated
    listnode *lnode;
    lnode=skipvaccinated->gethead();

    while(lnode!=NULL){

        skiplist *sl;
        sl=(skiplist*)lnode->getvalue();

        if(sl->getvname()==vname){

            //search in skiplist (vaccinated)
            citizen *cit;
            cit=sl->search(ID);

            //if citizen found then print message and return
            if(cit!=NULL){

                cout<<endl<<"ERROR: CITIZEN "<<ID;
                cout<<" ALREADY VACCINATED ON ";

                cvirus *cv;
                cv=findvirus(cit,vname);

                if(cv!=NULL){

                    cout<<cv->getdate()<<endl;
                }

                return;
            } 
        }

        lnode=lnode->getnext();
    }


    //checks if citizen exists in "not vaccinated" skip list
    lnode=skipnotvaccinated->gethead();

    while(lnode!=NULL){

        skiplist *skip;
        skip=(skiplist*)lnode->getvalue();

        if(skip->getvname()==vname){

            //search in skiplist for the ID
            citizen *cit;
            cit=skip->search(ID);


            //if citizen exists we remove it from (not vaccinated) list
            if(cit!=NULL){

                skip->remove(cit);

                cvirus *vir;
                vir=findvirus(cit,vname);
                
                string d;
                d=get_current_date();

                //if virus exists we change info as we vaccinate the citizen
                if(vir!=NULL){

                    //change "NO" to "YES" 
                    if(vir->getyesno()==false){

                        vir->setyesno(true);
                    }

                    //change the date to "today's date"
                    vir->setdate(d);

                //if virus doesn't exist we save the new virus and vaccinate the citizen
                }else{

                    insertCitizenRecord(ID,fname,lname,country,age,vname,"YES",
                    d,skipvaccinated,skipnotvaccinated,citizens,blooms,bloomsize);

                    return;
                }
            }


            //bloomfilter list insertion

            bloomvirus *bv=new bloomvirus(vname,NULL);

            bloomvirus *search=(bloomvirus*)blooms->search(bv,comparebloom);

            if(search==NULL){

                bloomfilter *filter=new bloomfilter(bloomsize);

                char* s = new char [ID.length() + 1];
                strcpy(s, ID.c_str());

                filter->insert((unsigned char*)s);

                delete[] s;

                bloomvirus *virus=new bloomvirus(vname,filter);

                blooms->listinsert(virus);

            }else{

                char* s = new char [ID.length() + 1];
                strcpy(s, ID.c_str());

                search->getbloom()->insert((unsigned char*)s);

                delete[] s;
            } 

            delete bv;


            //skilpist (vaccinated) insertion

            //checks if the virus already exists in list
            listnode *skiptmp=skipvaccinated->gethead();

            skiplist *found=NULL;

            while(skiptmp!=NULL){

                skiplist *sk=(skiplist*)skiptmp->getvalue();

                if(sk->getvname()==vname){

                    found=sk;
                    break;

                }else{

                    skiptmp=skiptmp->getnext();
                }
            }

            //we found a new virus
            if(found==NULL){

                skiplist *skiip=new skiplist(vname);

                skiip->insert(cit);

                skipvaccinated->listinsert(skiip);

            }else{

                found->insert(cit);
            }
        }

        lnode=lnode->getnext();
    }
}



cvirus *findvirus(citizen *c, string vname){

    list *cv;
    cv=c->getvirusinfo();

    listnode *lnode;
    lnode=cv->gethead();

    while(lnode!=NULL){

        cvirus *v;
        v=(cvirus*)lnode->getvalue();

        if(v->getvname()==vname){

            return v;
        }

        lnode=lnode->getnext();
    }

    return NULL;
}



citizen *findcitizen(list *citizens, string ID){

    listnode *lnode;
    lnode=citizens->gethead();

    while(lnode!=NULL){

        citizen *c;
        c=(citizen*)lnode->getvalue();

        if(c->getID()==ID){

            return c;
        }

        lnode=lnode->getnext();
    }

    return NULL;
}



void list_nonVaccinated_Persons(string vname, list *skipnotvaccinated){

    listnode *lnode;
    lnode=skipnotvaccinated->gethead();

    //search for the skiplist of the virus given
    while(lnode!=NULL){

        skiplist *sl;
        sl=(skiplist*)lnode->getvalue();

        //skiplist found, then print citizens and return
        if(sl->getvname()==vname){

            sl->printfirstlevel();

            return;
        }

        lnode=lnode->getnext();
    }
}



void populationStatus(string country, string vname, string date1, string date2, list *citizens){

    listnode *lnode;
    lnode=citizens->gethead();

    int population=0;           //population of the country
    int vaccinatedindates=0;    //vaccinated citizens (on the given dates)


    while(lnode!=NULL){

        citizen *c;
        c=(citizen*)lnode->getvalue();


        if(c->getcountry()==country){

            population++;   //population increases

            cvirus *cv;
            cv=findvirus(c,vname);

            if(cv!=NULL){

                //the citizen is vaccinated
                if(cv->getyesno()==true){


                    int i;
                    i=compdates(date1,cv->getdate());

                    int j;
                    j=compdates(cv->getdate(),date2);

                    //date is between date1 and date2
                    if((i==1) && (j==1)){
                        
                        vaccinatedindates++;    //vaccinated citizens (on the given dates) increases
                    }
                }
            }
        }

        lnode=lnode->getnext();
    }

    //print message
    cout<<endl<<"VIRUS: "<<vname<<endl<<endl;
    cout<<country<<" "<<vaccinatedindates<<" ";

    float p;
    p=(vaccinatedindates*100.0)/population;
    cout<<p<<"%"<<endl<<endl;
}



int compdates(string date1, string date2){

    //we separate date1 into day, month and year and save them in array
    string date[3];
    char *token;
 
    //from string to char*
    char* temp1 = new char[date1.length()];
    strcpy(temp1,date1.c_str());

    token=strtok(temp1,"-");

    int i=0;
    while(token!= NULL) {

        date[i]=token;
        i++;

        token=strtok(NULL,"-");
    }

    delete temp1;


    //we separate date2 into day, month and year and save them in array
    string datee[3];

    //from string to char*
    char* temp2 = new char[date2.length()];
    strcpy(temp2,date2.c_str());

    token=strtok(temp2,"-");

    i=0;
    while(token!= NULL) {

        datee[i]=token;
        i++;

        token=strtok(NULL,"-");
    }

    delete temp2;

    //compare years
    if(atoi(date[2].c_str())<atoi(datee[2].c_str())){

        return 1;

    }else if(atoi(date[2].c_str())==atoi(datee[2].c_str())){

        //compare months
        if(atoi(date[1].c_str())<atoi(datee[1].c_str())){

            return 1;  

        }else if(atoi(date[1].c_str())==atoi(datee[1].c_str())){

            //compare days
            if(atoi(date[0].c_str())<=atoi(datee[0].c_str())){

                return 1;
            }
        }
    }

    return 0;
}

 


void populationStatus(string vname, string date1, string date2, list *citizens){

    list *currentcountry=new list();

    listnode *lnode;
    lnode=citizens->gethead();

    cout<<endl<<"VIRUS: "<<vname<<endl<<endl;

    while(lnode!=NULL){

        citizen *c;
        c=(citizen*)lnode->getvalue();

        string tmpcountry=c->getcountry();
        string* newcountry=new string(tmpcountry);
        string *country;
        country=(string*)currentcountry->search(newcountry,comparestrings);

        if(country==NULL){

            //add country to the list
            currentcountry->listinsert(newcountry);

            //search in list for the country info
            listnode *tmpnode;
            tmpnode=citizens->gethead();

            int population=0;           //population of the country
            int vaccinatedindates=0;    //vaccinated citizens (on the given dates)

            while(tmpnode!=NULL){

                citizen *tmpc;
                tmpc=(citizen*)tmpnode->getvalue();

                if(tmpc->getcountry()==c->getcountry()){

                    population++;   //population increases

                    cvirus *cv;
                    cv=findvirus(tmpc,vname);

                    if(cv!=NULL){

                        //the citizen is vaccinated
                        if(cv->getyesno()==true){


                            int i;
                            i=compdates(date1,cv->getdate());

                            int j;
                            j=compdates(cv->getdate(),date2);

                            //date is between date1 and date2
                            if((i==1) && (j==1)){

                                vaccinatedindates++;    //vaccinated citizens (on the given dates) increases
                            }
                        }
                    }
                }

                tmpnode=tmpnode->getnext();
            }

            //print message
            cout<<c->getcountry()<<" "<<vaccinatedindates<<" ";

            float p;
            p=(vaccinatedindates*100.0)/population;
            cout<<p<<"%"<<endl;
        }

        lnode=lnode->getnext();
    }
}



void popStatusByAge(string country, string vname, string date1, string date2, list *citizens){

    listnode *lnode;
    lnode=citizens->gethead();

    int population[4];           //population of the country
    int vaccinatedindates[4];    //vaccinated citizens (on the given dates)

    int i;
    for(i=0; i<4; i++){

        population[i]=0;
        vaccinatedindates[i]=0;
    }


    while(lnode!=NULL){

        citizen *c;
        c=(citizen*)lnode->getvalue();


        if(c->getcountry()==country){


            //population (by age) increases
            if(c->getage()<=20){

                population[0]++;        //ages 0-20

            }else if(c->getage()<=40){

                population[1]++;        //ages 20-40

            }else if(c->getage()<=60){

                population[2]++;        //ages 40-60

            }else{

                population[3]++;        //ages 60+
            }
               

            cvirus *cv;
            cv=findvirus(c,vname);

            if(cv!=NULL){

                //the citizen is vaccinated
                if(cv->getyesno()==true){


                    int i;
                    i=compdates(date1,cv->getdate());

                    int j;
                    j=compdates(cv->getdate(),date2);

                    //date is between date1 and date2
                    if((i==1) && (j==1)){
                        

                        //vaccinated citizens (on the given dates) increases (by age)
                        if(c->getage()<=20){

                            vaccinatedindates[0]++;        //ages 0-20

                        }else if(c->getage()<=40){

                            vaccinatedindates[1]++;        //ages 20-40

                        }else if(c->getage()<=60){

                            vaccinatedindates[2]++;        //ages 40-60

                        }else{

                            vaccinatedindates[3]++;        //ages 60+
                        }
                    }
                }
            }
        }

        lnode=lnode->getnext();
    }

    //print message
    float p[4];

    for(i=0; i<4; i++){

        if(population[i]==0){

            p[i]=0;

        }else{

            p[i]=(vaccinatedindates[i]*100.0)/population[i];
        }
        
    }

    cout<<endl<<"VIRUS: "<<vname<<endl<<endl;
    cout<<country<<endl;
    
    cout<<"0-20: "<<vaccinatedindates[0]<<" "<<p[0]<<"%"<<endl;
    cout<<"20-40: "<<vaccinatedindates[1]<<" "<<p[1]<<"%"<<endl;
    cout<<"40-60: "<<vaccinatedindates[2]<<" "<<p[2]<<"%"<<endl;
    cout<<"60+: "<<vaccinatedindates[3]<<" "<<p[3]<<"%"<<endl<<endl;
}



void popStatusByAge(string vname, string date1, string date2, list *citizens){

    list *currentcountry=new list();

    listnode *lnode;
    lnode=citizens->gethead();

    cout<<endl<<"VIRUS: "<<vname<<endl<<endl;

    while(lnode!=NULL){

        citizen *c;
        c=(citizen*)lnode->getvalue();

        string tmpcountry=c->getcountry();
        string* newcountry=new string(tmpcountry);
        string *country;
        country=(string*)currentcountry->search(newcountry,comparestrings);

        if(country==NULL){

            //add country to the list
            currentcountry->listinsert(newcountry);

            //search in list for the country info
            listnode *tmpnode;
            tmpnode=citizens->gethead();


            int population[4];           //population of the country
            int vaccinatedindates[4];    //vaccinated citizens (on the given dates)

            int i;
            for(i=0; i<4; i++){

                population[i]=0;
                vaccinatedindates[i]=0;
            }


            while(tmpnode!=NULL){

                citizen *tmpc;
                tmpc=(citizen*)tmpnode->getvalue();

                if(tmpc->getcountry()==c->getcountry()){


                    //population (by age) increases
                    if(tmpc->getage()<=20){

                        population[0]++;        //ages 0-20

                    }else if(tmpc->getage()<=40){

                        population[1]++;        //ages 20-40

                    }else if(tmpc->getage()<=60){

                        population[2]++;        //ages 40-60

                    }else{

                        population[3]++;        //ages 60+
                    }


                    cvirus *cv;
                    cv=findvirus(tmpc,vname);

                    if(cv!=NULL){

                        //the citizen is vaccinated
                        if(cv->getyesno()==true){


                            int k;
                            k=compdates(date1,cv->getdate());

                            int j;
                            j=compdates(cv->getdate(),date2);

                            //date is between date1 and date2
                            if((k==1) && (j==1)){


                                //vaccinated citizens (on the given dates) increases (by age)
                                if(tmpc->getage()<=20){

                                    vaccinatedindates[0]++;        //ages 0-20

                                }else if(tmpc->getage()<=40){

                                    vaccinatedindates[1]++;        //ages 20-40

                                }else if(tmpc->getage()<=60){

                                    vaccinatedindates[2]++;        //ages 40-60

                                }else{

                                    vaccinatedindates[3]++;        //ages 60+
                                }
                            }
                        }
                    }
                }

                tmpnode=tmpnode->getnext();
            }

            //print message
            float p[4];

            for(i=0; i<4; i++){

                if(population[i]==0){

                    p[i]=0;

                }else{

                    p[i]=(vaccinatedindates[i]*100.0)/population[i];
                } 
            }

            cout<<c->getcountry()<<endl;
            
            cout<<"0-20: "<<vaccinatedindates[0]<<" "<<p[0]<<"%"<<endl;
            cout<<"20-40: "<<vaccinatedindates[1]<<" "<<p[1]<<"%"<<endl;
            cout<<"40-60: "<<vaccinatedindates[2]<<" "<<p[2]<<"%"<<endl;
            cout<<"60+: "<<vaccinatedindates[3]<<" "<<p[3]<<"%"<<endl<<endl;
        }

        lnode=lnode->getnext();
    }
}



int readinput(list *citizens, list *blooms, list *skipvaccinated, list *skipnotvaccinated, int bloomsize){

    char command[1000];
    int i=0;

    //we take user's input
    command[i]=getchar();

    while(command[i]!='\n'){

        i++;
        command[i]=getchar();
    }

    command[i]='\0';

    string comm[10];

    for(i=0; i<10; i++){

        comm[i]='-';
    }

    char *token;

    //we separate user's input into words and save them in array
    token=strtok(command," ");

    i=0;
    while(token!= NULL) {

        comm[i]=token;
        i++;

        token= strtok(NULL," ");
    }


    if(comm[0]=="/exit"){
        
        return 1;
    }


    //checks for the right input given
    if(comm[0]=="/vaccineStatusBloom"){

        //we need only 2 arguments
        if((comm[1]!="-") && (comm[2]!="-") && (comm[3]=="-")){

            vaccineStatusBloom(blooms,comm[1],comm[2]);

        }else{
            cout<<endl<<"Wrong number of arguments."<<endl;
        }


    }else if((comm[0]=="/vaccineStatus")){

        //(option 1) we need 2 arguments
        if((comm[1]!="-") && (comm[2]!="-") && (comm[3]=="-")){

            vaccineStatus(skipvaccinated,skipnotvaccinated,comm[1],comm[2]);

        //(option 2) we need only one argument
        }else if((comm[1]!="-") && (comm[2]=="-")){

            vaccineStatus(skipvaccinated,skipnotvaccinated,comm[1]);

        }else{
            cout<<endl<<"Wrong number of arguments."<<endl;
        }


    }else if(comm[0]=="/populationStatus"){

        //we have 4 arguments (country included)
        if((comm[1]!="-") && (comm[2]!="-") && (comm[3]!="-") && (comm[4]!="-") && (comm[5]=="-")){

            populationStatus(comm[1],comm[2],comm[3],comm[4],citizens);    //country, virusName, date1, date2, citizens list

        //we have 3 arguments (country not included)
        }else if((comm[1] !="-") && (comm[2]!="-") && (comm[3]!="-") && (comm[4]=="-")){

            populationStatus(comm[1],comm[2],comm[3],citizens);    //virusName, date1, date2, citizens list

        }else{
            cout<<endl<<"Wrong number of arguments."<<endl;
        }


    }else if(comm[0]=="/popStatusByAge"){

        //we have 4 arguments (country included)
        if((comm[1]!="-") && (comm[2]!="-") && (comm[3]!="-") && (comm[4]!="-") && (comm[5]=="-")){

            popStatusByAge(comm[1],comm[2],comm[3],comm[4],citizens); //country, virusName, date1, date2, citizens list

        //we have 3 arguments (country not included)
        }else if((comm[1] !="-") && (comm[2]!="-") && (comm[3]!="-") && (comm[4]=="-")){

            popStatusByAge(comm[1],comm[2],comm[3],citizens); //virusName, date1, date2, citizens list

        }else{
            cout<<endl<<"Wrong number of arguments."<<endl;
        }


    }else if(comm[0]=="/insertCitizenRecord"){

        //we have 8 arguments (date included (YES)) or 7 arguments (date not included (NO))
        if((comm[1]!="-") && (comm[2]!="-") && (comm[3]!="-") && (comm[4]!="-") && (comm[5]!="-")
        && (comm[6]!="-") && (comm[7]!="-") && (comm[9]=="-")){

            if((comm[7]=="YES" && comm[8]!="-") || (comm[7]=="NO" && comm[8]=="-")){

                int i;
                //checks if citizen is vaccinated or not
                i=checkcitizen(comm[1],comm[6],citizens);

                //if citizen is not already vaccinated
                if(i==0){

                    insertCitizenRecord(comm[1],comm[2],comm[3],comm[4],comm[5],comm[6],comm[7],comm[8],
                    skipvaccinated,skipnotvaccinated,citizens,blooms,bloomsize);

                    cout<<endl<<"Record successfully added to lists."<<endl;
                }

            }else{

                cout<<endl<<"Wrong input."<<endl;
            }

        }else{
            cout<<endl<<"Wrong number of arguments."<<endl;
        }


    }else if(comm[0]=="/vaccinateNow"){

        //we need only 6 arguments
        if((comm[1]!="-") && (comm[2]!="-") && (comm[3]!="-") && (comm[4]!="-") && (comm[5]!="-")
        && (comm[6]!="-") && (comm[7]=="-")){

            vaccinateNow(comm[1],comm[2],comm[3],comm[4],comm[5],comm[6],skipvaccinated,
            skipnotvaccinated,citizens,blooms,bloomsize);
            
        }else{
            cout<<endl<<"Wrong number of arguments."<<endl;
        }


    }else if(comm[0]=="/list-nonVaccinated-Persons"){

        //we need only one argument
        if((comm[1]!="-") && (comm[2]=="-")){

            list_nonVaccinated_Persons(comm[1],skipnotvaccinated);

        }else{
            cout<<endl<<"Wrong number of arguments."<<endl;
        }

    }else{
        
        cout<<endl<<"Wrong input."<<endl;
    }

    return 0;
}