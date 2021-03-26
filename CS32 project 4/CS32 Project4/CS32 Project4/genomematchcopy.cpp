/*#include "provided.h"
#include"Trie.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class GenomeMatcherImpl
{
public:
	GenomeMatcherImpl(int minSearchLength);
	void addGenome(const Genome& genome);
	int minimumSearchLength() const;
	bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
	bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
	int minimum;
	struct mypair {
		string g_name;
		int g_position;
		mypair(string name, int position) {
			g_name = name;
			g_position = position;
		}


	};
	Trie<mypair> myTrie;
	vector<Genome> m_genomes;
	bool addDNA(string n, int l, int p, vector<DNAMatch>& target) const;
	void test(vector<mypair> testtarget)const;//only for test;
};

struct mytestpair {
	string g_name;
	int g_position;
	mytestpair(string name, int position) {
		g_name = name;
		g_position = position;
	}


};








void GenomeMatcherImpl::test(vector<mypair> testtarget)const {
	if (testtarget.empty()) {
		cerr << "its empty" << endl;
	}
	for (int i = 0; i < testtarget.size(); i++) {
		cerr << "the genome name is" << testtarget[i].g_name << endl;
		cerr << "the genome position is" << testtarget[i].g_position << endl;
	}
}//merely for test. 

bool GenomeMatcherImpl::addDNA(string n, int l, int p, vector<DNAMatch>& target) const {
	DNAMatch newGuy{ n,l,p };
	for (int j = 0; j < target.size(); j++) {
		if (target[j].genomeName == n) {
			if (l > target[j].length) {
				target[j].length = l;
				target[j].position = p;
				return true;
			}
			else if (l == target[j].length) {
				if (p < target[j].position) {
					target[j].position = p;
					return true;
				}
				else if (p >= target[j].position) {
					return false;
				}
			}
			else if (l < target[j].length) {
				return false;
			}
		}
	}

	target.push_back(newGuy);
	return true;
}



GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
{
	minimum = minSearchLength;
	// This compiles, but may not be correct
}



void GenomeMatcherImpl::addGenome(const Genome& genome)
{
	m_genomes.push_back(genome);

	for (int i = 0; i <= genome.length() - minimumSearchLength(); i++) {
		string temp;
		genome.extract(i, minimumSearchLength(), temp);
		mypair newGuy(genome.name(), i);
		myTrie.insert(temp, newGuy);


	}
	// This compiles, but may not be correct
}

int GenomeMatcherImpl::minimumSearchLength() const
{
	return minimum;  // This compiles, but may not be correct
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
	bool findoneGenome = false;

	if (minimumLength < minimumSearchLength() || fragment.size() < minimumLength) {
		return false;
	}

	for (int j = minimumLength; j <= fragment.size(); j++) {
		string temp_frag = fragment.substr(0, j);
		cerr << "temp_frag is " << temp_frag << endl;
		vector<mypair>  temp_pair = myTrie.find(temp_frag, exactMatchOnly);

		test(temp_pair);//just for test;
		for (int i = 0; i < temp_pair.size(); i++) {
			string temp_name = temp_pair[i].g_name;
			int temp_pos = temp_pair[i].g_position;
			bool a = addDNA(temp_name, j, temp_pos, matches);
			if (a) {
				findoneGenome = true;
			}
		}

	}
	return findoneGenome;  // This compiles, but may not be correct
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
	return false;  // This compiles, but may not be correct
}

//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
	m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
	delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
	m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
	return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
	return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
	return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}




//----------for test===========
/*int main()
{


	cerr << "test begin" << endl;
	Genome testgenome1("Genome 1", "CGGTGTACNACGACTGGGGATAGAATATCTTGACGTCGTACCGGTTGTAGTCGTTCGACCGAAGGGTTCCGCGCCAGTAC");
	Genome testgenome2("Genome 2", "TAACAGAGCGGTNATATTGTTACGAATCACGTGCGAGACTTAGAGCCAGAATATGAAGTAGTGATTCAGCAACCAAGCGG");
	Genome testgenome3("Genome 3", "TTTTGAGCCAGCGACGCGGCTTGCTTAACGAAGCGGAAGAGTAGGTTGGACACATTNGGCGGCACAGCGCTTTTGAGCCA");

	GenomeMatcher testmatcher(3);
	testmatcher.addGenome(testgenome1);
	testmatcher.addGenome(testgenome2);
	testmatcher.addGenome(testgenome3);

	std::vector<DNAMatch>	matches;
	bool result;
	result = testmatcher.findGenomesWithThisDNA("AAT", 3, true, matches);

	if (matches.empty()) {
		cout << "the matches is empty. " << endl;
	}
	for (int i = 0; i < matches.size(); i++) {
		cout << matches[i].genomeName << "of length " << matches[i].length << " at position " << matches[i].position << endl;
	}


	//Genome testgenome3("Genome 3", "");
	//testmatcher.addGenome(testgenome3);
	cerr << "ignore the following " << endl;
	cout << "test begin" << endl;

	string A = "C";
	string B = A.substr(1);
	cout << "B is" << B << endl;

	Trie<mytestpair> mytrie;
	string myteststring = "ACG";
	mytestpair t1("genome1", 2);
	mytestpair t2("genome2", 5);
	mytrie.insert("GAATAC", t1);
	mytrie.insert("GAATAC", t2);
	mytrie.insert("GAATACN", t2);




	//string myteststring2 = myteststring.substr(3);
	//cout << "the result one is" << myteststring2<<"!" << endl;

	vector<mytestpair> testvector = mytrie.find("GAATACN", true);
	for (int i = 0; i < testvector.size(); i++) {
		cout << "name" << i + 1 << "is " << testvector[i].g_name << endl;
		cout << "pos " << i + 1 << "is " << testvector[i].g_position << endl;
	}



}

the old incorrect way;
for (int j = minimumLength; j <=fragment.size(); j++) {
		string temp_frag = fragment.substr(0, j);//complete
		string temp_piece = fragment.substr(j - minimumLength, minimumLength);//minlength;

		vector<mypair>  temp_pair = myTrie.find(temp_piece, false);
		for (int y = 0; y < temp_pair.size(); y++) {
			int p = temp_pair[y].g_place;
			string a;
			m_genomes[p].extract(temp_pair[y].g_position - (j - minimumLength), j - minimumLength, a);
			if (matchTwo(temp_frag, a, exactMatchOnly)) {
				bool a=addDNA(m_genomes[p].name(),j,temp_pair[y].g_position,matches);
				if (a) {
					findoneGenome = true;
				}
			}
		}
		
		
	struct mytestpair {
	string g_name;
	int g_position;
	mytestpair(string name, int position) {
		g_name = name;
		g_position = position;
	}


};*/
