import numpy as np
from time import perf_counter

def Erat(N):
    """
    The Sieve of Eratosthenes implemented with numpy arrays
    Returns the list of primes lower or equal than N
    the simplest with step 2
    """
    if N == 0 or N == 1 : return []
    elif N == 2: return [2]
    elif N == 3: return [2,3]
    N_bool = np.array([True]*(N+1))
    N_bool[0] = False
    N_bool[1] = False
    #N_bool[2] = True
    #N_bool[3] = True
    N_bool[2**2::2] = False # eiminating even numbers
    p = 3
    while p*p <= N:
        # start from p**2
        # because all the smaller composites have factors <p
        # and are already eliminated in previous steps
        N_bool[p**2::p] = False # python doesn't care if ::p goes beyond existing array
        p = p + 2 # only odd numbers can be primes larger than 3
        while not N_bool[p]:
            p = p + 2 # skip p if it has already been eliminated in previous steps
    # returns indices of nonzero elements, which in this case
    # ARE the correcponding natural numbers, which were not eliminated
    # Have to use index [0], because for technical reasons it produces a 2D array
    return list(np.nonzero(N_bool)[0])

def EratM(N):
    """
    The Sieve of Eratosthenes implemented with numpy arrays
    Returns the list of primes lower or equal than N
    modification with step 6
    """
    if N < 11:
        first_primes = ([],[],[2],[2,3],[2,3],[2,3,5],[2,3,5],[2,3,5,7],[2,3,5,7],[2,3,5,7],[2,3,5,7])
        return(first_primes[N]) 
    N_bool = np.array([True]*(N+1))
    N_bool[0] = False
    N_bool[1] = False
    N_bool[2**2::2] = False # eiminating even numbers
    N_bool[3**2::3] = False # eiminating multiples of 3
    p = 5
    while p*p <= N:
        # start from p**2
        # because all the smaller composites have factors <p
        # and are already eliminated in previous steps
        if N_bool[p]:
            N_bool[p**2::p] = False # python doesn't care if ::p goes beyond existing array
        if N_bool[p+2]:
            N_bool[(p+2)**2::(p+2)] = False
        p = p + 6 # we can move in steps of 6
    # returns indices of nonzero elements, which in this case
    # ARE the correcponding natural numbers, which were not eliminated
    # Have to use index [0], because for technical reasons it produces a 2D array
    return list(np.nonzero(N_bool)[0]) 

def Erat2(N):
    """
    The Sieve of Eratosthenes implemented with numpy arrays
    Returns the list of primes lower or equal than N
    Only tracks candidates 6k+1 and 6k-1
    Thus should be slightly more memory-efficient 
    """
    if N < 5:
        first_primes = ([],[],[2],[2,3],[2,3])
        return(first_primes[N]) 
    
    # only taking care of primes starting from 5 and 7
    # which all are in form 6k - 1 and 6k + 1
    top_index = N//6 # max k
    low_cand = np.array([True]*(top_index)) # to track 6k-1 candidates
    high_cand = np.array([True]*(top_index)) # to track 6k+1 candidates
    k = 1
    p = 6*k     
    while (p-1)*(p-1) <= N: # if even the lower part of the pair <= N, go in the cycle
        # 6*k - 1 sieve
        if low_cand[k-1]:
            p_low = p - 1
            t = p_low * p_low # start with p*p, as usual
            while t <= N: 
                # take action only if there is an index solution
                # for either low or high candidate                
                if (t + 1) % 6 == 0:
                    i = (t + 1)//6
                    low_cand[i-1] = False
                if (t - 1) % 6 == 0:
                    i = (t - 1)//6
                    high_cand[i-1] = False
                t += p_low
        # 6*k + 1 sieve
        if high_cand[k-1]:
            p_high = p + 1
            t = p_high * p_high # start with p*p, as usual
            while t <= N: 
                # take action only if there is an index solution
                # for either low or high candidate                
                if (t + 1) % 6 == 0:
                    i = (t + 1)//6
                    low_cand[i-1] = False
                if (t - 1) % 6 == 0:
                    i = (t - 1)//6
                    high_cand[i-1] = False
                t += p_high     
                
        k += 1 # go to the next pair
        p = 6*k 
        
    primes = [2,3] # by default
    for i in range(1,top_index+1):
        # convert the candidate arrays into actual numbers
        if low_cand[i-1]:
            primes.append(6*i - 1)
        if high_cand[i-1]:
            primes.append(6*i + 1)
    if primes[-1] > N: # need to do this check since the top of the last pair can cross the threshold
        primes.pop() # remove the last item
    return primes


N = 2*10**6

start = perf_counter()
res = sum([int(u) for u in Erat(N)]) # must recast as python's int to avoid overflow
end = perf_counter()

print(res)
print('Erat:', end - start,'sec')

start = perf_counter()
res = sum([int(u) for u in EratM(N)])
end = perf_counter()

print(res)
print('EratM:', end - start,'sec')

start = perf_counter()
res = sum([int(u) for u in Erat2(N)])
end = perf_counter()

print(res)
print('Erat2:', end - start,'sec')

