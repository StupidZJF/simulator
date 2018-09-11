test:node1.c node2.c simulator.c simulator.h
	gcc -c -o build/node1.o node1.c
	gcc -c -o build/node2.o node2.c
	gcc -c -o build/simulator.o simulator.c
	gcc -o bin/node1.elf build/node1.o
	gcc -o bin/node2.elf build/node2.o
	gcc -o bin/simulator.elf build/simulator.o -lm
	
	
clean:
	rm -rf build/*.o
	rm -rf bin/*.elf

