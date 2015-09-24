#ifndef MYPAGE_H
#define MYPAGE_H

#include "XML.h"

#include <string>

//! A page has a description and a URL. The description consists of 
//! 	one of the following (in criteria order):
	//!		1. The text contents of the <TITLE> tag.
	//!		2. The text contents of the first header tag (<H1>,<H2>,<H3>,...)
	//!		in the file, regardless of the number associated with the header, 
	//!			just whichever one comes first.
	//!		3. The first 100 characters found between the <BODY></BODY> tags, 
	//!			excluding tags and whitespace (i.e., when collecting the first
	//!			100 characters, do not count characters that are part of tags or
	//! 		that are whitespace). Note that the first 100 characters may be separated by tags.
class Page{
public:

	Page();
	
	//! create page with URL only
	Page(std::string url);
	
	//! create page with given url and description
	Page(std::string url, std::string description);
	
	~Page();
	
	//! Assignment operator 
	Page & operator=(const Page & other){
		if(this!=&other){
			pageURL = other.getURL();
			pageDescription = other.getDescription();
		}
		return *this;
	}
		
	//! @return URL string
	const std::string getURL() const;
	
	//!@param URL that page belongs to
	void setURL(const std::string& url);
	
	//!@return description of page
	const std::string getDescription() const;
	
	//! @param description of page
	void setDescription(const std::string& description);
	
	//! @return comparison of page URL's
	int compare(const Page& other) const;
	
	//! writes an XML representation of the word
	//! @param XML object to write to
	void WriteXML(XML* xml);
	
private:
	std::string pageURL;
	std::string pageDescription;
};

#endif

