run: build4
	./mylife4

build:
	gcc -o mylife3 mylife3.c

build4:
	gcc -o mylife4 mylife4.c lifegame/stage.c lifegame/util.c

clean_relate_file:
	rm gen*.lif

