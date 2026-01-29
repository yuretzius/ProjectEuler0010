#include <vector>
#include <iostream>
#include <numeric>

using namespace std;

/* this is basically Python script rewritten with C++ vectors etc.*/

//the sieve of Eratosthenes, the limit by both memory and time is about 10^10
// older version with step 2
// (10^10 takes about 137 seconds)
vector<unsigned long> Erat(unsigned long N){
    vector<unsigned long> primes;
    if (N == 0 || N == 1) return primes;
    primes.push_back(2);
    if (N == 2) return primes;
    primes.push_back(3);
    if (N == 3) return primes;
    vector<bool> Nbool(N+1, true);
    vector<bool>::iterator itr = Nbool.begin();
    *itr = false;
    *(itr+1) = false;
    //*(itr+2) = true;
    //*(itr+3) = true;
    for (itr = Nbool.begin() + 4; itr < Nbool.end(); itr += 2) *itr = false;
    unsigned long p = 3;
    while (p*p <= N) {
        for (itr = Nbool.begin() + p*p; itr < Nbool.end(); itr += p) *itr = false;
        p += 2;
        while (! Nbool[p]) p += 2;
    }
    for (itr = Nbool.begin()+5; itr < Nbool.end(); itr += 2) {
        if (*itr) primes.push_back(itr - Nbool.begin());
    }
    return primes;
}

//the sieve of Eratosthenes, the limit by both memory and time is about 10^10
// newer version with step 6
// (10^10 takes about 132 seconds)
vector<unsigned long> EratM(unsigned long N){
    vector<unsigned long> primes;
    if (N < 11) {
        if (N < 2) return primes;
        primes.push_back(2);
        if (N == 2) return primes;
        primes.push_back(3);
        if (N < 5) return primes;
        primes.push_back(5);
        if (N < 7) return primes;
        primes.push_back(7);
        return primes;
    }
    vector<bool> Nbool(N+1, true);
    vector<bool>::iterator itr = Nbool.begin();
    *itr = false;
    *(itr+1) = false;
    for (itr = Nbool.begin() + 4; itr < Nbool.end(); itr += 2) *itr = false;
    for (itr = Nbool.begin() + 9; itr < Nbool.end(); itr += 3) *itr = false;
    unsigned long p = 5;
    while (p*p <= N) {
        if (Nbool[p]) {
            for (itr = Nbool.begin() + p*p; itr < Nbool.end(); itr += p) *itr = false;
        }
        if (Nbool[p+2]) {
            for (itr = Nbool.begin() + (p+2)*(p+2); itr < Nbool.end(); itr += p+2) *itr = false;
        }
        p += 6;
    }
    primes.push_back(2);
    for (itr = Nbool.begin()+3; itr < Nbool.end(); itr += 2) {
        if (*itr) primes.push_back(itr - Nbool.begin());
    }
    return primes;
}

//the sieve of Eratosthenes, the limit by both memory and time is about 10^10
// Returns the list of primes lower or equal than N
// Only tracks candidates 6k+1 and 6k-1
// Thus should be slightly more memory-efficient 
// (10^10 takes about 92 seconds)

vector<unsigned long> Erat2(unsigned long N) {
    vector<unsigned long> primes;
    if (N < 5) {
        if (N < 2) return primes;
        primes.push_back(2);
        if (N == 2) return primes;
        primes.push_back(3);
        return primes; 
    }
    // only taking care of primes starting from 5 and 7
    // which all are in form 6k - 1 and 6k + 1
    unsigned long top_index = N / 6; // max k
    vector<bool> low_cand (top_index, true); // to track 6k-1 candidates
    vector<bool> high_cand (top_index, true); // to track 6k+1 candidates
    vector<bool>::iterator itr = low_cand.begin();
    unsigned long k = 1;
    unsigned long p = 6*k ; 
    unsigned long p_low, p_high, t, i;
    p_low = p_high = t = i = 0;    
    while ((p-1)*(p-1) <= N) 
    // if even the lower part of the pair <= N, go in the cycle
    {
        // 6*k - 1 sieve
        if (low_cand[k-1]) {
            p_low = p - 1;
            t = p_low * p_low; // start with p*p, as usual
            while (t <= N) { 
                // take action only if there is an index solution
                // for either low or high candidate                
                if ((t + 1) % 6 == 0) {
                    i = (t + 1)/6;
                    itr = low_cand.begin() + i - 1;
                    *itr = false;
                }
                if ((t - 1) % 6 == 0) {
                    i = (t - 1)/6 ;
                    itr = high_cand.begin() + i - 1;
                    *itr = false;
                }
                t += p_low;
            }
        }
        // 6*k + 1 sieve
        if (high_cand[k-1]) {
            p_high = p + 1;
            t = p_high * p_high; // start with p*p, as usual
            while (t <= N) { 
                // take action only if there is an index solution
                // for either low or high candidate                
                if ((t + 1) % 6 == 0) {
                    i = (t + 1)/6;
                    itr = low_cand.begin() + i - 1;
                    *itr = false;
                }
                if ((t - 1) % 6 == 0) {
                    i = (t - 1)/6;
                    itr = high_cand.begin() + i - 1;
                    *itr = false;
                }
                t += p_high; 
            }                
        }       
        k ++; // go to the next pair
        p = 6*k; 
    }
    primes.push_back(2);
    primes.push_back(3);
    for (i = 1; i != top_index+1; i++) {
        // convert the candidate arrays into actual numbers
        if (low_cand[i-1])  {primes.push_back(6*i - 1);}
        if (high_cand[i-1]) {primes.push_back(6*i + 1);}
    }
    if (*(primes.end() - 1) > N) 
    // need to do this check since the top of the last pair can cross the threshold
    {
        primes.pop_back(); // remove the last item
    }      
    return primes;
}



int main(){
    
    unsigned long N = 2000000;
    unsigned long summa;
    clock_t c_start = clock();
    vector <unsigned long> primes = Erat(N);
    summa = accumulate(primes.begin(), primes.end(), (unsigned long) 0);
    clock_t c_end = clock();
    cout << 1000.0*(c_end - c_start) / CLOCKS_PER_SEC << " ms\n";
    cout << (c_end - c_start) / CLOCKS_PER_SEC << " sec\n";
    cout << "Erat's result " << summa << endl;
    cout << endl;
    c_start = clock();
    primes = EratM(N);
    summa = accumulate(primes.begin(), primes.end(), (unsigned long) 0);
    c_end = clock();
    cout << 1000.0*(c_end - c_start) / CLOCKS_PER_SEC << " ms\n";
    cout << (c_end - c_start) / CLOCKS_PER_SEC << " sec\n";
    cout << "EratM's result " << summa << endl;
    cout << endl;
    c_start = clock();
    primes = Erat2(N);
    summa = accumulate(primes.begin(), primes.end(), (unsigned long) 0);
    c_end = clock();
    cout << 1000.0*(c_end - c_start) / CLOCKS_PER_SEC << " ms\n";
    cout << (c_end - c_start) / CLOCKS_PER_SEC << " sec\n";
    cout << "Erat2's result " << summa << endl;
    cout << endl;
   
    return 0;
}
