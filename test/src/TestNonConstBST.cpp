#ifndef TEST_NonConstBST_CPP
#define TEST_NonConstBST_CPP
#include "NonConstBST.h"
#include "tests.h"
#include "UnitTest.h"
#include "CS240Exception.h"
#include "URL.h"
#include <string>
#include <sstream>

using namespace std;
class TestNonConstBST{
	public:
		TestNonConstBST():success(true){}
		
		bool testNonConstBST(ostream & os){
			testSize(os);
			testCopyConstructor(os);
			testNodes(os);
			testFind(os);
			testInsert(os);
			testIterator(os);
			testXML(os);
			
			return success;
		}
	
	private:
		bool success;

		void testSize(ostream & os){
			printSubheader("SIZE",os);
			stringstream sizeStr;
			
			NonConstBST<string> NonConstBST;
			TESTM(NonConstBST.IsEmpty(),"Expected NonConstBST to be empty");
			sizeStr << NonConstBST.GetSize();
			TESTM(NonConstBST.GetSize() == 0,"Expected: 0\nActual: " << sizeStr << "\n");
			
			NonConstBST.Insert("hiya!");
			TESTM(!NonConstBST.IsEmpty(),"Expected NonConstBST not to be empty");
			sizeStr << NonConstBST.GetSize();
			TESTM(NonConstBST.GetSize() == 1,"Expected: 1\nActual: " << sizeStr << "\n");
			
			NonConstBST.Insert("hiya!");
			sizeStr << NonConstBST.GetSize();
			TESTM(NonConstBST.GetSize() == 1,"Expected: 1\nActual: " << sizeStr << "\n");
			
			NonConstBST.Insert("biya!");
			sizeStr << NonConstBST.GetSize();
			TESTM(NonConstBST.GetSize() == 2,"Expected: 1\nActual: " << sizeStr << "\n");
			
			NonConstBST.Clear();
			TESTM(NonConstBST.IsEmpty(),"Expected NonConstBST to be empty");
			sizeStr << NonConstBST.GetSize();
			TESTM(NonConstBST.GetSize() == 0,"Expected: 0\nActual: " << sizeStr << "\n");
			
		}
		
		void testCopyConstructor(ostream & os){
			printSubheader("COPY",os);
			NonConstBST<string> NonConstBST;
			NonConstBST.Insert("kaybs");
			NonConstBST.Insert("abes");
			NonConstBST.Insert("zabes");
			NonConstBST.Insert("wabes");
			NonConstBST.Insert("zzbes");
			
			NonConstBST<string> NonConstBST2(NonConstBST);
			NonConstBSTNode<string>* node = NonConstBST2.GetRoot();
			TESTM(node->GetValue() == "kaybs","Expected: kaybs\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetLeft()->GetValue() == "abes","Expected: abes\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetRight()->GetValue() == "zabes","Expected: zabes\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetRight()->GetLeft()->GetValue() == "wabes","Expected: zabes\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetRight()->GetRight()->GetValue() == "zzbes","Expected: zabes\nActual: " + node->GetValue() + "\n");
		
			NonConstBST<string> NonConstBST3(NonConstBST);
			node = NonConstBST3.GetRoot();
			TESTM(node->GetValue() == "kaybs","Expected: kaybs\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetLeft()->GetValue() == "abes","Expected: abes\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetRight()->GetValue() == "zabes","Expected: zabes\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetRight()->GetLeft()->GetValue() == "wabes","Expected: zabes\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetRight()->GetRight()->GetValue() == "zzbes","Expected: zabes\nActual: " + node->GetValue() + "\n");
		
		}
		void testNodes(ostream & os){
			printSubheader("NODES",os);
			NonConstBST<string> NonConstBST;
			NonConstBST.Insert("kaybs");
			NonConstBST.Insert("abes");
			NonConstBST.Insert("zabes");
			NonConstBST.Insert("wabes");
			NonConstBST.Insert("zzbes");
			
			NonConstBSTNode<string>* node = NonConstBST.GetRoot();
			TESTM(node->GetValue() == "kaybs","Expected: kaybs\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetLeft()->GetValue() == "abes","Expected: abes\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetRight()->GetValue() == "zabes","Expected: zabes\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetRight()->GetLeft()->GetValue() == "wabes","Expected: wabes\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetRight()->GetRight()->GetValue() == "zzbes","Expected: zzbes\nActual: " + node->GetValue() + "\n");
		
			//test comparisons
			NonConstBSTNode<string>* node1 = NonConstBST.GetRoot();//kaybs
			NonConstBSTNode<string>* node2 = NonConstBST.GetRoot()->GetLeft();//abes
			TESTM(node1->compare(*node2) > 0, "tested: " + node1->GetValue() + ".compare(" +
				node2->GetValue() + ")\nExpected positive\n");
			TESTM(node2->compare(*node1) < 0, "tested: " + node2->GetValue() + ".compare(" +
				node1->GetValue() + ")\nExpected negative\n");
			TESTM(node1->compare(*node1) == 0, "tested: " + node1->GetValue() + ".compare(" +
				node1->GetValue() + ")\nExpected zero\n");
		
		}
		void testFind(ostream & os){
			printSubheader("FIND",os);
			NonConstBST<string> NonConstBST;
			NonConstBST.Insert("kaybs");
			NonConstBST.Insert("abes");
			NonConstBST.Insert("zabes");
			NonConstBST.Insert("wabes");
			NonConstBST.Insert("zzbes");
			NonConstBSTNode<string>* node = NonConstBST.Find("zabes");
			TESTM(node != NULL,"Expected to find \"zabes\"\n");
			TESTM(node->GetLeft()->GetValue() == "wabes","Expected: wabes\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetRight()->GetValue() == "zzbes","Expected: zzbes\nActual: " + node->GetValue() + "\n");
			
			NonConstBST<string> NonConstBST2;
			TESTM(NonConstBST2.GetRoot() == NULL,"Expected: NULL\nActual: " + NonConstBST2.GetRoot()->GetValue() + "\n");
		}
		
		void testInsert(ostream & os){
			//almost the same as the testFind function; just need to make sure that
			//if something is in the NonConstBST, inserting again will return the original,
			//not NULL, as the original CS240 assignment stipulated.
			printSubheader("INSERT",os);
			NonConstBST<string> NonConstBST;
			NonConstBST.Insert("kaybs");
			NonConstBST.Insert("abes");
			NonConstBST.Insert("zabes");
			NonConstBST.Insert("wabes");
			NonConstBST.Insert("zzbes");
			NonConstBSTNode<string>* node = NonConstBST.Insert("zabes");
			TESTM(node != NULL,"Expected to find \"zabes\"\n");
			TESTM(node->GetLeft()->GetValue() == "wabes","Expected: wabes\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetRight()->GetValue() == "zzbes","Expected: zzbes\nActual: " + node->GetValue() + "\n");
			
			NonConstBST<string> NonConstBST2;
			TESTM(NonConstBST2.GetRoot() == NULL,"Expected: NULL\nActual: " + NonConstBST2.GetRoot()->GetValue() + "\n");
		}
		
		void testIterator(ostream & os){
			printSubheader("ITERATOR",os);
			NonConstBST<string> NonConstBST;
			NonConstBST.Insert("kaybs");
			NonConstBST.Insert("abes");
			NonConstBST.Insert("zabes");
			NonConstBST.Insert("wabes");
			NonConstBST.Insert("zzbes");
			
			NonConstBSTIterator<string> iter = NonConstBST.Iterator();
			
			
			NonConstBST<string> NonConstBST2;
			NonConstBSTIterator<string> iter2(&NonConstBST2);
			TESTM(!iter2.hasNext(),"Expected empty iterator\n");
			TESTE(iter2.next();,IllegalStateException,"Calling next() on empty iterator should throw error.");
		}
		void testXML(ostream & os){
			printSubheader("XML",os);
			URL url1("stuffcake.com");
			URL url2("cheesecake.com");
			URL url3("age.com");
			URL url4("deed.com");
			
			NonConstBST<URL> NonConstBST;
			NonConstBST.Insert(url1);
			NonConstBST.Insert(url2);
			NonConstBST.Insert(url3);
			NonConstBST.Insert(url4);
			
			stringstream xmlStream;
			stringstream errStream;
			XML xml(&xmlStream,&errStream);
			
			NonConstBST.WriteXML(&xml);
			
			stringstream expected;
			expected << "<url>" << endl << "\tage.com" << endl << "</url>" << endl <<
				"<url>" << endl << "\tcheesecake.com" << endl << "</url>" << endl <<
				"<url>" << endl << "\tdeed.com" << endl << "</url>" << endl <<
				"<url>" << endl << "\tstuffcake.com" << endl << "</url>" << endl;
			
			TESTM(xmlStream.str() == expected.str(),"\nExpected:\n" + expected.str() + "\nActual:\n" + xmlStream.str());
			TESTM(errStream.str() == "","\nErrors in XML:\n" + errStream.str() + "\n");
		
		}

};

#endif

