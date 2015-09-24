#ifndef STOPWORDS_H
#define STOPWORDS_H

#include <string>

class StopWords{
	//necessary to test isSorted()
	friend class TestStopWords;
	public:
	
		//! Constructor
		StopWords();
		
		//! Constructor which loads every line in the file into the array
		//! assumes that the file is pre-sorted; NOTHING WILL WORK IF THE FILE IS NOT SORTED!
		StopWords(const std::string fileName);

		//!destructor deletes entire array of stop words
		~StopWords();
		
		//!  @return true if the words are successfully loaded, false otherwise
	//! assumes that the file is pre-sorted; NOTHING WILL WORK IF THE FILE IS NOT SORTED!
	//! deletes previously loaded words
		bool load(const std::string fileName);
		
		//! true if the word is contained, false otherwise. Uses binary search in the words array.
		bool contains(std::string word)const;
		
		//! removes every word in the stop list by deleting the words array
		void clear();
		
		//!@return number of stored stop words
		int getSize()const;
		
	private:
		//the array of strings which will hold all of the words
		std::string* words;
		//size of words array
		int size;
		
		//checks that the words array is sorted. 
		//!@return true if the array is sorted in ascending order.
		//!prints warnings if two items are the same
		bool isSorted();
};
#endif

