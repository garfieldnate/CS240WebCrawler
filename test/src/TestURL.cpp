#ifndef  TEST_URL_CPP
#define TEST_URL_CPP
#include <iostream>

#include "URL.h"
#include "UnitTest.h"
#include "tests.h"
#include "CS240Exception.h"

#include <string>
#include <sstream>

using namespace std;

// void testAssignments(ostream & os);
// void testResolverDriver(ostream & os);
// void testMoveBaseDirDriver(ostream & os);
// void testMoveCurrentDirDriver(ostream & os);
// void testRemoveUntilSlashDriver(ostream & os);
// void testMoveUpDriver(ostream & os);
// void testPrefix(ostream & os);
// void testReduce(ostream & os);
// void testCompare(ostream & os);
// void testIsHTMLdriver(ostream & os);
// void testWriteXML(ostream & os);
// void testIsValidAbsURL(ostream & os);
// void testIsRelative(ostream & os);
// void testInSameScopeDriver(ostream & os);
class TestURL{
	public:
		TestURL():success(true){}
	
	
		bool testURL(ostream & os){
			success = true;
			// int expectedTestCount = 100;
			// int startTestCount = testCount;
			testAssignments(os);
			testIsRelative(os);
			testIsValidAbsURL(os);
			testResolverDriver(os);
			testMoveBaseDirDriver(os);
			testMoveCurrentDirDriver(os);
			testRemoveUntilSlashDriver(os);
			testReduce(os);
			testMoveUpDriver(os);
			testCompare(os);
			testPrefix(os);
			testIsHTMLdriver(os);
			testWriteXML(os);
			testInSameScopeDriver(os);
			// if(testCount - startTestCount != expectedTestCount){
				// success = false;
				// os << "Was supposed to do " << expectedTestCount << " tests, but only did " <<
					// testCount - startTestCount << endl;
			// }
			return success;
		}
		
		
	private:
		
		bool success;
		
		//! test assignments through constructors and = operator
		void testAssignments(ostream & os){
			printSubheader("ASSIGNMENT",os);
			const string testStr = "stuff.com";
			URL url1(testStr);
			TESTM(url1.getURLstring() == testStr,"Assigned: " + testStr + "\nActual: " + url1.getURLstring() + "\n");
			URL url2 = url1;
			TESTM(url2.getURLstring() == testStr,"Assigned: " + testStr + "\nActual: " + url2.getURLstring() + "\n");
			URL url3(url2);
			TESTM(url3.getURLstring() == testStr,"Assigned: " + testStr + "\nActual: " + url3.getURLstring() + "\n");
			
			TESTE(URL url4("http://");,InvalidURLException,"Expected error creating URL \"http://\"");
		
			URL url4("stoof.com");
			TESTM(url1 != url4,"Expected false");
			
			//test normalization
			URL url5("http://STUDENTS.CS.BYU.edu/~cs240ta/crawler_tests/crawler/valid.asp");
			TESTM(url5.getURLstring() == "http://students.cs.byu.edu/~cs240ta/crawler_tests/crawler/valid.asp",
				"expected: 'http://students.cs.byu.edu/~cs240ta/crawler_tests/crawler/valid.asp'\nActual: " +
				url5.getURLstring());
		}

		//! test URL compare method
		void testCompare(ostream & os){
			printSubheader("COMPARE",os);
			URL url1("hi");
			URL url2("hj");
			TESTM(url1.compare(url2) < 0, "tested: " + url1.getURLstring() + ".compare(" +
				url2.getURLstring() + ")\nExpected negative\n");
			TESTM(url2.compare(url1) > 0, "tested: " + url2.getURLstring() + ".compare(" +
				url1.getURLstring() + ")\nExpected positive\n");
			TESTM(url1.compare(url1) == 0, "tested: " + url1.getURLstring() + ".compare(" +
				url1.getURLstring() + ")\nExpected zero\n");
		}

		void testReduce(ostream & os){
			printSubheader("REDUCE",os);
			URL url("http://blah.com/stuff#morestuff");
			url.reduce();
			TESTM(url.getURLstring() == "http://blah.com/stuff", "expected: http://blah.com/stuff\nActual: " +
				url.getURLstring() + "\n");
		}

		void testInSameScope(ostream & os, const string & str1, const string & str2, bool truth){
			URL url1(str1);
			URL url2(str2);
			TESTM(url1.inSameScope(url2) == truth, "tested: " + url1.getURLstring() + " and " + url2.getURLstring() +
				"\nExpected: " + (truth ? "true" : "false") + "\n");
		}

		void testInSameScopeDriver(ostream & os){
			printSubheader("IN SAME SCOPE",os);
			
			// testInSameScope(os, "file:///stuff.com/buh.html", "file:///stuff.com/stuff.pl", true);
			// testInSameScope(os, "file:///stuff.com/buh.html", "file:///stuff.com", false);
			// testInSameScope(os, "file:///", "file:///stuff.com", true);
			// testInSameScope(os, "file:///", "file:///", true);
			// testInSameScope(os, "file:///", "file://", true);
			// testInSameScope(os, "file:///stuff.com/buh.html", "file:///stuff.com/buh.html", true);
			
			// testInSameScope(os, "http://stuff.com/buh.html", "http://stuff.com/stuff.pl", true);
			testInSameScope(os, "http://stuff.com/buh.html", "http://stuff.com", false);
			// testInSameScope(os, "http://a", "http://b", false);
			// testInSameScope(os, "http://stuff.com/", "http://stuff.com/buh.html", true);
			// testInSameScope(os, "http://stuff.com/buh.html", "http://stuff.com/buh.html", true);
			try{
				testInSameScope(os, "http://", "http://stuff.com", false);
				TESTM(false,"expected failure on http:// and http://stuff.com");
			}catch(InvalidURLException e){TEST(true);}
			
		}

		//! test parsing of URL prefix
		void testPrefix(ostream & os){
			printSubheader("PREFIX",os);
			string prefix("file:///");
			URL url1("file://");
			TESTM(url1.getPrefix() == prefix,"url: " + url1.getURLstring() + "\nExpected: " + prefix +
				"\nActual: " + url1.getPrefix() + "\n");
			URL url2("file:///");
			TESTM(url2.getPrefix() == prefix,"url: " + url2.getURLstring() + "\nExpected: " + prefix +
				"\nActual: " + url2.getPrefix() + "\n");
			
			URL url3("file:///hello.com/stuff");
			prefix = "file:///hello.com/";
			TESTM(url3.getPrefix() == prefix,"url: " + url3.getURLstring() + "\nExpected: " + prefix +
				"\nActual: " + url3.getPrefix() + "\n");
				
			URL url4("http://stuff.com");
			prefix = "http://stuff.com";
			TESTM(url4.getPrefix() == prefix,"url: " + url4.getURLstring() + "\nExpected: " + prefix +
				"\nActual: " + url4.getPrefix() + "\n");
		}

		void testIsHTML(ostream & os, const string & str, bool truth){
			TESTM(URL::isHTML(str) == truth, "tested: " + str + "\nExpected: " + (truth ? "true" : "false") + "\n");
		}

		//! test whether detection of HTML is correct
		void testIsHTMLdriver(ostream & os){
			printSubheader("IS HTML",os);
			testIsHTML(os,"/stuff",true);
			try{testIsHTML(os,"http://",false);}catch(InvalidURLException e){}
			
			testIsHTML(os,"http://www.stuff.com",true);
			testIsHTML(os,"http://www.stuff.com?thing=stuff",true);
			testIsHTML(os,"http://www.stuff.com?thing=stuff#morestuf",true);
			testIsHTML(os,"http://www.stuff.com#morestuff",true);
			
			testIsHTML(os,"http://www.stuff.com/morestuff.html",true);
			testIsHTML(os,"http://www.stuff.com/morestuff.html#stuff",true);
			testIsHTML(os,"http://www.stuff.com/morestuff.html?stuff=crappy",true);
			testIsHTML(os,"http://www.stuff.com/morestuff.html?stuff=crappy#stuff",true);
			
			testIsHTML(os,"http://www.stuff.com/morestuff.htm",true);
			testIsHTML(os,"http://www.stuff.com/morestuff.shtml",true);
			testIsHTML(os,"http://www.stuff.com/morestuff.cgi",true);
			testIsHTML(os,"http://www.stuff.com/morestuff.jsp",true);
			testIsHTML(os,"http://www.stuff.com/morestuff.asp",true);
			testIsHTML(os,"http://www.stuff.com/morestuff.aspx",true);
			testIsHTML(os,"http://www.stuff.com/morestuff.php",true);
			testIsHTML(os,"http://www.stuff.com/morestuff.pl",true);
			testIsHTML(os,"http://www.stuff.com/morestuff.cfm",true);
			testIsHTML(os,"http://www.stuff.com/morestuff",true);
			
			testIsHTML(os,"http://www.stuff.com/morestuff.buh",false);
			
			
			testIsHTML(os,"file:///www.stuff.com",true);
			testIsHTML(os,"file:///www.stuff.com?thing=stuff",true);
			testIsHTML(os,"file:///www.stuff.com?thing=stuff#morestuf",true);
			testIsHTML(os,"file:///www.stuff.com#morestuff",true);
			
			testIsHTML(os,"file:///www.stuff.com/morestuff.html",true);
			testIsHTML(os,"file:///www.stuff.com/morestuff.html#stuff",true);
			testIsHTML(os,"file:///www.stuff.com/morestuff.html?stuff=crappy",true);
			testIsHTML(os,"file:///www.stuff.com/morestuff.html?stuff=crappy#stuff",true);
			
			testIsHTML(os,"file:///www.stuff.com/morestuff.htm",true);
			testIsHTML(os,"file:///www.stuff.com/morestuff.shtml",true);
			testIsHTML(os,"file:///www.stuff.com/morestuff.cgi",true);
			testIsHTML(os,"file:///www.stuff.com/morestuff.jsp",true);
			testIsHTML(os,"file:///www.stuff.com/morestuff.asp",true);
			testIsHTML(os,"file:///www.stuff.com/morestuff.aspx",true);
			testIsHTML(os,"file:///www.stuff.com/morestuff.php",true);
			testIsHTML(os,"file:///www.stuff.com/morestuff.pl",true);
			testIsHTML(os,"file:///www.stuff.com/morestuff.cfm",true);
			testIsHTML(os,"file:///www.stuff.com/morestuff",true);
			
			testIsHTML(os,"file:///www.stuff.com/morestuff.buh",false);
		}

		//! test XML output
		void testWriteXML(ostream & os){
			printSubheader("XML",os);
			stringstream xmlStream;
			stringstream errStream;
			
			XML xml(&xmlStream,&errStream);
			URL url("http://testingrocks.com/buh.html");
			url.WriteXML(&xml);
			
			stringstream expected;
			expected << "<url>" << endl << "\thttp://testingrocks.com/buh.html" << endl << "</url>" << endl;
			
			TESTM(xmlStream.str() == expected.str(),"\nExpected:\n" + expected.str() + "\nActual:\n" + xmlStream.str());
			TESTM(errStream.str() == "","\nErrors in XML:\n" + errStream.str() + "\n");
		}

		void testIsValidAbsURL(ostream & os){
			printSubheader("VALIDATE ABS URL",os);
			string testString = "http://";
			TESTM(!URL::isValidAbsURL(&testString), "tested: " + testString + "\nExpected false\n");
			testString = "/stuff/buh.html";
			TESTM(!URL::isValidAbsURL(&testString), "tested: " + testString + "\nExpected false\n");
			testString = "file://";
			TESTM(URL::isValidAbsURL(&testString), "tested: " + testString + "\nExpected true\n");
			testString = "file:///";
			TESTM(URL::isValidAbsURL(&testString), "tested: " + testString + "\nExpected true\n");
			testString = "http://a";
			TESTM(URL::isValidAbsURL(&testString), "tested: " + testString + "\nExpected true\n");
			testString = "http://www.stuff.com";
			TESTM(URL::isValidAbsURL(&testString), "tested: " + testString + "\nExpected true\n");
		}

		//! test and non-methods of isRelative()
		void testIsRelative(ostream & os){
			printSubheader("IS RELATIVE",os);
			
			string testString = "file:///hiya!";
			URL url(testString);
			TESTM(!URL::isRelative(&testString), "tested: " + testString + "\nExpected false\n");
			TESTM(!url.isRelative(), "tested: " + testString + "\nExpected false\n");
			
			testString = "http://hiya!";
			URL url2(testString);
			TESTM(!URL::isRelative(&testString), "tested: " + testString + "\nExpected false\n");
			TESTM(!url2.isRelative(), "tested: " + testString + "\nExpected false\n");
			
			testString = "/hiya!";
			URL url3(testString);
			TESTM(URL::isRelative(&testString), "tested: " + testString + "\nExpected true\n");
			TESTM(url3.isRelative(), "tested: " + testString + "\nExpected true\n");
			
			testString = "non-html.jpg";
			URL url4(testString);
			TESTM(URL::isRelative(&testString), "tested: " + testString + "\nExpected true\n");
			TESTM(url4.isRelative(), "tested: " + testString + "\nExpected true\n");
		}

		//! test for correct URL resolution, and non-static
		//! @param os to print to
		//! @param relative URL
		//! @param base URL
		//! @param expected resolved URL string
		void testResolve(ostream & os, string rel, const string base, string res){
			URL url(rel);
			string baseCopy = base;
			url.resolve(baseCopy);
			TESTM(url.getURLstring() == res, "base: " + base + "\nrelative: " + rel + "\nexpected: " + 
				res + "\nactual: " + url.getURLstring() + "\n");
			
			string resolved = URL::resolve(base,rel);
			TESTM(resolved == res, "base: " + base + "\nrelative: " + rel + "\nexpected: " + 
				res + "\nactual: " + url.getURLstring() + "\n");
		}

		//! Runs many test cases for URL resolution.
		void testResolverDriver(ostream & os){
			printSubheader("RESOLVER",os);
			//following 28 tests made by CS240 TA's
			testResolve(os,"/images/nasdaq.jpg","http://www.cnn.com/news/financial/index.html",
				"http://www.cnn.com/images/nasdaq.jpg");
			testResolve(os,"/images/nasdaq.jpg","file:///news/financial/index.html",
				"file:///images/nasdaq.jpg");
			testResolve(os,"./images/nasdaq.jpg","http://www.cnn.com/news/financial/index.html",
				"http://www.cnn.com/news/financial/images/nasdaq.jpg");
			testResolve(os,"./images/nasdaq.jpg","file:///news/financial/index.html",
				"file:///news/financial/images/nasdaq.jpg");
			testResolve(os,"../images/nasdaq.jpg","http://www.cnn.com/news/financial/index.html",
				"http://www.cnn.com/news/images/nasdaq.jpg");
			testResolve(os,"../images/nasdaq.jpg","file:///news/financial/index.html",
				"file:///news/images/nasdaq.jpg");
			testResolve(os,"/images/nasdaq.jpg","http://www.cnn.com",
				"http://www.cnn.com/images/nasdaq.jpg");
			testResolve(os,"/images/nasdaq.jpg","file://",
				"file:///images/nasdaq.jpg");
			testResolve(os,"images/nasdaq.jpg","http://www.cnn.com",
				"http://www.cnn.com/images/nasdaq.jpg");
			testResolve(os,"images/nasdaq.jpg","file://",
				"file:///images/nasdaq.jpg");
			testResolve(os,"./images/nasdaq.jpg","http://www.cnn.com",
				"http://www.cnn.com/images/nasdaq.jpg");
			testResolve(os,"./images/nasdaq.jpg","file://",
				"file:///images/nasdaq.jpg");
			testResolve(os,"#HEADLINES","http://www.cnn.com/news/financial/index.html",
				"http://www.cnn.com/news/financial/index.html#HEADLINES");
			testResolve(os,"#HEADLINES","file:///news/financial/index.html",
				"file:///news/financial/index.html#HEADLINES");
			testResolve(os,"images/nasdaq.jpg","http://www.cnn.com/news/financial/index.html",
				"http://www.cnn.com/news/financial/images/nasdaq.jpg");
			testResolve(os,"images/nasdaq.jpg","file:///news/financial/index.html",
				"file:///news/financial/images/nasdaq.jpg");
			testResolve(os,"./../.././././.././././.././../images/nasdaq.jpg",
				"http://www.cnn.com/news/financial/folder/another/again/still/index.html",
				"http://www.cnn.com/news/images/nasdaq.jpg");
			testResolve(os,"./../.././././.././././.././../images/nasdaq.jpg",
				"file:///news/financial/folder/another/again/still/index.html",
				"file:///news/images/nasdaq.jpg");
			testResolve(os,"./go/here","http://www.test.com/queries?name=test",
				"http://www.test.com/go/here");
			testResolve(os,"./go/here","file:///queries?name=test",
				"file:///go/here");
			testResolve(os,"#stay","http://www.test.com/queries?name=test",
				"http://www.test.com/queries?name=test#stay");
			testResolve(os,"#stay","file:///queries?name=test",
				"file:///queries?name=test#stay");
			testResolve(os,"./queries?name=newquery","http://www.test.com/queries?name=test",
				"http://www.test.com/queries?name=newquery");
			testResolve(os,"./queries?name=newquery","file:///queries?name=test",
				"file:///queries?name=newquery");
			testResolve(os,"./images/nasdaq.jpg",
				"http://www.cnn.com/news;lang=english/financial;lang=english/index.html;lang=english",
				"http://www.cnn.com/news;lang=english/financial;lang=english/images/nasdaq.jpg");
			testResolve(os,"./images/nasdaq.jpg","file:///news;lang=english/financial;lang=english/index.html;lang=english",
				"file:///news;lang=english/financial;lang=english/images/nasdaq.jpg");
			testResolve(os,"./images;lang=english/nasdaq.jpg",
				"http://www.cnn.com/news;lang=english/financial;lang=english/index.html;lang=english",
				"http://www.cnn.com/news;lang=english/financial;lang=english/images;lang=english/nasdaq.jpg");
			testResolve(os,"./images;lang=english/nasdaq.jpg",
				"file:///news;lang=english/financial;lang=english/index.html;lang=english",
				"file:///news;lang=english/financial;lang=english/images;lang=english/nasdaq.jpg");
			
			//following tests by me!
			os << "Got here!" << endl;
			testResolve(os,"/hello.html","file:///news/h/buh.html",
				"file:///hello.html");
			TESTE(testResolve(os,"/news/h/buh.html","/hello.html",""),InvalidURLException,
				"Invalid absolute URL should have thrown error: /hello.html");
			TESTE(testResolve(os,"file:///news/h/buh.html","file:///hello.html",""),InvalidURLException,
				"Invalid relative URL should have thrown error: file:///news/h/buh.html");
			TESTE(testResolve(os,"","file:///hello.html",""),CS240Exception,
				"Empty relative URL should have thrown error");
		}

		void testMoveBaseDir(ostream & os, string url, string expectedBase){
			URL::moveBaseDir(&url);
			TESTM(url == expectedBase, "expected: " + expectedBase + "\n" + "actual: " + url + "\n");
		}

		void testMoveBaseDirDriver(ostream & os){
			printSubheader("MOVE BASE DIR",os);
			testMoveBaseDir(os,"http://stuff.com/home/morehome.html","http://stuff.com/");
			testMoveBaseDir(os,"file:///stuff.com/home/morehome.html","file:///");
			testMoveBaseDir(os,"http://stuff.com/home/a/morehome.html","http://stuff.com/");
			testMoveBaseDir(os,"http://stuff.com","http://stuff.com/");
			testMoveBaseDir(os,"file:///","file:///");
			testMoveBaseDir(os,"file:///images/nasdaq.jpg","file:///");
			testMoveBaseDir(os,"file://","file:///");
			
			TESTE(testMoveBaseDir(os,"http:/stuff.com",""),CS240Exception,
				"Invalid URL should have thrown error: http:/stuff.com");
		}

		void testMoveCurrentDir(ostream & os, string url, string expectedBase){
			URL::moveCurrentDir(&url);
			TESTM(url == expectedBase, "expected: " + expectedBase + "\n" + "actual: " + url + "\n");
		}

		void testMoveCurrentDirDriver(ostream & os){
			printSubheader("MOVE CURRENT DIR",os);
			testMoveCurrentDir(os,"http://stuff.com/home/morehome.html","http://stuff.com/home/");
			testMoveCurrentDir(os,"file:///","file:///");
			testMoveCurrentDir(os,"file:///hello.jpg","file:///");
			testMoveCurrentDir(os,"http://stuff.com/home/a/morehome.html","http://stuff.com/home/a/");
			testMoveCurrentDir(os,"http://stuff.com/home/a/","http://stuff.com/home/a/");
			testMoveCurrentDir(os,"http://stuff.com","http://stuff.com/");
			
			TESTE(testMoveCurrentDir(os,"http:/stuff.com",""),CS240Exception,
				"Invalid URL should have thrown error: http:/stuff.com");
		}

		void testMoveUp(ostream & os, string url, string expectedBase){
			URL::moveUpDir(&url);
			TESTM(url == expectedBase, "expected: " + expectedBase + "\n" + "actual: " + url + "\n");
		}

		void testMoveUpDriver(ostream & os){
			printSubheader("MOVE UP",os);
			testMoveUp(os,"http://stuff.com/home/morehome.html","http://stuff.com/");
			testMoveUp(os,"http://stuff.com/home/a/morehome.html","http://stuff.com/home/");
			testMoveUp(os,"http://stuff.com/home/a/","http://stuff.com/home/");
			
			TESTE(testMoveUp(os,"/hithere.html",""),InvalidURLException,
				"Should have thrown error trying to move up a directory from relative URL /hithere.html");
				
			TESTE(testMoveUp(os,"file:///",""),InvalidURLException,
				"Should have thrown error trying to move up a directory from file:///");
				TESTE(testMoveUp(os,"file://",""),InvalidURLException,
				"Should have thrown error trying to move up a directory from file://");
			TESTE(testMoveUp(os,"file:///hello.jpg",""),InvalidURLException,
				"Should have thrown error trying to move up a directory from file:///hello.jpg");
				
			TESTE(testMoveUp(os,"http://stuff.com",""),InvalidURLException,
				"Should have thrown error trying to move up a directory from http://stuff.com");
			TESTE(testMoveUp(os,"",""),InvalidURLException,
				"Should have thrown error with empty URL");
		}
		void testRemoveUntilSlash(ostream & os, string url, string expectedBase){
			URL::removeUntilSlash(&url);
			TESTM(url == expectedBase, "expected: " + expectedBase + "\n" + "actual: " + url + "\n");
		}

		void testRemoveUntilSlashDriver(ostream & os){
			printSubheader("REMOVE UNTIL SLASH",os);
			testRemoveUntilSlash(os,"http://stuff.com/home/morehome.html","http://stuff.com/home/");
			testRemoveUntilSlash(os,"file:///","file:///");
			testRemoveUntilSlash(os,"http://stuff.com/home/a/morehome.html","http://stuff.com/home/a/");
			TESTE(testRemoveUntilSlash(os,"",""),CS240Exception,
				"Should have thrown error trying to modify empty URL");
		}
};
#endif
