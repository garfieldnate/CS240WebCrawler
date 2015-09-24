#ifndef TEST_STACK_CPP
#define TEST_STACK_CPP
#include "Stack.h"
#include "tests.h"
#include "UnitTest.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
class TestStack{
	
	public:
		TestStack():success(true){}
		
		bool testStack(ostream & os){
			stringstream ss;
			Stack<string> stack;
			stack.push("hello!");
			stack.push("goodbye!");
			
			ss << stack.size();
			TESTM(stack.size() == 2,"Size Expected: 2\nActual: " + ss.str() + "\n");
			
			string temp = stack.pop();
			TESTM(temp == "goodbye!", "Expected: \"goodbye!\"\nActual: " + temp + "\n");
			
			temp = stack.pop();
			TESTM(temp == "hello!", "Expected: \"hello!\"\nActual: " + temp + "\n");
			
			stack.push("wha?!");
			ss << stack.size();
			TESTM(stack.size() == 1,"Size Expected: 1\nActual: " + ss.str() + "\n");
			
			temp = stack.pop();
			TESTM(temp == "wha?!", "Expected: \"wha?!\"\nActual: " + temp + "\n");
			
			TESTM(stack.size() == 0,"Size Expected: 0\nActual: " + ss.str() + "\n");
			
			TESTE(stack.pop(),IllegalStateException,"Stack of size 0 should throw error when popped\n");
			
			return success;
		}
	
	private:
		bool success;

};

#endif
