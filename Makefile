progTest: test.o follow.o strhash.o list.o main.o
	gcc -o bin/progTest obj/test.o obj/follow.o obj/strhash.o obj/list.o obj/main.o

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
	rm bin/progTest* obj/*.o
