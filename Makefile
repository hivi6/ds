vector-example:
	mkdir -p bin
	gcc -I . -o bin/vector-example examples/vector-example.c

clean:
	rm -rf bin
