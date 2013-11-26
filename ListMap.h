/**
@file ListMap.h
 
Zawiera deklaracje klasy ListMap.
Jest to mapa (Slownik) zrobiona na liscie/pierscieniu.
Implementacja tej klasy jest przedmiotem cwiczenia na laboratoriach z AISDI.

@author
Pawel Cichocki, Michal Nowacki

@date
last revision
- 2006.01.06 Michal Nowacki: wersja polska
- 2005.11.17 Michal Nowacki: prev pointer
- 2005.11.04 Pawel Cichocki: cosmetic code improvements
- 2005.11.04 Pawel Cichocki: a public method for obtaining the count from CCount
- 2005.11.03 Pawel Cichocki: const_iterator done properly now
- 2005.10.27 Pawel Cichocki: cosmetic changes
- 2005.10.27 Michal Nowacki: removed templates, and moved the implementation to another file
- 2005.10.25 Pawel Cichocki: wrote it

COPYRIGHT:
Copyright (c) 2005 Instytut Informatyki, Politechnika Warszawska
ALL RIGHTS RESERVED
*******************************************************************************/

#include <assert.h>
#include <stdlib.h>
#include <iterator>

#include <string>

/// Prosty licznik do podstawowej kontroli wyciek�w pami�ci
class CCount
{
private:
  static int count;
  CCount() {count++;}
  ~CCount()
  {
     assert(count>0);
     //if(count>0)
        count--;
     //else
     //   cerr<<"ERROR (CCount): More destructors than constructors called!"<<std::endl;
  }
  friend struct ListNode;
  //friend int Test2();
public:
   /// Publiczna metoda do pobierania warto�ci licznika.
   static int getCount() { return count; }
};

//////////////////////////////////////////////////////////////////////////////
// ListMap i zwi�zane klasy
//////////////////////////////////////////////////////////////////////////////

/// Klasa opakowywuj�ca dane
struct ListNode : CCount
{
   typedef std::pair<int,std::string> T;
   ListNode* next;   ///< Wska�nik na kolejny element na li�cie/pier�cieniu
   ListNode* prev;   ///< Wska�nik na poprzedni element nal li�cie/pier�cieniu
   T data;           ///< Dane
   void* internalDataPointer;   ///< wska�nik pomocniczy
   ListNode(const std::pair<int,std::string>& d) : next(nullptr), prev(nullptr), data(d), internalDataPointer(nullptr) {}
   ListNode(const std::pair<int,std::string>& d, ListNode* n, ListNode* p) : next(n), prev(p), data(d), internalDataPointer(nullptr) {}
};

/// Map'a z metodami jak std::map.
/// Mapa powinna zosta� zaimplementowana jako lista lub pier�cie�
/// w wersji jedno- lub dwukierunkowej zgodnie z wytycznymi prowadz�cych.
class ListMap
{
public:
   typedef int Key;
   typedef std::string Val;

protected:
   typedef ListNode/*<Key, Value>*/ Node;
   Node* first;

public:
   typedef size_t size_type;
   typedef std::pair<Key, Val> P;

   ListMap();
   ListMap( const ListMap& );
   ~ListMap();

   /// const_iterator.
   /// U�yty r�wnie� jako klasa bazowa dla  (not const) iterator.
   class const_iterator : public std::iterator<std::forward_iterator_tag,
                                               std::pair<Key, Val> >
   {
   public:
      typedef std::pair<Key, Val> T;

   protected:
      /// Points to the list element
      Node* node;
      friend class ListMap;

      const_iterator(Node* x) : node(x) {}
   public:
      const_iterator():node(nullptr) {}
      const_iterator(const const_iterator& a) : node(a.node) {}

      inline const T& operator*() const
      {
         return node->data;
      }

      inline const T* operator->() const
      {
         return &(node->data);
      }

      // preincrementacja
      /*inline*/ const_iterator& operator++();
      // postincrementacja
      /*inline*/ const_iterator operator++(int);
      // predekrementacja
      /*inline*/ const_iterator& operator--();
      // postdekrementacja
      /*inline*/ const_iterator operator--(int);

      inline bool operator==(const const_iterator& a) const
      {
         return node == a.node;
      }

      inline bool operator!=(const const_iterator& a) const
      {
         return node != a.node;
      }
   };

   /// Iterator.
   class iterator : public const_iterator
   {
      iterator(Node* x) : const_iterator(x) {}
      friend class ListMap;

   public:
      iterator() {}
      iterator(const iterator& a) { node = a.node; }

      inline T& operator*() const
      {
         return node->data;
      }
      inline T* operator->() const
      {
         return &(node->data);
      }

      iterator& operator++()
      {  // preincrementacja
         ++(*(const_iterator*)this);
         return (*this);
      }

      iterator operator++(int)
      {  // postincrementacja
         iterator temp = *this;
         ++*this;
         return temp;
      }

      iterator& operator--()
      {  // predekrementacja
         --(*(const_iterator*)this);
         return (*this);
      }

      iterator operator--(int)
      {  // postdekrementacja
         iterator temp = *this;
         --*this;
         return temp;
      }
   };

   /// Zwraca iterator addresuj�cy pierwszy element w mapie.
   iterator begin();
   const_iterator begin() const;

   /// Zwraca iterator addresuj�cy element za ostatnim w mapie.
   iterator end();
   const_iterator end() const;

   /// Wstawienie elementu do mapy.
   /// @returns Para, kt�rej komponent bool jest r�wny true gdy wstawienie zosta�o
   ///          dokonane, r�wny false gdy element identyfikowany przez klucz
   ///          ju� istnia� w mapie. Iterator ustawiony jest na ten wstawiony
   ///          lub istniej�cy ju� w mapie element.
   std::pair<iterator, bool> insert(const std::pair<Key, Val>& entry);

   /// Wstawienie elementu do mapy.
   /// Matoda zak�ada, �e w mapie nie wyst�puje element identyfikowany przez key
   iterator unsafe_insert(const std::pair<Key, Val>& entry);

   /// Zwraca iterator addresuj�cy element w mapie dla kt�rego klucz jest r�wny
   /// szukanemu kluczowi lub element za ostatnim gdy szukanego klucza brak w mapie.
   iterator find(const Key& k);
   const_iterator find(const Key& k) const;

   /// Udost�pnia warto�� powi�zan� z kluczem key. Wstawia element do mapy je�li
   /// nie istnia�.
   /// @returns Referencje do warto�ci powi�zanej z kluczem.
   Val& operator[](const Key& k);

   /// Sprawdzenie czy mapa jest pusta.
   bool empty( ) const;

   /// Zwraca ilo�� element�w w mapie.
   size_type size() const;

   /// Zwraza ilo�� element�w skojarzonych z kluczem key.
   size_type count(const Key& _Key) const;

   /// Usuwa element z mapy.
   /// @returns iterator adresuj�cy pierwszy element za usuwanym.
   iterator erase(iterator i);

   /// Usuwa zakres element�w z mapy.
   /// Zakres jest zdefiniowany poprzez iteratory first i last
   /// first jest okre�la pierwszy element do usuni�cia, a last okre�la element
   /// po ostatnim usuni�tym elemencie.
   /// @returns iterator adresuj�cy pierwszy element za usuwanym.
   iterator erase(iterator first, iterator last);

   /// Usuwa element z mapy.
   /// @returns Ilo�� usuni�tych element�w.
   ///          (nie jest to multimapa, wi�� mo�e by� to warto�� 1 lub 0 )
   size_type erase(const Key& key);

   /// Usuni�cie wszystkich element�w z mapy.
   void clear( );

   /// Por�wnanie strukturalne map.
   /// Czy reprezentacja danych jest identyczna.
   /// Zwraca true je�li wewn�trzne struktury map s� identyczne.
   bool struct_eq(const ListMap& another) const;
   /// Por�wnanie informacyjne map.
   /// Czy informacje trzymane w mapach s� identyczne.
   /// Zwraca true je�li mapy zwieraj� takie same pary klucz-warto��.
   bool info_eq(const ListMap& another) const;

   /// Zwraca true je�li mapy zwieraj� takie same pary klucz-warto��.
   inline bool operator==(const ListMap& a) const { return info_eq(a); }
};

