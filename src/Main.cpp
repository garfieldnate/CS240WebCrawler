#ifndef MAIN_CPP
#define MAIN_CPP

#include <iostream>

#include "Crawler.h"
#include "CS240Exception.h"

using namespace std;

int main(int argc, char* argv[]){
	if(argc != 4){
		cout << "Usage: crawler <start-url> <output-file> <stopword-file>" << endl;
		return -1;
	}
	try{
		Crawler crawler;
		crawler.loadStopWords(argv[3]);
		crawler.crawl(argv[1]);
		crawler.WriteXML(argv[2]);
	}catch(CS240Exception e){
		cerr << e.GetMessage();
	}
}
#endif
