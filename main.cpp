//
// Created by masno on 3/11/2023.
//

#include "main.h"


#include <iostream>

using namespace std;



template <typename Tip> class Lista {
public:
    Lista(const Lista&);
    Lista() {}
    virtual ~Lista() {}
    virtual int brojElemenata() const = 0;
    virtual Tip &trenutni() = 0;
    virtual Tip trenutni() const =0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const Tip &el) = 0;
    virtual void dodajIza(const Tip &el) = 0;
    virtual Tip const& operator[](int) const = 0;
    virtual Tip &operator[](int) = 0;
};

template <typename Tip> class NizLista : public Lista<Tip> {
    int br_el, tekuci, kapacitet;
    Tip **niz;
    void BacanjeIzuzetkaNaPrazno() {
        if (br_el==0)
            throw std::logic_error("NizLista je prazna");
    }
public:
    explicit NizLista();
    ~NizLista() override;
    NizLista (const NizLista &lista);
    NizLista operator = (NizLista &lista);
    int brojElemenata() const override;
    Tip &trenutni() override;
    Tip trenutni() const override;
    bool prethodni() override;
    bool sljedeci() override;
    void pocetak() override;
    void kraj() override;
    void obrisi() override;
    void dodajIspred(const Tip &el) override;
    void dodajIza(const Tip &el) override;
    Tip const& operator[](int broj) const override{return *niz[broj];};
    Tip &operator[](int) override;
};

template <typename Tip> NizLista<Tip>::NizLista() {
    br_el = 0;
    tekuci=0;
    kapacitet=1;
    niz = new Tip *[kapacitet]{};
}

template <typename Tip> NizLista<Tip>::~NizLista() {
    for (int i = 0; i < br_el; i++)
        delete niz[i];
    delete[] niz;
}

template <typename Tip>
NizLista<Tip>::NizLista (const NizLista<Tip> &lista){
    kapacitet = lista.kapacitet;
    br_el = lista.br_el;
    tekuci = lista.tekuci;
    niz = new Tip*[kapacitet];
    for (int i = 0; i < br_el; i++) {
        niz[i] = new Tip(*lista.niz[i]);
    }
}

template <typename Tip>
NizLista<Tip> NizLista<Tip>::operator = (NizLista &lista){
    if(this == &lista) return *this;
    for(int i=0; i < br_el; i++){
        delete niz[i];
    }
    delete[] niz;
    kapacitet = lista.kapacitet;
    br_el = lista.br_el;
    tekuci = lista.tekuci;
    niz = new Tip*[lista.kapacitet];
    for(int i = 0; i<br_el; i++)
        niz[i] = new Tip(*lista.niz[i]);
    return *this;
}

template <typename Tip> int NizLista<Tip>::brojElemenata() const {
    return br_el;
}

template <typename Tip> Tip &NizLista<Tip>::trenutni() {
    BacanjeIzuzetkaNaPrazno();
    return *niz[tekuci];
}

template <typename Tip> Tip NizLista<Tip>::trenutni() const {
    if(br_el==0) throw std::domain_error("NizLista je prazna");
    return *niz[tekuci];
}

template <typename Tip> bool NizLista<Tip>::prethodni() {
    BacanjeIzuzetkaNaPrazno();
    if (tekuci == 0)
        return false;
    else {
        tekuci = tekuci - 1;
        return true;
    }
}

template <typename Tip> bool NizLista<Tip>::sljedeci() {
    BacanjeIzuzetkaNaPrazno();
    if (tekuci == br_el - 1)
        return false;
    else {
        tekuci = tekuci + 1;
        return true;
    }
}

template <typename Tip> void NizLista<Tip>::pocetak() {
    BacanjeIzuzetkaNaPrazno();
    tekuci = 0;
}

template <typename Tip> void NizLista<Tip>::kraj() {
    BacanjeIzuzetkaNaPrazno();
    tekuci = br_el - 1;
}

template <typename Tip> void NizLista<Tip>::obrisi() {
    BacanjeIzuzetkaNaPrazno();
    if (tekuci == br_el - 1) {
        if (tekuci != 0) {
            delete niz[tekuci];
            tekuci--;
        }
    } else {
        for (int i = tekuci; i < br_el - 1; i++) {
            delete niz[i];
            niz[i] = new Tip(*niz[i + 1]);
        }
    }
    br_el--;
}

template <typename Tip> void NizLista<Tip>::dodajIspred(const Tip &el) {

    if (br_el == kapacitet) {
        kapacitet*= 2;
        Tip** niz2(new Tip * [kapacitet]);
        for (int i = 0; i < br_el; i++) {
            niz2[i] = niz[i];
        }
        delete[] niz;
        niz = niz2;
        niz2=nullptr;
    }
    for (int i(br_el); i > tekuci; i--)
        if (i == br_el) {
            niz[i] = new Tip(*niz[i - 1]);
        }
        else {
            *niz[i] = *niz[i - 1];
        }
    if (br_el == 0) {
        niz[tekuci] = new Tip(el);
    }
    else
        *niz[tekuci] = el;
    br_el++;
    if (br_el != 1) tekuci++;
}

template <typename Tip> void NizLista<Tip>::dodajIza(const Tip &el) {

    if (br_el == kapacitet) {
        kapacitet*= 2;
        Tip** niz2(new Tip * [kapacitet]);
        for (int i = 0; i < br_el; i++) {
            niz2[i] = niz[i];
        }
        delete[] niz;
        niz = niz2;
        niz2=nullptr;
    }
    for (int i(br_el); i > tekuci+1; i--)
    {
        if(br_el==i) niz[i] = new Tip(*niz[i - 1]);
        else *niz[i] = *niz[i - 1];
    }

    if (br_el == 0) {
        tekuci=tekuci-1;
    }
    niz[tekuci+1] = new Tip(el);
    //*niz[tekuci+1] = el;
    br_el++;
    if (br_el == 1) tekuci++;
}

template <typename Tip> Tip &NizLista<Tip>::operator[](int indeks){
    return *niz[indeks];
}


template <typename Tip> class JednostrukaLista : public Lista<Tip> {

    struct Cvor {
        Tip element;
        Cvor *sljedeci;
    };

    int lduzina, dduzina;
    Cvor *pocetni;
    Cvor *tekuci;
    Cvor *krajnji;
    void BacanjeIzuzetkaNaPrazno() {
        if (brojElemenata() == 0)
            throw std::logic_error("Lista je prazna");
    }
    void DodajPrvi(const Tip &el) {
        Cvor *p = new Cvor();
        p->element = el;
        tekuci = p;
        pocetni = tekuci;
        krajnji = tekuci;
        tekuci->sljedeci = nullptr;
        lduzina++;
        p = nullptr;
    }

public:
    explicit JednostrukaLista();
    ~JednostrukaLista();
    JednostrukaLista operator = (const JednostrukaLista&);
    JednostrukaLista(const JednostrukaLista&);
    int brojElemenata() const override;
    Tip &trenutni() override;
    Tip trenutni() const override;
    bool prethodni() override;
    bool sljedeci() override;
    void pocetak() override;
    void kraj() override;
    void obrisi() override;
    void dodajIspred(const Tip &el) override;
    void dodajIza(const Tip &el) override;
    Tip const& operator[](int indeks) const override;
    Tip &operator[](int indeks) override;
};

template <typename Tip> JednostrukaLista<Tip>::JednostrukaLista() :Lista<Tip>(){
    pocetni = nullptr;
    tekuci = nullptr;
    krajnji=nullptr;
    lduzina = 0;
    dduzina = 0;
}

template <typename Tip>
JednostrukaLista<Tip>::~JednostrukaLista(){
    while(pocetni != krajnji) {
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
JednostrukaLista<Tip> JednostrukaLista<Tip>::operator = (const JednostrukaLista &lista){
    if(this == &lista) return *this;

    while(pocetni != nullptr){
        tekuci = pocetni;
        pocetni = pocetni->sljedeci;
        delete tekuci;
    }
    dduzina = lista.dduzina;
    lduzina = lista.lduzina;
    Cvor *tmp(lista.pocetni), *tmp2(nullptr);
    while(tmp != nullptr){
        Cvor *novi = new Cvor;
        novi->element = tmp->element;
        if(tmp2 == nullptr){
            pocetni = novi;
        }else{
            tmp2->sljedeci = novi;
        }

        tmp2 = novi;
        if(tmp == lista.tekuci){
            tekuci = novi;
        }
        tmp = tmp->sljedeci;
    }
    krajnji = tmp2;
    return *this;
}

template <typename Tip>
JednostrukaLista<Tip>::JednostrukaLista(const JednostrukaLista &lista):pocetni(0), krajnji(0), tekuci(0){
    lduzina = lista.lduzina;
    dduzina=lista.dduzina;
    Cvor* pomocni = nullptr;
    Cvor* p = lista.pocetni;
    for(int i=0; i<lduzina+dduzina; i++) {
        Cvor* novi = new Cvor;
        novi->element = p->element;
        novi->sljedeci = nullptr;
        if(i==0) pocetni = novi;
        else if(i!=0) pomocni->sljedeci = novi;
        pomocni = novi;
        if(i == lduzina+dduzina-1) krajnji = novi;
        if(p == lista.tekuci) tekuci = novi;
        p = p->sljedeci;
        novi = nullptr;
    }
    pomocni = nullptr;
    p = nullptr;
}

template <typename Tip> int JednostrukaLista<Tip>::brojElemenata() const {
    /*int br = 0;
    do {
        tekuci=tekuci.sljedeci;
        br++;
    } while (tekuci.sljedeiiiiic != nullptr);
    return br; */
    return lduzina+dduzina;
}

template <typename Tip> Tip &JednostrukaLista<Tip>::trenutni() {
    BacanjeIzuzetkaNaPrazno();
    return tekuci->element;
}

template <typename Tip> Tip JednostrukaLista<Tip>::trenutni() const{
    if(brojElemenata()==0) throw std::domain_error("Lista je prazna");
    return tekuci->element;
}

template <typename Tip> bool JednostrukaLista<Tip>::prethodni() {
    BacanjeIzuzetkaNaPrazno();
    if (tekuci!=pocetni) {
        if (lduzina == 1) {
            tekuci = pocetni;
        } else {
            Cvor *p = pocetni;
            while (p->sljedeci != tekuci) {
                p = p->sljedeci;
            }
            tekuci = p;
            p=nullptr;
        }
        lduzina--;
        dduzina++;
    } else
        return false;
    return true;
}

template <typename Tip> bool JednostrukaLista<Tip>::sljedeci() {
    BacanjeIzuzetkaNaPrazno();
    if (tekuci == krajnji)
        return false;
    if (lduzina == 0) {
        tekuci = pocetni;
    } else {
        tekuci = tekuci->sljedeci;
    }
    lduzina++;
    dduzina--;
    return true;
}

template <typename Tip> void JednostrukaLista<Tip>::pocetak() {
    BacanjeIzuzetkaNaPrazno();
    tekuci = pocetni;
}

template <typename Tip> void JednostrukaLista<Tip>::kraj() {
    BacanjeIzuzetkaNaPrazno();
    tekuci = krajnji;
}

template <typename Tip> void JednostrukaLista<Tip>::obrisi() {
    BacanjeIzuzetkaNaPrazno();

    if (brojElemenata() == 1) {
        delete pocetni;
        pocetni = nullptr;
        krajnji = nullptr;
        tekuci = nullptr;
        lduzina--;
    } else if (tekuci == krajnji) {
        Cvor *p = krajnji;
        prethodni();
        tekuci->sljedeci = nullptr;
        delete p;
        krajnji = tekuci;
        lduzina--;
    } else if (tekuci == pocetni) {
        Cvor *p = pocetni;
        sljedeci();
        p->sljedeci = nullptr;
        delete p;
        p = nullptr;
        pocetni = tekuci;
        dduzina--;
    } else {
        Cvor *p = pocetni;
        Cvor *temp = tekuci->sljedeci;
        for (auto i = pocetni; i != tekuci; i = i->sljedeci) {
            if (i->sljedeci == tekuci) {
                p = i;
                break;
            }
        }
        tekuci->sljedeci = nullptr;
        delete tekuci;
        p->sljedeci = temp;
        tekuci = temp;
        temp=nullptr;
        dduzina--;
    }


    /*Cvor<Tip>* p = new Cvor<Tip>;
    if (lduzina == 0) {
        //Tip x = pocetni->element;
        p = pocetni;
        pocetni = p->sljedeci;
        delete p;
    }
    else if (dduzina == 0) {

    }else {
        Tip x = tekuci->sljedeci->element;
        p = tekuci->sljedeci;
        tekuci->sljedeci = p->sljedeci;
    }
    delete p;
    dduzina--;
    br_elemenata--;*/
}

template <typename Tip> void JednostrukaLista<Tip>::dodajIza(const Tip &el) {
    if (brojElemenata() == 0) {
        DodajPrvi(el);
    } else {
        Cvor *p = new Cvor();
        p->element = el;
        p->sljedeci=nullptr;
        if (lduzina==1 && dduzina==0) {
            tekuci->sljedeci = p;
            pocetni->sljedeci = p;
            krajnji=p;
            p=nullptr;
        } else {
            p = tekuci->sljedeci;
            Cvor* temp=new Cvor;
            temp->element=el;
            temp->sljedeci=p;
            tekuci->sljedeci = temp;
            p=pocetni;
            if (dduzina == 0)
                krajnji = p;
            p=nullptr;
            temp=nullptr;
        }
        dduzina++;
    }
}

template <typename Tip> void JednostrukaLista<Tip>::dodajIspred(const Tip &el) {
    if (brojElemenata() == 0) {
        DodajPrvi(el);
    } else {
        if (pocetni == tekuci) {
            Cvor *p = new Cvor();
            p->element = el;
            p->sljedeci = pocetni;
            pocetni = p;
            p = nullptr;
        } else {
            Cvor* p = pocetni;
            while (p->sljedeci != tekuci)
                p = p->sljedeci;
            Cvor* novi= new Cvor;
            novi->element=el;
            novi->sljedeci=tekuci;
            p = nullptr;
            novi = nullptr;
        }
        lduzina++;
    }
}

/*template <typename Tip> Tip JednostrukaLista<Tip>::operator[](int indeks) const {
  if (indeks < 0 || indeks >= brojElemenata())
    throw std::domain_error("Indeks van dosega!");
   Cvor<Tip>* pomocni = pocetni;
        for(int i=0; i<indeks; i++) {
            pomocni = pomocni->sljedeci;
        }
        Tip povratni = pomocni->element;
        pomocni = nullptr;
        return povratni;
}*/

template <typename Tip> Tip &JednostrukaLista<Tip>::operator[](int indeks){
    if (indeks < 0 || indeks >= brojElemenata())
        throw std::domain_error("Indeks van dosega!");
    Cvor* pomocni = pocetni;
    for(int i=0; i<indeks; i++) {
        pomocni = pomocni->sljedeci;
    }
    Tip &povratni = pomocni->element;
    pomocni = nullptr;
    return povratni;
}

template <typename Tip>
Tip const& JednostrukaLista<Tip>::operator[](int indeks) const {
    if (indeks < 0 || indeks >= brojElemenata())
        throw std::domain_error("Indeks van dosega!");
    Cvor* pomocni = pocetni;
    for(int i=0; i<indeks; i++) {
        pomocni = pomocni->sljedeci;
    }
    Tip &povratni = pomocni->element;
    pomocni = nullptr;
    return povratni;
}


template<typename Tip>
Tip dajMaksimum(const NizLista<Tip>& n) {
    int max=0;
    for(int i=0; i<n.brojElemenata(); i++){
        if(n[i]>n[max]) max=i;
    }
    return n[max];
}


template<typename Tip>
Tip dajMaksimum(const JednostrukaLista<Tip>& n) {
    JednostrukaLista<Tip> lista=n;
    lista.pocetak();
    Tip max=lista.trenutni();
    for(int i=0; i<lista.brojElemenata(); i++){
        lista.sljedeci();
        if(lista.trenutni()>max) max=lista.trenutni();
    }
    return max;
}



void TestirajdodajIspred(){
    JednostrukaLista<int> jednostruka;
    NizLista<int> nizLista;
    jednostruka.dodajIspred(1);
    jednostruka.dodajIspred(2);
    jednostruka.dodajIspred(8);
    nizLista.dodajIspred(1);
    nizLista.dodajIspred(2);
    nizLista.dodajIspred(8);
    for(int i=0;i<jednostruka.brojElemenata();i++){
        cout<<jednostruka.trenutni()<<" ";
        jednostruka.prethodni();
    }
    cout<<endl;
    for(int i=0;i<nizLista.brojElemenata();i++){
        cout<<nizLista.trenutni()<<" ";
        nizLista.prethodni();
    }
}

void TestirajdodajIza(){
    JednostrukaLista<int> jednostruka;
    NizLista<int> nizLista;
    jednostruka.dodajIza(1);
    jednostruka.dodajIza(2);
    jednostruka.dodajIza(8);
    nizLista.dodajIza(1);
    nizLista.dodajIza(2);
    nizLista.dodajIza(8);
    for(int i=0;i<jednostruka.brojElemenata();i++){
        cout<<jednostruka.trenutni()<<" ";
        jednostruka.sljedeci();
    }
    cout<<endl;
    for(int i=0;i<nizLista.brojElemenata();i++){
        cout<<nizLista.trenutni()<<" ";
        nizLista.sljedeci();
    }
}

void Testirajobrisi(){
    JednostrukaLista<int> jednostruka;
    NizLista<int> nizLista;
    jednostruka.dodajIza(1);
    nizLista.dodajIspred(2);
    jednostruka.obrisi();
    nizLista.obrisi();
    std::cout << "Broj elemenata jednopstruke: " <<jednostruka.brojElemenata()<<std::endl;
    std::cout << "Broj elemenata nizliste: " <<nizLista.brojElemenata()<<std::endl;
}

void TestirajPomjeranjeTekuceg(){
    JednostrukaLista<int> jednostruka;
    NizLista<int> nizLista;
    jednostruka.dodajIza(1);
    jednostruka.dodajIza(2);
    jednostruka.dodajIza(8);
    nizLista.dodajIza(1);
    nizLista.dodajIza(2);
    nizLista.dodajIza(8);
    std::cout <<"Jednostruka: ";
    cout<<jednostruka.trenutni()<<endl;
    jednostruka.pocetak();
    cout<<jednostruka.trenutni()<<endl;
    jednostruka.sljedeci();
    cout<<jednostruka.trenutni()<<endl;
    jednostruka.prethodni();
    cout<<jednostruka.trenutni()<<endl;
    jednostruka.obrisi();
    cout<<jednostruka.trenutni();
    jednostruka.kraj();
    cout <<jednostruka.trenutni()<<endl;
    std::cout <<"NizLista: ";
    cout<<nizLista.trenutni()<<endl;
    nizLista.pocetak();
    cout<<nizLista.trenutni()<<endl;
    nizLista.sljedeci();
    cout<<nizLista.trenutni()<<endl;
    nizLista.prethodni();
    cout<<nizLista.trenutni()<<endl;
    nizLista.obrisi();
    cout<<nizLista.trenutni();
    nizLista.kraj();
    cout <<nizLista.trenutni()<<endl;
}

void TestirajMaksimumzaNizLista(){
    NizLista<int> nizLista;
    nizLista.dodajIza(1);
    nizLista.dodajIza(289);
    nizLista.dodajIza(8);
    std::cout << "Maksimum je: " << dajMaksimum(nizLista)<<std::endl;
}

void TestirajMaksimumzaJednostrukaLista(){
    JednostrukaLista<int> jednostruka;
    jednostruka.dodajIza(1092);
    jednostruka.dodajIza(2);
    jednostruka.dodajIza(8);
    std::cout << "Maksimum je: " << dajMaksimum(jednostruka)<<std::endl;
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
    TestirajMaksimumzaNizLista();
    TestirajMaksimumzaJednostrukaLista();
}

