#include <bits/stdc++.h>

enum class Combustibil{
    Benzina,
    Motorina
};

class Exlocuri : public std::exception {
public:
    const char *what() const noexcept override {
            return "Nu exista autobuz cu mai putin de 8 locuri";
    }
};

class Extransmisie : public std::exception {
public:
    const char *what() const noexcept override {
            return "Nu exista sedan cu astfel de transmisie";
    }
};

class PS {
public:
    virtual void p(std::ostream&) const = 0;
    friend std::ostream& operator<<(std::ostream&, const PS&);
    virtual ~PS();
};

std::ostream& operator<<(std::ostream& os, const PS& obj) {
    obj.p(os);
    return os;
}
PS::~PS() = default;

class GenIdVeh {
protected:
    static int nr;
public:
    template<class T>
    static std::string getid();
};
int GenIdVeh::nr = 0;

class Automobil: public PS{
protected:
    std::string marca;
    int putere, anfab, greutate;
    double distanta, consumSpecific;
public:
    Automobil(const std::string &marca, int putere, int anfab,int greutate,double distanta, double consumSpecific);
    void p(std::ostream &ostream) const override;
    virtual double Consum() const = 0;
    virtual std::shared_ptr<Automobil> Clone() =0;
    int getputere() const;
    virtual ~Automobil();

};
int Automobil::getputere() const{
    return putere;
}
Automobil::Automobil(const std::string &marca, int putere, int anfab,int greutate,double distanta, double consumSpecific) :
        marca(marca),putere(putere),anfab(anfab), greutate(greutate), distanta(distanta),  consumSpecific(consumSpecific) {}
void Automobil::p(std::ostream &os) const{
    os<<marca<<' '<<putere<<' '<<anfab<<' '<< greutate<<' '<<distanta<<' '<<consumSpecific<<' ';
}
Automobil::~Automobil()=default;


class Autobuz:public Automobil{
protected:
    std::string id;
    int nrLocuri;
    double coefgr; //coeficient greutate
public:
    Autobuz(const std::string &marca, int putere, int anfab,int greutate,double distanta, double consumSpecific, int nrLocuri,double coefgr,const std::string &id);
    Autobuz& operator=(const Autobuz& bus);
    void p(std::ostream &ostream) const override;
    double Consum() const override;
    std::string getid() const;
    std::shared_ptr<Automobil> Clone() override{
        return std::make_shared<Autobuz>(marca,putere, anfab,greutate, distanta, consumSpecific,nrLocuri, coefgr,id);
    }

};
std::string Autobuz::getid() const{
    return id;
}
Autobuz &Autobuz::operator=(const Autobuz &bus) {
    Automobil::operator=(bus);
    nrLocuri=bus.nrLocuri;
    coefgr=bus.coefgr;
    return *this;
}
Autobuz::Autobuz(const std::string &marca, int putere, int anfab,int greutate,double distanta, double consumSpecific, int nrLocuri,double coefgr,const std::string &id):
        Automobil(marca,putere, anfab,greutate, distanta, consumSpecific), nrLocuri(nrLocuri), coefgr(coefgr), id(GenIdVeh::getid<Autobuz>()){
//    id=GenIdVeh::getid<Autobuz>();
    if(nrLocuri<8){
        throw Exlocuri();
    }
}
void Autobuz::p(std::ostream &os) const{
    Automobil::p(os);
    os<<nrLocuri<<' '<<coefgr<<' '<<id<<' ';
}
double Autobuz::Consum() const {
    return(distanta * consumSpecific) + (greutate * coefgr);
}


template<Combustibil T>
class Sedan:public Automobil{
protected:
    std::string id;
    std::string transmisie;
    double coefaero, vitezamed;
public:
    Sedan(const std::string &marca, int putere, int anfab,int greutate,double distanta, double consumSpecific, const std::string &transmisie, double coefaero,double vitezamed, const std::string &id);
    Sedan& operator=(const Sedan& sed);
    void p(std::ostream &ostream) const override;
    double Consum() const override;
    std::string getid() const;
    std::shared_ptr<Automobil> Clone() override{
        return std::make_shared<Sedan<T>>(marca,putere, anfab,greutate, distanta, consumSpecific, transmisie,coefaero,vitezamed,id);
    }
};
template<Combustibil T>
std::string Sedan<T>::getid() const{
    return id;
}
template<Combustibil T>
Sedan<T> &Sedan<T>::operator=(const Sedan &sed) {
    Automobil::operator=(sed);
    transmisie=sed.transmisie;
    coefaero=sed.coefaero;
    vitezamed=sed.vitezamed;
    return *this;
}

template<Combustibil T>
Sedan<T>::Sedan(const std::string &marca, int putere, int anfab,int greutate,double distanta, double consumSpecific, const std::string &transmisie,double coefaero,double vitezamed,const std::string &id):
        Automobil(marca,putere, anfab,greutate, distanta, consumSpecific), transmisie(transmisie), coefaero(coefaero), vitezamed(vitezamed), id(GenIdVeh::getid<Sedan<T>>()){
    if(transmisie!="manuala" and transmisie!="automata"){
        throw Extransmisie();
    }
}

template<Combustibil T>
double Sedan<T>::Consum() const {
    return (distanta * consumSpecific)+(putere*coefaero*vitezamed);
}

template<>
void Sedan<Combustibil::Benzina>::p(std::ostream &os) const{
    Automobil::p(os);
    os<<transmisie<<' '<<coefaero<<' '<<vitezamed<<' '<<id<<' '<<"benzina";
}

template<>
void Sedan<Combustibil::Motorina>::p(std::ostream &os) const{
    Automobil::p(os);
    os<<transmisie<<' '<<coefaero<<' '<<vitezamed<<' '<<id<<' '<<"motorina";
}

template<class T>
std::string GenIdVeh::getid() {
    std::string s;
    if(typeid(T)== typeid(Autobuz))
        s = "Bus";
    if(typeid(T)== typeid(Sedan<Combustibil::Motorina>))
        s="SedM";
    if(typeid(T)== typeid(Sedan<Combustibil::Benzina>))
        s="SedB";
    s+=std::to_string(nr++);
    return s;
}

//design pattern-ul numit Depozit se refera la un pattern de proiectare
//folosit pentru stocarea si destionarea vehiculelor. Acesta ofara operatii
//de adaugare si afisare a vehiculelor aflate in depozit
class Depozit{
private:
    std::vector<std::shared_ptr<Automobil>> automobile;
    Depozit() {}
public:

    std::vector<std::shared_ptr<Automobil>>& gatautomobile() {
        return automobile;
    }
    void adaugaauto(std::shared_ptr<Automobil> autom) {
        automobile.push_back(autom);
    }

    void afisare(){
        for(auto a:automobile){
            a->p(std::cout);
            std::cout<<std::endl;
        }
    }
    static Depozit& GetInstance() {
        static Depozit depozit;
        return depozit;
    }
};

template <class T, class Q>
std::string maxputere(const T& vehicul1, const Q& vehicul2) {
    if(vehicul1.getputere() < vehicul2.getputere())
        return vehicul2.getid();
    return vehicul1.getid();
}

int main() {
    auto &depoInstace = Depozit::GetInstance();

    auto s= std::make_shared<Sedan<Combustibil::Motorina>>("BMW", 122,2002, 2,15000,2,"manuala",3,80,GenIdVeh::getid<Sedan<Combustibil::Motorina>>());
    auto sb= std::make_shared<Sedan<Combustibil::Benzina>>("Fiat", 102,2022, 1,50000,4,"automata",2,50,GenIdVeh::getid<Sedan<Combustibil::Benzina>>());
    auto b= std::make_shared<Autobuz>("Merceddes",150,2012,10,30000,3,30,5,GenIdVeh::getid<Autobuz>());
    depoInstace.adaugaauto(s);
    depoInstace.adaugaauto(sb);
    depoInstace.adaugaauto(b);
    std::cout<<"Toate vehiculele din depozit sunt:\n";
    depoInstace.afisare();
    std::cout<<std::endl;
    std::cout<<"Intre vehiculul "<<b->getid()<<" si vehiculul "<<s->getid()<<" cel cu puterea mai mare este ";
    std::cout<<maxputere(*b,*s);
    std::cout<<std::endl<<std::endl;
    std::cout<<"Urmeaza sa se spuna despre fiecare vehicul din depozit daca e autobus sau nu\n";
    for (const auto& vehicul : depoInstace.gatautomobile()) {
        Autobuz* autobuz = dynamic_cast<Autobuz*>(vehicul.get());
        if (autobuz) {
            std::cout << "Vehiculul " << autobuz->getid() << " este un Autobuz.\n";
        }
        else
            std::cout << "Vehiculul nu este un Autobuz.\n";
    }
    std::cout<<std::endl;
    std::cout<<"Exemplificare try catch\n";
    try{
        auto m = Autobuz("Iveco",200,2016,20,20000,3,5,2,GenIdVeh::getid<Autobuz>());
        std::cout<<m;
    } catch(const Exlocuri& e){
        std::cout<<e.what();
    }
    std::cout<<std::endl;
    try{
        auto m = Sedan<Combustibil::Benzina>("VW",250,2023,2,25000,2.5,"aaaa", 1,80,GenIdVeh::getid<Sedan<Combustibil::Benzina>>());
        std::cout<<m;
    } catch(const Extransmisie& e){
        std::cout<<e.what();
    }
    std::cout<<std::endl<<std::endl;
    std::cout<<"Consumul vehiculului cu id-ul "<<b->getid()<<" este:\n";
    std::cout<<b->Consum();
    std::cout<<std::endl;
    std::cout<<"Consumul vehiculului cu id-ul "<<s->getid()<<" este:\n";
    std::cout<<s->Consum();


//    auto b= Autobuz("cjgac",15,2002,25,22,35,10,55,GenIdVeh::getid<Sedan<Combustibil::Benzina>>());
//    auto bb= Autobuz("cac",15,2002,25,22,35,10,55,GenIdVeh::getid<Sedan<Combustibil::Benzina>>());
    return 0;
}
