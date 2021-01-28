
output: main.o
		gcc out/main.o -o out/GameOfLife -l ncurses

main.o: src/main.c
		gcc -c src/main.c -o out/main.o -l ncurses

clean:
		rm out/*.o