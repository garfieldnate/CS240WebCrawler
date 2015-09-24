#ifndef OCCURENCE_H
#define OCCURENCE_H

#include "URL.h"
#include "XML.h"

//! This class is responsible for storing the number of occurences of a word in one URL.
class Occurrence{
	public:
	
		//! @param URL location of this occurrence
		//! @param number of occurrences at this URL
		Occurrence(URL Url, int occ);
		Occurrence();
		
		//! @return number of occurences
		int occurrences()const;
		void setOccurrences(int o);
		
		//!@return URL that the word occured in
		URL getURL()const;
		void setURL(URL url);
		
		//! Writes an XML representation of the occurence
		//! @param XML object to write to
		void WriteXML(XML* xml)const;
		
		//! return comparison of URL strings
		int compare(const Occurrence & other)const;
	private:
		int numOccurrences;
		URL url;
};

#endif
