CC = g++
CFLAGS = -g -O2 -Wfatal-errors -Wshadow -Wall -I/usr/local/include 
GMP=-lgmp 
LDLIBS = -L/usr/local/lib -lntl $(GMP) -lm
FHELIB = ../libs/HElib/fhe.a

all: paramsGenerator dataEncryptor dataProcessor dataDecryptor

paramsGenerator: paramsGenerator.cpp
	$(CC) $(CFLAGS) -o paramsGenerator.run $< $(FHELIB) $(LDLIBS)

dataEncryptor: dataEncryptor.cpp
	$(CC) $(CFLAGS) -o dataEncryptor.run $< $(FHELIB) $(LDLIBS)

dataProcessor: dataProcessor.cpp
	$(CC) $(CFLAGS) -o dataProcessor.run $< $(FHELIB) $(LDLIBS)

dataDecryptor: dataDecryptor.cpp
	$(CC) $(CFLAGS) -o dataDecryptor.run $< $(FHELIB) $(LDLIBS)

wea: prueba.cpp
	$(CC) $(CFLAGS) -o wea.run $< $(FHELIB) $(LDLIBS)	

clean:
	rm *.run *.key *.context