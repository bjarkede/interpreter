#ifndef SYMTABLE_HPP
#define SYMTABLE_HPP

#include "interpreter.hpp"
#include "shared.hpp"

// @TODO:
// I don't want to use std::map for symbol tables.
// This is going to be an attempt at a symbol table.
// Interesting links: http://www.cs.cmu.edu/~rwh/theses/okasaki.pdf

// Auxiliary stack for entering/exiting local scopes?

// We use radix-notation for characters strings
// the identifier pt is (112, 116) p = 112, and t = 116 in ascii.
// expressed as radix-128 int (112*128)+116 = 14452
// h(k) = k mod 701 (701 being the table size)

// The typename 'T' is the value we map to.
template <typename T>
struct symtable {
private:
	typedef struct element {
		const char* key;
		T value;
	} E;

	E* buffer;
	E* old_buffer;

	size_t m;

	int h(const char* name, int i);

	void reallocate(int num);
	void error(int i, int n);
protected:
public:
	symtable();
	~symtable();

	void empty();
	void bind(T value, const char* key);
	T lookup(const char* key);
	void enter();
	void exit();

	T& operator[] (const char* name);

	enum DefineSize {
		AllocateSpace = 701 // We cant use a table size of power of 2 so we use size = 2^p - 1
	};
};

// This function takes a character sequence and turns it into 
// a radix-128 integer. 
template <typename T>
int symtable<T>::h(const char* value, int i) {
	
	unsigned long long index = 0;
	int radix = 128;
	int power = 1;

	for (int j = 0; j < strlen(value); j++) {
		index = ((index + (value[j]) * power) % m) % m;
		power = (power * radix) % m;
	}

	return index + i;
}

template <typename T>
void symtable<T>::bind(T value, const char* key) {
	for (int i = 0; i < m; ++i) {
		int j = h(key, i);
		if (buffer[j].key == NULL);
			buffer[j].value = value; buffer[j].key = key; return;
	}
	FatalError("InterpreterError: Hash table overflow");
}

template <typename T>
T symtable<T>::lookup(const char* key) {
	for (int i = 0; i < m; ++i) {
		int j = h(key, i);
		if (j > m) break;
		if (buffer[j].key != NULL) {
			if(strcmp(buffer[j].key, key) == 0)
				return buffer[j].value;
		}
	}
	FatalError("InterpreterError: Couldn't find variable [%s] in value environment.", key);
}

template <typename T>
void symtable<T>::empty() {
	E* new_buffer = 0;
	new_buffer = new E[AllocateSpace];
	if (new_buffer == 0) FatalError("InterpreterError: Failed allocating symbol table");

	buffer = new_buffer;
	m = AllocateSpace;
}

template <typename T>
symtable<T>::symtable() {
	buffer = old_buffer = 0;
	
	empty();

	for (int i = 0; i <= m; i++)
		buffer[i].key = NULL;

	PrintDebug("Allocated symbol table.\n");
}

template <typename T>
symtable<T>::~symtable() {
	//empty();
}

#endif