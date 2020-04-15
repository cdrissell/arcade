all: arcade pong space

arcade: arcade.c
	gcc -lncurses -o arcade arcade.c

pong: pong.c
	gcc -lncurses -o pong pong.c

space: space.c
	gcc -lncurses -o space space.c

clean:
	rm arcade
	rm pong
	rm space
