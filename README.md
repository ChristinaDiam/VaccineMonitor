# Vaccine Monitor #

This Project implements a program that receives, processes, records and answers questions about vaccinations for citizens.

In particular, there is a set of structures (bloom filters, linked lists, skip lists) that allow  the input and querying of large volumes of citizenRecord type records. In addition a bash script was implemented to create .txt files for testing inputs (citizen records).

	The Project contains the following files: main.cpp, bloomfilter.cpp, bloomfilter.h, citizen.cpp, citizen.h, hash.cpp, hash.h, helpmain.cpp, helpmain.h, list.cpp, list.h, skiplst.cpp, skiplist.h, Makefile and testFile.sh.

The program reads data from a file, which it stores in memory, but first doing the necessary validity checks for each record. Then it prints out the options/actions a user can take. 

*(In order to carry out correct checks and to store records correctly, the file given to be read must contain one record per row and each record must be complete and in the correct order i.e. ID First Name Last Name Country Age Virus YES/NO Date, regardless of whether the record is  valid.)*


**The main program main.cpp creates the following lists where it stores the data:**

1) List of citizen records
2) List of Bloomfilters
3) List of skiplists for each virus for vaccinated citizens
4) List of skiplists for each virus for unvaccinated citizens


### Skip List structure ###

The skiplist was based on and created with the help of the simple list file (list.cpp), which contains generalized list which is used in many parts of the program.  
The design of skiplist is simple. The structure contains a list of levels, a virus name, a counter for the number of levels and a counter for the number of items in the list.  
Each level is a simply linked list with a 'dummy' node at the beginning in order to distinguish the 'heads' from the other nodes.   
Each node of the above list is a simply linked list (imagine vertical for easier understanding) whose nodes contain pointers to citizen type records. The sorting of data within the skiplist is done based on the ID of each citizen.  
Finally, the skiplist has no limit on levels, it can go as high as necessary to achieve O(log n) complexity of searching and inserting an item.


### Compiling and executing ###

To compile and execute the program, a Makefile was created which contains the necessary commands. 

To compile the program, just type **make**.

To run the program, type **make run**.  
*(This Makefile can automatically run the program with standard data (e.g. bloomsize=100,000),as long as the file given to read is named TestinputFile.txt)*

To clean up the temporary files, type **make clean**.

The program can also be executed with the command:

	./VaccineMonitor -c TestinputFile.txt -b bloomsize
or  
	
    ./VaccineMonitor -b bloomsize -c TestinputFile.txt 

*(the order of the definitions given on the command line is irrelevant,
as long as -c is followed by text file and -b is followed by a number for bloomsize)*