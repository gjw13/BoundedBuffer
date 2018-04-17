main: a
	./boundedbuffer 1 1 1 10
a: 
	gcc boundedbuffer.c -o boundedbuffer -l pthread
s: b
	./sem 3 4 3 1000
b:
	gcc sem.c -o sem -l pthread
	
