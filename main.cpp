//
// Created by masno on 3/11/2023.
//

#include "main.h"

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int pronadjiNajveci(vector<int> a){
    int max=0;
    for(int i=0; i<a.size(); i++){
        if(a[i]>a[max]) max=i;
    }
    return a[max];
}

void Sortiraj(vector<int> &a, int broj){
    vector<int> temp(a.size());
    int brojac[10]={0};
    for (int i = 0; i < a.size(); i++) brojac[(a[i]/ broj)%10]++;
    for (int i = 1; i < 10; i++) brojac[i] += brojac[i - 1];
    for (int i = a.size() - 1; i >= 0; i--) {
        brojac[(a[i]/ broj )%10]--;
        temp[brojac[(a[i]/ broj )%10]] = a[i];
    }
    a = temp;
}

void radixSort(vector<int> &a){
    if(a.size()==0) return;
    int najveci=pronadjiNajveci(a);
    for(int i=1; najveci/i>0; i*=10){
        Sortiraj(a, i);
    }
}



void popraviDolje(vector<int> &a, int i) {
    while (i < a.size()/2) {
        int temp1 = 2*i+1, temp2 =2*i+2;
        if (temp2 < a.size() && a[temp2] > a[temp1]) temp1 = temp2;
        if (a[i] > a[temp1]) return;
        std::swap(a[i], a[temp1]);
        i = temp1;
    }
}

void popraviGore(vector<int> &a, int i) {
    while (i!=0 && a[i] > a[(i-1)/2]) {
        std::swap(a[i], a[(i-1)/2]);
        i=(i-1)/2;
    }
}
void stvoriGomilu(vector<int> &a){
    for(int i=a.size()/2; i>=0; i--){
        popraviDolje(a,i);
    }
}

void umetniUGomilu(vector<int> &a, int umetnuti, int &velicina) {
    a.push_back(umetnuti);
    velicina++;
    popraviGore(a, a.size()-1);
}

void gomilaSort(vector<int> &a) {
    stvoriGomilu(a);
    vector<int> pomocni(a.size());
    int vel(a.size());
    for (int i=0; i<vel; i++) {
        pomocni[i] = a[0];
        a.erase(a.begin());
        stvoriGomilu(a);
    }
    for (int i=0; i<vel; i++) a.push_back(pomocni[vel-i-1]);
}


int izbaciPrvi(vector<int> &a, int &velicina) {
    if (a.empty()) return -1;
    velicina--;
    int temp(a[0]);
    std::swap(a[0], a[velicina]);
    vector<int> v(velicina);
    for (int i=0; i<v.size(); i++) v[i]=a[i];
    stvoriGomilu(v);
    for (int i=0; i<v.size(); i++) a[i]=v[i];
    return temp;
}


int main() {
    vector<int> v={1, 5, 182, 65, 87, 234, 987, 530};
    radixSort(v);
    for(auto x:v){
        cout << x <<" ";
    }
    return 0;
}
