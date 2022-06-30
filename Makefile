CC= g++
OPTIONS= -O3 -Wall -std=c++17 -o 
DEBUG= -g -O0 -Wall -o
TESSERACT= -llept -ltesseract
.PHONY: clean

main: main.cpp tableau.h
	$(CC) $(TESSERACT) $(OPTIONS) $@ $<
clean: 
	rm *.o ; find . -maxdepth 1 -type f -executable -delete
