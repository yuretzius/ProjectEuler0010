#include <vector>
#include <iostream>
#include <unordered_map>
#include <chrono>

using namespace std;

uint64_t floorSqrt(uint64_t n) {
    // initial search space
    uint64_t lo = 1, hi = n;
    uint64_t res = 1;
    
    while(lo <= hi) {
        uint64_t mid = lo + (hi - lo)/2;
        
        // if square of mid is less than or equal to n 
        // update the result and search in upper half
        if(mid*mid <= n) {
            res = mid;
            lo = mid + 1;
        }
        
        // if square of mid exceeds n, 
      	// search in the lower half
        else {
            hi = mid - 1;
        }
    }
    
    return res;
}


uint64_t LucyHedgehog(uint64_t n){
    /*
    extremely efficient algorithm
    for the sum of primes 
    below or equal to n 
    
    LIMIT OF THIS FUNCTION n = 4294967295
    
    */
    
    uint64_t sqn = floorSqrt(n);
    uint64_t p2, sp, val;
    vector<uint64_t> V;
    for (uint64_t i = 1; i < sqn + 1; i++){
        V.push_back(n/i);
    }
    for (uint64_t i = *(V.end()-1)-1; i > 0; i--){
        V.push_back(i);
    }
    // so e.g. for n = 10 V is {10, 5, 3, 2, 1}
    unordered_map<uint64_t, uint64_t> S;
    for (vector<uint64_t>::iterator it = V.begin(); it < V.end(); it++) {
        // can rewrite this to ensure first of division of either *it or *it + 1
        // this will improve the max possible value of n to 
        val = *it * (*it + 1) / 2 - 1;
        S.insert({*it, val});
    }
    // so for n = 10 it is {{10, 54}, {5, 14}, {3, 5}, {2, 2}, {1, 0}}
    for (uint64_t p = 2; p < sqn + 1; p++) {
        if(S[p] > S[p-1]) {
            sp = S[p-1]; // sum of primes smaller than p
            p2 = p*p;
            for (vector<uint64_t>::iterator it = V.begin(); it < V.end(); it++) {
                if (*it < p2) break;
                S[*it] -= p*(S[*it/p] - sp); // main recursive formula
            }
        }
    }
    return S[n];
}

// this is the prime COUNTING version
uint64_t LucyHedgehogNum(uint64_t n){
    /*
    extremely efficient algorithm
    for the *number* of primes 
    below or equal to n 
    */
    
    uint64_t sqn = floorSqrt(n);
    uint64_t p2, sp, val;
    vector<uint64_t> V;
    for (uint64_t i = 1; i < sqn + 1; i++){
        V.push_back(n/i);
    }
    for (uint64_t i = *(V.end()-1)-1; i > 0; i--){
        V.push_back(i);
    }
    // so e.g. for n = 10 V is {10, 5, 3, 2, 1}
    unordered_map<uint64_t, uint64_t> S;
    for (vector<uint64_t>::iterator it = V.begin(); it < V.end(); it++) {
        val = *it - 1;
        S.insert({*it, val});
    }
    // so for n = 10 it is {{10, 9}, {5, 4}, {3, 2}, {2, 1}, {1, 0}}
    for (uint64_t p = 2; p < sqn + 1; p++) {
        if(S[p] > S[p-1]) {
            sp = S[p-1]; // count of primes smaller than p
            p2 = p*p;
            for (vector<uint64_t>::iterator it = V.begin(); it < V.end(); it++) {
                if (*it < p2) break;
                S[*it] -= (S[*it/p] - sp); // main recursive formula
            }
        }
    }
    return S[n];
}


int main(){
    uint64_t n = 4294967295;
    auto start = chrono::high_resolution_clock::now();
    uint64_t res = LucyHedgehog(n);
    auto end = chrono::high_resolution_clock::now();
    auto time = chrono::duration<double, milli>(end - start).count();
    cout << "\nsum of primes below or equal to " << n << " is " << res <<", time " << time <<" ms" << endl;
    cout<< "\nNOTE: this is a 64bit version, good for n < 4294967296, because of n(n+1)/2 overflow" << endl;
}

// sum of primes below or equal to 4294967295 is 425649736193687430, time 338.966 ms

