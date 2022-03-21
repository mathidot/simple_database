CC = gcc
.PHONY: clean
db: main.c
	$(CC) -o db main.c
run:
	./db
clean:
	rm *.o