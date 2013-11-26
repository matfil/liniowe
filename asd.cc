/**
@file asd.cc

Plik do modyfikacji w ramach cwiczenia z AISDI.
Zawiera niekompletne implementacje metod klasy ListMap,
oraz mape podobna koncepcyjnie do tej z cwicznia 1 (SmallMap)
zaimplementowana w jescze bardziej szczatkowy sposob.
Jest tez prosta funkcja testujaca (void test()), ktora
jest wolana w funkcji main. Mozna w niej zaimplementowac
wlasne testy.
NALEZY ZMODYFIKOWAC I UZUPELNIC CIALA METOD KLASY LISTMAP.

@author
Pawel Cichocki, Michal Nowacki

@date
last revision
- 2006.01.06 Michal Nowacki: wersja polska
- 2005.11.17 Michal Nowacki: constructor
- 2005.11.04 Pawel Cichocki: copied comments from the header
- 2005.11.03 Pawel Cichocki: const_iterator done properly now
- 2005.10.27 Pawel Cichocki: cosmetic changes
- 2005.10.26 Michal Nowacki: removed some method bodies
- 2005.10.25 Pawel Cichocki: wrote it

COPYRIGHT:
Copyright (c) 2005 Instytut Informatyki, Politechnika Warszawska
ALL RIGHTS RESERVED
*******************************************************************************/

#include <assert.h>
#include <algorithm>

#include <iostream>

#ifdef _SUNOS
#include "ListMap.h"
#else
#include "ListMap.h"
#endif

//#define DEBUGI
#ifdef DEBUGI
#define DBGI(X) std::cout<<X;
#else
#define DBGI(X)
#endif

//#define DEBUG
#ifdef DEBUG
#define DBG(X) std::cout<<X;
#else
#define DBG(X)
#endif


//////////////////////////////////////////////////////////////////////////////
// ListMap and ListMap::iterator methods
//////////////////////////////////////////////////////////////////////////////

ListMap::ListMap()
{
	DBG("ListMap")
	first = new ListNode(std::make_pair(-1,"sentinel"),nullptr,nullptr);
	first->internalDataPointer = first;
};

ListMap::ListMap( const ListMap& m )
{
	DBG("ListMap kopiujący")
	ListNode *current, *copy, *sentinel;
	sentinel = new ListNode(std::make_pair(-1,"sentinel"),nullptr,nullptr);
	sentinel->internalDataPointer = sentinel;
	first = m.first;
	first->internalDataPointer = sentinel;
	for(current=m.first; current->data.first!=-1; current = current->next)
	{
		copy = new ListNode(current->data);
		copy->next = sentinel;
		copy->prev = sentinel->prev;
		if (sentinel->prev != nullptr)
			sentinel->prev->next = copy;
		copy->internalDataPointer = sentinel;
		sentinel->prev = copy;
	}
};

ListMap::~ListMap()
{
	DBG("ListMap destruktor")
	clear();
	delete first;
};

// Wstawienie elementu do mapy.
// @returns Para, kt�rej komponent bool jest r�wny true gdy wstawienie zosta�o
//          dokonane, r�wny false gdy element identyfikowany przez klucz
//          ju� istnia� w mapie. Iterator ustawiony jest na ten wstawiony
//          lub istniej�cy ju� w mapie element.
std::pair<ListMap::iterator, bool> ListMap::insert(const std::pair<Key, Val>& entry)
{
	DBG("ListMap ins")
	if (first == nullptr)
	{
		first = new ListNode(std::make_pair(-1,"sentinel"),nullptr,nullptr);
		   ListNode *nowy = new ListNode(entry,first,nullptr);
		   nowy->internalDataPointer = first;
		   first->prev = nowy;
		   first = nowy;
		return std::make_pair(iterator(first), (bool)true);
	}
   iterator i;
   for(i=begin(); i!=end()&& i.node!=nullptr; ++i)
   {
      if(i->first==entry.first)
        return std::make_pair(i, (bool)false);
   }
   ListNode *nowy = new ListNode(entry,first,nullptr);
   nowy->internalDataPointer = (Node*)first->internalDataPointer;
   first->prev = nowy;
   first = nowy;
   return std::make_pair(iterator(first), (bool)true);
}

// Wstawienie elementu do mapy.
// Matoda zakłada, że w mapie nie występuje element identyfikowany przez key
ListMap::iterator ListMap::unsafe_insert(const std::pair<Key, Val>& entry)
{
	DBG("ListMap unsafe_insert")
	ListNode *nowy = new ListNode(entry,first,nullptr);
    first = nowy;
    first->prev = nowy;
   return iterator(first);
}

// Zwraca iterator addresujący element w mapie dla którego klucz jest równy
// szukanemu kluczowi lub element za ostatnim gdy szukanego klucza brak w mapie.
ListMap::iterator ListMap::find(const Key& k)
{
	//DBG("ListMap find")
	iterator i;
	   for(i=begin(); i!=end() &&  i!= const_iterator(nullptr); ++i)
	   {
	      if(i->first==k)
	        return i;
	   }
   return end();
}

ListMap::const_iterator ListMap::find(const Key& k) const
{
	const_iterator i;
		   for(i=begin(); i!=end() &&  i!= const_iterator(nullptr); ++i)
		   {
		      if(i->first==k)
		        return i;
		   }
   return end();
}

// Udostępnia wartość powi�zan� z kluczem key. Wstawia element do mapy jeśli
// nie istnia�.
// @returns Referencje do warto�ci powi�zanej z kluczem.
ListMap::Val& ListMap::operator[](const Key& k)
{
	iterator it;
	it = find(k);
	if (it == end())
	{
		std::string value = "";
		return insert(std::make_pair(k,value)).first->second;
	}
	else
		return find(k)->second;
}

// Sprawdzenie czy mapa jest pusta.
bool ListMap::empty( ) const
{
	DBG("ListMap empty")
	if (first != nullptr)
		return first->next==nullptr;
	else
		return false;
}

// Zwraca ilość elementów w mapie.
ListMap::size_type ListMap::size( ) const
{
	DBG("ListMap size")
	const_iterator i; // użycie const_iterator wymuszane przez bycie const metodą
	int number;
	number = 0;
		   for(i = begin(); i!=end() &&  i!= const_iterator(nullptr); ++i)
		   {
		      ++number;
		   }
   return number;
}

// Zwraca ilość elementów skojarzonych z kluczem key.
ListMap::size_type ListMap::count(const Key& _Key) const
{
	DBG("ListMap count")
	const_iterator i;
	int C; //C ount
	C = 0;
	for(i = begin(); i!=end()  && i!= const_iterator(nullptr); ++i)
	{
		if (i->first == _Key)
			C++;
	}
	if(C != 0)
			return 1;
		else
			return 0;

	//return 1;  // this is not a multimap
	//return C;	 // a jednak dopuszczamy unsafe_insert
}

// Usuwa element z mapy.
// @returns iterator adresuj�cy pierwszy element za usuwanym.
ListMap::iterator ListMap::erase(ListMap::iterator i)
{
	ListNode *toErase;

	toErase = i.node;
	++i;
	if (toErase == first)
	{
		toErase->next->prev = toErase->prev;
		first =  toErase->next;
	}
	else
	{
	if(toErase->prev == nullptr)
	{
		toErase->next->prev = toErase->prev;
	}
	else
	{
		toErase->prev->next = toErase->next;
		toErase->next->prev = toErase->prev;
	}
	}
	delete toErase;
	return i;
}

// Usuwa zakres element�w z mapy.
// Zakres jest zdefiniowany poprzez iteratory first i last
// first jest okre�la pierwszy element do usuni�cia, a last okre�la element
// po ostatnim usuni�tym elemencie.
// @returns iterator adresuj�cy pierwszy element za usuwanym.
ListMap::iterator ListMap::erase(ListMap::iterator f, ListMap::iterator l)
{
	iterator i, last;
	for (i = f;i != l;++i)
	{
		last = erase(i);
	}
	return last;
}

// Usuwa element z mapy.
// @returns Ilo�� usuni�tych element�w.
//          (nie jest to multimapa, wi�� mo�e by� to warto�� 1 lub 0 )
ListMap::size_type ListMap::erase(const Key& key)
{
	iterator it;
	int number;
	number = 0;
	it = find(key);
	while(it != end() && it != iterator(nullptr))
	{
		erase(it);
		number++;
		it = find(key);
	}
	if(number != 0)
		return 1;
	else
		return 0;
}

// Usuni�cie wszystkich element�w z mapy.
void ListMap::clear( )
{
	DBG("ListMap clear")
	ListNode *current, *next, *sentinel;
	current = first;
	sentinel = (ListNode*)first->internalDataPointer;
	while(current != sentinel)
	{
		next = current -> next;
		delete current;
		current = next;
	}
	first = sentinel;
	first->next=nullptr;
	first->prev=nullptr;

}

// Por�wnanie strukturalne map.
// Czy reprezentacja danych jest identyczna.
// Zwraca true je�li wewn�trzne struktury map s� identyczne.
bool ListMap::struct_eq(const ListMap& another) const
{
	DBG("ListMap struct_eq")
	ListNode *currentHere,*currentThere;
	currentHere = first;
	currentThere = another.first;

	if (first == nullptr && another.first == nullptr)
		DBG("ListMap struct_eq")
		return true;

	while(currentHere != nullptr && currentThere != nullptr)
		{
			if (currentHere->data != currentThere->data)
				DBG("ListMap struct_eq")
				return false;
			currentHere = currentHere ->next;
			currentThere= currentThere->next;
		}
	if ((currentHere == nullptr && currentThere != nullptr) || (currentHere != nullptr && currentThere == nullptr))
		DBG("ListMap struct_eq")
		return false; // jak są różnej długości to powiedz fałsz
   return true;

}

// Por�wnanie informacyjne map.
// Czy informacje trzymane w mapach s� identyczne.
// Zwraca true je�li mapy zwieraj� takie same pary klucz-warto��.
bool ListMap::info_eq(const ListMap& another) const
{
	DBG("ListMap info_eq")
	ListNode *current = first;
	while(current != nullptr)
	{
		if (another.find(current->data.first)->second != current->data.second)
			return false;
		current = current->next;
	}
   return true;
}

// preincrementacja
ListMap::const_iterator& ListMap::const_iterator::operator++()
{
	DBGI("ListMap::iterator ++pre")
	if (node == node->internalDataPointer || node->next == nullptr)
	{
		node = (Node*)node->internalDataPointer;
		return *this;
	}
	node = node->next;
	   return *this;
}

// postincrementacja
ListMap::const_iterator ListMap::const_iterator::operator++(int)
{
	DBGI("ListMap::iterator ++post")
	if (node == node->internalDataPointer || node->next == nullptr)
	{
		node = (Node*)node->internalDataPointer;
		return *this;
	}
	node = node->next;
	return *this;
}

ListMap::const_iterator& ListMap::const_iterator::operator--()
{
	DBGI("ListMap::iterator --pre")
	if (node->prev == nullptr)
	{
		node = (Node*)node->internalDataPointer;
		return *this;
	}
	node = node->prev;
	return *this;
}

// postdecrementacja
ListMap::const_iterator ListMap::const_iterator::operator--(int)
{
	DBGI("ListMap::iterator --post")
	if (node->prev == nullptr)
	{
		node = (Node*)node->internalDataPointer;
		return *this;
	}
	node = node->prev;
	return *this;
}

/// Zwraca iterator addresuj�cy pierwszy element w mapie.
ListMap::iterator ListMap::begin()
{
	DBGI("ListMap::iterator begin")
	if (first != nullptr)
		return iterator(first);
	return iterator(first);
}

/// Zwraca iterator addresuj�cy pierwszy element w mapie.
ListMap::const_iterator ListMap::begin() const
{
	DBGI("ListMap::iterator begin const")
	if (first != nullptr)
		return iterator(first);

	return iterator(first);
}

/// Zwraca iterator addresuj�cy element za ostatnim w mapie.
ListMap::iterator ListMap::end()
{
	DBGI("ListMap::iterator end")
	if (first != nullptr)
		return iterator((Node*)first->internalDataPointer);
	return iterator(first);
}

/// Zwraca iterator addresuj�cy element za ostatnim w mapie.
ListMap::const_iterator ListMap::end() const
{
	DBGI("ListMap::iterator end const")
	if (first != nullptr)
		return iterator((Node*)first->internalDataPointer);
	return iterator(first);
}

//////////////////////////////////////////////////////////////////////////////
// SmallMap
//////////////////////////////////////////////////////////////////////////////

/// Przyk�ad map'y z implementacj� podobn� do std::map.
/// To jest jedynie przyk�ad!!!
/// Normalnie implementacja powinna opiera� si� o drzewo lub tablic� haszuj�c�.
template <class Key, class Val>
class SmallMap
{
   std::pair<Key, Val> tab[2];
   int isOcupied[2];
public:

   SmallMap()
   {
      for(unsigned i=0; i<2; ++i)
      {
         isOcupied[i]=0;
      }
   }

   typedef std::pair<Key, Val>* iterator;   ///< Ka�dy wska�nik jest te� iteratorem.

   iterator begin() { return tab; }
   iterator end() { return tab+2; }

   Val& operator[](const Key& k)
   {
      static Val val;
      for(unsigned i=0; i<2; ++i)
      {
         if(isOcupied[i]&&tab[i].first==k)
            return tab[i].second;
      }
      // create
      for(unsigned i=0; i<2; ++i)
      {
         if(!isOcupied[i])
         {
            tab[i].first=k;
            isOcupied[i]=1;
            return tab[i].second;
         }
      }
      //std::cout<<"Out of space! You should not put more than two Key-Value pairs into the SmallMap.\n";
      std::cerr<<"Out of space! You should not put more than two Key-Value pairs into the SmallMap.\n";
      //assert(0);
      return val;   // Ma�o sensowne, ale to jest tylko przyk��d
   }
};


//////////////////////////////////////////////////////////////////////////////
// Testy
//////////////////////////////////////////////////////////////////////////////

/// Funckcja pomocnicza do wypisania element�w.
void print(const std::pair<int, std::string>& p)
{
   std::cout<<p.first<<", "<<p.second<<std::endl;
}

#include <map>

/// Testy u�ytkownika
void test()
{
   // A typedef used by the test.
   //typedef std::map<int, std::string> TEST_MAP;
   //typedef SmallMap<int, std::string> TEST_MAP;
   typedef ListMap TEST_MAP;

   std::cout << "Testy uzytkownika" << std::endl;

        TEST_MAP m;

   m[2] = "Merry";
   m[4] = "Jane";
   m[8] = "Korwin";
   m[4] = "Magdalena";

   for_each(m.begin(), m.end(), print );
   //system("PAUSE");
}

