#ifndef CRAWLER_H
#define CRAWLER_H

#include "StopWords.h"
#include "BST.h"
#include "Word.h"
#include "XML.h"
#include "Page.h"
#include "URL.h"
#include "Queue.h"
#include "Pair.h"
#include <string>

//! This class is responsible for driving the overall crawling process. Load the stop words and
//! then call crawl(). crawl() can be called multiple times.
class Crawler{
	friend class TestCrawler;
	public:
		//!constructor
		Crawler();
		
		//!destructor
		~Crawler();
		
		//! returns true if words are loaded successfully, false otherwise
		bool loadStopWords(const std::string fileName);
		
		//!crawl, starting input url
		void crawl(std::string url);
		
		//!@return index of Words found during crawling activity so far
		BST<Word>* getWords();
		
		//!@return index of Pages processed during crawling activity so far
		BST<Page>* getPages();
		
		//! @return index of URL's found during crawling activity so far
		BST<URL>* getHistory();
		
		//!@return index
		
		//! writes XML representation to file
		void WriteXML(const std::string& fileName)const;
	private:
	
		//! Adds every word in the list to the tree, altering occurence information if the tree
		//! 	already contains the word.
		//! @param list to add words from
		//! @param url words were found in
		void addWords(BST < Pair < std::string,int > >* newOccurences, std::string url);
				
		StopWords* stopWords;
		Queue<URL>* urlQueue;
		BST<URL>* history;
		BST<Page>* pages;
		BST<Word>* words;
		URL startURL;
};
#endif
