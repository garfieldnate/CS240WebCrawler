#include "StopWords.h"
#include "CS240Exception.h"
#include "StringUtil.h"
#include <fstream>
#include <iostream>
#include <assert.h>

using namespace std;

	StopWords::StopWords():words(NULL),size(0){}
	
	StopWords::StopWords(const string fileName):words(NULL),size(0){
		load(fileName);
	}

	bool StopWords::load(const string fileName){
		if(words != NULL)
			clear();
		try{
			ifstream infile ( fileName.c_str() , ifstream::in );
			if(!infile || !infile.is_open())
				throw IOException("Couldn't open stop words file: " + fileName);
			
			//count number of lines in file
			int numOfLines = 0;
			string temp;
			while(getline(infile, temp)){
				numOfLines++;
				infile.clear();
			}
			
			if(numOfLines == 0)
				throw IOException("File is empty: " + fileName);
			
			assert(numOfLines > 0);
			
			//clear eof flag and go back to beginning of file
			infile.clear();
			infile.seekg(0,ios::beg);
			
			//words array is new array of strings, size number of lines in file
			words = new string [numOfLines];
			
			//open and read file again, this time into array
			int line = 0;
			while(line < numOfLines){
				getline(infile,words[line]);
				//cross platform issues made this necessary
				StringUtil::Trim(words[line]);
				line++;
			}
			size = line;
			
			assert(line == numOfLines);
			if(!isSorted()){
				throw InvalidArgumentException("Stop words file was not sorted: " + fileName);
			}
			
			return true;
		}catch(IOException e){
			cout << e.GetMessage() << endl;
		}
		return false;
	}
	
	StopWords::~StopWords(){
		clear();
	}

	bool StopWords::contains(string word)const{
		//throw error if words is NULL or empty
		if(words == NULL || size == 0)
			throw IllegalStateException("Need to load stop words file before searching stop words!");
		
		//binary search for word in words array
		int low = 0;
		int mid;
		int high = size-1;
		int compare;
		while(low <= high){
			//in case of large numbers 
			//(see http://googleresearch.blogspot.com/2006/06/extra-extra-read-all-about-it-nearly.html)
			mid = ((unsigned int)low + (unsigned int)high) >> 1;
			compare = words[mid].compare(word);
			if(compare < 0)
				low = mid + 1;
			else if(compare == 0)
				return true;
			else
				high = mid - 1;
		}
		
		return false;
	}
	
	void StopWords::clear(){
		if(words != NULL){
			delete[] words;
			words = NULL;
			size = 0;
		}
	}
	
	int StopWords::getSize()const{
		return size;
	}
	
	bool StopWords::isSorted(){
		if(words == NULL || size == 0)
			return false;
		for(int i = 1; i < size; i++)
			if(words[i] < words[i -1])
				return false;
			else if(words[i] == words[i - 1])
				cerr << "Warning: words " << i << " and " << i - 1 << " are the same: " << words[i] <<
					endl;
		return true;
	}
	
