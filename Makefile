all:
	gcc game.h logic.h logic.c rendering.h rendering.c main.c `sdl2-config --cflags --libs` -lSDL2_gfx
	mv a.out othello
	rm -f *.gch

clean:
	rm othello
