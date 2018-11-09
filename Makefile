all: p2

p2: main.o
	g++ main.o -o p2 -lglut -lGL 
main.o: main.cpp  
	g++ -c main.cpp -lglut -lGL 
clean:
	rm -rf *o p2
