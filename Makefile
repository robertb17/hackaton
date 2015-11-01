compile:
	g++ maxmin.cpp main.cpp trans_input.cpp
run:
	./a.out localhost 10000
clean:
	rm a.out
debug:
	g++ -g maxmin.cpp main.cpp trans_input.cpp
