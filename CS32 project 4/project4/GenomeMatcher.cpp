#include "provided.h"
#include"Trie.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include<map>
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
		int g_place;
		int g_position;
		mypair(int place, int position) {
			g_place = place;
			g_position = position;
		}
		
		
	};
	

	Trie<mypair> myTrie;
	vector<Genome> m_genomes;
	bool addDNA(string n, int l, int p, vector<DNAMatch>& target) const;
	bool addGenomeMatch(string name, double percent,vector<GenomeMatch>& target)const ;
	
static	bool matchTwo(string subject, string target, bool exactMatchOnly);
};


bool GenomeMatcherImpl::addGenomeMatch(string name, double percent, vector<GenomeMatch>& target)const {
	
	GenomeMatch newGuy{
		name,percent
	};
	target.push_back(newGuy);
	return true;
	
}




 bool GenomeMatcherImpl::matchTwo(string subject, string target, bool exactMatchOnly) {
	if (subject[0] != target[0]) {
		return false;
	}
	if (subject.size() != target.size()) {
		return false;
	}
	
	int difference = 0;
	for (int i = 0; i < subject.size(); i++) {
		if (subject[i] != target[i]) {
			difference++;
		}
	}
	if (exactMatchOnly == true) {
		return (difference == 0);
	}
	else if (exactMatchOnly == false) {
		return (difference <= 1);
	}
	
}





bool GenomeMatcherImpl::addDNA(string n, int l, int p, vector<DNAMatch>& target) const {
	DNAMatch newGuy{n,l,p };
	
	for (int j = 0; j < target.size(); j++) {
		if (target[j].genomeName == n) {//if same name;
			if (l > target[j].length) {// if larger length
				target[j].length = l;
				target[j].position = p;
				return true;
			}
			else if (l == target[j].length) {//if equal length
				if (p < target[j].position){  //if smaller position;
					target[j].position = p;
					return true;
				}
				else  {
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
	int x = m_genomes.size() - 1;
	for (int i = 0; i <= genome.length()- minimumSearchLength(); i++) {
		string temp;
		genome.extract(i, minimumSearchLength(), temp);
		mypair newGuy(x, i);
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

	if (minimumLength<minimumSearchLength()|| fragment.size() < minimumLength) {
		return false;
	}
	vector<mypair> possiblePairs = myTrie.find(fragment.substr(0, minimum), exactMatchOnly);//Length

	for (int x = 0; x < possiblePairs.size(); x++) {
		int place = possiblePairs[x].g_place;
		int pos = possiblePairs[x].g_position;
		string temp;
		m_genomes[place].extract(pos, minimum, temp);

		string temp2 = temp;
		int z = minimum;

		for (int y = 1; y <= fragment.size() - minimum; y++) {
			if (pos + minimum + y > m_genomes[place].length()) {
				break;
			}
			m_genomes[place].extract(pos, minimum+y, temp);
			if (!matchTwo(temp, fragment.substr(0, minimum + y), exactMatchOnly)) {
				break;
			}
			temp2 = temp;
			z++;

		}
		if (temp2.size() >= minimumLength) {
			bool a =addDNA(m_genomes[place].name(), temp2.size(), pos, matches);
			if (a) {
				findoneGenome = true;
			}
		}
    }
	return findoneGenome;  // This compiles, but may not be correct
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{   
	if (fragmentMatchLength < minimum) {
		return false;
	}
	bool findoneresult = false;

	map<string, int> DNAcount;
	for (int a = 0; a < m_genomes.size(); a++) {
		DNAcount[m_genomes[a].name()] = 0;
	}
	int num = query.length() / fragmentMatchLength;
	for (int i = 0; i < num; i++) {
		string subject;
		query.extract(fragmentMatchLength*i, fragmentMatchLength, subject);
		vector<DNAMatch> temp_matches;
		findGenomesWithThisDNA(subject, fragmentMatchLength, exactMatchOnly, temp_matches);
		for (int j = 0; j < temp_matches.size(); j++) {
			DNAcount[temp_matches[j].genomeName]++;
		}

	}
	for (auto it = DNAcount.begin(); it != DNAcount.end(); it++) {
		double count = it->second*1.0;
		if (count / num > matchPercentThreshold) {
			addGenomeMatch(it->first, count / num, results);
			findoneresult = true;
		}
	}
	
	bool happened;
	for (int g = 0; g < results.size() - 1; g++) {
		 happened = false;
		for (int k = 0; k < results.size() - g - 1; k++) {
			if (results[k].percentMatch < results[k + 1].percentMatch) {
				double temp = results[k].percentMatch;
				results[k].percentMatch= results[k + 1].percentMatch;
				results[k + 1].percentMatch = temp;

				string temps = results[k].genomeName;
				results[k].genomeName = results[k + 1].genomeName;
				results[k + 1].genomeName = temps;
				happened = true;
			}
		}
		if (happened == false) {
			break;
		}
	
	}


	return findoneresult;  // This is the end;
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


