//
// Created by masno on 3/11/2023.
//

#include "main.h"

// Zadaca5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <list>
#include <functional>

using namespace std;

template <typename Tip>
class UsmjereniGraf;
template <typename Tip>
class MatricaGraf;

template <typename Tip>
class Cvor {
    Tip oznaka;
    int redniBroj;
    std::list<Cvor> susjedni;
    UsmjereniGraf<Tip>* graf;
public:
    Cvor() {}
    ~Cvor() {}
    Cvor(UsmjereniGraf<Tip>* g, int num) {
        redniBroj = num;
        graf = g;
    }
    Cvor& operator = (const Cvor& c) {
        if (this == &c) return *this;
        redniBroj = c.redniBroj;
        graf = c.graf;
        oznaka = c.oznaka;
        susjedni = c.susjedni;
        return *this;
    }

    int sljedeciCvor() {
        return susjedni.front().redniNum;
    }

    void dodajSusjeda(Cvor& c) {
        susjedni.push_back(c);
    }

    int brojSusjeda() {
        return susjedni.size();
    }

    void postaviRedniBroj(int num) {
        redniBroj = num;
    }

    int dajRedniBroj() const {
        return redniBroj;
    }

    Tip dajOznaku() const {
        return oznaka;
    }

    Tip& dajOznaku() {
        return oznaka;
    }

    void postaviOznaku(Tip o) {
        oznaka = o;
    }

};

template <typename Tip>
class Grana {
    Cvor<Tip> ulazni;
    Cvor<Tip> izlazni;
    double tezina;
    Tip oznaka;
    UsmjereniGraf<Tip>* graf;
public:
    Grana() {}
    ~Grana() {}
    Grana(Cvor<Tip> i, Cvor<Tip> u, UsmjereniGraf<Tip>* g, double t = 0) {
        ulazni = u;
        tezina = t;
        izlazni = i;
        graf = g;
    }
    Grana(const Grana& g) {
        ulazni = g.ulazni;
        tezina = g.tezina;
        izlazni = g.izlazni;
        graf = g.graf;
        oznaka = g.oznaka;
    }

    Cvor<Tip> dajIzlazni() { return izlazni; }
    Cvor<Tip> dajUlazni() { return ulazni; }
    Cvor<Tip> dajPolazniCvor() { return dajIzlazni(); }
    Cvor<Tip> dajDolazniCvor() { return dajUlazni(); }

    Tip dajOznaku() const {
        return oznaka;
    }

    Tip& dajOznaku() {
        return oznaka;
    }

    void postaviOznaku(Tip o) {
        oznaka = o;
    }

    double dajTezinu() const {
        return tezina;
    }

    double& dajTezinu() {
        return tezina;
    }

    void postaviTezinu(double w) {
        tezina = w;
    }

};

template <typename Tip>
class UsmjereniGraf {
public:
    UsmjereniGraf() {}
    virtual ~UsmjereniGraf() {}
    virtual int dajBrojCvorova() const = 0;
    virtual void postaviBrojCvorova(int) = 0;
    virtual void dodajGranu(int, int, double) = 0;
    virtual void obrisiGranu(int, int) = 0;
    virtual int nadjiGranu(int, int) = 0;
    virtual void postaviTezinuGrane(int, int, double) = 0;
    virtual double dajTezinuGrane(int, int) = 0;
    virtual bool postojiGrana(int, int) = 0;
    virtual void postaviOznakuCvora(int, Tip) = 0;
    virtual void postaviOznakuGrane(int, int, Tip) = 0;
    virtual Tip dajOznakuCvora(int) const = 0;
    virtual Tip dajOznakuGrane(int, int) = 0;
    virtual Cvor<Tip>& dajCvor(int) = 0;
    virtual Grana<Tip>& dajGranu(int, int) = 0;
};

template <typename Tip>
class ListaGraf : public UsmjereniGraf<Tip> {
    std::vector<Cvor<Tip>> cvorovi;
    std::vector<Grana<Tip>> bridovi;
    std::vector<std::vector<bool>> matricaSusjedstva;
public:
    ListaGraf(int num) {
        cvorovi.resize(num);
        matricaSusjedstva = std::vector<std::vector<bool>>(num, std::vector<bool>(num));
        for (int i(0); i < num; i++) {
            Cvor<Tip> cvor(this, i);
            cvorovi[i] = cvor;
        }
    }
    ~ListaGraf() {}
    int dajBrojCvorova() const {
        return cvorovi.size();
    }
    void postaviBrojCvorova(int num) {
        if (cvorovi.size() >= num) throw "Ne valja";
        cvorovi.resize(num);
    }
    int dajBrojGrana() { return bridovi.size(); }
    void dodajGranu(int izlazni, int ulazni, double w = 0) {
        matricaSusjedstva[izlazni][ulazni] = true;
        bridovi.push_back(Grana<Tip>(dajCvor(izlazni), dajCvor(ulazni), this, w));
        cvorovi[izlazni].dodajSusjeda(cvorovi[ulazni]);
    }
    int nadjiGranu(int izlazni, int ulazni) {
        for (int i(0); i < bridovi.size(); i++) {
            if (bridovi[i].dajUlazni().dajRedniBroj() == ulazni && bridovi[i].dajIzlazni().dajRedniBroj() == izlazni)
                return i;
        }
        return -1;
    }
    void obrisiGranu(int izlazni, int ulazni) {
        if (postojiGrana(izlazni, ulazni)) {
            int k = nadjiGranu(izlazni, ulazni);
            matricaSusjedstva[izlazni][ulazni] = false;
            bridovi.erase(bridovi.begin() + k);
        }
        else throw "Ne valja";
    }
    void postaviTezinuGrane(int izlazni, int ulazni, double w = 0) {
        if (postojiGrana(izlazni, ulazni)) {
            bridovi[nadjiGranu(izlazni, ulazni)].postaviTezinu(w);
        }
    }
    double dajTezinuGrane(int izlazni, int ulazni) {
        if (postojiGrana(izlazni, ulazni)) {
            return bridovi[nadjiGranu(izlazni, ulazni)].dajTezinu();
        }
        else throw "Ne valja";
    }
    bool postojiGrana(int izlazni, int ulazni) {
        return matricaSusjedstva[izlazni][ulazni];
    }
    void postaviOznakuCvora(int c, Tip o) {
        cvorovi[c].postaviOznaku(o);
    }
    Tip dajOznakuCvora(int c) const {
        return cvorovi[c].dajOznaku();
    }
    void postaviOznakuGrane(int izlazni, int ulazni, Tip o) {
        bridovi[nadjiGranu(izlazni, ulazni)].postaviOznaku(o);
    }
    Tip dajOznakuGrane(int izlazni, int ulazni) {
        return bridovi[nadjiGranu(izlazni, ulazni)].dajOznaku();
    }
    Grana<Tip>& dajGranu(int izlazni, int ulazni) {
        return bridovi[nadjiGranu(izlazni, ulazni)];
    }
    Cvor<Tip>& dajCvor(int c) {
        return cvorovi[c];
    }
};


template <typename TipKljuca, typename TipVrijednosti>
class Mapa {

public:
    Mapa() {}
    virtual ~Mapa() {}
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca& kljuc) = 0;
    virtual TipVrijednosti& operator[](const TipKljuca& k) = 0;
    virtual TipVrijednosti operator[](const TipKljuca& k) const = 0;

};

template <typename TipKljuca, typename TipVrijednosti>
class HashMapaLan : public Mapa<TipKljuca, TipVrijednosti> {
    std::pair<TipKljuca, TipVrijednosti>* niz;
    std::function<unsigned int(TipKljuca, int)>* fun;
    int velicina;
    int kapacitet;
    std::list<int> lan;
public:
    HashMapaLan() {
        velicina = 0;
        kapacitet = 1000;
        fun = nullptr;
        niz = new std::pair<TipKljuca, TipVrijednosti>[1000];
        for (int i=0; i < 1000; i++) {
            niz[i].first = TipKljuca();
            niz[i].second = TipVrijednosti();
        }
    }
    ~HashMapaLan() {
        delete[] niz;
    }
    HashMapaLan(const HashMapaLan<TipKljuca, TipVrijednosti>& hm) {
        kapacitet = hm.kapacitet;
        velicina = hm.velicina;
        fun = hm.fun;
        niz = new std::pair<TipKljuca, TipVrijednosti>[kapacitet];
        for (int i=0; i < velicina; i++)
            niz[i] = hm.niz[i];
    }
    TipVrijednosti& operator[](const TipKljuca& k) {
        if (fun == nullptr) throw "Ne valja";
        int i=1, idx;
        if (velicina > 0) {
            while (i < kapacitet && niz != nullptr) {
                idx = (*fun)(k, i);
                if (niz[idx].first == k)
                    return niz[idx].second;
                i++;
            }
        }
        i = 1;
        while (i < kapacitet) {
            idx = (*fun)(k, i);
            if (niz != nullptr && niz[idx].second == TipVrijednosti()) {
                velicina++; niz[idx].first = k;
                return niz[idx].second;
            }

        }
    }
    void definisiHashFunkciju(std::function<unsigned int(TipKljuca, int)> f) { fun = &f; }
    TipVrijednosti operator[](const TipKljuca& k) const {
        int i=1, idx;
        while (i < kapacitet && niz != nullptr) {
            idx = (*fun)(k, i);
            if (niz[idx].first == k)
                return niz[idx].second;
            i++;
        }
        return TipVrijednosti();
    }
    int brojElemenata() const {
        return velicina;
    }
    void obrisi() {
        delete[] niz;
        niz = new std::pair<TipKljuca, TipVrijednosti>[kapacitet];
        velicina = 0;
    }
    void obrisi(const TipKljuca& kljuc) {
        int i=1, idx;
        while (i < kapacitet) {
            idx = (*fun)(kljuc, i);
            if (niz[idx].first == kljuc) {
                niz[idx].first = TipKljuca(); velicina--;
                break;
            }
            i++;
        }
    }
    HashMapaLan<TipKljuca, TipVrijednosti> operator =(const HashMapaLan<TipKljuca, TipVrijednosti>& hm) {
        if (this == &hm) return *this;
        delete[] niz;
        kapacitet = hm.kapacitet;
        velicina = hm.velicina;
        niz = new std::pair<TipKljuca, TipVrijednosti>[kapacitet];
        for (int i(0); i < velicina; i++)
            niz[i] = hm.niz[i];
        return *this;
    }
};

int main() {
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
