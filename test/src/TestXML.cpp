#ifndef TEST_XML_CPP
#define TEST_XML_CPP
#include "XML.h"
#include "tests.h"
#include "UnitTest.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
class TestXML{
	
	public:
		TestXML():success(true){}
		
		bool testXML(ostream & os){
			testOpenTag(os);
			testDestructor(os);
			testOpenAndCloseTag(os);
			testWriteText(os);
			return success;
		}
	
	private:
		bool success;
		
		//!test that tags are successfully opened
		void testOpenTag(ostream & os){
			printSubheader("OPEN TAG",os);
			stringstream out;
			stringstream err;
			
			XML xml(&out,&out);
			xml.openTag("hiya!");
			xml.openTag("<looks_good!>");
			
			string str = out.str();
			string expected = "<hiya!>\n\t<&lt;looks_good!&gt;>\n";
			TESTM(str == expected,"Expected:\n" + expected + "\nActual:\n" + str);
			
			xml.closeTag();
			xml.closeTag();
			
			TESTE(xml.openTag("what the!?"),InvalidArgumentException,"XML tag name \"what the!?\" should be illegal.");
			TESTE(xml.openTag("what\tthe!?"),InvalidArgumentException,
				"XML tag name \"what\tthe!?\" (\"what\tthe!?\") should be illegal.");
		}
		
		//read error stream printed to by destructor; should have error message because tags aren't closed
		void testDestructor(ostream & os){
			printSubheader("DESTRUCTOR",os);
			stringstream out;
			stringstream err;
			
			stringstream expected;
			expected << "Warning! Not all tags closed: [&lt;looks_good!&gt;] [hiya!]" << endl;
			
			testDestructor2(out,err);
			
			TESTM(err.str() == expected.str(),"Expected:\n" + expected.str() + "\nActual:\n" + err.str());
			
			err.str("");
			testDestructor3(out,err);
			TESTM(err.str() == "","Expected:\n[nothing]" + expected.str() + "\nActual:\n" + err.str());
		}
		
		//create and destroy an XML object, then read err in testDestructor
		void testDestructor2(ostream& out, ostream& err){
			XML xml(&out,&err);
			xml.openTag("hiya!");
			xml.openTag("<looks_good!>");
		}
		
		//create and destroy an XML object, then read err in testDestructor
		void testDestructor3(ostream& out, ostream& err){
			XML xml(&out,&err);
			xml.openTag("hiya!");
			xml.openTag("<looks_good!>");
			xml.closeTag();
			xml.closeTag();
		}
		
		//!test that tags are successfully opened and closed
		void testOpenAndCloseTag(ostream & os){
			printSubheader("CLOSE TAG",os);
			stringstream out;
			
			stringstream expected;
			expected << "<hiya!>" << endl << "\t<buh>" << endl << "\t</buh>" << endl << "</hiya!>" << endl;
		
			XML xml(&out,&cerr);
			xml.openTag("hiya!");
			xml.openTag("buh");
			xml.closeTag();
			xml.closeTag();
			
			TESTM(out.str() == expected.str(),"Expected:\n" + expected.str() + "\nActual:\n" + out.str());
		}
		void testWriteText(ostream & os){
			printSubheader("WRITE TEXT",os);
			stringstream out;
			
			stringstream expected;
			expected << "hi there!" << endl;
			
			XML xml(&out,&cerr);
			xml.writeText("hi there!");
			TESTM(out.str() == expected.str(),"Expected:\n" + expected.str() + "\nActual:\n" + out.str());
		}

};

#endif
