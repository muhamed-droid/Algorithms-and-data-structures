//
// Created by masno on 3/11/2023.
//

#include "main.h"
// Zadaca4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <ctime>
#include <vector>

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
class AVLStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {
private:
    struct Cvor {
        int visina;
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor* lijevoDijete, * desnoDijete, * roditelj;
        Cvor() { }
        Cvor(Cvor* p, TipKljuca key, TipVrijednosti value) {
            visina = 0;
            kljuc = key;
            vrijednost = value;
            roditelj = p; lijevoDijete = nullptr; desnoDijete = nullptr;
        }
    };
    Cvor* korijen;
    int velicina;

    int dajVisinu(Cvor* c) {
        if (c != nullptr) return c->visina;
        return -1;
    }

    Cvor* rotirajDesno(Cvor*& c) {
        Cvor* u = c->lijevoDijete;
        u->roditelj = c->roditelj;
        c->lijevoDijete = nullptr;
        u->desnoDijete = c;
        c->roditelj = u;
        c->visina = std::max(dajVisinu(c->lijevoDijete), dajVisinu(c->desnoDijete)) + 1;
        u->visina = std::max(dajVisinu(u->lijevoDijete), dajVisinu(c)) + 1;
        return u;
    }

    Cvor* rotirajLijevo(Cvor*& c) {
        Cvor* u = c->desnoDijete;
        u->roditelj = c->roditelj;
        c->desnoDijete = nullptr;
        u->lijevoDijete = c;
        c->roditelj = u;
        c->visina = std::max(dajVisinu(c->lijevoDijete), dajVisinu(c->desnoDijete)) + 1;
        u->visina = std::max(dajVisinu(c->desnoDijete), dajVisinu(c)) + 1;
        return u;
    }

    Cvor* duploLijevo(Cvor*& c) {
        c->desnoDijete = rotirajDesno(c->desnoDijete);
        return rotirajLijevo(c);
    }

    Cvor* duploDesno(Cvor*& c) {
        c->lijevoDijete = rotirajLijevo(c->lijevoDijete);
        return rotirajDesno(c);
    }

    void brisiSVrha(Cvor*& begin) {
        if (begin != nullptr) {
            brisiSVrha(begin->desnoDijete);
            brisiSVrha(begin->lijevoDijete);
            delete begin;
            begin = nullptr;
            velicina--;
        }
    }

    Cvor* dodaj(Cvor* beginParent, Cvor*& begin, TipKljuca key) {
        if (begin == nullptr) {
            begin = new Cvor(beginParent, key, TipVrijednosti());
            velicina++;
            return begin;
        }
        if (begin->kljuc == key) {
            return begin;
        }
        else if (begin->kljuc > key) {
            Cvor* u = dodaj(begin, begin->lijevoDijete, key);
            if (dajVisinu(u->lijevoDijete) - dajVisinu(u->desnoDijete) > 1) {
                if (key < u->lijevoDijete->kljuc)
                    u = rotirajDesno(u);
                else
                    u = duploDesno(u);
            }
            u->visina = std::max(dajVisinu(u->lijevoDijete), dajVisinu(u->desnoDijete)) + 1;
            return u;
        }
        else {
            Cvor* u = dodaj(begin, begin->desnoDijete, key);
            if (dajVisinu(u->desnoDijete) - dajVisinu(u->lijevoDijete) > 1) {
                if (key > u->desnoDijete->kljuc)
                    u = rotirajLijevo(u);
                else
                    u = duploLijevo(u);
            }
            u->visina = std::max(dajVisinu(u->lijevoDijete), dajVisinu(u->desnoDijete)) + 1;
            return u;
        }
    }

    TipVrijednosti NadjiVrijednost(Cvor* begin, TipKljuca key) const {
        if (begin == nullptr) {
            return TipVrijednosti();
        }
        if (begin->kljuc == key) return begin->vrijednost;
        else if (begin->kljuc > key) return NadjiVrijednost(begin->lijevoDijete, key);
        else return NadjiVrijednost(begin->desnoDijete, key);
    }

    Cvor* NadjiCvor(Cvor* begin, TipKljuca key) const {
        if (begin == nullptr) {
            return begin;
        }
        if (begin->kljuc == key) return begin;
        else if (begin->kljuc > key) return NadjiCvor(begin->lijevoDijete, key);
        else return NadjiCvor(begin->desnoDijete, key);
    }

    Cvor* dajDesni(Cvor* begin) {
        if (begin->desnoDijete == nullptr) return begin;
        return dajDesni(begin->desnoDijete);
    }

    void birisiCvor(Cvor*& begin, TipKljuca key) {
        Cvor* p = begin;
        Cvor* parent = nullptr;
        Cvor* m = nullptr, * pm = nullptr, * tmp = nullptr;
        while (p != nullptr && key != p->kljuc) {
            parent = p;
            if (key < p->kljuc) p = p->lijevoDijete;
            else p = p->desnoDijete;
        }
        if (p == nullptr) return;
        if (p->lijevoDijete == nullptr) m = p->desnoDijete;
        else if (p->desnoDijete == nullptr) m = p->lijevoDijete;
        else {
            pm = p;
            m = p->lijevoDijete;
            tmp = m->desnoDijete;
            while (tmp != nullptr) {
                pm = m;
                m = tmp;
                tmp = m->desnoDijete;
            }
            if (pm != p) {
                pm->desnoDijete = m->lijevoDijete;
                m->lijevoDijete = p->lijevoDijete;
            }
            m->desnoDijete = p->desnoDijete;
        }
        if (parent == nullptr) begin = m;
        else if (p == parent->lijevoDijete) parent->lijevoDijete = m;
        else parent->desnoDijete = m;
        delete p;
        velicina--;
    }

    void Copycat(Cvor*& p, Cvor* n, Cvor* parent) {
        if (n == nullptr) return;
        p = new Cvor(n->roditelj, n->kljuc, n->vrijednost);
        Copycat(p->lijevoDijete, n->lijevoDijete, n);
        Copycat(p->desnoDijete, n->desnoDijete, n);
    }
public:
    AVLStabloMapa() { korijen = nullptr; velicina = 0; }

    AVLStabloMapa(const AVLStabloMapa<TipKljuca, TipVrijednosti>& bsm) {
        Copycat(korijen, bsm.korijen, nullptr);
        velicina = bsm.velicina;
    }

    AVLStabloMapa<TipKljuca, TipVrijednosti> operator=(const AVLStabloMapa<TipKljuca, TipVrijednosti>& bsm) {
        if (this == &bsm) return *this;
        obrisi();
        Copycat(korijen, bsm.korijen, nullptr);
        velicina = bsm.velicina;
        return *this;
    }

    ~AVLStabloMapa() { obrisi(); }

    int brojElemenata() const override { return velicina; }

    void obrisi() override {
        brisiSVrha(korijen);
    }

    void obrisi(const TipKljuca& kljuc) override {
        birisiCvor(korijen, kljuc);
    }

    TipVrijednosti& operator[](const TipKljuca& k) override {
        Cvor* result = dodaj(nullptr, korijen, k);
        return result->vrijednost;
    }

    TipVrijednosti operator[](const TipKljuca& k) const override {
        TipVrijednosti result = NadjiVrijednost(korijen, k);
        return result;
    }
};


int main()
{
    AVLStabloMapa<int, int> AVLtree;
    BinStabloMapa<int, int> BINtree;
    std::clock_t time1, time2;
    for (int i = 0; i < 2150; i++) {
        BINtree[i] = i + 2;
    }
    for (int i = 0; i < 2150; i++) {
        AVLtree[i] = i + 2;
    }

    time1 = clock();
    BINtree[1823] = 8;
    time2 = clock();
    int timeTotal = (time2 - time1) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Pristup BIN: " << timeTotal << " ms\n";
    time1 = clock();
    AVLtree[1823] = 4;
    time2 = clock();
    timeTotal = (time2 - time1) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Pristup AVL: " << timeTotal << " ms\n";
    time1 = clock();
    BINtree.obrisi();
    time2 = clock();
    timeTotal = (time2 - time1) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Brisanje BIN: " << timeTotal << " ms\n";
    time1 = clock();
    AVLtree.obrisi();
    time2 = clock();
    timeTotal = (time2 - time1) / (CLOCKS_PER_SEC / 1000);
    std::cout << "Brisanje AVL: " << timeTotal << " ms\n";
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
