all: bfs
	gcc -o bfs obj/map.o obj/bfs.o obj/err.o obj/stackInt.o -Wall

bfs: bfs.c map stackInt
	gcc -c bfs.c -o obj/bfs.o -Wall

stackInt: stackInt.c stackInt.h
	gcc -c stackInt.c -o obj/stackInt.o -Wall

map: map.c map.h err
	gcc -c map.c -o obj/map.o -Wall

err: err.c err.h base
	gcc -c err.c -o obj/err.o -Wall

clean:
	rm -f *.o mapConstruct bfs -r obj a.out

base:
	if [ -d "obj" ]; then \
		echo "dir exist"; \
	else \
		mkdir obj; \
	fi