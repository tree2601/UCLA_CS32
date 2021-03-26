#ifndef SET_H
#define SET_H
#include<string>
using ItemType = std::string;
class Set
{
public:
	Set();
	~Set();
	Set(const Set& other);
	Set& operator=(const Set& rhs);
	bool empty() const;
	int size() const;
	bool insert(const ItemType& value);
	bool erase(const ItemType& value);
	bool contains(const ItemType& value) const;
	bool get(int pos, ItemType& value) const;
	void swap(Set& other);
	void dump() const;
private:

	struct Node {

		ItemType data;
		Node* next;
		Node* prev;
	};
	Node* head;
	int n;// the number of items in the set;
};


void unite(const Set& s1, const Set& s2, Set& result);
void subtract(const Set& s1, const Set& s2, Set& result);
#endif
