clean:
	rm -f hw2

debug:
	gdb hw2

hello:
	rm -f hello
	gcc -g hello.c -o hello

compile:
	make clean
	gcc -g main.c -o hw2

run:
	make compile && make hello
	./hw2

comde:
	make compile && make debug

