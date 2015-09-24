#ifndef MYWORD_H
#define MYWORD_H

#include <string>

#include "XML.h"
#include "Occurrence.h"
#include "Word.h"
#include "BST.h"

class Word{
public:

	Word & operator=(const Word & other);
	
	Word(const Word& other);
	
	Word();
	
	//!@param str value of word string
	Word(const std::string str);
	
	~Word();
	
	//! @return word string
	const std::string getValue() const;
	
	//!@param value to set word string to
	void setValue(const std::string& value);
	
	//! @param url where the word occurred 
	//! @return num of occurences of the word on that page
	int getOccurrence(const std::string url) const;
	
	//! @param url where the word occurred 
	//! @return num of occurences of the word on that page
	LinkedList<Occurrence>* getOccurrences() const;
	
	//! @param url string
	//! @param number of times word occured at the location
	void addOccurrence(std::string url, int num);
	
	//! @param occurrence to add to list of occurrences
	void addOccurrence(Occurrence occ)const;
	
	//! Writes an XML representation of the word and it's occurrences to out
	//! @param XML object to write to
	void WriteXML(XML* xml)const;
	
	//! @param other word to compare this word to; 
	//! @return comparison of word values (returns value string.Compare())
	int compare(const Word &other)const;
	
private:
	std::string wordValue;
	LinkedList<Occurrence>* occurrences;
};

#endif
