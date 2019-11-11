


puzzle: boardADT.o puzzle.o
	dcc boardADT.o puzzle.o -o puzzle 
boardADT.o: boardADT.c
	dcc boardADT.c -c 
puzzle.o: puzzle.c
	dcc puzzle.c -c 

clean:
	rm -rf boardADT.o puzzle.o
