#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <typeinfo>

using namespace std;

const int NR_TRANZACTII=5;
const int BONUS_CONT=10;
const int BONUS_CONT_DE_ECONOMII=5;
const double COST_RETRGERE=3.45;
const int anul_curent=2020;
class Cont
{
    protected:
    static int nr_conturi;
    string detinator;
    string data;
    string valuta;
    double sold;

    int nr_tranzactii_gratuite;
public:
    Cont()
    {
    nr_conturi++;
    sold=0;
    nr_tranzactii_gratuite=NR_TRANZACTII;
    }
   Cont(string detinator2,string data2,string valuta2, double s)
    {
    nr_conturi++;
    detinator=detinator2;
    data=data2;
    valuta=valuta2;
    sold=s;
    nr_tranzactii_gratuite=NR_TRANZACTII;
    }

    Cont(const Cont&c2);
    virtual ~Cont();
    static int get_numar_conturi();
    string get_detinator()const;
    double get_sold()const;
    void set_sold(double);
    void aduna_bonus_cont(int bon);
    virtual void retragere(double);
    virtual void depunere(double);
    virtual void citire(istream&is);
    virtual void afisare(ostream&os);
    friend istream&operator>>(istream&is,Cont&c);
    friend ostream&operator<<(ostream&os,const Cont&c);
    Cont operator=(const Cont&c2);
};

int Cont::nr_conturi=0;
Cont::Cont(const Cont&c2)
{
    nr_conturi++;
    detinator=c2.detinator;
    data=c2.data;
    sold=c2.sold;
    nr_tranzactii_gratuite=c2.nr_tranzactii_gratuite;
}

Cont::~Cont()
{
    nr_conturi--;
}

int Cont::get_numar_conturi()
{
    return nr_conturi;
}

double Cont::get_sold()const
{
    return sold;
}

void Cont::set_sold(double sold_nou)
{
    sold=sold_nou;
}

string Cont::get_detinator()const
{
    return detinator;
}

void Cont::aduna_bonus_cont(int bon)
{
    nr_tranzactii_gratuite=nr_tranzactii_gratuite+bon;
}

void Cont::depunere(double suma)
{
    sold=sold+suma;
}

void Cont::retragere(double suma)
{
    if(nr_tranzactii_gratuite)
        try
        {
            if(suma>sold)
                throw 0;
            nr_tranzactii_gratuite--;
            sold=sold-suma;
        }
        catch(int a)
        {
            cout<<"Fonduri insuficiente. Retragerea nu a fost aprobata!\n";
        }
    else
        try
        {
            if(suma+COST_RETRGERE>sold)
                throw 0;
            sold=sold-suma-COST_RETRGERE;
        }
        catch(int a)
        {
            cout<<"Fonduri insuficiente. Retragerea nu a fost aprobata!\n";
        }

}

istream&operator>>(istream&is,Cont&c)
{
    cout<<"Detinator: ";
    is>>c.detinator;
    cout<<"Data: ";
    is>>c.data;
    cout<<"Valuta: ";
    is>>c.valuta;
    cout<<"Sold: ";
    is>>c.sold;
    return is;
}

void Cont::citire(istream&is)
{
    is>>(*this);
}

ostream&operator<<(ostream&os,const Cont&c)
{
    os<<"Detinator: "<<c.detinator<<"\n";
    os<<"Data deschiderii: "<<c.data<<"\n";
    os<<"Sold: "<<c.sold<<c.valuta<<"\n";
    os<<"Nr. tranzactii gratuite ramase: "<<c.nr_tranzactii_gratuite<<"\n";
    return os;
}

void Cont::afisare(ostream&os)
{
    os<<(*this);
}

Cont Cont::operator=(const Cont&c2)
{
    if(this!=&c2)
    {
        detinator=c2.detinator;
        data=c2.data;
        sold=c2.sold;
        nr_tranzactii_gratuite=c2.nr_tranzactii_gratuite;
    }
    return (*this);
}


/********************************************/
class Cont_de_Economii:public Cont
{
    int anul_deschiderii_contului;
    double rata_dobanda;
    int interval;
    string istoric;
public:
    Cont_de_Economii();
    Cont_de_Economii(string,string,string,double,int,double,int);
    Cont_de_Economii(const Cont_de_Economii&);
    ~Cont_de_Economii();
    void retragere(double);
    void depunere(double);
    void citire(istream&is);
    void afisare(ostream&os);
    void aplicare_dobanda()
    {
        int i;
        for (i=1;i<=anul_curent-anul_deschiderii_contului;i++)
            {int cop=12;
            while(cop!=0)
                {cop=cop-interval;
                sold=sold+rata_dobanda*sold;}}

    }
    Cont_de_Economii operator=(const Cont_de_Economii&c2);
    friend istream &operator>>(istream &is,Cont_de_Economii &c);
    friend ostream &operator<<(ostream &os,Cont_de_Economii &c);
};

istream &operator>>(istream &is,Cont_de_Economii &c)
{
    c.Cont::citire(is);
    cout<<"Rata dobanda: ";
    is>>c.rata_dobanda;
    cout<<"Intervalul la care se aplica dobanda, exprimat in luni(3,6 sau 12): ";
    is>>c.interval;
    cout<<"Anul in care a fost creat: ";
    is>>c.anul_deschiderii_contului;
    c.istoric+="A fost creat contul de economii cu suma:";
    c.istoric+=to_string(c.get_sold());
    c.aplicare_dobanda();
    try
    {
        if(c.interval!=3||c.interval!=6||c.interval!=12)
            throw -1;
        if(c.rata_dobanda<0)
            throw 'a';
    }
    catch (int x)
    {
        cout<<"Pentru detinatorul "<<c.get_detinator()<<" intervalul este invalid!\n";
    }
    catch (char x)
    {
        cout<<"Pentru detinatorul "<<c.get_detinator()<<" rata dobanzii este invalida!\n";
    }
    return is;

}
ostream &operator<<(ostream &os,Cont_de_Economii &c)
{

    c.Cont::afisare(os);
    cout<<"Rata dobanda:";
    os<<c.rata_dobanda<<endl;
    cout<<"anul crearii:";
    os<<c.anul_deschiderii_contului;
    cout<<endl;
    os<<c.istoric;
    return os;
}
Cont_de_Economii::Cont_de_Economii():Cont()
{
    anul_deschiderii_contului=0;
    rata_dobanda=0;
    interval=0;
    string temp=std::to_string(get_sold());
    istoric+=temp;
}

Cont_de_Economii::Cont_de_Economii(string det,string data,string valuta,double sold,int anul_deschiderii,double rata,int interv):Cont(det,data,valuta,sold)
{
    anul_deschiderii_contului=anul_deschiderii;
    rata_dobanda=rata;
    interval=interv;
    string temp=to_string(get_sold());
    istoric+="A fost creat contul de economii cu suma:";
    istoric+=temp;
    aplicare_dobanda();
    try
    {
        if(interval!=3||interval!=6||interval!=12)
            throw -1;
        if(rata_dobanda<0)
            throw 'a';
    }
    catch (int x)
    {
        cout<<"Pentru detinatorul "<<this->get_detinator()<<" intervalul este invalid!\n";
    }
    catch (char x)
    {
        cout<<"Pentru detinatorul "<<this->get_detinator()<<" rata dobanzii este invalida!\n";
    }

}

Cont_de_Economii::Cont_de_Economii(const Cont_de_Economii&c2):Cont(c2)
{
    rata_dobanda=c2.rata_dobanda;
    interval=c2.interval;
    string temp;
    istoric+=" ";
    temp=to_string(get_sold());
    istoric+= temp;
}

Cont_de_Economii::~Cont_de_Economii() {}

void Cont_de_Economii::depunere(double suma)
{
    set_sold(get_sold()+suma);
    string temp;
    istoric+=" ";
    temp=to_string(suma);
    istoric+="S-a depus suma de:";
    istoric+=temp;
    istoric+=",suma actuala devine:";
    temp=to_string(get_sold());
    istoric+= temp;
}

void Cont_de_Economii::retragere(double suma)
{
    if(nr_tranzactii_gratuite)
        try
        {
            if(suma>(this->get_sold()))
                throw 0;

            nr_tranzactii_gratuite--;
            set_sold(get_sold()-suma);
            string temp;
            istoric+=" ";
            temp=to_string(get_sold());
            istoric+= temp;
        }
        catch(int a)
        {
            cout<<"Fonduri insuficiente. Retragerea nu a fost aprobata!\n";
        }
    else
        try
        {
            if(suma+COST_RETRGERE>(this->get_sold()))
                throw 0;
            set_sold(get_sold()-suma-COST_RETRGERE);
            string temp;
            istoric+=" ";
            temp=to_string(get_sold());
            istoric+= temp;
        }
        catch(int a)
        {
            cout<<"Fonduri insuficiente. Retragerea nu a fost aprobata!\n";
        }
}

void Cont_de_Economii::citire(istream&is)
{
    this->Cont::citire(is);
    cout<<"Rata dobanda: ";
    is>>rata_dobanda;
    cout<<"Interval: ";
    is>>interval;
    is.get();
    string temp;
    istoric+=" ";
    temp=to_string(get_sold());
    istoric+= temp;
    try
    {
        if(interval!=3||interval!=6||interval!=12)
            throw -1;
        if(rata_dobanda<0)
            throw 'a';
    }
    catch (int x)
    {
        cout<<"Pentru detinatorul "<<this->get_detinator()<<" intervalul este invalid!\n";
    }
    catch (char x)
    {
        cout<<"Pentru detinatorul "<<this->get_detinator()<<" rata dobanzii este invalida!\n";
    }
}

void Cont_de_Economii::afisare(ostream&os)
{

    this->Cont::afisare(os);
    os<<"Rata dobanzii: "<<rata_dobanda<<"% \n";
    os<<"Interval: "<<interval<<" luni\n";
    os<<"Istoric: "<<istoric<<"\n";
}

Cont_de_Economii Cont_de_Economii::operator=(const Cont_de_Economii&c2)
{
    if(this!=&c2)
    {
        this->Cont::operator=(c2);
        rata_dobanda=c2.rata_dobanda;
        interval=c2.interval;
        istoric=c2.istoric;
    }
    return (*this);
}


/********************************************/
template <typename T> class GestionareConturi
{
    vector<T*>Vector_Conturi_Bancare;
    static int index;
     int nr_conturi;
public:
    GestionareConturi(int);
    ~GestionareConturi();
    void adaugare_cont();
    void afisare_conturi_banca();
};

template<typename T> int GestionareConturi<T>::index=0;

template<typename T> GestionareConturi<T>::GestionareConturi(int nr_conturi)
{
    this->nr_conturi=nr_conturi;
    index++;
};
template<typename T> GestionareConturi<T>::~GestionareConturi()
{
    index--;
};
template<typename T>void GestionareConturi<T>::adaugare_cont()
{
    int aux=nr_conturi;
    while(aux)
    {
    int control;
    cout<<"Introduceti date pentru "<<this->nr_conturi<<" conturi ;"<<endl;
    cout<<"Tasta 1 pentru cont normal , Tasta 0 pentru cont de economii :"<<endl;
    cout<<"Optiunea aleasa este :"<<endl;
    cin>>control;
    if(control==0)

        {

        Cont_de_Economii *c=new Cont_de_Economii;
        cin>>*c;
        Vector_Conturi_Bancare.push_back(c);
        aux--;
        }



    if(control==1)
    {
        Cont *c=new Cont;
        cin>>*c;
        Vector_Conturi_Bancare.push_back(c);
        aux--;
    }
    if(control==-1)
    {
        break;
    }

    }

}

template<typename T> void GestionareConturi<T>::afisare_conturi_banca()
{
    for(int i=0; i<nr_conturi; i++)
    {
       cout<<(*Vector_Conturi_Bancare[i]);
    }
}


int main()
{

    //Cont a;
    Cont_de_Economii b("viezure","01/02/2020","euro",500,2019,0.1,4);
    //cin>>a;
    //cout<<a;

    b.depunere(500);
    b.retragere(500);
    cout<<b<<endl;
    GestionareConturi<Cont> *Banca1=new GestionareConturi<Cont>(1);
    Banca1->adaugare_cont();
    Banca1->afisare_conturi_banca();
    return 0;
}
