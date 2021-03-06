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
#time ./dataProcessor.run --mnc 1 --lac 13527 --cid 85696527 --file ../data/data.cipher
#time ./dataProcessor.run --mnc 1 --lac 13527 --cid 85696524 --file ../data/data.cipher
time ./dataProcessor.run --mnc 1 --lac 5514 --cid 33186528 --file ../data/data.cipher
echo "Homomorphic Processing Done"
echo

echo "Begin Decryption Processing..."
time ./dataDecryptor.run  --file ../data/processed.cipher
echo "Decryption Processing Done"
echo
