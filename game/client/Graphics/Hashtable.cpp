#include "Hashtable.h"

template <class T>
int HashTable<T>::Size() const {
	return _map.size();
}

template <class T>
void HashTable<T>::Enter(const char *key, T value) {
	_map.insert(pair<const char *, T>(key, value));
}

template <class T>
void HashTable<T>::Remove(const char *key) {
	_map.erase(key);
}

template <class T>
T HashTable<T>::Lookup(const char *key) {
	typename map<const char *, T>::iterator curr, last;
	T value = NULL;
	if (_map.count(key) != 0) {
		curr = _map.find(key);
		last = _map.upper_bound(key);
		do {
			value = curr->second;
			++curr;
		} while (curr != last);
	}
	return value;
}

template <class T>
HashTable<T>* HashTable<T>::MakeCopy() {
	HashTable<T>* copy = new HashTable<T>();
	typename map<const char*, T>::iterator it;
	it = _map.begin();
	while (it != _map.end()) {
		copy->Enter(it->first, it->second);
		++it;
	}
	return copy;
}

template <class T>
Iterator<T> HashTable<T>::GetIterator() {
	return Iterator<T>(_map);
}

template <class T>
T Iterator<T>::GetNextValue() {
	return (cur == end ? NULL : (*cur++).second);
}