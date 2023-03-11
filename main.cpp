//
// Created by masno on 3/11/2023.
//

#include "main.h"



#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

template <typename Tip>
void bubble_sort(Tip* niz, int vel) {
    for (int i = 0; i < vel; i++) {
        for (int j = i+1; j < vel; j++) {
            if (niz[i] > niz[j]) {
                Tip temp = niz[j];
                niz[j] = niz[i];
                niz[i] = temp;
            }
        }
    }
}

template <typename Tip>
void selection_sort(Tip* niz, int vel) {
    for (int i = 0; i < vel - 1; i++) {
        Tip min = niz[i];
        int pmin = i;
        for (int j = i + 1; j < vel; j++) {
            if (niz[j] < min) {
                min = niz[j];
                pmin = j;
            }
        }
        niz[pmin] = niz[i];
        niz[i] = min;
    }

}
template <typename Tip>
void qsort(Tip* niz, int prvi, int zadnji) {
    int i = prvi, j = zadnji;
    Tip pivot(niz[(prvi + zadnji) / 2]), temp;
    while (i <= j) {
        while (niz[i] < pivot) i++;
        while (niz[j] > pivot) j--;
        if (i <= j) {
            temp = niz[i];
            niz[i] = niz[j];
            niz[j] = temp;
            i++; j--;
        }
    }
    if (prvi < j) qsort(niz, prvi, j);
    if (i < zadnji) qsort(niz, i, zadnji);
}

template <typename Tip>
void quick_sort(Tip* niz, int vel) {
    qsort(niz, 0, vel - 1);
}


template <typename Tip>
void merge(Tip* niz, int l, int p, int q, int u) {
    int i = 0, j = q - l, k(l);
    Tip* B = new Tip[u - l + 1];
    for (int m = 0; m <= u - l; m++)
        B[m] = niz[l + m];
    while (i <= p - l && j <= u - l) {
        if (B[i] < B[j])
            niz[k] = B[i++];
        else
            niz[k] = B[j++];
        k++;
    }
    while (i <= p - l)
        niz[k++] = B[i++];
    while (j <= u - l)
        niz[k++] = B[j++];
    delete[] B;
}

template <typename Tip>
void msort(Tip* niz, int l, int u) {
    if (u > l) {
        int p = (l + u - 1) / 2;
        int q = p + 1;
        msort(niz, l, p);
        msort(niz, q, u);
        merge(niz, l, p, q, u);
    }
}

template <typename Tip>
void merge_sort(Tip* niz, int vel) {
    msort(niz, 0, vel - 1);
}

void ucitaj(std::string filename, int*& niz, int& vel) {
    std::ifstream ulazniTok(filename);
    std::vector<int> v;
    int value;
    while (1) {
        if (!ulazniTok)
            break;
        ulazniTok >> value;
        v.push_back(value);
    }
    vel = v.size() - 1;
    niz = new int[vel];
    for (int i = 0; i < vel; i++) {
        niz[i] = v[i];
    }
}

void upisi(std::string filename, int*& niz, int& vel) {
    std::ofstream izlazniTok(filename);
    if (!izlazniTok) throw std::logic_error("Otvaranje:Greska");
    for (int i = 0; i < vel; i++) {
        izlazniTok << niz[i] << " ";
    }
}

void generisi(std::string filename, int vel) {
    std::ofstream izlazniTok(filename);
    srand(time(NULL));
    while (vel--)
        izlazniTok << 5 + rand() % 80 << " ";
}


void testBubbleSort() {
    int niz[10] = { 2,4,5,6,3,2,11,345,23,-23 };
    merge_sort(niz, 10);
    int ocekivani[10] = { -23,2,2,3,4,5,6,11,23,345 };
    for (int i = 0; i < 10; i++) {
        if (ocekivani[i] != niz[i]) {
            std::cout << "NE\n";
            return;
        }
    }
    std::cout << "DA\n";
}

void testSelectionSort() {
    int niz[10] = { 2,4,5,6,3,2,11,345,23,-23 };
    selection_sort(niz, 10);
    int ocekivani[10] = { -23,2,2,3,4,5,6,11,23,345 };
    for (int i = 0; i < 10; i++) {
        if (ocekivani[i] != niz[i]) {
            std::cout << "NE\n";
            return;
        }
    }
    std::cout << "DA\n";
}

void testQuickSort() {
    int niz[10] = { 2,4,5,6,3,2,11,345,23,-23 };
    quick_sort(niz, 10);
    int ocekivani[10] = { -23,2,2,3,4,5,6,11,23,345 };
    for (int i = 0; i < 10; i++) {
        if (ocekivani[i] != niz[i]) {
            std::cout << "NE\n";
            return;
        }
    }
    std::cout << "DA\n";
}


void testMergeSort() {
    int niz[10] = { 2,4,5,6,3,2,11,345,23,-23 };
    merge_sort(niz, 10);
    int ocekivani[10] = { -23,2,2,3,4,5,6,11,23,345 };
    for (int i = 0; i < 10; i++) {
        if (ocekivani[i] != niz[i]) {
            std::cout << "NE\n";
            return;
        }
    }
    std::cout << "DA\n";
}


void Funkcija(){



    int *niz, vel = 0, vel1;

    std::cout << "Unesite koliko želite brojeva u nizu da se generiše: " << std::endl;
    std::cin >> vel1;

    generisi("autotest.txt", vel1);

    ucitaj("autotest.txt", niz, vel);

    std::cout << "Generisani niz:" << std::endl;
    for(int i = 0; i < vel; i++){
        std::cout << niz[i] << " ";
    }

    std::cout << "\nIzaberite opciju:" << std::endl <<
              "1 - Bubble sort" << std::endl <<
              "2 - Selection sort" << std::endl <<
              "3 - Quick sort" << std::endl <<
              "4 - Merge sort" << std::endl;

    int opcija;
    std::cin >> opcija;
    if(opcija == 1)
    {
        clock_t vrijeme1 = clock();
        bubble_sort(niz, vel);
        clock_t vrijeme2 = clock();
        int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
        std::cout << "Vrijeme izvrsenja: " << ukvrijeme << " ms." << std::endl;

        for(int i(0); i<vel-1; i++)
            if(niz[i] > niz[i+1]) {
                std::cout << "Greska: i=" << i << " " << niz[i] << ">" << niz[i+1] << std::endl;
                return;
            }

        std::ofstream izlazni_tok("autotest.txt");
        for(int i = 1; i <= vel; i++)
            izlazni_tok << niz[i] << std::endl;

        std::cout << "Sortiran niz:" << std::endl;
        for(int i = 0; i < vel; i++){
            std::cout << niz[i] << " ";
        }

    }
    else if(opcija == 2)
    {
        clock_t vrijeme1 = clock();
        selection_sort(niz, vel);
        clock_t vrijeme2 = clock();
        int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
        std::cout << "Vrijeme izvrsenja: " << ukvrijeme << " ms." << std::endl;

        for(int i(0); i<vel-1; i++)
            if(niz[i] > niz[i+1]) {
                std::cout << "Greska: i=" << i << " " << niz[i] << ">" << niz[i+1] << std::endl;
                return;
            }

        std::ofstream izlazni_tok("autotest.txt");
        for(int i = 1; i <= vel; i++)
            izlazni_tok << niz[i] << std::endl;

        std::cout << "Sortiran niz:" << std::endl;
        for(int i = 0; i < vel; i++){
            std::cout << niz[i] << " ";
        }

    }
    else if(opcija == 3)
    {
        clock_t vrijeme1 = clock();
        quick_sort(niz, vel);
        clock_t vrijeme2 = clock();
        int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
        std::cout << "Vrijeme izvrsenja: " << ukvrijeme << " ms." << std::endl;

        for(int i(0); i<vel-1; i++)
            if(niz[i] > niz[i+1]) {
                std::cout << "Greska: i=" << i << " " << niz[i] << ">" << niz[i+1] <<std::endl;
                return;
            }

        std::ofstream izlazni_tok("autotest.txt");
        for(int i = 1; i <= vel; i++)
            izlazni_tok << niz[i] << std::endl;

        std::cout << "Sortiran niz:" << std::endl;
        for(int i = 0; i < vel; i++){
            std::cout << niz[i] << " ";
        }

    }
    else if(opcija == 4)
    {
        clock_t vrijeme1 = clock();
        merge_sort(niz, vel);
        clock_t vrijeme2 = clock();
        int ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
        std::cout << "Vrijeme izvrsenja: " << ukvrijeme << " ms." << std::endl;

        for(int i(0); i<vel-1; i++)
            if(niz[i] > niz[i+1]) {
                std::cout << "Greska: i=" << i <<" " << niz[i] << ">" << niz[i+1] << std::endl;
                return;
            }

        std::ofstream izlazni_tok("autotest.txt");
        for(int i = 1; i <= vel; i++)
            izlazni_tok << niz[i] << std::endl;

        std::cout << "Sortiran niz:" << std::endl;
        for(int i = 0; i < vel; i++){
            std::cout << niz[i] << " ";
        }

    }else{
        std::cout << "Neispravna opcija!" << std::endl;
    }
}

int main()
{
    testBubbleSort();
    testMergeSort();
    testSelectionSort();
    testQuickSort();
    Funkcija();
}
