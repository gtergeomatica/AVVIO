# GTER copyleft
# Author: Alessandro Fermi
# Build options for the creation of the static library jsmn for parsing json string
# and compiling all source files that implement the configuration update process
#-include config.mk

#path to executables and source files
BINDIR = /usr/local/bin 
SRC    = ./SRC

#include parameters of the compiler - when the debug phase will be finished an optimization option can be added
CFLAGS = -Wall -g -pedantic -Wno-unused-but-set-variable -ansi -I$(SRC) -I.. 
LDFLAGS  = -lm -lrt 

#target for the creation of the static library jsmn
target1       : libjsmn.a 

#this is the set of rules for compiling and creating the static library jsmn for parsing
libjsmn.a     : jsmn.o
	$(AR) rc $@ $^

jsmn.o           : $(SRC)/jsmn.c $(SRC)/jsmn.h
	$(CC) -c -o $@ $< $(CFLAGS)  

#these are the set of rules for the compilation and creation of the corresponding executable
#simple_example: example/simple.o libjsmn.a
all           : comunication
comunication  : comunication.o conf_update.o libjsmn.a

comunication.o: comunication.c
	$(CC) -c $(CFLAGS) $(LDFLAGS) comunication.c
conf_update.o : $(SRC)/conf_update.c  
	$(CC) -c $(CFLAGS) $(LDFLAGS) $(SRC)/conf_update.c
	

comunication.o: $(SRC)/conf_update.h
conf_update.o : $(SRC)/conf_update.h

install       : 
	cp comunication $(BINDIR)
 
clean         :
	rm -f *.o $(SRC)/*.o
	rm -f *.a *.so
	rm -f comunication

.PHONY: all clean test

