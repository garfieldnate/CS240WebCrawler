#include "URL.h"
#include "StringUtil.h"
#include "CS240Exception.h"
#include <iostream>

#include <string>
#include <assert.h>

using namespace std;

	URL::URL(){}

	URL::URL(string str) : url(str){
		//program will only understand 'file:///', not file://
		if(url == "file://")
			url.append("/");
		if(!isRelative() && !isValidAbsURL(&url))
			throw InvalidURLException("Invalid URL: " + url);
		normCaps();
	}

	URL::URL(const URL & other) : url(other.getURLstring()){}
	
	URL & URL::operator=(const URL & other){
			url = other.getURLstring();
			return *this;
	}
	
	bool URL::operator!=(const URL & other){
		return url.compare(other.getURLstring()) != 0;
	}
	
	string URL::getURLstring() const{
		return url;
	}
	
	string URL::getPrefix()const{
		if(!isValidAbsURL(&url))
			throw InvalidURLException("Can't get prefix from invalid absolute URL: " + url);

		size_t thirdSlash = url.find('/',protocolLength);
		//if there is no slash after the protocol type, just return the whole thing
		if(thirdSlash == string::npos)
			return url;
		//otherwise return just up to the last slash
		return url.substr(0,url.find_last_of('/')+1);
	}
	
	void URL::reduce(){
		size_t charLoc = url.find('#');
		if(charLoc != string::npos)
			url.erase(charLoc,url.length());
	}
	
	bool URL::inSameScope(const URL & other) const{
		if(StringUtil::IsPrefix(other.getURLstring(), getPrefix()))
			return true;
		return false;
	}
	
	void URL::WriteXML(XML* xml) const{
		xml->openTag("url");
			xml->writeText(url);
		xml->closeTag();
	}
	
	bool URL::isHTML(const string & str){
		if(!isValidAbsURL(&str) && !isRelative(&str))
			return false;
		if(StringUtil::IsPrefix(str,"mailto:"))
			return false;
		string checkString(str);
		//remove query and fragment
		size_t loc = str.find('#');
		if(loc != string::npos)
			checkString = str.substr(0,loc);
		loc = checkString.find('?');
		if(loc != string::npos)
			checkString = str.substr(0,loc);
		
		//if no file extension, then it's HTML
		size_t lastSlash = checkString.find_last_of('/');
		size_t dot = checkString.find('.',lastSlash);
		if(dot == string::npos)
			return true;
			
		//if http://www.blah.com
		if(StringUtil::IsPrefix(checkString,"http://")){
			size_t thirdSlash = checkString.find('/',protocolLength);
			if(thirdSlash == string::npos)
				return true;
		}
		
		//if file:///www.blah.com
		if(StringUtil::IsPrefix(checkString,"file://")){
			size_t fourthSlash = checkString.find('/',protocolLength+1);
			if(fourthSlash == string::npos)
				return true;
		}
		
		//test each of the extension types
		return hasHTMLextension(checkString);
	}
	
	bool URL::hasHTMLextension(const string str){
		string extensions[] = {"html", ".htm", ".shtml", ".cgi", ".jsp",
			".asp", ".aspx", ".php", ".pl", ".cfm","/"};
		int numExtensions = 10;
		for(int i = 0; i < numExtensions; i++)
			if(StringUtil::IsSuffix(str,extensions[i]))
				return true;
		return false;
	}
	
	void URL::resolve(URL base){
		url = resolve(base.getURLstring(), url);
	}
	
	string URL::resolve(const string base, const string rel){
		if(!isValidAbsURL(&base))
			throw InvalidURLException("Invalid base URL: " + base);
			
		if(!isRelative(&rel))
			throw InvalidURLException("Invalid relative URL: " + base);
			
		if(rel == "")
			throw CS240Exception("Empty relative URL: " + rel);
		
		//add missing slash to file urls
		if(StringUtil::IsPrefix(base,"file://") && base[protocolLength] != '/')
			return _resolve(base + "/", rel);
		
		return _resolve(base, rel);
	}
	
	string URL::_resolve(const string base, const string rel){
		string baseURL = base;
		string relURL = rel;
		
		switch (relURL[0]){
			//# -> concat
			case '#':
				return baseURL + relURL;
				//'/' -> concat after 3rd slash
			case '/':
				moveBaseDir(&baseURL);
				relURL.erase(0,1);//get rid of leading slash
				return resolve(baseURL,relURL);
				break;
			case '.':
	//			./ -> current dir
				if(relURL[1] == '/'){
					relURL.erase(0,2);//remove leading "./"
					moveCurrentDir(&baseURL);
					return resolve(baseURL,relURL);
				}
				//'../' -> move up one
				else
					if(StringUtil::IsPrefix(relURL,"../") != 0){
						moveUpDir(&baseURL);
						//get rid of leading "../"
						relURL.erase(0,3);
						return resolve(baseURL, relURL);
					}

			//relative URL starts with a character other than #, / and .
			default:
				moveCurrentDir(&baseURL);
				string retVal(baseURL);
				retVal += relURL;
				return retVal;
		}
		// assert(false);
		return "NOT IMPLEMENTED";
	}
	
	void URL::moveBaseDir(std::string* urlString){
		assert(urlString != NULL);
		if(!isValidAbsURL(urlString))
			throw InvalidURLException("Absolute URL is invalid: " + *urlString);
			
		//add missing slash to file urls
		if(StringUtil::IsPrefix(*urlString,"file://") && (*urlString)[protocolLength] != '/')
			*urlString += "/";	
		
		size_t thirdSlash = urlString->find('/',protocolLength);
		if(thirdSlash == string::npos){
			urlString->append("/");
			return;
		}
		urlString->erase(thirdSlash+1,urlString->length());
	}
	
	void URL::moveCurrentDir(std::string* urlString){
		assert(urlString != NULL);
		if(!isValidAbsURL(urlString))
			throw InvalidURLException("URL is invalid: " + *urlString);
		
		// search for another '/' after protocol
		if(urlString->find('/',protocolLength) != string::npos)
			//remove from end to last slash in baseURL
			removeUntilSlash(urlString);
		if(!StringUtil::IsSuffix(*urlString,"/"))
			urlString->append("/");
		//otherwise there's nothing to be done with the baseURL
	}
	
	void URL::moveUpDir(string* urlString){
		assert(urlString != NULL);
		if(!isValidAbsURL(urlString))
			throw InvalidURLException("URL is invalid: " + *urlString);
		
		//both 'http://' has 7 characters, and "file:///" has 8
		//search after the slash in the protocol type to make sure that there is another directory
		//to move up to.
		if(StringUtil::IsPrefix(*urlString,"file://")){
			size_t fourthSlash = urlString->find('/',protocolLength+1);
			if(fourthSlash == string::npos)
				throw InvalidURLException("Unable to move up a directory in " + *urlString);
		}
		else{
			size_t thirdSlash = urlString->find('/',protocolLength);
			if(thirdSlash == string::npos)
				throw InvalidURLException("Unable to move up a directory in " + *urlString);
		}
		removeUntilSlash(urlString);
		urlString->erase(urlString->length()-2);
		removeUntilSlash(urlString);
	}
	
	void URL::removeUntilSlash(string* urlString){
		if(!isValidAbsURL(urlString))
			throw InvalidURLException("URL is invalid: " + *urlString);
		assert(urlString != NULL);
		//if already ends in a slash, return
		if((*urlString)[urlString->length()-1] == '/')
			return;
		urlString->erase(urlString->find_last_of('/')+1,urlString->length());
	}
	
	bool URL::isRelative()const{
		return isRelative(&url);
	}	
	
	bool URL::isRelative(const string* str){
		if(!StringUtil::IsPrefix(*str,"http://") && !StringUtil::IsPrefix(*str,"file://"))
			return true;
		return false;
	}
	
	int URL::compare(const URL & other)const{
		return url.compare(other.getURLstring());
	}
	
	bool URL::isValidAbsURL(const string* urlString){
		if(isRelative(urlString))
			return false;
			
		if(urlString->length() <= protocolLength && StringUtil::IsPrefix(*urlString,"http://"))
			return false;
			
		return true;
	}
	
	void URL::normCaps(){
		size_t thirdSlash = url.find('/',protocolLength);
		//no point in lower casing "file:///"
		if(thirdSlash != protocolLength +1 && thirdSlash != string::npos){
			StringUtil::ToLower(url,0,thirdSlash);
		}
	}
	
