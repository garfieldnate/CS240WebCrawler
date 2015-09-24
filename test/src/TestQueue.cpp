#ifndef TEST_QUEUE_CPP
#define TEST_QUEUE_CPP
#include "Queue.h"
#include "tests.h"
#include "UnitTest.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
class TestQueue{
	
	public:
		TestQueue():success(true){}
		
		bool testQueue(ostream & os){
			stringstream ss;
			Queue<string> queue;
			queue.add("hello!");
			queue.add("goodbye!");
			
			ss << queue.getSize();
			TESTM(queue.getSize() == 2,"Size Expected: 2\nActual: " + ss.str() + "\n");
			
			string temp = queue.remove();
			TESTM(temp == "hello!", "Expected: \"hello!\"\nActual: " + temp + "\n");
			
			temp = queue.remove();
			TESTM(temp == "goodbye!", "Expected: \"goodbye!\"\nActual: " + temp + "\n");
			
			queue.add("wha?!");
			ss << queue.getSize();
			TESTM(queue.getSize() == 1,"Size Expected: 1\nActual: " + ss.str() + "\n");
			
			temp = queue.remove();
			TESTM(temp == "wha?!", "Expected: \"wha?!\"\nActual: " + temp + "\n");
			
			TESTM(queue.getSize() == 0,"Size Expected: 0\nActual: " + ss.str() + "\n");
			
			TESTE(queue.remove(),IllegalStateException,"Queue of size 0 should throw error when removed from\n");
			
			return success;
		}
	
	private:
		bool success;
};

#endif
