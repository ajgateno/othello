all:
	gcc main.c `sdl2-config --cflags --libs`
	mv a.out othello
	rm -f *.gch

clean:
	rm othello
