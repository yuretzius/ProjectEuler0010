import numpy as np
from tqdm import trange

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
    N_bool[2**2::2] = False # eliminating even numbers
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
    return np.nonzero(N_bool)[0]
    
    
N = 10000000

with open("primes.csv", "w") as f:
    primes = Erat(N)
    for i in trange(len(primes)-1, ascii = True):
        print(primes[i], end = ",\n", file = f)
    print(primes[-1], file = f)
