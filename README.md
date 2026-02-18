# ProjectEuler0010
My work on 

[problem #10 of projecteuler.net](https://projecteuler.net/problem=10):

Summation of Primes

The sum of the primes below $10$ is $2 + 3 + 5 + 7 = 17$.
Find the sum of all the primes below two million.

Completed on Sun, 21 Feb 2021, 00:37

#######################################################################################
Please note: Project Euler's policy allows publication of solutions for the first 100 problems,
that's why I am sharing my work here for reference and educational purposes.
#######################################################################################

In problems [7](https://github.com/yuretzius/ProjectEuler0007) and [9](https://github.com/yuretzius/ProjectEuler0009) I have already built myself two prime calculating functions implementing the sieve of Eratosthenes, Erat and EratM.
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

I used these functions to create a tool for writing primes into a file, which can then be read in other programms, so that I don't have
to worry about generating primes there. primes.py uses Erat, primes.cpp Erat2, I include a generated CSV file primes.csv with 664,579 primes below 10 million.
C++ version generates and records 455,052,511 primes under $10^{10}$ in 2.5 min (but the resulting file is almost 5GB, so it is better to generate it when
necessary rather than keep).

Another topic to mention here is, of course, the famous Lucy Hedgehog algorithm for summing primes, which [was published in the Project Euler problem 10 forum](https://projecteuler.net/thread=10;page=5#111677) (one has to be logged in and have solved the problem 10 to be able to see this post). It is incredibly efficient,
so I have included python and C++ implementations of it here, since I am using it in other problems. 

Not necessary here, but it may be handy later: a hash function for 128-bit integers.

```C++
// custom hash function for uint128, to use as a key in unordered map
template <>
struct std::hash<uint128>
{
    std::size_t operator()(const uint128& k) const noexcept
    {
        uint64_t high = static_cast<uint64_t>(k >> 64);
        uint64_t low  = static_cast<uint64_t>(k);

        uint64_t seed = high;
        seed ^= low + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2); // the constant is the golden ratio mix constant used by Boost
        return static_cast<std::size_t>(seed);
    }
};
```