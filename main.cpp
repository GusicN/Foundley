#include<string>
#include<fstream>
#include <iostream>
#include<filesystem>
using namespace std;

namespace fs = std::filesystem;


#include "DigitalnoStablo.h"


void trazi(Stablo &s, const char *kljuc)
{
      cout << "Trazim kljuc: " << kljuc << endl;
      char *rez = s.dohvatiInfo(kljuc);
      if( rez )
            cout << "<" << kljuc << ">" << " : " << rez << endl << endl;
      else
            cout << "Ne postoji kljuc: " << kljuc << endl << endl;
}

void azuriraj_frekfenciju(DigitalnoStablo* ds, string rec)
{
    int n;
    cout << "Unesite novu frekfenciju\n";
    cin >> n;
    const char* c = rec.c_str();
    DigitalnoStablo::DSList* list = ds->pronadjiCvor(c);
    if (list)
    {
        list->postaviFrek(n);
        cout << "frekfencija je postavljena \n";
    }
    else
    {
        cout << "Rec " << rec << " nije dodata u recnik\n";
    }
    return;
}
string prebaci_u_malo(string word)
{
    for (int i = 0; i < word.length(); i++)
    {
        if (word[i] > 'A' && word[i] < 'Z')
        {
            word[i] = word[i] + ('a'-'A');
        }
    }
    return word;
}
void dodaj_u_stablo(DigitalnoStablo* ds, string word)
{
    string rec = prebaci_u_malo(word);
    const char* c = rec.c_str();
    DigitalnoStablo::DSList* l= ds->pronadjiCvor(c);
    if(!l)
        ds->umetni(c, c);
    else
    {
        l->inkrementirajFrek();
    }
    return;
}
void dodaj_iz_dat(string imeD, DigitalnoStablo* ds)
{
    ifstream file;
    file.open(imeD);
    if (!file.is_open())
    {
        cout << "datoteka nije otvorena\n";
        return;
    }
    string word;
    while (file >> word)
    {
        
        dodaj_u_stablo(ds, word);
    }
    file.close();
    return;
}
void dodaj_iz_dat(const fs::path ime, DigitalnoStablo* ds)
{
    ifstream file;
    file.open(ime);
    if (!file.is_open())
    {
        cout << "datoteka nije otvorena\n";
        return;
    }
    string word;
    while (file >> word)
    {
         
        dodaj_u_stablo(ds, word);
    }
    file.close();
    return;
}

int main()
{
    int izbor;
    bool kraj = false;
    Stablo* recnik = 0;
    bool recnik_napravljen = false;
    while (!kraj)
    {
        cout << "\n1.Napravi recnik\n2.Izbrisi recnik\n3.Dodaj reci\n4.Pretrazivanje reci\n5.Azuriranje frekfencija\n6.Predvidjanje reci\n7.Izlaz\n\n";
        cin >> izbor;
        switch (izbor)
        {
        case 1:
            if (recnik_napravljen)
                cout << "Recnik vec posoji\n";
            else
            {
                recnik_napravljen = true;
                recnik = new DigitalnoStablo;
                cout << "Recnik je napravljen\n";
            }
            break;
        case 2:
            if (recnik_napravljen)
            {
                recnik_napravljen = false;

                delete recnik;
                cout << "Recnik je izbrisan\n";
            }
            else
            {
                cout << "Prvo morate napraviti recnik\n";
            }
            break;
        case 3:
            if (recnik_napravljen)
            {
                int n;
                cout << "1.Dodaj iz datoteke\n2.Dodaj iz fajla(vise datoteka)\n3.Dodaj sa standardnog ulaza\n";
                cin >> n;
                if (n == 1)
                {
                    string ime_datoteke;
                    cout << "Unesite ime datoteke\n";
                    cin >> ime_datoteke;
                    dodaj_iz_dat(ime_datoteke, (DigitalnoStablo*)recnik);
                    cout << "Reci su dodate\n";
                }
                else if (n == 2)
                {
                    string putanja;
                    cout << "Unesite putanju do fajla\n";
                    cin >> putanja;
                    for (const auto& entry : fs::directory_iterator(putanja))
                        dodaj_iz_dat(entry.path(), (DigitalnoStablo*)recnik);
                }
                else if (n == 3)
                {
                    string rec;
                    cout << "Unesite rec \n";
                    cin >> rec;
                    const char* c = rec.c_str();
                    dodaj_u_stablo((DigitalnoStablo*) recnik, c);
                }
                else
                {
                    cout << "Bira se od 1 do 3\n";
                }
                
            }
            else
            {
                cout << "Napravite recnik\n";
            }
            break;
        case 4:
            if (recnik_napravljen)
            {
                string word;
                cout << "Unesite rec za pretragu\n";
                cin >> word;
                string rec = prebaci_u_malo(word);
                const char* c = rec.c_str();
                ((DigitalnoStablo*)recnik)->ispisiInfoJBT(c);
            }
            else
            {
                cout << "Napravite recnik\n";
            }
            break;
        case 5:
            if (recnik_napravljen)
            {
                string rec;
                cout << "Unesite rec\n";
                cin >> rec;
                azuriraj_frekfenciju((DigitalnoStablo*)recnik, rec);
            }
            else
            {
                cout << "Napravite recnik\n";
            }
            break;
        case 6:
            if (recnik_napravljen)
            {
                string rec;
                cout << "rec:\n";
                cin >> rec;
                cout << endl;
                const char* c = rec.c_str();
                ((DigitalnoStablo*)recnik)->predvidjanje(c);
            }
            else
            {
                cout << "Napravite recnik\n";
            }
            break;
        case 7:
            kraj = true;
            break;
        default:
            cout << "Bira se od 1 do 7\n";
            break;
        }
    }
    return 0;
}