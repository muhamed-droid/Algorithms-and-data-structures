//
// Created by masno on 3/11/2023.
//

#include "main.h"

#include <iostream>

using namespace std;

int fib2_0(int n, int a=1, int b=1){
    if(n<=1) return n;
    if(n==2) return a;
    return fib2_0(n-1,a+b,a);
}

int nzd(int x, int y){
    if(y==0) return x;
    return nzd(y, x%y);
}

int main() {
    std::cout << "Pripremna Zadaca Za Vjezbu 5, Zadatak 1";
    return 0;
}
