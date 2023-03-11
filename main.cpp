//
// Created by masno on 3/11/2023.
//

#include "main.h"

// pripremnaZadaca5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <list>

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
class MatricaGraf : public UsmjereniGraf<Tip> {
    std::vector<Grana<Tip>> bridovi;
    std::vector<Cvor<Tip>> cvorovi;
    std::vector<std::vector<bool>> matricaSusjedstva;
public:
    MatricaGraf(int num) {
        cvorovi.resize(num);
        matricaSusjedstva = std::vector<std::vector<bool>>(num, std::vector<bool>(num));
        for (int i = 0; i < num; i++) {
            Cvor<Tip> cvor(this, i);
            cvorovi[i] = cvor;
        }
    }
    ~MatricaGraf() {}
    int dajBrojCvorova() const {
        return cvorovi.size();
    }
    void postaviBrojCvorova(int num) {
        if (cvorovi.size() >= num) throw "Greska";
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
        else throw "Greska";
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
        else throw "Greska";
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


int main() {
    UsmjereniGraf<int>* graph = new MatricaGraf<int>(5);
    graph->dodajGranu(0, 1, 0); graph->dodajGranu(0, 2, 1); graph->dodajGranu(2, 1, 2);
    graph->dodajGranu(3, 2, 0); graph->dodajGranu(0, 4, 2); graph->dodajGranu(4, 3, 1);
    graph->postaviOznakuGrane(0, 2, 2903);
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
