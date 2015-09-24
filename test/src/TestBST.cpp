#ifndef TEST_BST_CPP
#define TEST_BST_CPP
#include "BST.h"
#include "tests.h"
#include "UnitTest.h"
#include "CS240Exception.h"
#include "URL.h"
#include <string>
#include <sstream>

using namespace std;
class TestBST{
	public:
		TestBST():success(true){}
		
		bool testBST(ostream & os){
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
			
			BST<string> bst;
			TESTM(bst.IsEmpty(),"Expected bst to be empty");
			sizeStr << bst.GetSize();
			TESTM(bst.GetSize() == 0,"Expected: 0\nActual: " << sizeStr << "\n");
			
			bst.Insert("hiya!");
			TESTM(!bst.IsEmpty(),"Expected bst not to be empty");
			sizeStr << bst.GetSize();
			TESTM(bst.GetSize() == 1,"Expected: 1\nActual: " << sizeStr << "\n");
			
			bst.Insert("hiya!");
			sizeStr << bst.GetSize();
			TESTM(bst.GetSize() == 1,"Expected: 1\nActual: " << sizeStr << "\n");
			
			bst.Insert("biya!");
			sizeStr << bst.GetSize();
			TESTM(bst.GetSize() == 2,"Expected: 1\nActual: " << sizeStr << "\n");
			
			bst.Clear();
			TESTM(bst.IsEmpty(),"Expected bst to be empty");
			sizeStr << bst.GetSize();
			TESTM(bst.GetSize() == 0,"Expected: 0\nActual: " << sizeStr << "\n");
			
		}
		
		void testCopyConstructor(ostream & os){
			printSubheader("COPY",os);
			BST<string> bst;
			bst.Insert("kaybs");
			bst.Insert("abes");
			bst.Insert("zabes");
			bst.Insert("wabes");
			bst.Insert("zzbes");
			
			BST<string> bst2(bst);
			BSTNode<string>* node = bst2.GetRoot();
			TESTM(node->GetValue() == "kaybs","Expected: kaybs\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetLeft()->GetValue() == "abes","Expected: abes\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetRight()->GetValue() == "zabes","Expected: zabes\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetRight()->GetLeft()->GetValue() == "wabes","Expected: zabes\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetRight()->GetRight()->GetValue() == "zzbes","Expected: zabes\nActual: " + node->GetValue() + "\n");
		
			BST<string> bst3(bst);
			node = bst3.GetRoot();
			TESTM(node->GetValue() == "kaybs","Expected: kaybs\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetLeft()->GetValue() == "abes","Expected: abes\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetRight()->GetValue() == "zabes","Expected: zabes\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetRight()->GetLeft()->GetValue() == "wabes","Expected: zabes\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetRight()->GetRight()->GetValue() == "zzbes","Expected: zabes\nActual: " + node->GetValue() + "\n");
		
		}
		void testNodes(ostream & os){
			printSubheader("NODES",os);
			BST<string> bst;
			bst.Insert("kaybs");
			bst.Insert("abes");
			bst.Insert("zabes");
			bst.Insert("wabes");
			bst.Insert("zzbes");
			
			BSTNode<string>* node = bst.GetRoot();
			TESTM(node->GetValue() == "kaybs","Expected: kaybs\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetLeft()->GetValue() == "abes","Expected: abes\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetRight()->GetValue() == "zabes","Expected: zabes\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetRight()->GetLeft()->GetValue() == "wabes","Expected: wabes\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetRight()->GetRight()->GetValue() == "zzbes","Expected: zzbes\nActual: " + node->GetValue() + "\n");
		
			//test comparisons
			BSTNode<string>* node1 = bst.GetRoot();//kaybs
			BSTNode<string>* node2 = bst.GetRoot()->GetLeft();//abes
			TESTM(node1->compare(*node2) > 0, "tested: " + node1->GetValue() + ".compare(" +
				node2->GetValue() + ")\nExpected positive\n");
			TESTM(node2->compare(*node1) < 0, "tested: " + node2->GetValue() + ".compare(" +
				node1->GetValue() + ")\nExpected negative\n");
			TESTM(node1->compare(*node1) == 0, "tested: " + node1->GetValue() + ".compare(" +
				node1->GetValue() + ")\nExpected zero\n");
		
		}
		void testFind(ostream & os){
			printSubheader("FIND",os);
			BST<string> bst;
			bst.Insert("kaybs");
			bst.Insert("abes");
			bst.Insert("zabes");
			bst.Insert("wabes");
			bst.Insert("zzbes");
			BSTNode<string>* node = bst.Find("zabes");
			TESTM(node != NULL,"Expected to find \"zabes\"\n");
			TESTM(node->GetLeft()->GetValue() == "wabes","Expected: wabes\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetRight()->GetValue() == "zzbes","Expected: zzbes\nActual: " + node->GetValue() + "\n");
			
			BST<string> bst2;
			TESTM(bst2.GetRoot() == NULL,"Expected: NULL\nActual: " + bst2.GetRoot()->GetValue() + "\n");
		}
		
		void testInsert(ostream & os){
			//almost the same as the testFind function; just need to make sure that
			//if something is in the BST, inserting again will return the original,
			//not NULL, as the original CS240 assignment stipulated.
			printSubheader("INSERT",os);
			BST<string> bst;
			bst.Insert("kaybs");
			bst.Insert("abes");
			bst.Insert("zabes");
			bst.Insert("wabes");
			bst.Insert("zzbes");
			BSTNode<string>* node = bst.Insert("zabes");
			TESTM(node != NULL,"Expected to find \"zabes\"\n");
			TESTM(node->GetLeft()->GetValue() == "wabes","Expected: wabes\nActual: " + node->GetValue() + "\n");
			TESTM(node->GetRight()->GetValue() == "zzbes","Expected: zzbes\nActual: " + node->GetValue() + "\n");
			
			BST<string> bst2;
			TESTM(bst2.GetRoot() == NULL,"Expected: NULL\nActual: " + bst2.GetRoot()->GetValue() + "\n");
		}
		
		void testIterator(ostream & os){
			printSubheader("ITERATOR",os);
			BST<string> bst;
			bst.Insert("kaybs");
			bst.Insert("abes");
			bst.Insert("zabes");
			bst.Insert("wabes");
			bst.Insert("zzbes");
			
			BSTIterator<string> iter = bst.Iterator();
			
			
			BST<string> bst2;
			BSTIterator<string> iter2(&bst2);
			TESTM(!iter2.hasNext(),"Expected empty iterator\n");
			TESTE(iter2.next();,IllegalStateException,"Calling next() on empty iterator should throw error.");
		}
		void testXML(ostream & os){
			printSubheader("XML",os);
			URL url1("stuffcake.com");
			URL url2("cheesecake.com");
			URL url3("age.com");
			URL url4("deed.com");
			
			BST<URL> bst;
			bst.Insert(url1);
			bst.Insert(url2);
			bst.Insert(url3);
			bst.Insert(url4);
			
			stringstream xmlStream;
			stringstream errStream;
			XML xml(&xmlStream,&errStream);
			
			bst.WriteXML(&xml);
			
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

