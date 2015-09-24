#ifndef STACK_H
#define STACK_H

#include "LinkedList.h"
#include "CS240Exception.h"
#include <assert.h>
#include <iostream>

//! This class implements a stack template; the type of object contained in it is not required to
//! 	implement any specific methods.

//make LinkedList visible here
template <typename T> class LinkedList;
template <typename T> class LLNode;

template <class T>
class Stack {
	public:
	
		Stack(){
			list = new LinkedList<T>;
		}
		
		~Stack(){
			delete list;
		}
	
		//! @param object to place on the stack
		void push(T item){
			//insert the item add the end of the linked list
			list->Insert(item,NULL);
		}
		
		//! @return object on top of stack
		T pop(){
			if(list->GetSize() == 0)
				throw IllegalStateException("Attempt to pop from empty stack!");
			assert(list->GetSize() > 0);
			//get the first item in the linked list (the newest), extract
			//its value, remove from linked list and return value
			LLNode<T>* node = list->GetFirst();
			T retVal = node->GetValue();
			list->Remove(node);
			return retVal;
		}
		
		//! @return number of nodes in stack
		int size(){
			return list->GetSize();
		}
	
	private:
		LinkedList<T>* list;
};

#endif
