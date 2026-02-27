from time import perf_counter

def LucyHedgehog(n):
    '''
    extremely efficient algorithm
    for the sum of primes 
    below or equal to n 
    '''
    r = int(n**0.5)
    # assert r*r <= n and (r+1)**2 > n
    V = [n//i for i in range(1,r+1)] # only need integers n//i, because only they are called in recursion
    V += list(range(V[-1]-1,0,-1)) # continue the list to 1
    # so for n = 10 it is [10, 5, 3, 2, 1]
    S = {v:v*(v+1)//2-1 for v in V} # sum of all integers below value except 1
    # so for n = 10 it is {10: 54, 5: 14, 3: 5, 2: 2, 1: 0}
    for p in range(2,r+1):
        if S[p] > S[p-1]:  # p is prime
            sp = S[p-1]    # sum of primes smaller than p
            p2 = p*p
            for v in V:
                if v < p2: break
                S[v] -= p*(S[v//p] - sp) # main recursive formula: S(v,p)=S(v,p−1)−p(S(vp,p−1)−S(p−1,p−1)).
    return S[n]

# this is the prime COUNTING version
def LucyHedgehogNum(n):
    '''
    extremely efficient algorithm
    for the *number* of primes 
    below or equal to n 
    '''
    if n == 0: return 0
    if n < 4: return n-1
    r = int(n**0.5)
    # assert r*r <= n and (r+1)**2 > n
    V = [n//i for i in range(1,r+1)] # only need integers n//i, because only they are called in recursion
    V += list(range(V[-1]-1,0,-1)) # continue the list to 1
    # so for n = 10 it is [10, 5, 3, 2, 1]
    S = {v:v-1 for v in V} # count of all integers below value except 1
    # so for n = 10 it is {10: 9, 5: 4, 3: 2, 2: 1, 1: 0}
    for p in range(2,r+1):
        if S[p] > S[p-1]:  # p is prime
            sp = S[p-1]    # count of primes smaller than p
            p2 = p*p
            for v in V:
                if v < p2: break
                S[v] -= (S[v//p] - sp) # main recursive formula: S(v,p)=S(v,p−1)−(S(vp,p−1)−S(p−1,p−1)).
    return S[n]

n = 10**13

start = perf_counter()
res = LucyHedgehog(n)
end = perf_counter()
print("\nsum of primes below or equal to {} is {}, time {} sec".format(n, res, end - start))

# sum of primes below or equal to 10000000000000 is 1699246443377779418889494, time 267.06651339999996 sec

