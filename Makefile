all: bfs
	gcc -o bfs obj/map.o obj/bfs.o obj/err.o obj/stack.o obj/binaryTree.o -Wall

bfs: bfs.c map stack binaryTree
	gcc -c bfs.c -o obj/bfs.o -Wall

stack: stack_C/stack.c stack_C/stack.h
	gcc -c stack_C/stack.c -o stack.o -Wall
	cp stack.o obj/stack.o
	rm stack.o

binaryTree: binaryTree_C/binaryTree.c binaryTree_C/binaryTree.h
	gcc -c binaryTree_C/binaryTree.c -o binaryTree.o -Wall
	cp binaryTree.o obj/binaryTree.o
	rm binaryTree.o


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