# FHE_Distributed
Test for distributed evaluation of functions applied with FHE encryption. This project studies the evaluation of the application of different functions available in a FHE scheme implemented in the HElib library of shaih and how it works in a distributed schemme.

To use its necesary have pre-installed:

1. GMP
⋅⋅A. For UBUNTU: sudo apt-get install libgmp3-dev
⋅⋅B. For Others OS: 
----1. Download GMP from http://www.gmplib.org
----2. Uncompress and cd into the directory gmp-XXX
----3. On the command line:
		./configure
		make
		sudo make install
1.B.d. This should install GMP into /usr/local

2. NTL (v6.1 or higher, we include v6.2)
2.1. Navigate to /libs/ntl
2.2. Uncompress and cd into the directory ntl-6.2.0/src
2.3. On the command line:
		./configure NTL_GMP_LIP=on
		make
		sudo make install
2.4. This should install NTL into /usr/local

3. Compile HElib
3.1. Navigate to libs/HElib at this project
3.2. On the command line:
		make
3.3. It will output the static library in file fhe.a


1. First ordered list item
2. Another item
⋅⋅* Unordered sub-list. 
1. Actual numbers don't matter, just that it's a number
⋅⋅1. Ordered sub-list
4. And another item.
