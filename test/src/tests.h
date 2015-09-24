#ifndef TESTS_H
#define TESTS_H

#include <iostream>
#include <string>

//bool testURL(std::ostream & os);
bool testCrawler(std::ostream & os);
bool testHTMLparser(std::ostream & os);
bool testOccurrence(std::ostream & os);
bool testPage(std::ostream & os);
bool testStopWords(std::ostream & os);
bool testWord(std::ostream & os);
bool testXML(std::ostream & os);


void printHeader(std::string str, std::ostream & os);

void printSubheader(std::string str, std::ostream & os);

void printEnder(std::string testName, bool success, std::ostream & os);

#endif

