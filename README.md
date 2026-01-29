# ProjectEuler0010
My work on 

problem #10 of projecteuler.net:

Summation of Primes

The sum of the primes below $10$ is $2 + 3 + 5 + 7 = 17$.
Find the sum of all the primes below two million.

Completed on Sun, 21 Feb 2021, 00:37

#######################################################################################
Please note: Project Euler's policy allows publication of solutions for the first 100 problems,
that's why I am sharing my work here for reference and educational purposes.
#######################################################################################

In problems 7 and 9 I have already built myself two prime calculating functions implementing the sieve of Eratosthenes, Erat and EratM.
This problem is an excellent excuse to build myself another one :)

The idea is that since starting from 5 all prime candidates come in pairs $6k \pm 1$, $k \in \mathbb{N}$,
we do not need to use an $N$-sized boolean array: most of its space is wasted for numbers divisible by 2 and 3, which
we do not really use when sieving. Instead we can have two $N/6$-sized arrays, for lower and higher candidates of each pair.
This, of course, creates the need for a lot of index arithmetic, but these operations are cheap. Worse, this makes it
impossible to use efficient NumPy array operations in Python, instead we have to rely on several for/while cycles and condion checks.

The resulting Erat2 is rather slow in Python, actually 10 times slower than Erat and EratM. But unlike those two it does accept $N = 10^{10}$,
so saving on memory pays off. Of course, it takes it about 3 hours to finish (ouch!). On the other hand, the C++ version of Erat2 is 1.5 times
MORE efficient than Erat and EratM. It crunches through 10 billion in about 90 seconds.

The actual problem of summing all primes below 2 million all three functions solve with ease (Python's Erat2 takes more than a second,
compared to 0.15 sec for Erat and EratM, but in C++ all three take ~15ms).
