/*
//this tree is only for string value storage;
#include <string>
#include <vector>
using namespace std;//by me
#include<iostream>//by me


class Trie
{
public:
	Trie();
	~Trie();
	void reset();
	void insert(const std::string& key, const string& value);
	std::vector<string> find(const std::string& key, bool exactMatchOnly) const;
	

	// C++11 syntax for preventing copying and assignment
	Trie(const Trie&) = delete;
	Trie& operator=(const Trie&) = delete;

private:
	struct Node {
		vector<string> m_value;//for storing values;
		vector<Node*> m_child;//for storing child pointers;

		Node() {
			for (int i = 0; i < 26; i++) {
				m_child.push_back(nullptr);
			}
		}

	};

	Node* m_root;
	void deleteAllNodesFrom(Node* ptr);
	void find_helper(Node* current, const string& key, bool exactMatchOnly, vector<string>& target) const;

	
};


void Trie::deleteAllNodesFrom(Node* ptr) {
	if (ptr == nullptr) {
		return;
	}
	
	for (unsigned int i = 0; i < ptr->m_child.size(); i++) {
		deleteAllNodesFrom(ptr->m_child[i]);
	}
	delete ptr;

}

Trie::Trie() {

	m_root = new Node;
};

Trie::~Trie() {

	deleteAllNodesFrom(m_root);
};

void Trie::reset() {
	deleteAllNodesFrom(m_root);
	m_root = new Node;
}

void Trie::insert(const std::string& key, const string& value) {
	Node* current = m_root;
	for (unsigned int i = 0; i < key.size(); i++) {
		if (current->m_child[(key[i] - 'A')] == nullptr) {
			current->m_child[(key[i] - 'A') ]= new Node;
		}
		current = current->m_child[(key[i] - 'A')];
		
	}
	current->m_value.push_back(value);
}

void Trie::find_helper(Node* current, const string& key, bool exactMatchOnly, vector<string>& target)const {
	if (key.empty()) {
		
		for (int i = 0; i < current->m_value.size(); i++) {
			target.push_back(current->m_value[i]);
		}
		return;
	}
	
	if (current->m_child[key[0] - 'A'] != nullptr) {
		
		find_helper(current->m_child[key[0] - 'A'], key.substr(1), exactMatchOnly, target);
		
	}
	
	if (exactMatchOnly == false){
		for (int j = 0; j < current->m_child.size(); j++) {
				if (('A' + j) != key[0] && current->m_child[j] != nullptr) {
					
					find_helper(current->m_child[j], key.substr(1), true, target);
				}
		}
		
	}
	
	
}

std::vector<string> Trie::find(const std::string& key, bool exactMatchOnly) const {
	vector<string> target_values;
	if (m_root->m_child[key[0] - 'A'] != nullptr) {
		
		find_helper((m_root->m_child[key[0] - 'A']), key.substr(1), exactMatchOnly, target_values);
	}
	
	return target_values;
}

/*int main() {
	cout << "test begin" << endl;



	Trie mytrie;
	string myteststring = "ACG";
	mytrie.insert("FIRSTA","10");
	mytrie.insert("FIRSTA", "11");
	mytrie.insert("FIRSTB", "15");
	mytrie.insert("FIRSTB", "16");
	mytrie.insert("FIRSTC", "40");
	mytrie.insert("FIRSTB", "41");
	mytrie.insert("FIRSTD", "20");

	mytrie.insert("SEGOND", "1");
	mytrie.insert("SEGOND", "2");
	mytrie.insert("SEGOND", "3");
	mytrie.insert("SEGOND", "4");

	mytrie.insert("SECOND", "30");
	mytrie.insert("SECOND", "25");
	mytrie.insert("SECOND", "65");
	
	//string myteststring2 = myteststring.substr(3);
	//cout << "the result one is" << myteststring2<<"!" << endl;
	
	 vector<string> testvector= mytrie.find("SEGOND", false);
	 for (int i = 0; i < testvector.size(); i++) {
		 cout << "value " << i + 1 << "is " << testvector[i] << endl;
	 }
	 string test="origin";
	 //getline(cin, test);
	 cout << test << endl;
	 
	//cout <<"the result is "<< myteststring[1] - 'A' << endl;


	cout << "test end" << endl;
}   */
// TRIE_INCLUDED

//template< typename ValueType>
//<ValueType>







//----------for test===========
/*int main()
{


	cerr << "test begin" << endl;
	Genome testgenome1("Genome 1", "CGGTGTACNACGACTGGGGATAGAATATCTTGACGTCGTACCGGTTGTAGTCGTTCGACCGAAGGGTTCCGCGCCAGTAC");
	Genome testgenome2("Genome 2", "TAACAGAGCGGTNATATTGTTACGAATCACGTGCGAGACTTAGAGCCAGAATATGAAGTAGTGATTCAGCAACCAAGCGG");
	Genome testgenome3("Genome 3", "TTTTGAGCCAGCGACGCGGCTTGCTTAACGAAGCGGAAGAGTAGGTTGGACACATTNGGCGGCACAGCGCTTTTGAGCCA");

	Genome testgenome4("Genome 4", "CGGTGTACNACGACTGGGGATAGAAGAGCCAGAATATGAAGTAGTGATTCTNGGCGGCACAGCGCTTTTGAGCCA");
	GenomeMatcher testmatcher(4);
	testmatcher.addGenome(testgenome1);
	testmatcher.addGenome(testgenome2);
	testmatcher.addGenome(testgenome3);
	testmatcher.addGenome(testgenome4);
	//GTTACGAATC
	std::vector<DNAMatch>	matches;
	bool result;
	result =testmatcher.findGenomesWithThisDNA("GCTTTTGAGCCA", 12, true, matches);
	vector<GenomeMatch> matches2;
	testmatcher.findRelatedGenomes(testgenome4, 25, true, 0.01, matches2);
	if (matches2.empty()) {
		cout << "the matches2 is empty. " << endl;
	}
	for (int i = 0; i < matches2.size(); i++) {
		cout << matches2[i].genomeName << " has percentage of " << matches2[i].percentMatch << endl;
	}

	if (matches.empty()) {
		cout << "the matches is empty. " << endl;
	}
	for (int i = 0; i < matches.size(); i++) {
		cout << matches[i].genomeName << "of length " << matches[i].length << " at position " << matches[i].position << endl;
	}

	cerr << "test end" << endl;
	//Genome testgenome3("Genome 3", "");
	//testmatcher.addGenome(testgenome3);
	map<string, int> mytestmap;
	mytestmap["first"] = 0;
	mytestmap["first"]++;
	cerr << mytestmap["first"] << endl;



}*/





//template< typename ValueType>
//<ValueType>

/*added
if (key.size() == 1) {
	if (current->m_child[key[0] - 'A'] != nullptr) {
		for (int i = 0; i < current->m_child[key[0] - 'A']->m_value.size(); i++) {
			target.push_back(current->m_child[key[0] - 'A']->m_value[i]);
		}
		return;
	}
	else if (exactMatchOnly == false) {
		for (int j = 0; j < current->m_child.size(); j++) {
			if (('A' + j) != key[0] && current->m_child[j] != nullptr) {

				for (int i = 0; i < current->m_child[j]->m_value.size(); i++) {
					target.push_back(current->m_child[j]->m_value[i]);
				};
				return;
			}
		}
	}
}
finished*/



/*void GenomeMatcherImpl::test(vector<mypair> testtarget)const  {
	if (testtarget.empty()) {
		cerr << "its empty" <<endl;
	}
	for (int i = 0; i < testtarget.size(); i++) {
		cerr << "the genome name is"<<testtarget[i].g_name << endl;
		cerr <<"the genome position is"<< testtarget[i].g_position << endl;
		 }
}*/
//merely for test. 




/*int main() {

	ifstream myinfile("C:/Users/tree2601/Desktop/data/Halorubrum_chaoviator.txt");
	if (!myinfile) {
		cerr << "loaded fail" << endl;
	}
	cerr << "test begin" << endl;
	Genome G("random","AACCTTGGNN");
	string T;


	vector<Genome> mytestgenomes;
	G.load(myinfile, mytestgenomes);
	if (mytestgenomes.empty()) {
		cerr << "it is still empty" << endl;
	}
	for (int k = 0; k < mytestgenomes.size(); k++) {
		cerr << "genome name " << k + 1 << " is " << mytestgenomes[k].name()<<endl;
		string sequence;
		mytestgenomes[k].extract(0, 200, sequence);
		cerr << "genome sequence " << k + 1 << " is " << sequence << endl;
	}
	cerr << "test end" << endl;
}*/