CC = g++
CFLAGS = -g -O2 -Wfatal-errors -Wshadow -Wall -I/usr/local/include 
GMP=-lgmp 
LDLIBS = -L/usr/local/lib -lntl $(GMP) -lm
FHELIB = ../libs/HElib/fhe.a

paramsGenerator: paramsGenerator.cpp
	$(CC) $(CFLAGS) -o paramsGenerator.run $< $(FHELIB) $(LDLIBS)

clean:
	rm *.run *.key *.contex