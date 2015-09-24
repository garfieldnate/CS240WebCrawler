#include "HTMLparser.h"
#include "StringUtil.h"
#include "StopWords.h"
#include "URL.h"
#include "URLInputStream.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <cctype>
#include <sstream>

using namespace std;

		HTMLparser::HTMLparser(URL* url, StopWords* sw):
			URLs(NULL),words(NULL),urlInputStream(NULL),tokenizer(NULL),page(url->getURLstring()),
			parseSuccess(false),inHeader(false),inTitle(false),inScript(false),inBody(false),
			inHTML(false),headerDone(false){
			try{
				urlInputStream = new URLInputStream(url->getURLstring());
			}catch(FileException e){
				cerr << "File exception while loading " << url->getURLstring() << "\n" +
					e.GetMessage() + "\n";
				return;
			}catch(NetworkException e){
				cerr << "Network exception while loading " << url->getURLstring() << "\n" +
					e.GetMessage() + "\n";
				return;
			}catch(IllegalStateException e){
				cerr << "State exception while loading " << url->getURLstring() << "\n" +
					e.GetMessage() + "\n";
				return;
			}
			
			tokenizer = new HTMLTokenizer(urlInputStream);
			words = new BST< Pair<std::string,int> >;
			URLs = new LinkedList<URL>;
			stopWords = sw;
			
			//download and parse!			
			//for description processing, save all three options and just choose the best later
			titleString = "";
			headerString = "";
			first100 = "";//hold first 100 chars of BODY text
			first100length = 0;//hold current size of first100 (not counting white space)
			
			//temp variables for token type and value
			HTMLTokenType tokenType;
			std::string tokenValue;
			
			while(tokenizer->HasNextToken()){
				HTMLToken token = tokenizer->GetNextToken();
				tokenType = token.GetType();
				tokenValue = token.GetValue();
				StringUtil::ToLower(tokenValue);
				
				//stop processing when EOF is reached
				if(tokenType == END)
					break;
				//ignore comments
				if(tokenType == COMMENT)
					continue;
				
				//set processing flags according to start/end tags
				if(tokenType == TAG_START)
					processStart(tokenValue);
				
				if(tokenType == TAG_END)
					processEnd(tokenValue);
				
				//check flags to see if we should keep processing this
				if(noProcess())
					continue;
				
				if(tokenType == TAG_START && tokenValue == "a")
					processLink(token,url);
				
				//process text
				if(tokenType == TEXT)
					processText(tokenValue);
			}
			
			//use the best description string available
			setTitle();
			
			page.setURL(urlInputStream->GetLocation());
			
			parseSuccess = true;
		}
		
		bool HTMLparser::noProcess(){
			//don't have to process anything not within <html></html>
			if(!inHTML)
				return true;
				
			//don't have to process text outside title and body
			if(!inBody && !inTitle)
				return true;
				
			//don't have to process any script text
			if(inScript)
				return true;
			return false;
		}
		
		void HTMLparser::processStart(string tokenValue){
			if(tokenValue == "html")
				inHTML = true;
			if(tokenValue == "body")
				inBody = true;
			if(tokenValue == "title")
				inTitle = true;
			if(tokenValue == "script")
				inScript = true;
			if(isHeader(tokenValue) && !headerDone)
				inHeader = true;
		}
		
		void HTMLparser::processEnd(string tokenValue){
			if(tokenValue == "html")
				inHTML = false;
			if(tokenValue == "body")
				inBody = false;
			if(tokenValue == "title")
				inTitle = false;
			if(tokenValue == "script")
				inScript = false;
			if(isHeader(tokenValue)){
				inHeader = false;
				headerDone = true;
			}
		}
		
		void HTMLparser::processText(string tokenValue){
			//add to title
			if(inTitle)
				titleString.append(tokenValue);
			if(inHeader)
				headerString.append(tokenValue);
			if(first100length < first100target)
				addTo100(first100, first100length, tokenValue);
			//get rid of white space, then add to index
			StringUtil::Trim(tokenValue);
			index(tokenValue);
		}
		
		void HTMLparser::processLink(HTMLToken token, URL* url){
			string link = token.GetAttribute("HREF");
			if(link == "")
				return;
			try{
				URL newURL(link);
				if(newURL.isRelative()){
					newURL.resolve(*url);
				}
				if(URL::isHTML(newURL.getURLstring())){
					newURL.reduce();
					URLs->Insert(newURL,NULL);
				}
			}catch(InvalidURLException e){
				cerr << "Skipping invalid URL " << link << endl;
			}
		}
		
		void HTMLparser::setTitle(){
			if(titleString != "")
				page.setDescription(titleString);
			else if(headerString != "")
				page.setDescription(headerString);
			else{
				page.setDescription(first100);
			}
		}
		
		HTMLparser::~HTMLparser(){
			if(words != NULL)
				delete words;
			if(URLs != NULL)
				delete URLs;
			if(urlInputStream != NULL)
				delete urlInputStream;
			if(tokenizer != NULL)
				delete tokenizer;
		}
		
		bool HTMLparser::success(){
			return parseSuccess;
		}
		
		Page HTMLparser::getPage(){
			return page;
		}
		
		BST< Pair<std::string,int> >* HTMLparser::getWords(){
			return words;
		}
		
		LinkedList<URL>* HTMLparser::getURLs(){
			return URLs;
		}
		
		bool HTMLparser::isHeader(std::string text){
			if(text.size() != 2)
				return false;
			if(tolower(text[0]) != 'h')
				return false;
			int num = atoi(&text[1]);
			if(num < 1 || num > 6)
				return false;
			return true;
		}
		
		void HTMLparser::addTo100(string & first100, int & first100length, string text){
			for(size_t i = 0; i < text.length(); i++){
				if(first100length == first100target)
					return;
				first100 += text[i];
				//only increase count if not white space
				if(!isspace(text[i]))
					first100length++;
			}
		}
		
		void HTMLparser::index(const std::string & text){
			char c;
			string word = "";//keep track of current word
			bool inWord = false;//keep track of whether in a word or not
			for(size_t i = 0; i < text.length(); i++){
				c = text[i];
				if(inWord){
					if(isWordChar(c))
						word += c;
					else{
						indexWord(word);
						word = "";
						inWord = false;
					}
				}
				else
					if(isWordChar(c)){
						word += c;
						inWord = true;
					}
			}
			if(word != "")
				indexWord(word);
		}
		
		bool HTMLparser::isWordChar(char c){
			if(isalnum(c))
				return true;
			if(c == '-' || c == '_')
				return true;
			return false;
		}
		
		void HTMLparser::indexWord(const std::string word){
			if(word.length() == 0)
				return;
			if(!isalpha(word[0]))
				return;
			string wordLc = StringUtil::ToLowerCopy(word);
			//index the word IF it isn't in the stop words BST
			if(stopWords->contains(word))
				return;
			//grab a new node or the already existent one, and add 1 to its value.
			BSTNode<Pair<string,int> >* node = words->Insert(Pair<string,int>(wordLc,0));
			node->GetNonConstValue().incrementSecond();
		}
