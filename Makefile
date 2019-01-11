DIRS=bin obj
CFLAGS=-g -lm -rdynamic `pkg-config --cflags gtk+-3.0`
LDFLAGS=`pkg-config --libs gtk+-3.0`

all: gtk-follow.o follow.o strhash.o list.o main.o
	gcc -o bin/gtk-diff obj/gtk-follow.o obj/follow.o obj/strhash.o obj/list.o $(CFLAGS) $(LDFLAGS)

gtk-follow.o: src/gtk-follow.c src/gtk-follow.h
	gcc -c src/gtk-follow.c -o obj/gtk-follow.o $(CFLAGS) $(LDFLAGS)

test: test.o follow.o strhash.o list.o main.o
	gcc -o bin/progTest obj/test.o obj/follow.o obj/strhash.o obj/list.o obj/main.o -lm

test.o: src/test.c src/test.h
	gcc -c src/test.c -o obj/test.o

follow.o: src/follow.c src/follow.h
	gcc -c src/follow.c -o obj/follow.o -lm

strhash.o: src/strhash.c src/strhash.h
	gcc -c src/strhash.c -o obj/strhash.o -lm

list.o: src/list.c src/list.h
	gcc -c src/list.c -o obj/list.o

main.o: src/main.c src/follow.h src/strhash.h src/list.h
	gcc -c src/main.c -o obj/main.o

clean:
	rm bin/progTest* bin/gtk-diff* obj/*.o; \
	rmdir bin obj

$(info $(shell mkdir -p $(DIRS)))
