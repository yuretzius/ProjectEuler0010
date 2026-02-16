#include <vector>
#include <iostream>
#include <fstream> 
#include <chrono>
#include <string>

using namespace std;

//the sieve of Eratosthenes, the limit by both memory and time is about 10^10
// Returns the list of primes lower or equal than N
// Only tracks candidates 6k+1 and 6k-1
// Thus should be slightly more memory-efficient 
// (10^10 takes about 92 seconds)

vector<uint64_t> Erat2(uint64_t N) {
    vector<uint64_t> primes;
    if (N < 5) {
        if (N < 2) return primes;
        primes.push_back(2);
        if (N == 2) return primes;
        primes.push_back(3);
        return primes; 
    }
    // only taking care of primes starting from 5 and 7
    // which all are in form 6k - 1 and 6k + 1
    uint64_t top_index = N / 6; // max k
    vector<bool> low_cand (top_index, true); // to track 6k-1 candidates
    vector<bool> high_cand (top_index, true); // to track 6k+1 candidates
    vector<bool>::iterator itr = low_cand.begin();
    uint64_t k = 1;
    uint64_t p = 6*k ; 
    uint64_t p_low, p_high, t, i;
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
    
    uint64_t N = 10000000;
    string filename = "primes1.csv";
    auto start = chrono::high_resolution_clock::now();
    vector <uint64_t> primes = Erat2(N);
    vector <uint64_t>::iterator it;
    cout<< "\nrecording " << primes.size() << " primes into file " << filename << endl;
    ofstream wfile(filename);
    for (it = primes.begin(); it != primes.end(); it++) {
        wfile << *it <<",\n";
    }
    wfile.close();
    auto end = chrono::high_resolution_clock::now();
    auto time = chrono::duration<double, milli>(end - start).count();
    cout << time << " ms" << endl;
   
    return 0;
}
