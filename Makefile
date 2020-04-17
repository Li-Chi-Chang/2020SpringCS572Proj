all: bfs
	gcc -o bfs obj/map.o obj/bfs.o obj/err.o obj/stack.o obj/linkedList.o -Wall

bfs: bfs.c map stack linkedList
	gcc -c bfs.c -o obj/bfs.o -Wall

stack: stack/stack.c stack/stack.h
	gcc -c stack/stack.c -o stack.o -Wall
	cp stack.o obj/stack.o
	rm stack.o

linkedList: linkedList/linkedList.c linkedList/linkedList.h
	gcc -c linkedList/linkedList.c -o linkedList.o -Wall
	cp linkedList.o obj/linkedList.o
	rm linkedList.o


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