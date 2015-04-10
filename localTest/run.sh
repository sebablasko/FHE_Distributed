#!/bin/bash

echo "Begin Compilation..."
make all
echo "Compilation Done"
echo

echo "Begin Parameter Generation..."
START=$(date +%s)
./paramsGenerator.run
END=$(date +%s)
DIFF=$(( $END - $START ))
echo "Parameter Generation Done ("$DIFF")"
echo

echo "Begin Encryption Process..."
START=$(date +%s)
./dataEncryptor.run
END=$(date +%s)
DIFF=$(( $END - $START ))
echo "Encryption Process Done ("$DIFF")"
echo

echo "Begin Homomorphic Processing..."
START=$(date +%s)
./dataProcessor.run
END=$(date +%s)
DIFF=$(( $END - $START ))
echo "Homomorphic Processing Done ("$DIFF")"
echo

echo "Begin Decryption Processing..."
START=$(date +%s)
./dataDecryptor.run
END=$(date +%s)
DIFF=$(( $END - $START ))
echo "Decryption Processing Done ("$DIFF")"
echo
