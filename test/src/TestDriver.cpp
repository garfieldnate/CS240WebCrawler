#include <iostream>
#include "tests.h"
#include "TestURL.cpp"
#include "TestStack.cpp"
#include "TestQueue.cpp"
#include "TestXML.cpp"
#include "TestPage.cpp"
#include "TestPair.cpp"
#include "TestOccurrence.cpp"
#include "TestWord.cpp"
#include "TestStopWords.cpp"
#include "TestLinkedList.cpp"
#include "TestBST.cpp"
#include "TestHTMLparser.cpp"
#include "TestCrawler.cpp"

#include "CS240Exception.h"
#include "UnitTest.h"
#include <string>

using namespace std;

int main(int argc, char* argv[]){
	if(argc == 0){
		cerr << "USAGE: test.exe <directory name>" << endl;
		return -1;
	}
 
	cerr << "Hello! input was " << argv[1] << endl;

	#ifdef WIN32
		string fileProtocol = "file:///";
		string dataDir ("H:/CS 240 Corey Barker/Projects/crawler/");
	#else
		string fileProtocol = "file://";
		//string dataDir("/media/USB DISK/CS 240 Corey Barker/Projects/crawler/");
		string dataDir("/users/guest/n/nglenn/Desktop/crawler/");
	#endif
	
	cout << "datadir is " << dataDir << endl;
	//dataDir.erase(dataDir.length()-12, dataDir.length());
	dataDir.append("test/data/");
	cout << "data stored in " << dataDir << endl;
	
	
	printHeader("URL",cout);
	TestURL testurl;
	bool success = testurl.testURL(cout);
	printEnder("URL",success,cout);
	
	printHeader("STOPWORDS",cout);
	try{
		TestStopWords testStopWords(dataDir);
		success = testStopWords.testStopWords(cout);
	}catch(IOException e){
		cout << e.GetMessage();
	}
	printEnder("STOPWORDS",success,cout);
	
	printHeader("LinkedList",cout);
	TestLinkedList testLinkedList;
	success = testLinkedList.testLinkedList(cout);
	printEnder("LINKEDLIST",success,cout);
	
	printHeader("BST",cout);
	TestBST testBST;
	success = testBST.testBST(cout);
	printEnder("BST",success,cout);
	
	printHeader("Stack",cout);
	TestStack testStack;
	success = testStack.testStack(cout);
	printEnder("STACK",success,cout);
	
	printHeader("Queue",cout);
	TestStack testQueue;
	success = testQueue.testStack(cout);
	printEnder("QUEUE",success,cout);
	
	printHeader("XML",cout);
	TestXML testXML;
	success = testXML.testXML(cout);
	printEnder("XML",success,cout);
	
	printHeader("PAGE",cout);
	TestPage testPage;
	success = testPage.testPage(cout);
	printEnder("PAGE",success,cout);
	
	printHeader("OCCURRENCE",cout);
	TestOccurrence testOccurrence;
	success = testOccurrence.testOccurrence(cout);
	printEnder("OCCURRENCE",success,cout);
	
	printHeader("WORD",cout);
	TestWord testWord;
	success = testWord.testWord(cout);
	printEnder("WORD",success,cout);
	
	printHeader("PAIR",cout);
	TestPair testPair;
	success = testPair.testPair(cout);
	printEnder("PAIR",success,cout);
	
	try{
	      printHeader("HTML PARSER",cout);
	      TestHTMLparser testHTMLparser(fileProtocol,dataDir);
	      success = testHTMLparser.testHTMLparser(cout);
	      printEnder("HTML PARSER",success,cout);
	}catch(InvalidURLException e){
		cout << e.GetMessage() << endl;
	}catch(FileException e){
		cout << e.GetMessage() << endl;
	}
	
	try{
		printHeader("CRAWLER",cout);
		TestCrawler testCrawler(fileProtocol, dataDir);
		success = testCrawler.testCrawler(cout);
		printEnder("CRAWLER",success,cout);
	}catch(InvalidURLException e){
		cout << e.GetMessage() << endl;
	}catch(FileException e){
		cout << e.GetMessage() << endl;
	}
	
	cout << "Should have run 450 tests." << endl;
	cout << "Total TESTS RUN:    " << testCounter << endl;
	cout << "Total TESTS FAILED: " << failedCount << endl;
}

void printHeader(std::string str, std::ostream & os){
	os << "******************************" << std::endl;
	os << "|||||TESTING " + str + "|||||" << std::endl;
	os << "------------------------------" << std::endl;
}

void printSubheader(std::string str, std::ostream & os){
	os << "-----" << str << "-----" << std::endl;
}

void printEnder(std::string testName, bool success, std::ostream & os){
	os << testName + ": " << (success == true ? "PASS" : "FAIL") << endl <<
		"______________________________" << endl;
}

