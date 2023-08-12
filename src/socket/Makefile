clean:
	rm -f hw5c
	rm -f hw5s

compile:
	make clean
	gcc -g client.c -o hw5c
	gcc -g server.c -o hw5s

run:
	make compile
	./hw5s data_base.csv 30000

client: 
	./hw5c localhost 30000

debug:
	make compile && gdb ./hw5s

