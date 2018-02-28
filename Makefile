# BILLARD
# Author: Iacopo Sprenger
# Version: 1.1
# Date: 25.02.2018
# Filename: Makefile

CFLAGS=-Wall -w

LIBS=-framework GLUT -framework OpenGL -framework Cocoa

OFILES=game.o physics.o graphics.o

all : Billard

Billard : $(OFILES)
	gcc $(CFLAGS) $(OFILES) -o $@ $(LIBS)
	
game.o : game.c game.h physics.h graphics.h constants.h
	gcc $(CFLAGS) -c $< -o $@
	
physics.o : physics.c physics.h constants.h game.h
	gcc $(CFLAGS) -c $< -o $@
	
graphics.o : graphics.c graphics.h
	gcc $(CFLAGS) -c $< -o $@

	
clean : 
	@rm -f *.x *.o
	@echo "CLEANED"
