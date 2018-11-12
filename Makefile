all: p3

p3: main.o
	g++ main.o -o p3 -lglut -lGL 
main.o: main.cpp  
	g++ -c main.cpp -lglut -lGL 
clean:
	rm -rf *o p3
