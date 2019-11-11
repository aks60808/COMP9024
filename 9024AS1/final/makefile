


puzzle: boardADT.o puzzle.o
	gcc boardADT.o puzzle.o -o puzzle 
boardADT.o: boardADT.c
	gcc boardADT.c -c 
puzzle.o: puzzle.c
	gcc puzzle.c -c 

clean:
	rm -rf boardADT.o puzzle.o