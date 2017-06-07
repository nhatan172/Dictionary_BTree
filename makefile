all: abc run
abc:
	gcc -o tudien tudien.c libbt.a
run:
	./tudien
clean:
	rm --recursive --force abc
