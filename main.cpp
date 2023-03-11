//
// Created by masno on 3/11/2023.
//

#include "main.h"


// pripremnaZadaca9.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <functional>
#include <ctime>

using namespace std;


unsigned int hash(std::string ulaz, int max) {
    unsigned int suma(1312);
    for (int i=0; i < ulaz.length() / 2; i++)
        suma += ulaz[i] * 13 + ulaz[ulaz.length() - 1 - i] * 21;
    return suma % max;
}

unsigned int hash_1(int ulaz, int max) {
    unsigned int suma(111);
    suma = ulaz * 21 + ulaz * 13;
    return suma % max;
}



template <typename TipKljuca, typename TipVrijednosti>
class Mapa {
public:
    Mapa() {};
    virtual ~Mapa() {};
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca& kljuc) = 0;
    virtual TipVrijednosti& operator[](const TipKljuca& k) = 0;
    virtual TipVrijednosti operator[](const TipKljuca& k) const = 0;
};


template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
    struct Cvor {
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor* lijevoDijete, * desnoDijete, * roditelj;
        Cvor(Cvor* p, TipKljuca key, TipVrijednosti value) {
            kljuc = key;
            vrijednost = value;
            roditelj = p; lijevoDijete = nullptr; desnoDijete = nullptr;
        }
    };
    Cvor* korijen;
    int velicina;

    void brisiSVrha(Cvor*& korijen) {
        if (korijen != nullptr) {
            (korijen->lijevoDijete);
            brisiSVrha(korijen->lijevoDijete);
            brisiSVrha(korijen->desnoDijete);
            delete korijen;
            korijen = nullptr;
            velicina--;
        }
    }

    Cvor* funkcija(Cvor* beginParent, Cvor*& begin, TipKljuca key) {
        if (begin == nullptr) {
            begin = new Cvor(beginParent, key, TipVrijednosti());
            velicina++;
            return begin;
        }
        if (begin->kljuc == key) return begin;
        else if (begin->kljuc > key) return funkcija(begin, begin->lijevoDijete, key);
        else return funkcija(begin, begin->desnoDijete, key);
    }

    TipVrijednosti NadjiVrijednost(Cvor* korijen, TipKljuca key) const { // edo 'const'
        if (korijen == nullptr) {
            return TipVrijednosti();
        }
        if (korijen->kljuc == key) return korijen->vrijednost;
        else if (korijen->kljuc > key) return NadjiVrijednost(korijen->lijevoDijete, key);
        else return NadjiVrijednost(korijen->desnoDijete, key);
    }

public:
    BinStabloMapa() {
        korijen = nullptr;
        velicina = 0;
    }

    ~BinStabloMapa() {
        obrisi();
    }

    void obrisi() override {
        brisiSVrha(korijen);
    }

    void obrisi(const TipKljuca& kljuc) override {
        Cvor* p = korijen;
        Cvor* roditelj = nullptr;
        Cvor* tmp1 = nullptr, * tmp2 = nullptr, * tmp3 = nullptr;
        while (p != nullptr && kljuc != p->kljuc) {
            roditelj = p;
            if (kljuc < p->kljuc) p = p->lijevoDijete;
            else p = p->desnoDijete;
        }
        if (p == nullptr) return;
        if (p->lijevoDijete == nullptr) tmp1 = p->desnoDijete;
        else if (p->desnoDijete == nullptr) tmp1 = p->lijevoDijete;
        else {
            tmp2 = p;
            tmp1 = p->lijevoDijete;
            tmp3 = tmp1->desnoDijete;
            while (tmp3 != nullptr) {
                tmp2 = tmp1;
                tmp1 = tmp3;
                tmp3 = tmp1->desnoDijete;
            }
            if (tmp2 != p) {
                tmp2->desnoDijete = tmp1->lijevoDijete;
                tmp1->lijevoDijete = p->lijevoDijete;
            }
            tmp1->desnoDijete = p->desnoDijete;
        }
        if (roditelj == nullptr) korijen = tmp1;
        else if (p == roditelj->lijevoDijete) roditelj->lijevoDijete = tmp1;
        else roditelj->desnoDijete = tmp1;
        delete p;
        velicina--;
    }

    int brojElemenata() const override { return velicina; }

    TipVrijednosti& operator[](const TipKljuca& kljuc) override {
        Cvor* rezultat = funkcija(nullptr, korijen, kljuc);
        return rezultat->vrijednost;
    }

    TipVrijednosti operator[](const TipKljuca& kljuc) const override {
        TipVrijednosti rezultat = NadjiVrijednost(korijen, kljuc);
        return rezultat;
    }
};


template <typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca, TipVrijednosti> {
    std::pair<TipKljuca, TipVrijednosti>** niz;
    int kapacitet;
    int velicina;
public:
    NizMapa() {
        velicina = 0;
        kapacitet = 1000;
        niz = new std::pair<TipKljuca, TipVrijednosti>*[kapacitet];
        for (int i = 0; i < kapacitet; i++) niz[i] = nullptr;
    }
    ~NizMapa() {
        for (int i = 0; i < velicina; i++) {
            delete niz[i];
        }
        delete[] niz;
        niz = nullptr;
        velicina = 0;
    }
    int brojElemenata() const override {
        return velicina;
    }
    void obrisi() override {
        for (int i = 0; i < velicina; i++) {
            delete niz[i];
            niz[i] = nullptr;
        }
        velicina = 0;
    }
    void obrisi(const TipKljuca& kljuc) override {
        for (int i = 0; i < velicina; i++) {
            if (niz[i]->first == kljuc) {
                delete niz[i];
                for (int j(i + 1); j < velicina; j++)
                    niz[j - 1] = niz[j];
                niz[--velicina] = nullptr;
            }
        }
    }

    TipVrijednosti operator[](const TipKljuca& k) const override {
        if (velicina > 0)
            for (int i = 0; i < velicina; i++)
                if (niz[i]->first == k)
                    return niz[i]->second;
        return TipVrijednosti();
    }

    TipVrijednosti& operator[](const TipKljuca& k) override {
        if (velicina > 0)
            for (int i = 0; i < velicina; i++)
                if (niz[i]->first == k)
                    return niz[i]->second;

        if (velicina == kapacitet) {
            std::pair<TipKljuca, TipVrijednosti>** arrTmp = new std::pair<TipKljuca, TipVrijednosti>*[kapacitet];
            for (int i = 0; i < velicina; i++) arrTmp[i] = new std::pair<TipKljuca, TipVrijednosti>(niz[i]->first, niz[i]->second);
            for (int i = 0; i < velicina; i++)
                delete niz[i];
            delete[] niz;
            kapacitet *= 2;
            niz = new std::pair<TipKljuca, TipVrijednosti>*[kapacitet];
            for (int i = 0; i < kapacitet; i++) {
                niz[i] = nullptr;
                if (i < velicina) {
                    niz[i] = arrTmp[i];
                    arrTmp[i] = nullptr;
                }
            }
            delete[] arrTmp;
        }

        std::pair<TipKljuca, TipVrijednosti>* p = new std::pair<TipKljuca, TipVrijednosti>(k, TipVrijednosti());
        niz[velicina++] = p;
        return niz[velicina - 1]->second;
    }
};

template <typename TipKljuca, typename TipVrijednosti>
class HashMapa : public Mapa<TipKljuca, TipVrijednosti> {
    std::pair<TipKljuca, TipVrijednosti>* niz;
    std::vector<int> vektor;
    int kapacitet;
    int velicina;
    std::function<unsigned int(TipKljuca, int)>* fun;

    int Insert(std::pair<TipKljuca, TipVrijednosti>* a, TipKljuca k, TipVrijednosti v) {
        int idx = (*fun)(k, kapacitet);
        int newIdx = (*fun)(k, kapacitet);
        bool prekid=false;
        for (int i=0; i < kapacitet - idx; i++) {
            if (vektor[i + idx] == -1)
            {
                prekid = true; break;
            }
            if (k == a[i + idx].first)
            {
                a[i + idx].second = v; return i + idx;
            }
            newIdx++;
        }
        if (prekid == false) {
            newIdx = 0;
            for (int i=0; i < idx; i++) {
                if (vektor[i] == -1)
                {
                    prekid = true; break;
                }
                if (k == a[i].first)
                {
                    a[i].second = v; return i;
                }
                newIdx++;
            }
        }
        if (vektor[newIdx] == -1 || vektor[newIdx] == 0)
            a[newIdx] = std::pair<TipKljuca, TipVrijednosti>(k, v), vektor[newIdx] = 1;
        return newIdx;
    }

public:
    HashMapa() {
        velicina = 0;
        kapacitet = 1000;
        niz = new std::pair<TipKljuca, TipVrijednosti>[1000];
        vektor = std::vector<int>(1000, -1);
        fun = nullptr;
    }

    ~HashMapa() {
        delete[] niz;
        //delete fun;
        vektor.resize(0);
        velicina = 0;
    }

    HashMapa(const HashMapa<TipKljuca, TipVrijednosti>& hm) {
        fun = hm.fun;
        velicina = hm.velicina;
        kapacitet = hm.kapacitet;
        niz = new std::pair<TipKljuca, TipVrijednosti>[hm.kapacitet];
        for (int i=0; i < kapacitet; i++)
            niz[i] = std::pair<TipKljuca, TipVrijednosti>(hm.niz[i].first, hm.niz[i].second);
        vektor = hm.vektor;
    }

    HashMapa<TipKljuca, TipVrijednosti>& operator =(const HashMapa<TipKljuca, TipVrijednosti>& hm) {
        if (this == &hm) return *this;
        delete[] niz;
        fun = hm.fun;
        kapacitet = hm.kapacitet;
        velicina = hm.velicina;
        niz = new std::pair<TipKljuca, TipVrijednosti>[hm.kapacitet];
        for (int i=0; i < kapacitet; i++)
            niz[i] = std::pair<TipKljuca, TipVrijednosti>(hm.niz[i].first, hm.niz[i].second);
        vektor = hm.vektor;
        return *this;
    }

    int brojElemenata() const override { return velicina; }

    void definisiHashFunkciju(std::function<unsigned int(TipKljuca, int)> f) { fun = &f; }

    void obrisi() override {
        delete[] niz;
        velicina = 0;
        kapacitet = 1000;
        vektor.resize(0);
        vektor.resize(1000, -1);
        niz = new std::pair<TipKljuca, TipVrijednosti>[1000];
    }

    void obrisi(const TipKljuca& kljuc) override {
        int idx = (*fun)(kljuc, kapacitet);
        int newIdx = (*fun)(kljuc, kapacitet);
        bool prekid=false;
        for (int i=0; i < kapacitet - idx; i++) {
            if (vektor[i + idx] == -1)
            {
                prekid = true; break;
            }
            if (kljuc == niz[idx + i].first) {
                vektor[i + idx] = 0;
                niz[i].second = TipVrijednosti();
                velicina--;
            }
            newIdx++;
        }
        if (prekid == false) {
            newIdx = 0;
            for (int i=0; i < idx; i++) {
                if (vektor[i] == -1)
                {
                    prekid = true; break;
                }
                if (kljuc == niz[i].first && vektor[i] == 1) {
                    vektor[i] = 0;
                    niz[i].second = TipVrijednosti();
                    velicina--;
                }
                newIdx++;
            }
        }
    }
    TipVrijednosti& operator[](const TipKljuca& k) override {
        if (fun == nullptr) throw "Nema Hash Funkcije";
        int idx = (*fun)(k, kapacitet);
        int newIdx = (*fun)(k, kapacitet);
        bool prekid=false;
        for (int i=0; i < kapacitet - idx; i++) {
            if (vektor[i + idx] == -1)
            {
                prekid = true; break;
            }
            if (k == niz[i + idx].first && vektor[i + idx] == 1)
                return niz[i + idx].second;
        }
        if (prekid == false) {
            newIdx = 0;
            for (int i(0); i < idx; i++) {
                if (vektor[i] == -1)
                {
                    prekid = true; break;
                }
                if (k == niz[i].first && vektor[i] == 1)
                    return niz[i].second;
                newIdx++;
            }
        }
        if (velicina < kapacitet) {
            if (vektor[newIdx] == -1 || vektor[newIdx] == 0)
                niz[newIdx] = std::pair<TipKljuca, TipVrijednosti>(k, TipVrijednosti());
            vektor[newIdx] = 1;
            velicina++;
            return niz[newIdx].second;
        }
        else {
            std::pair<TipKljuca, TipVrijednosti>* a = new std::pair<TipKljuca, TipVrijednosti>[kapacitet * 2];
            kapacitet *= 2;
            vektor.resize(0);
            vektor.resize(kapacitet, -1);
            for (int i=0; i < velicina; i++) {
                Insert(a, niz[i].first, niz[i].second);
            }
            delete[] niz;
            niz = a;
            a = nullptr;

            int index = Insert(niz, k, TipVrijednosti());
            vektor[index] = 1;
            velicina++;
            return niz[index].second;
        }
    }
    TipVrijednosti operator[](const TipKljuca& k) const override {
        if (fun == nullptr) return TipVrijednosti();
        int idx = (*fun)(k, kapacitet);
        int newIdx = (*fun)(k, kapacitet);
        bool prekid=false;
        for (int i=0; i < kapacitet - idx; i++) {
            if (vektor[i + idx] == -1)
            {
                prekid = true; break;
            }
            if (k == niz[idx + i].first && vektor[idx + i] == 1)
                return niz[idx + i].second;
            newIdx++;
        }
        if (prekid == false) {
            newIdx = 0;
            for (int i=0; i < idx; i++) {
                if (vektor[i] == -1)
                {
                    prekid = true; break;
                }
                if (k == niz[i].first && vektor[i] == 1)
                    return niz[i].second;
                newIdx++;
            }
        }
        return TipVrijednosti();
    }
};

int main()
{
    NizMapa<int, double> nizMapa;
    BinStabloMapa<int, double> binMapa;
    HashMapa<int, double> hashMapa;
    hashMapa.definisiHashFunkciju(hash_1);
    const int size = 1892;
    std::clock_t time1 = clock();
    for (int i(1); i <= size; i++) {

        binMapa[i] = i * 2.11 + i % 5 * 1.5;
    }
    std::clock_t time2 = clock();
    int totalTime = (time2 - time1) / (CLOCKS_PER_SEC / 1000);
    std::cout << "BinStabloMapa dodavanje: " << totalTime << " ms.\n";
    time1 = clock();
    for (int i(1); i <= size; i++) {
        nizMapa[i] = i * 2.11 + i % 5 * 1.5;

    }
    time2 = clock();
    totalTime = (time2 - time1) / (CLOCKS_PER_SEC / 1000);
    std::cout << "NizMapa dodavanje: " << totalTime << " ms.\n";
    time1 = clock();
    for (int i(1); i <= size; i++) {

        hashMapa[i] = i * 2.11 + i % 5 * 1.5;
    }
    time2 = clock();
    totalTime = (time2 - time1) / (CLOCKS_PER_SEC / 1000);
    std::cout << "HashMapa dodavanje: " << totalTime << " ms.\n";
    //Za dodavanje je najbrza HashMapa

    time1 = clock();
    binMapa[1222] = 21;
    time2 = clock();
    totalTime = (time2 - time1) / (CLOCKS_PER_SEC / 10000); // 10 000
    std::cout << "BinStabloMapa pristup: " << totalTime << " manja mjerna od ms.\n";
    time1 = clock();
    nizMapa[1222] = 21;
    time2 = clock();
    totalTime = (time2 - time1) / (CLOCKS_PER_SEC / 10000); //
    std::cout << "NizMapa pristup: " << totalTime << " manja mjerna od ms.\n";
    time1 = clock();
    hashMapa[1222] = 21;
    time2 = clock();
    totalTime = (time2 - time1) / (CLOCKS_PER_SEC / 10000); //
    std::cout << "HashMapa pristup: " << totalTime << " manja mjerna od ms.\n";
    //Za pristup je najbrza HashMapa

    time1 = clock();
    binMapa.obrisi(1222);
    time2 = clock();
    totalTime = (time2 - time1) / (CLOCKS_PER_SEC / 1000);
    std::cout << "BinStabloMapa brisanje elementa: " << totalTime << " ms.\n";
    time1 = clock();
    nizMapa.obrisi(1222);
    time2 = clock();
    totalTime = (time2 - time1) / (CLOCKS_PER_SEC / 1000);
    std::cout << "NizMapa brisanje elementa: " << totalTime << " ms.\n";
    time1 = clock();
    hashMapa.obrisi(1222);
    time2 = clock();
    totalTime = (time2 - time1) / (CLOCKS_PER_SEC / 1000);
    std::cout << "HashMapa brisanje elementa: " << totalTime << " ms.\n";

    time1 = clock();
    binMapa.obrisi();
    time2 = clock();
    totalTime = (time2 - time1) / (CLOCKS_PER_SEC / 1000);
    std::cout << "BinStabloMapa brisanje : " << totalTime << " ms.\n";
    time1 = clock();
    nizMapa.obrisi();
    time2 = clock();
    totalTime = (time2 - time1) / (CLOCKS_PER_SEC / 1000);
    std::cout << "NizMapa brisanje : " << totalTime << " ms.\n";
    time1 = clock();
    hashMapa.obrisi();
    time2 = clock();
    totalTime = (time2 - time1) / (CLOCKS_PER_SEC / 1000);
    std::cout << "HashMapa brisanje : " << totalTime << " ms.\n";
    //Opet mi ove nule izlaze???

    return 0;
}

