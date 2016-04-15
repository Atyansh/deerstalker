#pragma once

#include <map>
#include <string.h>

using namespace std;

struct ltstr {
	bool operator()(const char* s1, const char* s2) const
	{
		return strcmp(s1, s2) < 0;
	}
};

template <class T> class Iterator;

template<class T> class HashTable {

private:
	map<const char*, T, ltstr> _map;

public:
	// constructor
	HashTable() {

	}

	// basic hash table operations
	int Size() const;
	void Enter(const char *key, T value);
	void Remove(const char *key);
	T Lookup(const char *key);

	// helper function
	Iterator<T> GetIterator();
	HashTable<T>* MakeCopy();

};

template <class T>
class Iterator {

	friend class HashTable<T>;

private:
	typename map<const char*, T, ltstr>::iterator cur, end;
	Iterator(map<const char*, T, ltstr>& map)
		: cur(map.begin()), end(map.end()) {}

public:
	T GetNextValue();
};