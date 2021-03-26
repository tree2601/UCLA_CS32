using namespace std;
#include"Set.h"
#include<iostream>


Set::Set() {
	head = new Node;
	head->next = head;
	head->prev = head;
	n = 0;
}//create an empty list with a dummy node pointed by head;

Set::Set(const Set& other) {
	head = new Node;
	head->next = head;
	head->prev = head;
	n =0;//create an empty list with a dummy node pointed by head;
	if (other.head->next != other.head) {//check if the other set is empty;
		Node* p = other.head->next;
		for (p = other.head->next; p!= other.head; p = p->next) {
			insert(p->data);
		}//add every element in the other set to the set;
		
	}
	


	
}

Set& Set:: operator=(const Set& rhs) {
	if (this != &rhs) {// if the two sets are different
		Set temp(rhs);//temp is a copy of rhs;
		swap(temp);//swap the set and temp;
	}
	return *this;
}


Set::~Set() {
	Node* p = head;//starting from the dummy node;
	while (p->next != head) {
		Node* temp = p;//temp stores p's current information;
		p = p->next;//p moves to the next node;
		delete temp;//delete the last node;
	}//finally p will stop at the last node;
	delete p;//delete the last node;
	head = nullptr;
}


bool Set::empty() const {
	if (head->next == head||n==0) {
		return true;
	}//if the dummy node's next points to itself, then it is an empty list;
	return false;
}
int Set::size() const {
	return n;
}

bool Set::insert(const ItemType& value) {
		Node* p = head->next;
			for (p = head->next; p!= head; p = p->next) {
				if (p->data == value) {
					return false;
				}//check if the value is already in the set;
			}
			//the loop sets p to go through the entire list and return to the dummy node;
		
		
		Node* newone = new Node;
		newone->data = value;//let the new node store the value;
		
		p->prev->next = newone;//let the last item point to newone;
		newone->prev = p->prev;//let the new item point to the old last item;
		newone->next = head;//let the new item point to the dummy node;
		head->prev = newone;// let the dummy node point to the new item;
		n++;
	
		return true;
	}//this function adds a new item to the end of the list;


bool Set::erase(const ItemType& value) {
	Node *p = head->next;
		for (p = head->next; p!= head; p = p->next) {
			if (p->data == value) {
				p->next->prev = p->prev;//change the prev pointer of the next node to point to the last node;
				p->prev->next = p->next;//change the next pointer of the previous node point to the next node;
				delete p;//clear the value stored in p;
				n--;
				return true;
			}
		}
		
		
		return false;
	}

	bool Set::contains(const ItemType& value) const {
		Node* p = head->next;
		for (p = head->next; p!= head; p = p->next) {
			if (p->data == value) {
				return true;
			}
		}//loop through every element to see if any data matches with value;
		
		return false;
	}

	bool Set::get(int pos, ItemType& value) const {
		
		for (Node* p = head->next; p != head; p = p->next) {
			int count = 0;//record the number of elements greater than the ith element;
			for (Node*q = head->next; q != head; q = q->next) {
				if (p->data > q->data) {
					count++;
				}
			}

			if (count == pos) {
				value = p->data;
				return true;
			}//if count matches with pos, copy it into value;
		}//compare each element with all the elements in the list;

		return false;
	}

	void Set::swap(Set& other) {
		Node* temp = head;
		head = other.head;
		other.head = temp;//swap the head pointer;

		int temp1 = n;
		n = other.n;
		other.n = temp1;//swap the number of nodes;
	}



	void Set::dump() const {
		Node* p = head;
		if (p->next == head) {
			cerr << "empty set" << endl;
		}
		else {
			for (p = head->next; p->next != head; p = p->next) {
				cerr << "data is " << p->data << endl;
			}
			cerr << "data is " << p->data << endl;
			
		}
		cerr << "n is equal to" << n << endl;
	
	}


	void unite(const Set& s1, const Set& s2, Set& result) {
		Set copy(s1);//make a copy of s1;
		result.swap(copy);//swap result and s1 so now result becomes s1;
		
		for (int i = 0; i < s2.size(); i++) {
			ItemType temp;
			s2.get(i, temp);
			if (!result.contains(temp)) {
				result.insert(temp);
			}
		}//loop through s2, if one element exists in result and not in s1, add it to result;
		
	}
	void subtract(const Set& s1, const Set& s2, Set& result) {
		Set copy(s1);//make a copy of s1;
		result.swap(copy);//swap result and s1 so now result becomes s1;
		
		for (int i = 0; i < s1.size(); i++) {
			ItemType temp;
			s1.get(i,temp);
			if (s2.contains(temp)) {
				
				result.erase(temp);
			}
		}//loop through result; if one element is found in s2, erase it;

	}
	