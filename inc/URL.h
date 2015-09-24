#ifndef URL_H
#define URL_H

#include <string>

#include "XML.h"

//! This class is responsible for all URL processing; resolving relative URL's, removing unnecessary
//! portions, checking if the URL is HTML, and checking that the URL is within a certain scope

//! Known issue: does not check if URL is "file://something", which is invalid.
class URL{
	public:
		
		URL();
		
		//!@param url string
		URL(std::string str);
		
		//! Copy Constructor
		URL(const URL & other);
		
		URL & operator=(const URL & other);
		
		//!@return comparison of URL strings
		bool operator!=(const URL & other);
		
		//!@return URL string
		std::string getURLstring() const;
	
		//!@return prefix of URL; everything except file name, query and fragment.
	//!@throws InvalidURLException if the URL is not a valid absolute URL
		std::string getPrefix() const;
	
		//! removes fragment portion of URL
		void reduce();
		
		//!@return true if the URL is in the same scope as the current URL, false otherwise
		bool inSameScope(const URL & other) const;
		
		//! writes an XML representation of this object
		//! @param XML object to write to
		void WriteXML(XML* xml) const;
		
		//! @return true if the URL is an HTML link, false otherwise
		//! A URL is deemed HTML if: 
		//! 	The <path> part of the URL is a directory name
		//! 	The file name in the URL's <path> does not end with a file extension
		//! 	The file name in the URL's <path> ends with one of the following extensions:
		//!			.html, .htm, .shtml, .cgi, .jsp, .asp, .aspx, .php, .pl, .cfm
		//! Also returns true if the URL ends in '/'; this is because the retrieved version will
		//! be index.html or some such. Note that this only works on absolute URLs.
		static bool isHTML(const std::string & str);
		
		//! resolves URL with base into new absolute URL.
		//! @param absolute url to to use as base URL during resolution
		//! @throws InvalidURLException if this URL is not absolute
		//! @throws CS240Exception if this URL is empty
		void resolve(URL base);
		
		//! resolves relative and base URL into a new absolute URL
		//! @param absolute url to to use as base URL during resolution
		//! @throws InvalidURLException if rel is not relative, or base is not absolute or is invalid
		//! @throws CS240Exception if this URL is empty
		static std::string resolve(std::string base, std::string rel);
		
		//! @return true if the URL is relative, false otherwise
		static bool isRelative(const std::string*);
		
		//!@return true if the URL is relative, false otherwise
		bool isRelative()const;
		
		//!@return negative if other is less, positive if more, 0 otherwise
		int compare(const URL & other)const;
	
		//Other functions necessary for URL resolution:
		
		//! Moves input URL string to its base directory
		//! @throws InvalidURLException if the string is not a valid absolute URL
		//! @param url to modify
		static void moveBaseDir(std::string* urlString);
	
		//! Removes all characters after specification of current directory
		//! @throws InvalidURLException if the string is not a valid absolute URL
		//! @param url to modify
		static void moveCurrentDir(std::string* urlString);
	
		//! Move the url to the base of one directory up
		//! @throws InvalidURLException if the string is not a valid absolute URL
		//! @param url to modify
		static void moveUpDir(std::string* urlString);
		
		//! if baseURL does not end in a '/', this removes everything until it does end in a '/'
		//! @throws CS240Exception if the string is not a valid absolute URL
		//! @param url to modify
		static void removeUntilSlash(std::string* urlString);
		
		//! @return true if is a valid absolute url (starts with "http://x" or "file://",
		static bool isValidAbsURL(const std::string* urlString);
		
	private:
		
		//makes everything up to the third slash lowercase
		void normCaps();
		//!@returns true if one of several extensions are on str
		static bool hasHTMLextension(std::string str);
	
		//! resolves relative and base URL into a new absolute URL using recursion
		//! @param absolute url to to use as base URL during resolution
		//! @throws IllegalStateException if rel is not relative
		static std::string _resolve(std::string base, std::string rel);
		std::string url;
		//http:// and file:// are both 7 characters
		static const unsigned int protocolLength = 7;
	
};
#endif
