#ifndef TEST_STOPWORDS_CPP
#define TEST_STOPWORDS_CPP
#include "StopWords.h"
#include "tests.h"
#include "UnitTest.h"
#include "CS240Exception.h"
#include <sstream>
#include <stdio.h>

//test files for this are stored in test/data

using namespace std;
class TestStopWords{
	public:
		//!@param directory where datafiles are stored
		TestStopWords(string dataDir):success(true),dataDirectory(dataDir){}
		
		bool testStopWords(ostream & os){
			testSize(os);
			testErrors(os);
			testContainsDriver(os);
			//testLargeRead(os);
			return success;
		}
	
	private:
		bool success;
		string dataDirectory;
		
		void testSize(ostream & os){
			printSubheader("SIZE",os);
			
			stringstream sizeStr;
			StopWords stopWords;
			
			sizeStr << stopWords.getSize();
			TESTM(stopWords.getSize() == 0, "Expected: 0\nActual: " + sizeStr.str() + "\n");
			
			stopWords.load(dataDirectory + "stopwordsOdd.txt");
			sizeStr << stopWords.getSize();
			TESTM(stopWords.getSize() == 13, "Expected: 13\nActual: " + sizeStr.str() + "\n");
			
			stopWords.load(dataDirectory + "stopwordsEvenOdd.txt");
			sizeStr << stopWords.getSize();
			TESTM(stopWords.getSize() == 14, "Expected: 14\nActual: " + sizeStr.str() + "\n");
			
			stopWords.load(dataDirectory + "stopwordsOne.txt");
			sizeStr << stopWords.getSize();
			TESTM(stopWords.getSize() == 1, "Expected: 1\nActual: " + sizeStr.str() + "\n");
			
			stopWords.clear();
			sizeStr << stopWords.getSize();
			TESTM(stopWords.getSize() == 0, "Expected: 0\nActual: " + sizeStr.str() + "\n");
		}
		
		
		//void testLargeRead(ostream & os){
			//printSubheader("LARGE FILE READ",os);
			//FILE * file;
			//string filename = dataDirectory + "largeFileTest.txt";
			//file = fopen (filename.c_str(),"w");
			//stringstream temp;
			//if (file!=NULL){
				//int i;
				//for(i = 0; i < 100000, i++){
					//temp << i;
					//fputs(temp.c_str(),file);
					//fputs("\n",file);
				//}
				//fputs ("fopen example",pFile);
				//fclose (file);
			//}
			//else{
				//TESTM(false,"Could not create " + dataDirectory + "largeFileTest.txt\n");
				//return;
			//}
			//StopWords sw;
			//TESTM(sw.load(dataDirectory + "largeFileTest.txt"),
				//"Could not load " + dataDirectory + "largeFileTest.txt\n");
		//}
		
		
		void testErrors(ostream & os){
			printSubheader("EXCEPTIONS",os);
			os << "--Ignore the error messages in this section--" << endl;
			
			//make sure that bad file reads return false
			StopWords sw;
			TESTM(!sw.load(dataDirectory + "nothing.txt"),
				"Expected false for loading non existent file: " + dataDirectory + "nothing.txt\n");
			TESTM(!sw.load(dataDirectory + "stopwordsEmpty.txt"),
				"Expected false for loading empty file: " + dataDirectory + "stopwordsEmpty.txt\n");
			
			stringstream sizeStr;
			sizeStr << sw.getSize();
			TESTM(sw.getSize() == 0, "Expected size 0, but size was " + sizeStr.str() + "\n");
			
			TESTE(sw.contains("naithin'");,IllegalStateException,
				"Expected error querying empty StopWords");
			
			TESTE(sw.load(dataDirectory + "stopwordsNotSorted.txt"),InvalidArgumentException,
				"Should have thrown exception for a stop words file that wasn't sorted!:\n" +
					dataDirectory + "stopwordsNotSorted.txt\n");
		}
		
		void testContains(ostream & os, StopWords & sw, const string word, bool truth){
			TESTM(sw.contains(word) == truth, "tested: " + word + "\nExpected: " +
				(truth ? "true" : "false") + "\n");
		}
		void testContainsDriver(ostream & os){
			printSubheader("CONTAINS",os);
			StopWords sw;
			
			sw.load(dataDirectory + "stopwordsOdd.txt");
			testContains(os,sw,"a",true);
			testContains(os,sw,"an",true);
			testContains(os,sw,"he",true);
			testContains(os,sw,"her",true);
			testContains(os,sw,"him",true);
			testContains(os,sw,"it",true);
			testContains(os,sw,"it's",true);
			testContains(os,sw,"its",true);
			testContains(os,sw,"she",true);
			testContains(os,sw,"the",true);
			testContains(os,sw,"their",true);
			testContains(os,sw,"them",true);
			testContains(os,sw,"they",true);
			
			sw.load(dataDirectory + "stopwordsEvenOdd.txt");
			testContains(os,sw,"a",true);
			testContains(os,sw,"an",true);
			testContains(os,sw,"he",true);
			testContains(os,sw,"her",true);
			testContains(os,sw,"him",true);
			testContains(os,sw,"it",true);
			testContains(os,sw,"it's",true);
			testContains(os,sw,"its",true);
			testContains(os,sw,"she",true);
			testContains(os,sw,"the",true);
			testContains(os,sw,"their",true);
			testContains(os,sw,"them",true);
			testContains(os,sw,"they",true);
			testContains(os,sw,"zey",true);
			
			sw.load(dataDirectory + "stopwordsEvenEven.txt");
			testContains(os,sw,"a",true);
			testContains(os,sw,"an",true);
			testContains(os,sw,"he",true);
			testContains(os,sw,"her",true);
			testContains(os,sw,"him",true);
			testContains(os,sw,"it",true);
			testContains(os,sw,"it's",true);
			testContains(os,sw,"its",true);
			testContains(os,sw,"she",true);
			testContains(os,sw,"the",true);
			testContains(os,sw,"their",true);
			testContains(os,sw,"them",true);
			
			sw.load(dataDirectory + "stopwordsTwo.txt");
			testContains(os,sw,"a",true);
			testContains(os,sw,"an",true);
			
			sw.load(dataDirectory + "stopwordsOne.txt");
			testContains(os,sw,"a",true);
			testContains(os,sw,"notthere!",false);
		}

};

#endif

