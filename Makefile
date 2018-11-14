progTest: list.o main.o
	gcc -o bin/progTest obj/list.o obj/main.o

list.o: src/list.c src/list.h
	gcc -c src/list.c -o obj/list.o

main.o: src/main.c src/list.h
	gcc -c src/main.c -o obj/main.o

clean:
	rm bin/progTest* obj/*.o
