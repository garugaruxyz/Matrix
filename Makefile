
main.exe: main.o 
	g++ -g -std=c++0x main.o -o main.exe

main.o: main.cpp Matrice3D.h Matrice2D.h
	g++ -c -std=c++0x main.cpp -o main.o

.PHONY: clean

clean:
	rm *.exe *.o