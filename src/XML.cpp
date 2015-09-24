#include "XML.h"
#include "StringUtil.h"

#include <string>
#include <iostream>
#include <assert.h>

using namespace std;

	XML::XML(ostream* out, ostream* err = &cerr):outStream(out),errStream(err),tabNum(0){
		openedTags = new Stack<string>();
	}
	
	XML::~XML(){
		assert(openedTags != NULL);
		if(openedTags->size()!=0){
			*errStream << "Warning! Not all tags closed:";
			while(openedTags->size() != 0)
				*errStream << " [" <<openedTags->pop() << "]";
			*errStream << endl;
		}
		delete openedTags;
	}

	void XML::openTag(const string str){
		assert(openedTags != NULL);
		//there is certainly much more whitespace that could be checked for, but just these for now
		if(str.find(' ') != string::npos || str.find('\t') != string::npos)
			throw InvalidArgumentException("XML tag name should not contain whitespace");
		
		//convert to XML before saving or printing
		string escaped = StringUtil::EncodeToXmlCopy(str);
		for(int i = 0; i < tabNum; i++)
			*outStream << "\t";
			
		
		*outStream << "<" << escaped << ">" << endl;
		tabNum++;
		
		openedTags->push(escaped);
	}
	
	void XML::closeTag(){
		assert(openedTags != NULL);
		
		tabNum--;
		for(int i = 0; i < tabNum; i++)
			*outStream << "\t";
		*outStream << "</" << openedTags->pop() << ">" << endl;
	}
	
	void XML::writeText(const string str){
		//convert to xml and print
		string escaped = StringUtil::EncodeToXmlCopy(str);
		for(int i = 0; i < tabNum; i++)
			*outStream << "\t";
		*outStream << escaped << endl;
	}
	
	