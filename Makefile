all:
	gcc main.c `sdl2-config --cflags --libs`
	mv a.out othello
	rm *.gch

clean:
	rm othello
