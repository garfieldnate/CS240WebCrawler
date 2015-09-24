#ifndef TEST_HTML_PARSER_CPP
#define TEST_HTML_PARSER_CPP
#include "HTMLparser.h"
#include "Page.h"
#include "tests.h"
#include "StopWords.h"
#include "CS240Exception.h"
#include "UnitTest.h"
#include <iostream>
#include <sstream>
// #include <stdexception>
using namespace std;
class TestHTMLparser{

	public:
		TestHTMLparser(const string fp, const string dir):success(true),fileProtocol(fp),dataDir(dir){}
		
		bool testHTMLparser(ostream & os){
			testDownloadSuccess(os);
			testDescription(os);
			testIsWordChar(os);
			testIndexSingleWords(os);
			testIndexText(os);
			testTags(os);
			testURLSdriver(os);
			testStopWords(os);
			return success;
		}
		
	private:
		bool success;
		string dataDir;
		string fileProtocol;
		void testDownloadSuccess(ostream & os){
			printSubheader("LOADING",os);
			
			StopWords sw(dataDir + "stopwordsEvenOdd.txt");
			
			//test file location
			string urlString = fileProtocol + dataDir + "parserEmpty.html";
			URL url(urlString);
			HTMLparser parser(&url,&sw);
			TESTM(parser.success(),"Failed to parse " + urlString + "\n");
			
			//test http
			string urlString2 = "http://students.cs.byu.edu/~cs240ta/fall2011/";
			URL url2(urlString2);
			HTMLparser parser2(&url2,&sw);
			TESTM(parser2.success(),"Failed to parse " + urlString2 + "\n");
			
			//test file reading
			string urlString3 = fileProtocol + dataDir + "parserTitle.html";
			URL url3(urlString3);
			HTMLparser parser3(&url3,&sw);
			TESTM(parser3.success(),"Failed to parse " + urlString3 + "\n");
			
			
			os << "--Ignore the following errors--" << endl;
			URL url4(fileProtocol + dataDir + "parserNothing.html");
			HTMLparser parser4(&url4,&sw);
			TESTM(!parser4.success(),"Expected failure to load non-existent file: " + url4.getURLstring() + "\n");
		}
		
		void testDescription(ostream & os){
			printSubheader("PAGE",os);
			
			StopWords sw(dataDir + "stopwordsEvenOdd.txt");
			
			//test that <title> is used for description
			string urlString = fileProtocol + dataDir + "parserTitle.html";
			URL url(urlString);
			HTMLparser parser(&url,&sw);
			
			TESTM(parser.getPage().getDescription() == "this is the title of the page!",
				"Expected: \"this is the title of the page!\"\nActual: \"" + 
				parser.getPage().getDescription() + "\"\n");
				
			TESTM(parser.getPage().getURL() == urlString,
				"Expected: \"" + urlString + "\nActual: \"" + 
				parser.getPage().getURL() + "\"\n");
				
			//test that <h[123456]> is used for title
			urlString = fileProtocol + dataDir + "parserH1.html";
			URL url2(urlString);
			HTMLparser parser2(&url2,&sw);
			
			TESTM(parser2.getPage().getDescription() == "this is the h1 title of the page!",
				"Expected: \"this is the title of the page!\"\nActual: \"" + 
				parser2.getPage().getDescription() + "\"\n");
			
			TESTM(parser2.getPage().getURL() == urlString,
				"Expected: \"" + urlString + "\nActual: \"" + 
				parser2.getPage().getURL() + "\"\n");
				
			urlString = fileProtocol + dataDir + "parserH3.html";
			URL url3(urlString);
			HTMLparser parser3(&url3,&sw);
			
			TESTM(parser3.getPage().getDescription() == "this is the h3 title of the page!",
				"Expected: \"this is the h3 title of the page!\"\nActual: \"" + 
				parser3.getPage().getDescription() + "\"\n");
			
			TESTM(parser3.getPage().getURL() == urlString,
				"Expected: \"" + urlString + "\nActual: \"" + 
				parser3.getPage().getURL() + "\"\n");
				
			urlString = fileProtocol + dataDir + "parserFirst100body.html";
			URL url4(urlString);
			HTMLparser parser4(&url4,&sw);
			string expected = "\r\n\t	\r\n\taaaaa bbbbb ccccc ddddd eeeee fffff ggggg hhhhh iiiii jjjjj\r\n\tkkkkk lllll mmmmm nnnnn ooooo ppppp qqqqq rrrrr sssss ttttt";
			
			TESTM(parser4.getPage().getDescription() == expected,
				"Expected: \"" + expected + "\"\nActual: \"" + 
				parser4.getPage().getDescription() + "\"\n");
				
			urlString = fileProtocol + dataDir + "http://students.cs.byu.edu/~cs240ta/fall2011/";
			URL url5(urlString);
			HTMLparser parser5(&url5,&sw);
			expected = "http://students.cs.byu.edu/~cs240ta/fall2011/index.html";
			
			TESTM(parser5.getPage().getURL() == urlString,
				"Expected: \"" + expected + "\nActual: \"" + 
				parser5.getPage().getURL() + "\"\n");
				
				
		}
		
		void testIsWordChar(ostream & os){
			printSubheader("IS WORD CHAR",os);
			// alphabetic characters
			// numeric characters
			// underscore (_)
			// dash (-)
			TESTM(HTMLparser::isWordChar('a'),"Expected 'a' to be a word character.\n");
			TESTM(HTMLparser::isWordChar('A'),"Expected 'A' to be a word character.\n");
			TESTM(HTMLparser::isWordChar('1'),"Expected '1' to be a word character.\n");
			TESTM(HTMLparser::isWordChar('_'),"Expected '_' to be a word character.\n");
			TESTM(HTMLparser::isWordChar('-'),"Expected '-' to be a word character.\n");
			TESTM(!HTMLparser::isWordChar('.'),"Expected '.' NOT to be a word character.\n");
		}
		
		void testIndexSingleWords(ostream & os){
			printSubheader("INDEX SINGLE WORDS",os);
			string urlString = fileProtocol + dataDir + "parserEmpty.html";
			URL url(urlString);
			StopWords sw(dataDir + "stopwordsEvenOdd.txt");
			HTMLparser parser(&url,&sw);
			BST< Pair<std::string,int> >* words = parser.getWords();
			
			
			stringstream sizeStr;
			parser.indexWord("index_test");
			parser.indexWord("InDeX_tEsT");
			sizeStr << words->GetSize();
			TESTM(words->GetSize() == 1,"Expected: 1\nActual: " + sizeStr.str());
			testWords(os,words,"index_test",true);
			
			BSTNode<Pair<string,int> >* wordPair = words->Find(Pair<string,int>("index_test",0));
			if(wordPair == NULL){
				TESTM(false,"(Count as 2 fails) Could not find 'index_test'");
			}else{
				TESTM(wordPair->GetValue().getFirst() == "index_test","Expected: \"index_test\"\nActual: " + wordPair->GetValue().getFirst() + "\n");
				sizeStr << wordPair->GetValue().getSecond();
				TESTM(wordPair->GetValue().getSecond() == 2,"Expected: 2\nActual: " + sizeStr.str() + "\n");
			}
			//random word
			testWords(os,words,"hello",false);
			//check isalpha check
			parser.indexWord("1stuff");
			testWords(os,words,"1stuff",false);
			testWords(os,words,"stuff",false);
			parser.indexWord("-thith");
			testWords(os,words,"-thith",false);
			testWords(os,words,"thith",false);
			parser.indexWord("-value");
			testWords(os,words,"-value",false);
			testWords(os,words,"value",false);
			//word that is in the stopwords file
			parser.indexWord("their");
			testWords(os,words,"their",false);
		}
		
		void testIndexText(ostream & os){
			printSubheader("PARSE",os);
			string urlString = fileProtocol + dataDir + "parserEmpty.html";
			URL url(urlString);
			StopWords sw(dataDir + "stopwordsEvenOdd.txt");
			HTMLparser parser(&url,&sw);
			BST< Pair<std::string,int> >* words = parser.getWords();
			
			parser.index("Hi! _ny 1mame my name -iz var-name -name -value is Bay-heen_Gary_G23&enn. buh");
			testWords(os,words,"_ny",false);
			testWords(os,words," ",false);
			testWords(os,words,"-iz",false);
			testWords(os,words,"-name",false);
			testWords(os,words,"-value",false);
			testWords(os,words,"&",false);
			testWords(os,words,"bay-heen_gary_g23",true);
			testWords(os,words,"enn",true);
			testWords(os,words,"buh",true);
			testWords(os,words,"var-name",true);
			testWords(os,words,"var",false);
						
			parser.index("I live on 3rd 5th south");
			testWords(os,words,"rd",false);
			testWords(os,words,"th",false);
			testWords(os,words,"3rd",false);
		}
		
		//!print to os, look in words for word, expected to be there if truth == true
		void testWords(ostream& os, BST< Pair<std::string,int> >* words, const string & word, bool truth){
			TESTM((words->Find(Pair<string,int>(word,-1)) != NULL) == truth, word + (true ? " found\n" : " not found\n"));
		}
		
		void testTags(ostream& os){
			printSubheader("TAGS",os);
			
			StopWords sw(dataDir + "stopwordsEvenOdd.txt");

			string urlString = fileProtocol + dataDir + "parserEnvironments.html";
			URL url(urlString);
			HTMLparser parser(&url,&sw);
			BST< Pair<std::string,int> >* words = parser.getWords();
			
			testWords(os,words,"before_html",false);
			testWords(os,words,"before_title",false);
			testWords(os,words,"in_title_before_script",true);
			testWords(os,words,"in_title_script",false);
			testWords(os,words,"in_title_after_script",true);
			testWords(os,words,"in_body_before_script",true);
			testWords(os,words,"in_body_script",false);
			testWords(os,words,"in_body_after_script",true);
			testWords(os,words,"after_body",false);
			testWords(os,words,"after_html",false);
		}
		
		void testStopWords(ostream & os){
			//for pass off
			StopWords sw(dataDir + "stopwordsNumbers.txt");
			URL url("http://lightplanet.com/mormons/people/joseph_smith/carthage_jail.html");
			HTMLparser parser(&url,&sw);
			BST< Pair<std::string,int> >* words = parser.getWords();
			testWords(os,words,"00",false);
			
			StopWords sw2(dataDir + "stopwordsNumbers.txt");
			URL url2("http://students.cs.byu.edu/~cs240ta/crawler_tests/crawler/testFive.html");
			HTMLparser parser2(&url2,&sw2);
			words = parser2.getWords();
			testWords(os,words,"1",false);
			testWords(os,words,"1803-1887",false);
			testWords(os,words,"2nd",false);
			testWords(os,words,"439-73",false);
			
			URL url3("http://students.cs.byu.edu/~cs240ta/crawler_tests/crawler/valid.asp");
			HTMLparser parser3(&url3,&sw2);
			words = parser3.getWords();
			testWords(os,words,"included",true);
			
		}
		
		void testURL(ostream & os, LinkedList<URL>* urls, string url, bool truth){
			TESTM((urls->Find(URL(url),NULL) != NULL) == truth, url + (true ? " not found\n" : " found\n"));
		}
		
		void testURLSdriver(ostream & os){
			printSubheader("URLS",os);
			
			StopWords sw(dataDir + "stopwordsEvenOdd.txt");

			string urlString = fileProtocol + dataDir + "parserURLs.html";
			URL url(urlString);
			HTMLparser parser(&url,&sw);
			LinkedList<URL>* urls = parser.getURLs();
			
			testURL(os, urls, "http://beforehtml.com", false);
			testURL(os, urls, "http://beforetitle.com", false);
			testURL(os, urls, "http://intitlescript.com", false);
			testURL(os, urls, "http://inbodyscript.com", false);
			testURL(os, urls, "http://afterbody.com", false);
			testURL(os, urls, "http://afterhtml.com", false);
			testURL(os, urls, "mailto:ignoreme@dumbness.com", false);
			
			testURL(os, urls, "http://inbodyafterscript.com", true);
			testURL(os, urls, "http://inbodybeforescript.com", true);
			testURL(os, urls, "http://intitleafterscript.com", true);
			testURL(os, urls, "http://intitlebeforescript.com", true);
			testURL(os, urls, "file:///hiImRelative", true);
			//because one of them is just a #URL.
			testURL(os, urls, fileProtocol + dataDir + "parserURLs.html", true);
			
			
			//for pass off
			URL url2("http://students.cs.byu.edu/~cs240ta/crawler_tests/crawler/testFive.html");
			HTMLparser parser2(&url2,&sw);
			LinkedList<URL>* urls2 = parser2.getURLs();
			testURL(os,urls2,"http://students.cs.byu.edu/~cs240ta/crawler_tests/crawler/valid.asp",true);
			
			//for pass off
			URL url3(fileProtocol + dataDir + "non-html.html");
			HTMLparser parser3(&url3,&sw);
			LinkedList<URL>* urls3 = parser3.getURLs();
			//XML xml(&os,&cerr);
			//urls3->WriteXML(&xml);
			testURL(os,urls3,fileProtocol + dataDir + "non-html.jpg",false);
			testURL(os,urls3,fileProtocol + dataDir + "non-html.odt",false);
			testURL(os,urls3,fileProtocol + dataDir + "non-html1.pl",true);
			testURL(os,urls3,fileProtocol + dataDir + "non-html1.cfm",true);
			testURL(os,urls3,fileProtocol + dataDir + "non-html1.cgi",true);
		}
};
 #endif
 
 
