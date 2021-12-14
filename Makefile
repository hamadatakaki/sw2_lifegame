run: build
	./mylife3

build:
	gcc -o mylife3 mylife3.c

clean_relate_file:
	rm gen*.lif