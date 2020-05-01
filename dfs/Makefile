all: dfs
	gcc -o out/dfs obj/map.o obj/err.o obj/dfs.o obj/stack.o obj/binaryTree.o -Wall

dfs: dfs.c map stack binaryTree
	gcc -c dfs.c -o dfs.o -Wall
	cp dfs.o obj/dfs.o
	rm dfs.o

stack: ../stack_C/stack.c ../stack_C/stack.h
	gcc -c ../stack_C/stack.c -o stack.o -Wall
	cp stack.o obj/stack.o
	rm stack.o

binaryTree: ../binaryTree_C/binaryTree.c ../binaryTree_C/binaryTree.h
	gcc -c ../binaryTree_C/binaryTree.c -o binaryTree.o -Wall
	cp binaryTree.o obj/binaryTree.o
	rm binaryTree.o

map: ../map/map.c ../map/err.c base
	gcc -c ../map/err.c -o err.o -Wall
	gcc -c ../map/map.c -o map.o -Wall
	cp err.o obj/err.o
	rm err.o
	cp map.o obj/map.o
	rm map.o

clean:
	rm -f *.o mapConstruct bfs -r obj a.out

base:
	if [ -d "obj" ]; then echo ""; else mkdir obj; fi
	if [ -d "out" ]; then echo ""; else mkdir out; fi
	cp ../in/mapWithName.txt out/mapWithName.txt