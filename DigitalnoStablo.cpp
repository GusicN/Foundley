// DigitalnoStablo.cpp: implementation of the DigitalnoStablo class.
//
//////////////////////////////////////////////////////////////////////

#include "DigitalnoStablo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DigitalnoStablo::DSUnutrasnjiCvor::~DSUnutrasnjiCvor()
{
      while(m_prvi)
      {
      ListElem *cvor = m_prvi;

            m_prvi=m_prvi->sledeci;
            delete cvor->podstablo;
            delete cvor;
      }
}

DigitalnoStablo::DSCvor * DigitalnoStablo::DSUnutrasnjiCvor::pronadjiPodstablo(char deoKljuca)
{
      if( ! m_prvi )
            return 0;

      ListElem *cvor = m_prvi;
      while( cvor )
      {
            if( cvor->deoKljuca == deoKljuca )
                  return cvor->podstablo;

            cvor = cvor->sledeci;
      }

      return 0;
}


void DigitalnoStablo::DSUnutrasnjiCvor::dodajPodstablo(char deoKljuca, DigitalnoStablo::DSCvor *cvor)
{
      ListElem *novi = new ListElem(deoKljuca, cvor);

      if( ! m_prvi )
      {
            m_prvi = novi;
      }
      else
      {
      ListElem *tek = m_prvi, *preth = 0;

            while( tek && tek->deoKljuca < deoKljuca )
            {
                  preth = tek;
                  tek = tek->sledeci;
            }

            if( preth )
                  preth->sledeci = novi;
            else
                  m_prvi = novi;
            
            novi->sledeci = tek;
      }

      m_brojPodstabala++;
}

void DigitalnoStablo::DSUnutrasnjiCvor::ukloniPodstablo(char deoKljuca)
{
      if( ! m_prvi )
            return;

      ListElem *tek = m_prvi, *preth = 0;

      while( tek && tek->deoKljuca != deoKljuca )
      {
            preth = tek;
            tek = tek->sledeci;
      }

      if( ! tek )
            return;

      if( preth )
            preth->sledeci = tek->sledeci;
      else
            m_prvi = m_prvi->sledeci;

      delete tek;
      m_brojPodstabala--;
}

void DigitalnoStablo::DSUnutrasnjiCvor::poseti(ostream &it)
{

      for( ListElem *tek = m_prvi; tek; tek = tek->sledeci )
            tek->podstablo->poseti(it);
}


//----------------------------------------------

DigitalnoStablo::DSList::~DSList()
{
      if( m_info )
            delete []m_info;
}

void DigitalnoStablo::DSList::postaviInfo(const char *tekst)
{
      if( m_info )
      {
            delete []m_info;
            m_info = 0;
      }

      if( tekst )
      {
            m_info = new char[strlen(tekst) + 1];
            strcpy(m_info, tekst);
      }
}

void DigitalnoStablo::DSList::poseti(ostream &it)
{
      if( m_info )
            it << m_info << endl;
}


//-----------------------------------------------

DigitalnoStablo::DigitalnoStablo()
: m_koren(0)
{

}

DigitalnoStablo::~DigitalnoStablo()
{
      if( m_koren )
            delete m_koren;
}

DigitalnoStablo::DSList *DigitalnoStablo::pronadjiCvor(const char *kljuc)   // pronalazi list koji sadrzi dati kljuc
{
      if( ! m_koren )
            return 0;

      DSCvor *tek = m_koren;

      for( int i = 0; i < strlen(kljuc) && tek; i++ )
            tek = tek->pronadjiPodstablo( kljuc[i] );

      if( tek )
            tek = tek->pronadjiPodstablo( EOK );

      if (tek)
      {
          return (DSList*)tek;
      }

      return 0;
}

bool DigitalnoStablo::umetni(const char *kljuc, const char *podaci)
{
      if( ! m_koren )
      {
            m_koren = new DSUnutrasnjiCvor(0);
            m_brCvorova++;
      }

      DSCvor *tek = m_koren;
      DSCvor *sledeci = 0;

      for( int i = 0; i < strlen(kljuc); i++ )
      {
      
            sledeci = tek->pronadjiPodstablo( kljuc[i] );

            if( ! sledeci )
            {
                  sledeci = new DSUnutrasnjiCvor(tek);
                  tek->dodajPodstablo( kljuc[i], sledeci );
                  m_brCvorova++;
            }

            tek = sledeci;
      }

      bool rezultat = true;  // ako je kljuc prvi put umetnut, onda true
      if( tek->pronadjiPodstablo(EOK) )
            rezultat = false;
      else
      {
            sledeci = new DSList(tek, 0);
            tek->dodajPodstablo( EOK, sledeci );
            m_brKljuceva++;
      }

      sledeci->postaviInfo(podaci);      

      return rezultat;    
}


bool DigitalnoStablo::obrisi(const char *kljuc)
{
      if( ! m_koren )
            return false;

      DSCvor *tek = pronadjiCvor(kljuc);
      if( ! tek )
            return false;

      tek->postaviInfo(0);
      int i = strlen(kljuc);
      
      while( i >= 0 && tek && tek->brojPodstabala() == 0)
      {
      DSCvor *roditelj = tek->roditelj();

            delete tek;
            m_brCvorova--;

            if( roditelj )
                  roditelj->ukloniPodstablo( kljuc[i--] );
            else
                  m_koren = 0;
            tek = roditelj;
      }

      m_brKljuceva--;

      return true;
}

char *DigitalnoStablo::dohvatiInfo(const char *kljuc)
{
      DSList *list = pronadjiCvor(kljuc);

      if( list )
            return list->dohvatiInfo();

      return 0;
}



void DigitalnoStablo::obidji(ostream &it)
{
      if( m_koren )
            m_koren->poseti(it);
}



void DigitalnoStablo::ispisiInfoJBT(const char* kljuc)
{
    DSList* list = pronadjiCvor(kljuc);
    if (list)
    {
        list->inkrementirajFrek();
        list->ispisiInfo();
    }
    else
    {
        cout << "Rec " << kljuc << " ne postoji u recniku\n";
    }
    return;
}

int uporedi(DigitalnoStablo::DSList* p, DigitalnoStablo::DSList* q)
{
    if (p->dohvatiFrek() > q->dohvatiFrek())
        return 1;
    else
        return 0;
}

void sortiraj_niz(DigitalnoStablo::DSList* niz[3], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (niz[i]->dohvatiFrek() > niz[j]->dohvatiFrek())
            {
                DigitalnoStablo::DSList* temp = niz[i];
                niz[i] = niz[j];
                niz[j] = temp;
            }
        }
    }
    return;
}



void DigitalnoStablo::DSUnutrasnjiCvor::prodji(DigitalnoStablo::DSCvor** niz,int& n)
{
    for (ListElem* tek = m_prvi; tek; tek = tek->sledeci)
        tek->podstablo->prodji(niz,n);
}

void DigitalnoStablo::DSList::prodji(DigitalnoStablo::DSCvor** niz,int& n)
{
    if (m_info)
    {
        if (n < 3)
        {
            niz[n] = this;
            n++;
            sortiraj_niz((DigitalnoStablo::DSList**)niz,n);
        }
        else
        {
            
            if (uporedi(this, (DigitalnoStablo::DSList*)niz[0]) == 1)
            {
                niz[0] = this;
                sortiraj_niz((DigitalnoStablo::DSList**)niz, 3);
            }
        }
    }
    return;
}

void DigitalnoStablo::predvidjanje(const char* c)
{
    if (!m_koren)
        return;
    DSCvor* tek = m_koren;
    DSCvor* max_niz[3];
    int broj_el_niza = 0;
    for (int i = 0; i < 3; i++)
        max_niz[i] = nullptr;

    for (int i = 0; i < strlen(c) && tek; i++)
    {
        tek = tek->pronadjiPodstablo(c[i]);
    }
    //podstablo na koje je pokazivalo poslednje slovo
    if (tek)
    {
        ((DSUnutrasnjiCvor*)tek)->prodji(max_niz,broj_el_niza);
        for (int i = 0; i < broj_el_niza; i++)
            ((DigitalnoStablo::DSList*)max_niz[i])->ispisiInfo();
    }
    else
    {
        cout << "rec sa tim prefiksom ne postoji\n";
    }
    return;
}