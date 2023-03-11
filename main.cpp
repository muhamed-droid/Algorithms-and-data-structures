//
// Created by masno on 3/11/2023.
//

#include "main.h"

#include <iostream>
#include <vector>
#include <string>


using namespace std;

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
        Cvor* lijevoDijete, * desnoDijete, *roditelj;
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

    void obrisi() override{
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

    /* bool jelRootNull() {
        if (korijen == nullptr) return true;
        else return false;
    } */

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

int main() {

    NizMapa<double, double> nizMapa;
    BinStabloMapa<double, double> binStabloMapa;
    srand(time(0));
    for (int i(0); i < 2000; i++) {
        if (i == 1000) {
            nizMapa[1000] = 1000;
            binStabloMapa[1000] = 1000;
        }
        else {
            double x = rand() % 10000;
            nizMapa[x] = x;
            binStabloMapa[x] = x;
        }
    }

    clock_t vrijeme1 = clock();
    nizMapa[1000];
    clock_t vrijeme2 = clock();
    int ukupno1 = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 10000);

    vrijeme1 = clock();
    binStabloMapa[1000];
    vrijeme2 = clock();
    int ukupno2 = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 10000);


    std::cout << "Broj elemenata nizMapa: " << nizMapa.brojElemenata() << std::endl;
    std::cout << "Broj elemenata binStabloMapa: " << binStabloMapa.brojElemenata() << std::endl;


    //Ne znam iz kojeg razloga ovdje nula pise za svaki od ovih

    std::cout << "Vrijeme pristupa postojećem elementu u nizMapa: 0." << ukupno1 << "ms." << std::endl;
    std::cout << "Vrijeme pristupa postojećem elementu u binStabloMapa: 0." << ukupno2 << "ms." << std::endl;

    vrijeme1 = clock();
    nizMapa[10000] = 10000;
    vrijeme2 = clock();
    ukupno1 = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 10000);

    vrijeme1 = clock();
    binStabloMapa[10000] = 10000;
    vrijeme2 = clock();
    ukupno2 = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 10000);

    std::cout << "Vrijeme unosa novog elementa u nizMapa: 0." << ukupno1 << "ms." << std::endl;
    std::cout << "Vrijeme unosa novog elementa u binStabloMapa: 0." << ukupno2 << "ms." << std::endl;

    return 0;

}
