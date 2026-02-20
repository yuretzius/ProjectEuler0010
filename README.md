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

I used these functions to create a tool for writing primes into a file, which can then be read in other programs, so that I don't have
to worry about generating primes there. primes.py uses Erat, primes.cpp Erat2, I include a generated CSV file primes.csv with 664,579 primes below 10 million.
C++ version generates and records 455,052,511 primes under $10^{10}$ in 2.5 min (but the resulting file is almost 5GB, so it is better to generate it when
necessary rather than keep).

Another topic to mention here is, of course, the famous Lucy Hedgehog algorithm for summing primes, which [was published in the Project Euler problem 10 forum](https://projecteuler.net/thread=10;page=5#111677) (one has to be logged in and have solved the problem 10 to be able to see this post). It is incredibly efficient,
so I have included python and C++ implementations of it here, since I am using it in other problems. 

The algorithm sums primes without explicitly computing each of them. It is based on the sieve of Eratosthenes, but instead of actually applying it it follows what happens when the sieve is applied.

It considers the states of $S(v, p)$ when the sieve is run, and evolves them from number to number. $S(v, p)$ is the sum of integers below or equal to $v$ (including $v$ and excluding $1$) after sieving with integer $p$ (not necessarily a prime!). Two key observations are made: in the sieving (see e.g. the most obvious implementation in Erat here) we do changes starting from $p^2$ and above it, everything below $p^2$ is already sieved. And if $p$ is not prime, then $S(p, p-1) = S(p-1,p-1)$, which is a consequence of the former statement: after sieving with $p - 1$ $p$ is already in the fully sieved zone, and if it is not prime, it would not change the sum. For the same reason $S(p-1,p-1)$ is simply the sum of primes below or equal to $p-1$.

So we start by creating an array of numbers $v$, starting with the one we are interested in ( $n$ ) and down to $1$. Another key observation: the recursion we are using to evolve the sums $S(v,p)$ will only involve values of $\lfloor n/i \rfloor$ with $1 \le i \le \lfloor \sqrt{n} \rfloor$, all others are irrelevant. So there is no need to deal with the whole set of integers from $n$ down to $1$, the full set is needed only down from $\lfloor \sqrt{n} \rfloor$, and above $\lfloor n/i \rfloor$ are enough. 

Then we initate $S$ with what we can designate as $S(v, 1)$, i.e. full sums of integers: $S(v, 1) = v(v+1)/2 - 1$. E.g. if $n = 20$, we start with:

$$V = \\{20, 10, 6, 5, 4, 3, 2, 1 \\} \text{ and } S = \\{ 209, 54, 20, 14, 9, 5, 2, 0 \\}.$$

Now we start evolving the set $S$. We don't know any primes, so we simply cycle from $2$ to $\lfloor \sqrt{n} \rfloor$. After we're done with the last one, $n$ will be below the level of sieving, meaning the value of $S(n, p)$ will no longer change and we can stop and read it, it is the answer. And in the sieving we can stop when $v < p^2$: all the $S$ have already achieved their final value in the previous steps. The rule of $p^2$ is pretty neat, huh? It is useful in all kinds of ways.

Ok, now, how do we evolve $S$? As was already mentioned, first we check if $S(p, p-1) = S(p-1,p-1)$. If they do, $p$ is not prime and nothing will change, we simply go to the next one.

If  $S(p, p-1) > S(p-1,p-1)$ then we use the following:

$$S(v, p) = S(v, p-1) - p\\,[S(\lfloor v/p \rfloor, p-1) - S(p-1,p-1)].$$

To understand this relation we need to remember, what happens during sieving with a prime $p$: we remove all the numbers that $p$ divides, except those that have prime divisors below $p$, because those were already removed in the previous steps (and only the sum of primes, because, again, all non-primes were already removed). Suppose we deal with all integers below or equal to $10$, exluding $1$, i.e. $\\{2,3,4,5,6,7,8,9,10\\}$. And we sieve with $p = 3$. In the previous step of sieving with $2$ we have already eliminated $\\{2,4,6,8,10\\}$. We are left with $\\{3,5,7,9\\}$, of which we are going to remove $\\{ 3, 9 \\}$. So we remove all integers in the form $p \cdot i$ where $1 \leq i \leq \lfloor 10/3 \rfloor$, i.e. $\\{3 \cdot 1, 3 \cdot 2, 3 \cdot 3 \\}$. But now we've overdone it, because $3 \cdot 2$ was already removed when sieving with $2$, so we need to return it. $3$ can be pulled out in both cases, and we are left with the difference of $\\{1,2,3\\}$ and $\\{ 2\\}$.

In our example of $n = 20$ the evolution goes as follows. First we sieve with $2$. We check that $S(2) = 2$ is larger than $S(1) = 0$, so $2$ is a prime. We apply the above formula and now have:

$$S(20,2) = S(20,1) - 2 S(10,1) = 101, \\, S(10,2) = S(10,1) - 2 S(5,1) = 26, \\, S(6,2) = S(6,1) - 2 S(3,1) = 10,$$
$$S(5,2) = S(5,1) - 2 S(2,1) = 10, \\, S(4,2) =  S(4,1) - 2S(2,1) = 5.$$

We'ver reached $p^2 = 4$, the rest stays the same. The term $S(p-1,p-1)$ in this case is just $S(1,1) = 0$, which conforms with the fact that the sum of primes below $2$ is just $0$, there are no primes below $2$.

Now $p = 3$. Again check $S(3,2) = 5$ and $S(2,2) = 2$. The former is larger, so $p = 3$ is a prime. This time we'll have to take into account $S(p-1,p-1) = S(2,2) = 2$:

$$S(20,3) = S(20,2) - 3 [S(10,2) - S(2,2)] = 77, S(10,3) = S(10,2) - 3 [S(5,2) - S(2,2)] = 17.$$

And this is it, because the next value of $v$ is $6$, and that's already below $p^2 = 9$. The next $p = 4$. We check $S(4,3) = 5$, and it is the same as $S(3,3) = 5$, so $p = 4$ is not a prime, and we don't do anything. And here we stop, because next $p$ is 5, and $5^2 > 20$, so $p^2 > n$. We can just read the current value $S(20) = 77$, and that's the sum of primes we seek. Indeed: $2+3+5+7+11+13+17+19 = 77$. It still feels like magic somehow, even when you know the inner workings.

Some detailed analysis of the algorithm can be found e.g. [here](https://gbroxey.github.io/blog/2023/04/09/lucy-fenwick.html).

The bottleneck, if directly translated from python into C++, is that at some point the product $n(n+1)$ is evaluated, which limits $n < 4294967296$ due to overflow in 64-bit integers. So I made a version with 128-integers.

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