.PHONY : all
all : seq para

seq : football.o seq.o
	gcc -o seq football.o seq.o

para : football.o para.o
	mpicc -o para football.o para.o

seq.o : football.h seq.c
	gcc -c seq.c

para.o : football.h para.c
	mpicc -c para.c

football.o : football.h football.c
	gcc -c football.c

.PHONY : clean
clean : 
	rm *.o
