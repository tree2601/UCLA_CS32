#include "provided.h"
#include <string>
#include <vector>
#include <iostream>

#include<fstream>//added by me
using namespace std;

class GenomeImpl
{
public:
	GenomeImpl(const string& nm, const string& sequence);
	static bool load(istream& genomeSource, vector<Genome>& genomes);
	int length() const;
	string name() const;
	bool extract(int position, int length, string& fragment) const;
private:
	string m_name;
	string m_sequence;
	static bool checkValidName( string &s) ;
	static bool checkValidSequence( string &s) ;
	static void addNewGenome(string n, string s, vector<Genome>& target);
};

bool GenomeImpl::checkValidName( string& s) {
	if (s.size() > 1) {
		if (s[0] == '>') {
			return true;
		}
	}
	return false;
	

}
bool GenomeImpl::checkValidSequence( string& s) {
	if (s.size() >= 1 && s.size() <= 80) {
		for (int i = 0; i < s.size(); i++) {
			if (s[i] != 'a'&&s[i] != 'A'&&s[i] != 'c'&&s[i] != 'C'&&s[i] != 't'&&s[i] != 'T'&&s[i] != 'g'&&s[i] != 'G'&&s[i] != 'n'&&s[i] != 'N') {
				return false;
			}
			s[i] = toupper(s[i]);
			
		}
		return true;
	}
	return false;
}

void GenomeImpl::addNewGenome(string n, string s, vector<Genome>& target) {
	Genome newGuy(n, s);
	target.push_back(newGuy);
}

GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
{
	m_name = nm;
	m_sequence = sequence;
	// This compiles, but may not be correct
}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes)
{
	if (!genomeSource) {
		cerr << "not loaded" << endl;
		return false;
	}
	
	
	string temp_name = "";
	string temp_sequence = "";
	string temp;
	if (getline(genomeSource, temp)) {
		if (checkValidName(temp)) {
			
			temp_name = temp.substr(1);
		}
		else {
			return false;
		}

	}

	while (getline(genomeSource, temp)) {
		
		
		if (checkValidSequence(temp)) {
			
			temp_sequence += temp;
		}
		if (checkValidName(temp)) {
			if (temp_sequence.empty()) {
				return false;
			}
			addNewGenome(temp_name, temp_sequence,genomes);
			temp_name = temp.substr(1);
			temp_sequence = "";
		}
		if (!checkValidSequence(temp) && !checkValidName(temp)) {
			return false;
		}
		
	}

	if (!temp_name.empty() && !temp_sequence.empty()) {
		addNewGenome(temp_name, temp_sequence, genomes);
		
		return true;
		
	}
	else {
		return false;
	}
}
int GenomeImpl::length() const
{
	return m_sequence.size();  // This compiles, but may not be correct
}

string GenomeImpl::name() const
{
	return m_name;  // This compiles, but may not be correct
}

bool GenomeImpl::extract(int position, int length, string& fragment) const
{
	if (position + length <= m_sequence.size()) { 
		fragment = m_sequence.substr(position, length); 
		return true;
	}

	return false;  // This compiles, but may not be correct
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
	m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
	delete m_impl;
}

Genome::Genome(const Genome& other)
{
	m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
	GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
	delete m_impl;
	m_impl = newImpl;
	return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes)
{
	return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
	return m_impl->length();
}

string Genome::name() const
{
	return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
	return m_impl->extract(position, length, fragment);
}

//this is the end;





