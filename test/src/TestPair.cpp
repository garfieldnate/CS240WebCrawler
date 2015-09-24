#ifndef TEST_PAIR_CPP
#define TEST_PAIR_CPP

#include "Pair.h"
#include "tests.h"
#include "UnitTest.h"

#include <sstream>
using namespace std;
class TestPair{

	public:
		TestPair():success(true){}
		
		bool testPair(ostream & os){
			testValues(os);
			testCopy(os);
			testCompare(os);
			return success;
		}
		
	private:
		bool success;
		void testValues(ostream& os){
			printSubheader("VALUES",os);
			Pair<string,string> pair("stuff","stoof");
			TESTM(pair.getFirst() == "stuff","expected: stuff\nActual: " + pair.getFirst() + "\n");
			TESTM(pair.getSecond() == "stoof","expected: stoof\nActual: " + pair.getSecond() + "\n");
			
			pair.setFirst("junk");
			pair.setSecond("gunk");
			TESTM(pair.getFirst() == "junk","expected: junk\nActual: " + pair.getFirst() + "\n");
			TESTM(pair.getSecond() == "gunk","expected: gunk\nActual: " + pair.getSecond() + "\n");
		}
		
		void testCopy(ostream& os){
			printSubheader("COPY",os);
			Pair<string,string> pair("stuff","stoof");
			TESTM(pair.getFirst() == "stuff","expected: stuff\nActual: " + pair.getFirst() + "\n");
			TESTM(pair.getSecond() == "stoof","expected: stoof\nActual: " + pair.getSecond() + "\n");
			
			pair.setFirst("junk");
			pair.setSecond("gunk");
			TESTM(pair.getFirst() == "junk","expected: junk\nActual: " + pair.getFirst() + "\n");
			TESTM(pair.getSecond() == "gunk","expected: gunk\nActual: " + pair.getSecond() + "\n");
		}
		
		void testCompare(ostream& os){
			printSubheader("COMPARE",os);
			Pair<string,string> pair1("hithere","what's that?");
			Pair<string,string> pair2("stuff","stoof");
			
			TESTM(pair1.compare(pair2) < 0,"expected positive\n");
			TESTM(pair2.compare(pair1) > 0,"expected negative\n");
			TESTM(pair1.compare(pair1) == 0,"expecte: zero\n");
			
		}

};
 #endif
 
 