CC = gcc
.PHONY: clean

db: main.c buffer.o pager.o types.o node.o table.o cursor.o statement.o
	$(CC) -g main.c buffer.o pager.o types.o node.o table.o cursor.o statement.o -o db

statement.o: src/statement.c buffer.o table.o node.o
	$(CC) -c src/statement.c buffer.o table.o node.o -o statement.o

cursor.o: src/cursor.c pager.o node.o
	$(CC) -c src/cursor.c pager.o node.o -o cursor.o

table.o : src/table.c node.o 
	$(CC) -c src/table.c node.o -o table.o

node.o: src/node.c types.o pager.o
	$(CC) -c src/node.c types.o pager.o -o node.o

types.o: src/types.c
	$(CC) -c src/types.c -o types.o

pager.o: src/pager.c
	$(CC) -c src/pager.c -o pager.o

buffer.o: src/buffer.c
	$(CC) -c src/buffer.c -o buffer.o

run:
	./db mydb.db

test:
	rspec test.rb

clean:
	rm -rf *.o; rm *.db; rm db
