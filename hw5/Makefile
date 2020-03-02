clean:
	rm -f hw5c
	rm -f hw5s

compile:
	make clean
	gcc -g client.c -o hw5c
	gcc -g server.c -o hw5s

run:
	make compile
	./hw5c

debug:
	make compile && gdb hw5s

