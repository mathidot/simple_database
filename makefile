CC = gcc
.PHONY: clean
db: main.c
	$(CC) -g main.c src/buffer.c src/table.c src/types.c -o db
run:
	./db
test:
	rspec test.rb
clean:
	rm db 