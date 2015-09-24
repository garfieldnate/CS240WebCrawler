#ifndef XML_H
#define XML_H

#include <ostream>
#include "Stack.h"

//! This class is responsible for handling XML processing and output. The constructor takes an
//! 	ostream object, and all of the methods write to that ostream. Entity escaping will be
//!		taken care of in the methods. Nothing handles attributes because we don't use them!

//so XML knows about stack, which includes this file
template <class T> class Stack;
class XML{
	public:
		//! @param stream to write XML data to
		//! @param stream to write errors to
		XML(std::ostream* out, std::ostream* err);
		
		//! The destructor will print an error if the opened tags stack is not empty
		~XML();
	
		//! @param name of opening tag to write
		//! @throws IllegalArgumentException if the string contains whitespace
		void openTag(const std::string string);
		
		//! closes the last opened tag
		//! @throws exception if there is no tag on the stack to close
		void closeTag();
		
		//! @param text to write. XML entities will be escaped, and tabs and a newline will be added.
		void writeText(const std::string string);
		
		// //! test the methods of the class. Failed tests print information to os.
		// //! @return true if successful test, false otherwise.
		// static bool Test(std::ostream & os);
	
	private:
		//!all information will be written to this stream
		std::ostream* outStream;
		
		//!errors in destructor will be written to here
		std::ostream * errStream;
		
		//! keeps track of opened tags
		Stack<std::string>* openedTags;
		
		//! keep track of indentation
		int tabNum;
		
};
#endif
