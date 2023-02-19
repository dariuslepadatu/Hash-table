build:
	gcc -g tema1.c -o tema1
check: build
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./tema1 input/in0.txt
run:
	./tema1
clean:
	rm -rf tema1