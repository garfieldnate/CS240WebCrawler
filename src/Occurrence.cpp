#include "Occurrence.h"

#include "URL.h"
#include "XML.h"

#include <stdlib.h>
#include <sstream>

	Occurrence::Occurrence(){}
	
	Occurrence::Occurrence(URL Url, int occ):numOccurrences(occ),url(Url){}
	
	int Occurrence::occurrences()const{
		return numOccurrences;
	}
	
	URL Occurrence::getURL()const{
		return url;
	}
	void Occurrence::setURL(URL u){
		url = u;
	}
	
	void Occurrence::setOccurrences(int o){
		numOccurrences = o;
	}
	
	int Occurrence::compare(const Occurrence & other)const{
		return url.compare(other.getURL().getURLstring());
	}
	
	void Occurrence::WriteXML(XML* xml)const{
		std::stringstream numString;
		numString << numOccurrences;
		
		xml->openTag("occurrence");
			url.WriteXML(xml);
			xml->openTag("count");
				xml->writeText(numString.str());
			xml->closeTag();
		xml->closeTag();
	}
