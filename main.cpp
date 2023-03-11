//
// Created by masno on 3/11/2023.
//

#include "main.h"


#include <iostream>

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
    }else if(lduzina==1){
        IzbaciPrvi();
    }else if(dduzina==0){
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
    else if (dduzina == 0) {
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

template <typename Tip> Tip dajMaksimum(const DvostrukaLista<Tip>& n) {
    DvostrukaLista<Tip> lista = n;
    lista.pocetak();
    Tip max = lista.trenutni();
    for (int i = 0; i < lista.brojElemenata(); i++) {
        lista.sljedeci();
        if (lista.trenutni() > max)
            max = lista.trenutni();
    }
    return max;
}

template <typename Tip>
class Iterator {
    const DvostrukaLista<Tip>* lista;
    typename DvostrukaLista<Tip>::Cvor* trenutniLista;
public:
    Iterator(const DvostrukaLista<Tip>*& lista) :lista(lista), trenutniLista(0) {};
    Tip trenutni() {
        if (lista->brojElemenata() != 0)
            return trenutniLista->element;
    }
    void prethodni() {
        if (lista->brojElemenata() != 0)
            lista->prethodni();
        trenutniLista = lista->trenutni();
    }
    void sljedeci() {
        if (lista->brojElemenata() != 0)
            lista->sljedeci();
        trenutniLista = lista->trenutni();
    }
    void pocetak() {
        if (lista->brojElemenata() != 0)
            lista->pocetak();
        trenutniLista = lista->trenutni();
    }
    void kraj() {
        if (lista->brojElemenata() != 0)
            lista->kraj();
        trenutniLista = lista->trenutni();
    }
};

void TestirajdodajIspred(){
    DvostrukaLista<int> dvostruka;
    dvostruka.dodajIspred(1);
    dvostruka.dodajIspred(2);
    dvostruka.dodajIspred(8);
    for (int i = 0; i < dvostruka.brojElemenata(); i++) {
        cout << dvostruka.trenutni() << " ";
        dvostruka.prethodni();
    }
    cout<<endl;
}

void TestirajdodajIza(){
    DvostrukaLista<int> dvostruka;
    dvostruka.dodajIza(1);
    dvostruka.dodajIza(2);
    dvostruka.dodajIza(8);
    for (int i = 0; i < dvostruka.brojElemenata(); i++) {
        cout << dvostruka.trenutni() << " ";
        dvostruka.sljedeci();
    }
    cout<<endl;
}

void Testirajobrisi(){
    DvostrukaLista<int> dvostruka;
    dvostruka.dodajIza(1);
    dvostruka.obrisi();
    std::cout << "Broj elemenata jednopstruke: " << dvostruka.brojElemenata()
              << std::endl;
}

void TestirajPomjeranjeTekuceg(){
    DvostrukaLista<int> dvostruka;
    dvostruka.dodajIza(1);
    dvostruka.dodajIza(2);
    dvostruka.dodajIza(8);
    std::cout << "Dvostruka: ";
    cout << dvostruka.trenutni() << endl;
    dvostruka.pocetak();
    cout << dvostruka.trenutni() << endl;
    dvostruka.sljedeci();
    cout << dvostruka.trenutni() << endl;
    dvostruka.prethodni();
    cout << dvostruka.trenutni() << endl;
    dvostruka.obrisi();
    cout << dvostruka.trenutni();
    dvostruka.kraj();
    cout << dvostruka.trenutni() << endl;
}

void TestirajMaksimumzaDvostrukaLista(){
    DvostrukaLista<int> dvostruka;
    dvostruka.dodajIza(1092);
    dvostruka.dodajIza(2);
    dvostruka.dodajIza(8);
    std::cout << "Maksimum je: " << dajMaksimum(dvostruka) << std::endl;
}



int main() {
    cout << endl << "Test za dodajIspred: " <<endl;
    TestirajdodajIspred();
    cout << endl << "Test za dodajIza: " <<endl;
    TestirajdodajIza();
    cout << endl << "Test za obrisi: " <<endl;
    Testirajobrisi();
    cout << endl << "Test za pomjeranje tekuceg(pocetak(), kraj(), sljedeci(), prethodni()): " <<endl;
    TestirajPomjeranjeTekuceg();
    cout << endl << "Test za maksimum: " <<endl;
    TestirajMaksimumzaDvostrukaLista();
    return 0;
}
