#include "Crawler.h"
#include <fstream>
#include <iostream>

#include "StopWords.h"
#include "BST.h"
#include "Word.h"
#include "XML.h"
#include "Page.h"
#include "URL.h"
#include "Pair.h"
#include "HTMLparser.h"
#include <string>
#include <iostream>
using namespace std;

	Crawler::Crawler(){
		stopWords = new StopWords;
		urlQueue = new Queue<URL>;
		history = new BST<URL>;
		pages = new BST<Page>;
		words = new BST<Word>;
	}
	
	Crawler::~Crawler(){
		delete words;
		delete stopWords;
		delete urlQueue;
		delete history;
		delete pages;
	}
	
	bool Crawler::loadStopWords(const string fileName){
		return stopWords->load(fileName);
	}
	
	void Crawler::crawl(string url){
		//ALGORITHM
		//set up- load stop words, add start URL to queue,
		startURL = URL(url);
		startURL.reduce();
		urlQueue->add(startURL);
		URL currentURL;
		
		//if still have URL's to process
		while(!urlQueue->isEmpty()){
			//get the next one from the queue
			currentURL = urlQueue->remove();
			//if not in the history and within scope
			if(history->Find(currentURL)==NULL && startURL.inSameScope(currentURL)){
				//then parse it
				HTMLparser currentHTML = HTMLparser(&currentURL,stopWords);
				
				if(!currentHTML.success()){
					cout << "failed to load " + currentURL.getURLstring() << endl;
					continue;
				}
				//and add the information to the indeces
				pages->Insert(currentHTML.getPage());
				addWords(currentHTML.getWords(),currentURL.getURLstring());
				//store history
				history->Insert(currentURL);
				//put newly found URL's into the queue
				urlQueue->addAll(currentHTML.getURLs());
			}
		}
	}
	
	void Crawler::addWords(BST < Pair < string,int > >* newOccurrences, string url){
		BSTIterator<Pair <string,int> > iter = newOccurrences->Iterator();
		BSTNode<Pair<string,int> > newNode(Pair<string,int>("",-1));
		BSTNode<Word>* oldNode;
		Occurrence occ;
		occ.setURL(url);
		while(iter.hasNext()){
			newNode = iter.next();
			//is either a new node or an old node
			oldNode = words->Insert(Word(newNode.GetValue().getFirst()));
			occ.setOccurrences(newNode.GetValue().getSecond());
			oldNode->GetValue().addOccurrence(occ);
		}
	}
	
	BST<Word>* Crawler::getWords(){
		return words;
	}
	
	BST<Page>* Crawler::getPages(){
		return pages;
	}
	
	BST<URL>* Crawler::getHistory(){
		return history;
	}
	
	void Crawler::WriteXML(const std::string& fileName)const{
		std::ofstream out(fileName.c_str());
		if(!out)
			throw FileException("Couldn't open file for writing XML: " + fileName);
		XML xml(&out, &cerr);
		xml.openTag("website");
		
		xml.openTag("start-url");
		xml.writeText(startURL.getURLstring());
		xml.closeTag();
		
		xml.openTag("pages");
		pages->WriteXML(&xml);
		xml.closeTag();
		
		xml.openTag("index");
		words->WriteXML(&xml);
		xml.closeTag();
		
		xml.closeTag();
		
		out.close();
	}
	