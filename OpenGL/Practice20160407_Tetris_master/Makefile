CC=g++
CFLAGS= -Wall
PROG = tetris

#--- File Dependencies---
SRCS = Tetris.cpp Game.cpp Figure.cpp Block.cpp
#--- Library
LIBS= -lglut -lGLU -lGL

all:  $(PROG) 

$(PROG): 	$(SCRS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG) *~ *#*