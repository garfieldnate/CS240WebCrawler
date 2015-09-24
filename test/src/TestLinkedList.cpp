#ifndef TEST_LINKEDLIST_CPP
#define TEST_LINKEDLIST_CPP
#include "LinkedList.h"
#include "URL.h"
#include "tests.h"
#include "UnitTest.h"
#include "CS240Exception.h"
#include <sstream>

using namespace std;
class TestLinkedList{
	public:
		TestLinkedList():success(true){}
		
		bool testLinkedList(ostream & os){
			testSize(os);
			testConstructors(os);
			testNodes(os);
			testFind(os);
			testRemove(os);
			testAddAll(os);
			testXML(os);
			
			return success;
		}
	
	private:
		bool success;
		
		void testFind(ostream & os){
			printSubheader("FIND",os);
			
			LinkedList<string> ll;
			ll.Insert("Muh!",NULL);
			ll.Insert("Buh!",NULL);
			ll.Insert("Juh!",NULL);
			ll.Insert("Buh!",NULL);
			
			LLNode<string>* node;
			node = ll.Find("Buh!",NULL);
			TESTM(node->GetPrevious() == NULL,"Expected: NULL\nActual: " + node->GetPrevious()->GetValue());
			TESTM(node->GetNext()->GetValue() == "Juh!","Expected: Juh!\nActual: " + node->GetNext()->GetValue());
			TESTM(node->GetValue() == "Buh!","Expected: Buh!\nActual: " + node->GetValue());
			
			LLNode<string>* node2 = ll.Find("Buh!",ll.GetFirst());
			if(node2 == NULL)
			TESTM(node2->GetPrevious()->GetValue() == "Juh!","Expected: Juh!\nActual: " + node2->GetPrevious()->GetValue());
			TESTM(node2->GetNext()->GetValue() == "Muh!","Expected: Muh!\nActual: " + node2->GetNext()->GetValue());
			TESTM(node2->GetValue() == "Buh!","Expected: Buh!\nActual: " + node2->GetValue());
		}
		
		void testSize(ostream & os){
			printSubheader("SIZE",os);
			stringstream sizeStr;
			
			LinkedList<string> ll;
			
			TESTM(ll.IsEmpty(),"Expected ll to be empty");
			
			sizeStr << ll.GetSize();
			TESTM(ll.GetSize() == 0,"Expected: 0\nActual: " << sizeStr << "\n");
			
			ll.Insert("Buh!",NULL);
			sizeStr << ll.GetSize();
			TESTM(ll.GetSize()==1,"Expected: 0\nActual: " << sizeStr << "\n");
			TESTM(!ll.IsEmpty(),"Expected ll not to be empty");
			
			ll.Insert("Buh!",NULL);
			sizeStr << ll.GetSize();
			TESTM(ll.GetSize()==2,"Expected: 2\nActual: " << sizeStr << "\n");
			
			
			ll.Insert("Muh!",NULL);
			ll.Insert("Buh!",NULL);
			ll.Insert("Juh!",NULL);
			ll.Clear();
			sizeStr << ll.GetSize();
			TESTM(ll.GetSize() == 0,"Expected: 0\nActual: " << sizeStr << "\n");
		}
		
		void testConstructors(ostream & os){
			printSubheader("CONSTRUCTORS",os);
			stringstream sizeStr;
			LinkedList<string> ll;
			ll.Insert("Muh!",NULL);
			ll.Insert("Buh!",NULL);
			ll.Insert("Juh!",NULL);
			
			//test copy constructor
			LinkedList<string> ll2(ll);
			LLNode<string>* node = ll2.GetFirst();
			sizeStr << ll2.GetSize();
			TESTM(ll2.GetSize() == 3,"Expected: 3\nActual: sizeStr\n");
			TESTM(node->GetValue() == "Juh!","Expected: Juh!\nActual: " + node->GetValue());
			node = node->GetNext();
			TESTM(node->GetValue() == "Buh!","Expected: Buh!\nActual: " + node->GetValue());
			node = node->GetNext();
			TESTM(node->GetValue() == "Muh!","Expected: Muh!\nActual: " + node->GetValue());
			node = node->GetNext();
			TESTM(node == NULL,"Expected: NULL\nActual: " + node->GetValue());
			
			//test operator=
			LinkedList<string> ll3 = ll2;
			node = ll3.GetFirst();
			sizeStr << ll3.GetSize();
			TESTM(ll3.GetSize() == 3,"Expected: 3\nActual: sizeStr\n");
			TESTM(node->GetValue() == "Juh!","Expected: Juh!\nActual: " + node->GetValue());
			node = node->GetNext();
			TESTM(node->GetValue() == "Buh!","Expected: Buh!\nActual: " + node->GetValue());
			node = node->GetNext();
			TESTM(node->GetValue() == "Muh!","Expected: Muh!\nActual: " + node->GetValue());
			node = node->GetNext();
			TESTM(node == NULL,"Expected: NULL\nActual: " + node->GetValue());
		}
		
		void testNodes(ostream & os){
			printSubheader("NODES",os);
			LinkedList<string> ll;
		
			TESTM(ll.GetFirst()==NULL,"ll first should be null");
			TESTM(ll.GetLast()==NULL,"ll last should be null");
			
			ll.Insert("Buh!",NULL);
			TESTM(ll.GetFirst()->GetValue().compare("Buh!")==0,"Expected: Buh!\nActual: " + 
				ll.GetFirst()->GetValue());
			TESTM(ll.GetLast()->GetValue().compare("Buh!")==0,"Expected: Buh!\nActual: " + 
				ll.GetLast()->GetValue());
			
			ll.Insert("Muh!",NULL);
			TESTM(ll.GetLast()->GetValue().compare("Buh!")==0,"Expected: Buh!\nActual: " + 
				ll.GetLast()->GetValue());
			TESTM(ll.GetFirst()->GetValue().compare("Muh!")==0,"Expected: Muh!\nActual: " + 
				ll.GetFirst()->GetValue());
				
			LLNode<string>* buh = ll.GetLast();
			TESTM(buh->GetNext() == NULL,"Expected: NULL\nActual: " + buh->GetNext()->GetValue());
			TESTM(buh->GetPrevious()->GetValue() == "Muh!","Expected: Muh!\nActual: " + buh->GetPrevious()->GetValue());
			
			LLNode<string>* muh = ll.GetFirst();
			TESTM(muh->GetNext()->GetValue() == "Buh!","Expected: Buh!\nActual: " + muh->GetNext()->GetValue());
			TESTM(muh->GetPrevious() == NULL,"Expected: NULL\nActual: " + muh->GetPrevious()->GetValue());
			
			//node comparisons
			LLNode<string>* node1 = ll.GetFirst();//Muh!
			LLNode<string>* node2 = ll.GetLast();//Buh!
			TESTM(node1->compare(*node2) > 0, "tested: " + node1->GetValue() + ".compare(" +
				node2->GetValue() + ")\nExpected positive\n");
			TESTM(node2->compare(*node1) < 0, "tested: " + node2->GetValue() + ".compare(" +
				node1->GetValue() + ")\nExpected negative\n");
			TESTM(node1->compare(*node1) == 0, "tested: " + node1->GetValue() + ".compare(" +
				node1->GetValue() + ")\nExpected zero\n");
			
		}

		void testRemove(ostream & os){
			printSubheader("REMOVE",os);
			LinkedList<string> ll;
			ll.Insert("Muh!",NULL);
			ll.Insert("Buh!",NULL);
			ll.Insert("Juh!",NULL);
			
			LLNode<string>* node = ll.Find("Muh!",NULL);
			ll.Remove(node);
			node = ll.GetFirst();
			TESTM(node->GetValue() == "Juh!","Expected: Juh!\nActual: " + node->GetValue());
			TESTM(node->GetNext()->GetValue() == "Buh!","Expected: Buh!\nActual: " + node->GetNext()->GetValue());
		}
		
		void testAddAll(ostream & os){
			printSubheader("ADDALL",os);
			LinkedList<string> ll1;
			ll1.Insert("Muh!",NULL);
			ll1.Insert("Buh!",NULL);
			
			LinkedList<string> ll2;
			ll2.Insert("Juh!",NULL);
			ll2.Insert("Kuh!",NULL);
			
			ll1.addAll(&ll2);
			
			LLNode<string>* node = ll1.GetFirst();
			TESTM(node->GetValue() == "Juh!","Expected: Juh!\nActual: " + node->GetValue());
			node = node->GetNext();
			TESTM(node->GetValue() == "Kuh!","Expected: Kuh!\nActual: " + node->GetValue());
			node = node->GetNext();
			TESTM(node->GetValue() == "Buh!","Expected: Buh!\nActual: " + node->GetValue());
			node = node->GetNext();
			TESTM(node->GetValue() == "Muh!","Expected: Muh!\nActual: " + node->GetValue());
			node = node->GetNext();
			TESTM(node == NULL,"Expected: NULL\nActual: " + node->GetValue());
			
		}
		
		void testXML(ostream & os){
			printSubheader("XML",os);
			URL url1("http://stuffcake.com");
			URL url2("http://cheesecake.com");
			
			LinkedList<URL> ll;
			ll.Insert(url1,NULL);
			ll.Insert(url2,NULL);
			
			stringstream xmlStream;
			stringstream errStream;
			XML xml(&xmlStream,&errStream);
			
			ll.WriteXML(&xml);
			
			stringstream expected;
			expected << "<url>" << endl << "\thttp://cheesecake.com" << endl << "</url>" << endl <<
				"<url>" << endl << "\thttp://stuffcake.com" << endl << "</url>" << endl;
			
			TESTM(xmlStream.str() == expected.str(),"\nExpected:\n" + expected.str() + "\nActual:\n" + xmlStream.str());
			TESTM(errStream.str() == "","\nErrors in XML:\n" + errStream.str() + "\n");
		}
};

#endif

