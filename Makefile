progTest: strhash.o list.o main.o
	gcc -o bin/progTest obj/strhash.o obj/list.o obj/main.o

strhash.o: src/strhash.c src/strhash.h
	gcc -c src/strhash.c -o obj/strhash.o -lm

list.o: src/list.c src/list.h
	gcc -c src/list.c -o obj/list.o

main.o: src/main.c src/strhash.h src/list.h
	gcc -c src/main.c -o obj/main.o

clean:
	rm bin/progTest* obj/*.o
