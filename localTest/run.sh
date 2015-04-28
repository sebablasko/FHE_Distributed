#!/bin/bash

echo "Begin Compilation..."
make all
echo "Compilation Done"
echo

echo "Begin Parameter Generation..."
time ./paramsGenerator.run
echo "Parameter Generation Done"
echo

echo "Begin Encryption Process..."
time ./dataEncryptor.run
echo "Encryption Process Done"
echo

echo "Begin Homomorphic Processing..."
time ./dataProcessor.run 1 13527 85696527
#time ./dataProcessor.run 1 13527 85696524
#time ./dataProcessor.run 1 13527 33186528
echo "Homomorphic Processing Done"
echo

echo "Begin Decryption Processing..."
time ./dataDecryptor.run
echo "Decryption Processing Done"
echo
