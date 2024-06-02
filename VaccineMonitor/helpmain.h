#ifndef HELPMAIN
#define HELPMAIN

#include<iostream>
#include<cstdio>
#include<string>
#include<cstring>
#include <time.h>

#include "citizen.h"
#include "bloomfilter.h"
#include "skiplist.h"


//checks every record 
int checkrecord(listnode *tmp,int flag,string ID, string fname, string lname, string country,string age, 
string vname, string yn, string date, list *blooms, int bloomsize, list *skipvaccinated, list *skipnotvaccinated);

//function to print the input options
void printoptions();

//reads and checks the user input
int readinput(list *citizens, list *blooms, list *skipvaccinated, list *skipnotvaccinated, int bloomsize);

//vaccineStatusBloom function
void vaccineStatusBloom(list *blooms, string ID, string vname);

//vaccineStatus function (virus name included)
void vaccineStatus(list *skipvaccinated, list *skipnotvaccinated, string ID, string vname);

//vaccineStatus function (virus name not included)
void vaccineStatus(list *skipvaccinated, list *skipnotvaccinated, string ID);

//insertCitizenRecord function
void insertCitizenRecord(string ID, string fname,string lname,string country,string age,string vname,
string yn, string date, list *skipvaccinated, list *skipnotvaccinated, list *citizens, list *blooms, int bloomsize);

//checks if citizen is vaccinated
int checkcitizen(string ID, string vname, list *citizens);

//vaccinateNow function
void vaccinateNow(string ID,string fname, string lname,string country,string age ,string vname,
list *skipvaccinated, list *skipnotvaccinated, list *citizens, list *blooms, int bloomsize);

//function that finds citizen with ID in citizen list
citizen *findcitizen(list *citizens, string ID);

//function that finds a virus with "vname" in a citizen
cvirus *findvirus(citizen *c, string vname);

//list nonVaccinated Persons function
void list_nonVaccinated_Persons(string vname, list *skipnotvaccinated);

//populationStatus function (country included)
void populationStatus(string country, string vname, string date1, string date2, list *citizens);

//populationStatus function (country not included)
void populationStatus(string vname, string date1, string date2, list *citizens);

//compare dates
int compdates(string date1, string date2);

//popStatusByAge function (country included)
void popStatusByAge(string country, string vname, string date1, string date2, list *citizens);

//popStatusByAge function (country not included)
void popStatusByAge(string vname, string date1, string date2, list *citizens);

#endif //HELPMAIN