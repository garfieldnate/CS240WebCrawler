#ifndef TEST_CRAWLER_CPP
#define TEST_CRAWLER_CPP
#include "Crawler.h"
#include "tests.h"
#include "UnitTest.h"
#include <string>
#include <sstream>
using namespace std;
class TestCrawler{
	public:
		TestCrawler(const string fp, const string dir):success(true),fileProtocol(fp),dataDir(dir){}
		
		bool testCrawler(ostream & os){

			testAddWords(os);
			testData(os);
			//run it once
			Crawler crawler;
			crawler.loadStopWords(dataDir + "stopwordsEvenOdd.txt");
			crawler.crawl(fileProtocol + dataDir + "file/index.html");
			
			string outputDir = dataDir + "index.xml";
			crawler.WriteXML(outputDir.c_str());
			return success;
		}
	
	private:
		bool success;
		string dataDir;
		string fileProtocol;
		
		void testAddWords(ostream & os){
			printSubheader("ADD WORDS",os);
			BST < Pair < string,int > >* words = new BST < Pair < string, int > >;
			words->Insert(Pair<string,int>("buh",3));
			words->Insert(Pair<string,int>("stuff",4));
			string urlString = "http://www.junk.com";
			
			Crawler crawler;
			crawler.addWords(words,urlString);
			BST<Word>* crawledWords = crawler.getWords();
			testWords(os,crawledWords,"buh",urlString,3);
			testWords(os,crawledWords,"stuff",urlString,4);
			
			delete words;
		}
		
		void testWords(ostream & os, BST<Word>* words, string word, string url, int occ){
			BSTNode<Word>* found = words->Find(Word(word));
			if(found == NULL && occ > 0){
				TESTM(false, word + " not found\n");
				return;
			}
			if(found == NULL){//prevent calling methods on NULL
				TESTM(true,"");//pass if occ == 0 and nothing was found
				return;
			}
			stringstream sizeStr;
			sizeStr << found->GetValue().getOccurrence(url);
			stringstream occStr;
			occStr << occ;
			TESTM(found->GetValue().getOccurrence(url) == occ, word + " Expected: " + occStr.str() + "\nfound: " + sizeStr.str() + "\n");
		}
		
		void testData(ostream & os){
			printSubheader("WORDS",os);
			stringstream numStr;
			Crawler crawler;
			crawler.loadStopWords(dataDir + "stopwordsEvenOdd.txt");
			
			string urlString1 = fileProtocol + dataDir + "crawlerTest1.html";
			string urlString2 = fileProtocol + dataDir + "crawlerTest2.html";
			crawler.crawl(urlString1);

			BST<Word>* words = crawler.getWords();
			testWords(os,words,"junky",urlString1,1);
			testWords(os,words,"junky",urlString2,1);
			
			
			printSubheader("PAGES",os);
			BST<Page>* pages = crawler.getPages();
			BSTNode<Page>* page = pages->Find(Page(urlString1));
			TESTM(page != NULL,"Expected to find page: " + urlString1 + "\n");
			page = pages->Find(Page(urlString2));
			TESTM(page != NULL,"Expected to find page: " + urlString2 + "\n");
			
			printSubheader("URLS",os);
			BST<URL>* history = crawler.getHistory();
			BSTNode<URL>* url = history->Find(URL(urlString1));
			testURL(os,history,urlString1,true);
			testURL(os,history,urlString2,true);
			testURL(os,history,"http://junk.com",false);
			
			//Added tests for pass off
			printSubheader("MORE WORDS",os);
			Crawler crawler2;
			crawler2.loadStopWords(dataDir + "stopwordsEvenOdd.txt");//redundant; fix if there's time
			BST<Word>* words2 = crawler2.getWords();
			string urlString3 = fileProtocol + dataDir + "makefile.php";
			crawler2.crawl(urlString3);
			BSTNode<Word>* found = words2->Find(Word("value"));

			testWords(os,words2,"value",urlString3,0);
			testWords(os,words2,"var-name",urlString3,1);
			testWords(os,words2,"rd",urlString3,0);
			testWords(os,words2,"th",urlString3,0);
			testWords(os,words2,"nobs",urlString3,0);
			testWords(os,words2,"buh",urlString3,0);
			
			printSubheader("MORE URLS",os);
			Crawler crawler3;
			crawler3.loadStopWords(dataDir + "stopwordsEvenOdd.txt");
			history = crawler3.getHistory();
			crawler3.crawl("http://students.cs.byu.edu/~cs240ta/crawler_tests/crawler/testFive.html");
			testURL(os,history,"http://students.cs.byu.edu/~cs240ta/crawler_tests/crawler/valid.asp",true);
			
		}
		
		void testURL(ostream & os, BST<URL>* urls, string url, bool truth){
			TESTM((urls->Find(URL(url)) != NULL) == truth, url + (true ? " not found\n" : " found\n"));
		}

};

#endif
