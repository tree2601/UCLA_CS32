#ifndef TRIE_H
#define TRIE_H



#include <string>
#include <vector>
using namespace std;//by me
#include<iostream>//by me

template< typename ValueType>
class Trie
{
public:
	Trie();
	~Trie();
	void reset();
	void insert(const std::string& key, const ValueType& value);
	std::vector< ValueType > find(const std::string& key, bool exactMatchOnly) const;


	// C++11 syntax for preventing copying and assignment
	Trie(const Trie&) = delete;
	Trie& operator=(const Trie&) = delete;

private:
	struct Node {
		vector<ValueType> m_value;//for storing values;
		vector<Node*> m_child;//for storing child pointers;

		Node() {
			for (int i = 0; i < 26; i++) {
				m_child.push_back(nullptr);
			}
		}

	};

	Node* m_root;
	void deleteAllNodesFrom(Node* ptr);
	void find_helper(Node* current, const string& key, const int& s,bool exactMatchOnly, vector<ValueType>& target) const;


};

template< typename ValueType>
void Trie<ValueType>::deleteAllNodesFrom(Node* ptr) {
	if (ptr == nullptr) {
		return;
	}

	for (unsigned int i = 0; i < ptr->m_child.size(); i++) {
		deleteAllNodesFrom(ptr->m_child[i]);
	}
	delete ptr;

}
template< typename ValueType>
Trie<ValueType>::Trie() {

	m_root = new Node;
};

template< typename ValueType>
Trie<ValueType>::~Trie() {

	deleteAllNodesFrom(m_root);
};

template< typename ValueType>
void Trie<ValueType>::reset() {
	deleteAllNodesFrom(m_root);
	m_root = new Node;
}

template< typename ValueType>
void Trie<ValueType>::insert(const std::string& key, const ValueType& value) {
	Node* current = m_root;
	for (unsigned int i = 0; i < key.size(); i++) {
		if (current->m_child[(key[i] - 'A')] == nullptr) {
			current->m_child[(key[i] - 'A')] = new Node;
		}
		current = current->m_child[(key[i] - 'A')];

	} 
	
	current->m_value.push_back(value);
}

template< typename ValueType>
void Trie<ValueType>::find_helper(Node* current, const string& key, const int& s,bool exactMatchOnly, vector<ValueType>& target)const {
	//cerr << key[0] << endl;
	if (key.empty()) {

		for (int i = 0; i < current->m_value.size(); i++) {
			
			target.push_back(current->m_value[i]);

		}
		return;
	}
	
	if (key.size() == s && current->m_child[key[0] - 'A'] == nullptr) {
		
		return;
	}

	if (key.size() == 1) {
		if (current->m_child[key[0] - 'A'] != nullptr) {
			for (int j = 0; j < current->m_child[key[0] - 'A']->m_value.size(); j++) {
				target.push_back(current->m_child[key[0] - 'A']->m_value[j]);
			}
		}
		if (exactMatchOnly == false) {
			for (int j = 0; j < current->m_child.size(); j++) {
				if (('A' + j) != key[0] && current->m_child[j] != nullptr) {
					for (int k = 0; k < current->m_child[j]->m_value.size(); k++) {
						target.push_back(current->m_child[j]->m_value[k]);
					}
				}
				
			}
		}
		return;
	}

	if (current->m_child[key[0] - 'A'] != nullptr) {

		find_helper(current->m_child[key[0] - 'A'], key.substr(1),s, exactMatchOnly, target);

	}

	 if (exactMatchOnly == false) {
		 
		 
		for (int j = 0; j < current->m_child.size(); j++) {
			if (('A' + j) != key[0] && current->m_child[j] != nullptr) {

				find_helper(current->m_child[j], key.substr(1),s, true, target);
			}
		}

	}


}


template< typename ValueType>
std::vector<ValueType> Trie<ValueType>::find(const std::string& key, bool exactMatchOnly) const {
	vector<ValueType> target_values;
	int size = key.size();
		find_helper(m_root, key, size, exactMatchOnly, target_values);

	return target_values;
}





//this is the end;









#endif
// TRIE_INCLUDED

