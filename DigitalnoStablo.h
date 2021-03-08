// DigitalnoStablo.h: interface for the DigitalnoStablo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIGITALNOSTABLO_H__3A24581B_2902_4E94_9213_DD2E7CFBE607__INCLUDED_)
#define AFX_DIGITALNOSTABLO_H__3A24581B_2902_4E94_9213_DD2E7CFBE607__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Stablo.h"

#define EOK 0

class DigitalnoStablo : public Stablo  
{
public:

      class DSCvor
      {
      protected:

            DSCvor      *m_roditelj;

      public:
            DSCvor(DSCvor *roditelj) : m_roditelj(roditelj) { }
            virtual ~DSCvor() { }

            DSCvor *roditelj() { return m_roditelj; }
            virtual char *dohvatiInfo() { return 0; }
            virtual void postaviInfo(const char *tekst) { }
            virtual void poseti(ostream &it) = 0;
            virtual DSCvor *pronadjiPodstablo(char deoKljuca) { return 0; }
            virtual void dodajPodstablo(char deoKljuca, DSCvor *cvor) { }
            virtual void ukloniPodstablo(char deoKljuca) { }
            virtual int brojPodstabala() const { return 0; }
            virtual void prodji(DSCvor** niz,int& n) = 0;
      };

      class DSUnutrasnjiCvor : public DSCvor
      {
      protected:

            struct ListElem
            {
                  char                    deoKljuca;
                  DSCvor                  *podstablo;
                  ListElem                *sledeci;

                  ListElem(char _deoKljuca, DSCvor *p)
                  {
                        deoKljuca = _deoKljuca;
                        podstablo = p;
                        sledeci = 0;
                  }
            };

            ListElem    *m_prvi;
            int         m_brojPodstabala;


      public:

            DSUnutrasnjiCvor(DSCvor *roditelj) 
            : DSCvor(roditelj), m_prvi(0), m_brojPodstabala(0)
            {
            }

            ~DSUnutrasnjiCvor();

	      virtual DSCvor *pronadjiPodstablo(char deoKljuca);
	      virtual void dodajPodstablo(char deoKljuca, DSCvor *cvor);
            virtual void ukloniPodstablo(char deoKljuca);

            virtual int brojPodstabala() const { return m_brojPodstabala; }
            virtual void poseti(ostream &it);
            virtual void prodji(DSCvor** niz,int& n);
      };

      class DSList : public DSCvor
      {
      protected:
            char        *m_info;
            int frekfencija;

      public:

            DSList(DSCvor *roditelj, char *info) 
            : DSCvor(roditelj), m_info(0) 
            { 
                frekfencija = 0;
                postaviInfo(info); 
            }
            
            ~DSList();
            virtual void ispisiInfo()
            {
                cout << "Kljuc: " << dohvatiInfo() << "   Frekfencija: " << dohvatiFrek() << endl;
            }
            virtual char *dohvatiInfo() { return m_info; }
            virtual int dohvatiFrek() { return frekfencija; }
            virtual void inkrementirajFrek() { frekfencija++; }
            virtual void postaviFrek(int n) { frekfencija = n; }
            virtual void postaviInfo(const char *tekst);
            virtual void poseti(ostream &it);
            virtual void prodji(DSCvor** niz,int& n);
      };
            

      DSUnutrasnjiCvor  *m_koren;

      DSList    *pronadjiCvor(const char *kljuc);   // pronalazi list koji sadrzi dati kljuc
public:
	DigitalnoStablo();
	virtual ~DigitalnoStablo();

    virtual void ispisiInfoJBT(const char* kljuc);
	virtual bool umetni(const char *kljuc, const char *podaci);
	virtual bool obrisi(const char *kljuc);

    virtual char *dohvatiInfo(const char *kljuc);
	
	virtual void obidji(ostream &it);

    virtual void predvidjanje(const char* c);
};

#endif // !defined(AFX_DIGITALNOSTABLO_H__3A24581B_2902_4E94_9213_DD2E7CFBE607__INCLUDED_)
