#ifndef CS240_LINKED_LIST_H
#define CS240_LINKED_LIST_H

#include <string>
#include <iostream>
#include "XML.h"

//! These classes implement a LinkedList. They are implemented as Templates, and the type of object
//! contained in the list must have the following functions:
//! 1. optional: void WriteXML(XML): writes an XML representation of the object. Only necessary
//!		if you need to write an XML representation of the list.
//! 2. & operator =(const T & other): assignment operator which makes complete copy of itself and
//!		returns a reference to itself
//! 3. T(const T & other): copy constructor (default will be used if none is provided).
//! 4. the copy constructor

//so LLNode knows about LinkedList
template <typename T> class LinkedList;

//so both classes know about XML, which includes this file
class XML;

//! LLNode implements a doubly-linked list node
template <typename T>
class LLNode {
		friend class LinkedList<T>;  //!< LinkedList can access private members of LLNode
	public:
		//!  Constructors
		
		LLNode<T>() {}
		
		LLNode<T>(const T & v, LLNode<T> * p, LLNode<T> * n) :
		  value(v), prev(p), next(n){}
		
		//! Copy Constructor 
		LLNode(const LLNode<T> & other) : 
		   value(other.value),prev(other.prev),next(other.next){
		}
	
		//!  Read-only public methods for use by clients of the LinkedList class
		const T & GetValue() const{
		  return value;
		}
	
		LLNode<T> * GetPrevious()const{
		  return prev;
		}
	
		LLNode<T> * GetNext()const{
		  return next;
		}
		
		//! Assignment operator 
		LLNode<T> & operator=(const LLNode<T> & other){
			if(this!=&other){
				value=other.value;
				prev=other.prev;
				next=other.next;
			}
			return *this;
		}
		
		//! compares the node values
		int compare(const LLNode & other)const{
			return value.compare(other.GetValue());
		}
		
		//! writes an XML representation of the value of this node
		void WriteXML(XML* xml){
			value.WriteXML(xml);
		}
	
	private:
		T value;        					//!< value stored in the node
		LLNode<T> * prev;            	//!< pointer to previous node in the list
		LLNode<T> * next;            	//!< pointer to next node in the list
};


//! LinkedList implements a doubly-linked list
template <typename T>
class LinkedList{
	public:
	
		//!  No-arg constructor.  Initializes an empty linked list
		LinkedList(){
			beginning = NULL;
			end = NULL;
			size = 0;
		}


		//!  Copy constructor.  Makes a complete copy of its argument
		LinkedList(const LinkedList<T> & other){
			beginning = NULL;
			end = NULL;
			size = 0;
			Copy(other);
		}

		void Copy(const LinkedList<T> & other){
			if(&other == this)
				return;
			Clear();
			size = 0;
			if(other.IsEmpty())
				return;
			beginning = new LLNode<T>(*other.beginning);
			size++;

			LLNode<T>* current = beginning;
			LLNode<T>* next = other.beginning->next;
			while(next != NULL){
				current->next = new LLNode<T>(*next);
				current->next->prev = current;
				current = current->next;
				next = next->next;
			 size++;
			}
			current->next = NULL;
			end = current;
		}

		//!  Destructor
		~LinkedList(){
		    Clear();
		}

		//!  Removes all values from the list
		void Clear(){
			if(IsEmpty())
				return;
			LLNode<T>* current = beginning;
			LLNode<T>* next = beginning->next;
			while(next != NULL){
				delete current;
				current = next;
				next = next->next;
			}
			delete current;
			beginning = NULL;
			end = NULL;
			size = 0;
		}

		//!  @return true if the list is empty, or false if the list is not empty
		bool IsEmpty() const{
			return size == 0;
		}

		//!  @return the number of values in the list
		int GetSize() const{
			return size;
		}

		//!  @return a pointer to the first node in the list, or NULL if the list is empty
		LLNode<T> * GetFirst()const{
			return beginning;
		}

		//!  @returns a pointer to the last node in the list, or NULL if the list is empty
		LLNode<T> * GetLast()const{
			return end;
		}


		//!  Inserts value v into the list after node n
		//!  
		//!  @param v The new value being inserted
		//!  @param n A node that is already in the list after which the new node should 
		//!      be inserted.
		//!      If n is NULL, the new node should be inserted at the beginning of the list.
		//!
		//!  @return a pointer to the newly inserted node
		LLNode<T> * Insert(const T & v, LLNode<T> * n){
			LLNode<T>* newNode;
			if(IsEmpty()){
				newNode = new LLNode<T>(v,NULL,NULL);
				beginning = newNode;
				end = newNode;
				size++;
				//assert(newNode != NULL);
				return newNode;
			}
			if(n == NULL){
				size++;
				newNode = new LLNode<T>(v,NULL,beginning);
				beginning->prev = newNode;
				beginning = newNode;
				//assert(newNode != NULL);
				return newNode;
			}
		    newNode = new LLNode<T>(v,n,n->next);
			if(newNode->next == NULL)
				end = newNode;
		    else
				newNode->next->prev = newNode;
		    newNode->prev->next = newNode;
		    size++;
				//assert(newNode != NULL);
			return newNode;
		}


		//! Searches for the first occurrence of value v that appears in the list 
		//!   after node n
		//!   
		//!  @param v The value being searched for
		//!  @param n The node in the list after which the search should begin.
		//!      If n is NULL, the list should be searched from the beginning.
		//!
		//!  @return a pointer to the node containing v, or NULL if v is not found
		LLNode<T> * Find(const T & v, LLNode<T> * n) const{
		    LLNode<T>* node = n;
			if(IsEmpty())
				return NULL;
		    if(n == NULL)
			 node = beginning;
			else
				node = node->next;
		    while(node != NULL && node->GetValue().compare(v)!=0){
				node = node->GetNext();
			}
			return node;
		}


		//!  Removes node n from the list
		//!  
		//!  @param n The node being removed from the list
		void Remove(LLNode<T> * n){
			if(n == NULL)
				return;
			if(Find(n->GetValue(),NULL)==NULL)
				return;
			if(n == end && end == beginning){
				delete n;
				end = NULL;
				beginning = NULL;
				size--;
				return;
			}
			if(n == end){
				n->prev->next = NULL;
				end = n->prev;
				delete n;
			}
			else if(n == beginning){
				n->next->prev = NULL;
				beginning = n->next;
				delete n;
			}
			else{
				n->prev->next = n->next;
				n->next->prev = n->prev;
				delete n;
			}
		    size--;
		}
		
		//! add every item in the other to this list
		void addAll(LinkedList<T>* other){
			LLNode<T>* node = other->GetFirst();
			while(node != NULL){
				Insert(node->GetValue(),NULL);
				node = node->GetNext();
			}
		}
		
		//! Writes an XML representation of the list; does not print a root node
		//! @param XML object to write to
		void WriteXML(XML* xml)const{
			if(IsEmpty())
				return;
			LLNode<T>* node = beginning;
			while(node != NULL){
				node->WriteXML(xml);
				node = node->next;
			}
		}
	private:
		//!  two dummy nodes to keep track of the beginning and end of the list.
		LLNode<T>* beginning;
		LLNode<T>* end;
		int size;
};


#endif

