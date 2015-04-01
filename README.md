# FHE_Distributed
Test for distributed evaluation of functions applied with FHE encryption. This project studies the evaluation of the application of different functions available in a FHE scheme implemented in the HElib library of shaih and how it works in a distributed schemme.

Dependencies
------------
To use its necesary to have pre-installed:

1. GMP

  A. For UBUNTU: `sudo apt-get install libgmp3-dev`.

  B. For Others OS: 
    1. Download GMP from http://www.gmplib.org
    2. Uncompress and cd into the directory gmp-XXX
    3. On the command line: `./configure & make & sudo make install`
    4. This should install GMP into /usr/local

2. NTL (v6.1 or higher, we include v6.2)
  1. Navigate to /libs/ntl
  2. Uncompress and cd into the directory ntl-6.2.0/src
  3. On the command line: `./configure NTL_GMP_LIP=on &  make & make & sudo make install`
  4. This should install NTL into /usr/local

3. Compile HElib
  1. Navigate to libs/HElib at this project
  2. On the command line: `make`
  3. It will output the static library in file fhe.a
