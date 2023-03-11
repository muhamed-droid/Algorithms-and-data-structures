//
// Created by masno on 3/11/2023.
//

#include "main.h"

#include <iostream>
#include <vector>

using namespace std;

template <typename Tip>
class Iterator;

template <typename Tip> class Lista {
public:
    Lista(const Lista&);
    Lista() {}
    virtual ~Lista() {}
    virtual int brojElemenata() const = 0;
    virtual Tip& trenutni() = 0;
    virtual Tip trenutni() const = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const Tip& el) = 0;
    virtual void dodajIza(const Tip& el) = 0;
    virtual Tip const& operator[](int) const = 0;
    virtual Tip& operator[](int) = 0;
    friend class Iterator<Tip>;
};

template <typename Tip> class DvostrukaLista : public Lista<Tip> {

    struct Cvor {
        Tip element;
        Cvor* sljedeci;
        Cvor* prethodni;
    };

    int lduzina, dduzina;
    Cvor* pocetni;
    Cvor* tekuci;
    Cvor* krajnji;
    void BacanjeIzuzetkaNaPrazno() {
        if (brojElemenata() == 0)
            throw std::logic_error("Lista je prazna");
    }
    void DodajPrvi(const Tip& el) {
        Cvor* p = new Cvor();
        p->element = el;
        tekuci = p;
        pocetni = tekuci;
        krajnji = tekuci;
        tekuci->sljedeci = nullptr;
        tekuci->prethodni = nullptr;
        lduzina++;
        p = nullptr;
    }

    /*
    void IdiNaPoziciju (int i){
         if (i < 0 or i>= lduzina + dduzina)
            throw std::domain_error ("Indeks izvan dozvoljenog raspona");
        int ld = lduzina;
        int dd = dduzina;
        int k;
        dduzina = dduzina + lduzina - i;
        lduzina = i;
        if (i < abs(i-ld) && (i < (ld+dd)/2)) {
            k=i;
            tekuci=pocetni;
        } else if (abs(i-ld) <= abs(ld+dd-i))
            k = i-ld;
        else{
            k = i-ld-dd-1;
            tekuci = krajnji;
        }
        if (k < 0) {
            for (int i=0; i<abs(k); i++)
                tekuci = tekuci->prethodni;
        }

        else{
            for (int i=0; i<abs(k); i++)
                tekuci = tekuci->sljedeci;
        }
    } */

    void Umetni(const Tip& el) {
        Cvor* p = new Cvor();
        p->element = el;
        p->prethodni = tekuci;
        p->sljedeci = tekuci->sljedeci;
        tekuci->sljedeci = p;
        p->sljedeci->prethodni = p;
        dduzina = dduzina + 1;
    }

    void DodajNaKraj(const Tip& el) {
        Cvor* p = new Cvor();
        p->element = el;
        p->prethodni = krajnji;
        p->sljedeci = nullptr;
        krajnji->sljedeci = p;
        krajnji = p;
        dduzina = dduzina + 1;
    }

    void DodajNaPocetak(const Tip& el) {
        Cvor* p = new Cvor();
        p->element = el;
        p->prethodni = nullptr;
        p->sljedeci = pocetni;
        pocetni->prethodni = p;
        pocetni = p;
        dduzina = dduzina + 1;
    }

    void Izbaci() {
        if (dduzina == 0)
            throw std::domain_error("Nista za izbaciti");
        Cvor* p = tekuci->sljedeci;
        p->sljedeci->prethodni = tekuci;
        tekuci->sljedeci = p->sljedeci;
        delete p;
        dduzina = dduzina - 1;
    }
    void IzbaciJedini(){
        Cvor* p = tekuci;
        tekuci=nullptr;
        pocetni=nullptr;
        krajnji=nullptr;
        delete p;
        dduzina = dduzina - 1;
    }
    void IzbaciPrvi(){
        Cvor* p = tekuci;
        //tekuci->prethodni=nullptr;
        tekuci = p->sljedeci;
        pocetni=tekuci;
        delete p;
        dduzina = dduzina - 1;
    }
    void IzbaciPosljedni(){
        Cvor* p = tekuci;
        tekuci = p->prethodni;
        //tekuci->sljedeci=nullptr;
        krajnji=tekuci;
        delete p;
        lduzina--;
    }
    void DodajIspredPrvog(const Tip& el){
        Cvor *p=new Cvor();
        p->element=el;
        p->sljedeci=tekuci;
        p->prethodni=nullptr;
        tekuci->prethodni=p;
        pocetni=p;
        lduzina++;
    }
    void DodajIzaPosljednjeg(const Tip& el){
        Cvor *p=new Cvor();
        p->element=el;
        p->prethodni=tekuci;
        p->sljedeci=nullptr;
        tekuci->sljedeci=p;
        krajnji=p;
        dduzina++;
    }
public:
    explicit DvostrukaLista();
    ~DvostrukaLista();
    DvostrukaLista operator = (const DvostrukaLista&);
    DvostrukaLista(const DvostrukaLista&);
    int brojElemenata() const override;
    Tip& trenutni() override;
    Tip trenutni() const override;
    bool prethodni() override;
    bool sljedeci() override;
    void pocetak() override;
    void kraj() override;
    void obrisi() override;
    void dodajIspred(const Tip& el) override;
    void dodajIza(const Tip& el) override;
    Tip const& operator[](int indeks) const override;
    Tip& operator[](int indeks) override;
    friend class Iterator<Tip>;
};

template <typename Tip> DvostrukaLista<Tip>::DvostrukaLista() : Lista<Tip>() {
    pocetni = nullptr;
    tekuci = nullptr;
    krajnji = nullptr;
    lduzina = 0;
    dduzina = 0;
}

template <typename Tip> DvostrukaLista<Tip>::~DvostrukaLista() {
    while (pocetni != krajnji) {
        tekuci = pocetni->sljedeci;
        delete pocetni;
        pocetni = tekuci;
    }
    delete krajnji;
    pocetni = nullptr;
    krajnji = nullptr;
    tekuci = nullptr;
}

template <typename Tip>
DvostrukaLista<Tip> DvostrukaLista<Tip>::operator=(const DvostrukaLista& lista) {
    if (this == &lista) return *this;

    while (pocetni != nullptr) {
        tekuci = pocetni;
        pocetni = pocetni->sljedeci;
        delete tekuci;
    }
    pocetni = nullptr;
    tekuci = nullptr;
    krajnji = nullptr;

    Cvor* p(lista.pocetni), * q(nullptr);
    lduzina = lista.lduzina;
    dduzina = lista.dduzina;
    while (p != nullptr) {
        Cvor* novi = new Cvor;
        novi->element = p->element;
        novi->sljedeci = nullptr;
        if (q == nullptr) {
            this->pocetni = novi;
            novi->prethodni = nullptr;
        }
        else {
            q->sljedeci = novi;
            novi->prethodni = q;
        }

        q = novi;
        if (p == lista.tekuci) {
            this->tekuci = novi;
        }
        p = p->sljedeci;
    }
    krajnji = q;
    krajnji->sljedeci = nullptr;
    return *this;
}

template <typename Tip>
DvostrukaLista<Tip>::DvostrukaLista(const DvostrukaLista& lista) {
    if(&lista!=this) {
        Cvor *novi = lista.pocetni;
        pocetni = new Cvor();                                          // novi cvor
        if(lista.pocetni == lista.tekuci) tekuci = pocetni;               // ako je treutni jedank pocetnom
        Cvor *pomocni = pocetni;
        pomocni->sljedeci = nullptr;
        pomocni->prethodni = nullptr;
        pomocni->element = novi->element;

        while(novi->sljedeci != nullptr){

            pomocni->sljedeci = new Cvor();
            pomocni->sljedeci->prethodni = pomocni;
            pomocni->sljedeci->element = novi->sljedeci->element;
            if(novi->sljedeci == lista.tekuci) {
                tekuci = pomocni->sljedeci;
                tekuci->prethodni = pomocni;
            }

            novi = novi->sljedeci;
            pomocni = pomocni->sljedeci;
        }
        pomocni->sljedeci = nullptr;
        krajnji = pomocni;
        lduzina = lista.lduzina;
        dduzina = lista.dduzina;
    }
}

template <typename Tip> int DvostrukaLista<Tip>::brojElemenata() const {
    return lduzina + dduzina;
}

template <typename Tip> Tip& DvostrukaLista<Tip>::trenutni() {
    BacanjeIzuzetkaNaPrazno();
    return tekuci->element;
}

template <typename Tip> Tip DvostrukaLista<Tip>::trenutni() const {
    if (brojElemenata() == 0)
        throw std::domain_error("Lista je prazna");
    return tekuci->element;
}

template <typename Tip> bool DvostrukaLista<Tip>::prethodni() {
    BacanjeIzuzetkaNaPrazno();
    if (lduzina != 0) {
        tekuci = tekuci->prethodni;
        lduzina = lduzina - 1;
        dduzina = dduzina + 1;
        return true;
    }
    return false;
}

template <typename Tip> bool DvostrukaLista<Tip>::sljedeci() {
    BacanjeIzuzetkaNaPrazno();
    if (dduzina != 0) {
        tekuci = tekuci->sljedeci;
        lduzina = lduzina + 1;
        dduzina = dduzina - 1;
        return true;
    }
    return false;
}

template <typename Tip> void DvostrukaLista<Tip>::pocetak() {
    BacanjeIzuzetkaNaPrazno();
    tekuci = pocetni;
}

template <typename Tip> void DvostrukaLista<Tip>::kraj() {
    BacanjeIzuzetkaNaPrazno();
    tekuci = krajnji;
}

template <typename Tip> void DvostrukaLista<Tip>::obrisi() {
    BacanjeIzuzetkaNaPrazno();
    if(brojElemenata()==1){
        IzbaciJedini();
    }else if(tekuci==pocetni){
        IzbaciPrvi();
    }else if(tekuci==krajnji){
        IzbaciPosljedni();
    }
    else{
        prethodni();
        Izbaci();
        sljedeci();
    }

}

template <typename Tip> void DvostrukaLista<Tip>::dodajIza(const Tip& el) {
    if (brojElemenata() == 0) {
        DodajPrvi(el);
    }
    else if (tekuci==krajnji) {
        DodajNaKraj(el);
    }
    else {
        Umetni(el);
    }
}

template <typename Tip> void DvostrukaLista<Tip>::dodajIspred(const Tip& el) {
    if (brojElemenata() == 0) {
        DodajPrvi(el);
    }
        //else if(lduzina==1){
        //  DodajIspredPrvog(el);
        //}
    else if (lduzina >1) {
        prethodni();
        Umetni(el);
        sljedeci();
        sljedeci();
    }
    else {
        DodajNaPocetak(el);
    }
}

template <typename Tip> Tip& DvostrukaLista<Tip>::operator[](int indeks) {
    if (indeks < 0 || indeks > brojElemenata())
        throw std::domain_error("Indeks van dosega!");
    Cvor* p = pocetni;
    for (int i = 0; i < indeks; i++) {
        p = p->sljedeci;
    }
    return p->element;
}

template <typename Tip>
Tip const& DvostrukaLista<Tip>::operator[](int indeks) const {
    if (indeks < 0 || indeks >= brojElemenata())
        throw std::domain_error("Indeks van dosega!");
    Cvor* p = pocetni;
    for (int i = 0; i < indeks; i++) {
        p = p->sljedeci;
    }
    return p->element;
}

template<typename Tip>
class Stek: public DvostrukaLista<Tip>{
    struct Cvor {
        Tip element;
        Cvor* sljedeci;
        Cvor* prethodni;
    };
    DvostrukaLista<Tip> lista;
    int broj_elemenata;
public:
    Stek(){
        broj_elemenata=0;
    }
    //Stek (const Stek<Tip> &stek){  lista = stek.lista;}
    //Stek (Stek<Tip> &&stek){  lista = stek.lista; }
    //Stek<Tip> operator= (Stek<Tip> &&stek) { lista = stek.lista; }
    //Stek<Tip> &operator= (const Stek<Tip> &stek) { lista = stek.lista; }
    void brisi(){
        lista.pocetak();
        while(lista.brojElemenata()>0){
            lista.obrisi();
            broj_elemenata--;
        }
    }
    void stavi(const Tip &el){
        if(broj_elemenata>0) lista.kraj();
        lista.dodajIza(el);
        lista.kraj();
        broj_elemenata++;
    }
    Tip skini(){
        Tip element=lista.trenutni();
        lista.obrisi();
        broj_elemenata--;
        return element;
    }
    Tip &vrh(){
        if(broj_elemenata==0) throw std::domain_error("Stek je prazan");
        return lista.trenutni();
    }
    int brojElemenata(){
        return broj_elemenata;
    }
};

void pretraga(Stek<vector<int>>& s, int trazeni){
if(s.brojElemenata() == 0){
std::cout << "Nema elementa" << std::endl;
return;
}

vector<vector<int>> vektor;
int br_el=s.brojElemenata();
int br=br_el;
while(br>0){
vektor.push_back(s.skini());
br--;
}
for(int i=br_el-1; i>=0; i--){
s.stavi(vektor[i]);
}
int odgovarajuci_vektor=0;
int indeks=-1;
for(int i=0; i<vektor.size(); i++){
if(vektor.at(i).size()==0);
else if(vektor.at(i).at(vektor.at(i).size()-1) >= trazeni && vektor.at(i).at(0)<=trazeni) {
odgovarajuci_vektor=i;
break;
}
}
int dno = 0;
int vrh = vektor.at(odgovarajuci_vektor).size()-1;
while (vrh >= dno) {
int srednji = (dno+vrh)/2;
if (vektor.at(odgovarajuci_vektor).at(srednji) == trazeni) indeks=srednji;
else if (vektor.at(odgovarajuci_vektor).at(srednji) > trazeni)
vrh = srednji-1;
else
dno = srednji+1;
}
if(indeks==-1) cout <<"Nema elementa";
else {
cout << indeks <<" "<<odgovarajuci_vektor;
}
}

int main() {
    Stek<vector<int> > s;
    vector<int> a;
    for (int i(1); i<6; i++)
        a.push_back(i);
    vector<int> b;
    b.push_back(6);b.push_back(8);
    vector<int> c;
    c.push_back(9);c.push_back(10);c.push_back(130);
    vector<int> d;
    d.push_back(157);d.push_back(226);d.push_back(3424);
    s.stavi(a);
    s.stavi(b);
    s.stavi(c);
    s.stavi(d);
    pretraga(s,10);
}