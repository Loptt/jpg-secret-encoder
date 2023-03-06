all: encoder decoder

encoder: encoder.o jpg_lib.o
	gcc -o encoder encoder.o jpg_lib.o

decoder: decoder.o jpg_lib.o
	gcc -o decoder decoder.o jpg_lib.o

encoder.o: encoder.c jpg_lib.h
	gcc -c encoder.c -o encoder.o

decoder.o: decoder.c jpg_lib.h
	gcc -c decoder.c -o decoder.o

jpg_lib.o: jpg_lib.c jpg_lib.h
	gcc -c jpg_lib.c

clean:
	rm encoder decoder *.o encoded.jpeg