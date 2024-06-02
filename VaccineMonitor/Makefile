# set compiler options here
CPPFLAGS=-g -Wall -g3

# set the name of the executable file to compile here
program=VaccineMonitor

# list object files needed to compile the program
objects=main.o citizen.o list.o helpmain.o bloomfilter.o hash.o skiplist.o

args= -c TestinputFile.txt -b 100.000 

# this makes the program. 
$(program) : $(objects)
	g++ $(CPPFLAGS) $(objects) -o $(program) 


main.o: citizen.h
helpmain.o: helpmain.h
citizen.o: citizen.h
list.o: list.h
bloomfilter.o: bloomfilter.h
hash.o: hash.h
skiplist.o: skiplist.h

# this allows to quickly remove all generated files with 'make clean'
clean:
	rm $(program) $(objects)

# run the program with 'make run'.
run: $(program)
	./$(program) $(args)


# run the program with 'valgrind'.
valgrind: $(program)
	valgrind ./$(program) $(args)