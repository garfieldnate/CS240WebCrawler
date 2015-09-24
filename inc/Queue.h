#ifndef QUEUE_H
#define QUEUE_H
#include "CS240Exception.h"
#include "LinkedList.h"

//! Implements a queue, using a linked list under the hood.

template <typename T>
class Queue{
	public:
		Queue(){
			linkedList = new LinkedList<T>();
		}
		~Queue(){
			delete linkedList;
		}
		
		//!adds all items in the list to the queue
		void addAll(LinkedList<T>* list){
			linkedList->addAll(list);
		}
		
		//!adds an item to the queue
		void add(T item){
			//insert the item add the beginning of the linked list
			linkedList->Insert(item,NULL);
		}
		//!removes and returns the next item in the queue
		T remove(){
			if(linkedList->GetSize() == 0)
				throw IllegalStateException("Attempt to pop from empty stack!");
			//get the last item in the linked list (the oldest), extract
			//its value, remove from linked list and return value
			LLNode<T>* node = linkedList->GetLast();
			T retVal = node->GetValue();
			linkedList->Remove(node);
			return retVal;
		}
		
		//!@return true if the queue is empty, false otherwise
		bool isEmpty()const{
			return linkedList->IsEmpty();
		}
		
		//!@return number of items in the queue
		int getSize()const{
			return linkedList->GetSize();
		}
		
	private:
		LinkedList<T>* linkedList; 
		
};
#endif
