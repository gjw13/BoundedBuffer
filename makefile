main: a s
	./boundedbuffer 6 5 3 1000
a: 
	gcc boundedbuffer.c -o boundedbuffer -l pthread
s: b
	./sem 6 5 3 1000
b:
	gcc sem.c -o sem -l pthread
	
