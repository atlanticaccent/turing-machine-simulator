all: 
	g++ *.cpp -std=c++11 -O3 -o runtm

verbose:
	g++ *.cpp -std=c++11 -Wall -Wextra -o runtm

debug:
	g++ *.cpp -std=c++11 -g -Wall -Wextra -o debug

analyse:
	g++ *.cpp -std=c++11 -pg -o analysis

opt:
	g++ *.cpp -std=c++11 -O3 -frename-registers -fprofile-use -o runtm

pgogen:
	g++ *.cpp -std=c++11 -O3 -frename-registers -fprofile-generate -o runtm

clean:
	rm *.o
	rm runtm
	rm debug
