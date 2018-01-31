CC=g++
CXXFLAGS=-std=c++11 -Wall
OUT=main
SRC=network.cpp main.cpp util.cpp urlExtraction.cpp sharedData.cpp Statis.cpp

all: clean main #test

main: $(SRC)
	$(CC) $(CXXFLAGS) $(SRC) -o $(OUT)

# test: test.cpp
# 	$(CC) test.cpp -o test

clean:
	rm -f $(OUT)  #test