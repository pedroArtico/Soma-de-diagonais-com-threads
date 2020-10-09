CC     = gcc
RM     = rm -f
OBJS   = main.o \
         thread/thread.o \
         file/fileMngr.o \
         dataStructures/array/arrayMngr.o \
         dataStructures/matrix/matrixMngr.o \
         util/util.o

LIBS   = -lpthread
CFLAGS =

.PHONY: projso.o clean clean-after

all: projso.o

clean:
	$(RM) $(OBJS) projso.o

clean-after:
	$(RM) $(OBJS)

projso.o: $(OBJS)
	$(CC) -Wall -s -o $@ $(OBJS) $(LIBS)

main.o: main.c util/util.h datadefine.h dataStructures/array/arrayMngr.h dataStructures/matrix/matrixMngr.h thread/thread.h file/fileMngr.h
	$(CC) -Wall -s -c $< -o $@ $(CFLAGS)

thread/thread.o: thread/thread.c thread/thread.h util/util.h datadefine.h dataStructures/array/arrayMngr.h dataStructures/matrix/matrixMngr.h
	$(CC) -Wall -s -c $< -o $@ $(CFLAGS)

file/fileMngr.o: file/fileMngr.c file/fileMngr.h datadefine.h dataStructures/matrix/matrixMngr.h
	$(CC) -Wall -s -c $< -o $@ $(CFLAGS)

dataStructures/array/arrayMngr.o: dataStructures/array/arrayMngr.c datadefine.h
	$(CC) -Wall -s -c $< -o $@ $(CFLAGS)

dataStructures/matrix/matrixMngr.o: dataStructures/matrix/matrixMngr.c datadefine.h
	$(CC) -Wall -s -c $< -o $@ $(CFLAGS)

util/util.o: util/util.c datadefine.h file/fileMngr.h
	$(CC) -Wall -s -c $< -o $@ $(CFLAGS)

