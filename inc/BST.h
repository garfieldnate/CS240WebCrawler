#ifndef CS240_BST_H
#define CS240_BST_H
#include <assert.h>
#include <string>
#include "CS240Exception.h"
#include <iostream>

#include "XML.h"
using namespace std;
//! These classes implement a Binary Search Tree. They are templates, and the type of object held 
//! in the tree must have the following functions:
//! 1. int compare(T): returns 0 if the objects are equal, 1 or -1 for greater than and less than
//! 	(which one is which doesn't matter)
//! 2. optional: void WriteXML(XML): writes an XML representation of the object.
//! 3. & operator =(const T & other): assignment operator which makes complete copy of itself and
//!		returns a reference to itself
//! 4. T(const T & other): copy constructor (default will be used if none is provided).
//! 5. the copy constructor

//to allow use of XML
class XML;
template <typename T> class BST;

//!  BSTNode implements a binary search tree node
template <typename T>
class BSTNode {
		friend class BST<T>;   //!< BST can access private members of BSTNode
	
	public:
	
		//!  Constructor
		BSTNode(const T & v) :
		  value(v), left(NULL), right(NULL){
		}
		
		//! Copy Constructor
		BSTNode(const BSTNode<T> & other) : 
		  value(other.value),left(other.left),right(other.right){
		}
	
		T & GetNonConstValue(){
			return value;
		}
	
		//!  Read-only public methods for use by clients of the BST class
		const T & GetValue() const{
		  return value;
		}
	
		BSTNode<T> * GetLeft()const{
		  return left;
		}
	
	
		BSTNode<T> * GetRight()const{
		  return right;
		}
		
		//! Assignment operator 
		BSTNode<T> & operator=(const BSTNode<T> & other){
			if(this!=&other){
				value=other.value;
				left=other.left;
				right=other.right;
			}
			
			return *this;
		}
		
		//! compares the node values
		int compare(const BSTNode<T> & other)const{
			return value.compare(other.GetValue());
		}
	
		//! Recursive XML writing method. Writes the contents of this node and all of its children
		//! as XML to out.
		//! @out ostream to write XML to
		void WriteXML(XML* xml){
			assert(xml != NULL);
			if(left != NULL)
				left->WriteXML(xml);
				
			//assert(value != NULL);
			value.WriteXML(xml);
			
			if(right != NULL)
				right->WriteXML(xml);
		}
	private:
		T value;  //!< value stored in the node
		BSTNode<T> * left;     //!< pointer to the node's left child
		BSTNode<T> * right;    //!< pointer to the node's right child
};

//!This class abstracts away from the action of iterating through a BST. Implements a pre-iterator.
//Note: had to store nodes instead of node* because a comparison method is needed.
template <typename T>
class BSTIterator {
	friend class BST<T>;
	public:
		BSTIterator(BST<T>* bst){
			stack = new Stack<BSTNode<T> >;
			if(bst->GetSize() != 0)
				stack->push(*bst->GetRoot());
		}
		~BSTIterator(){
			delete stack;
		}
		bool hasNext(){
			return stack->size() != 0;
		}
		BSTNode<T> next(){
			if(stack->size() == 0)
				throw IllegalStateException("No items to return from iterator");
				
			//pop next node and add its daughters to the stack if existent
			BSTNode<T> node = stack->pop();
			if(node.GetLeft() != NULL)
				stack->push(*node.GetLeft());
			if(node.GetRight() != NULL)
				stack->push(*node.GetRight());	
			
			return node;
		}
	
	private:
		Stack<BSTNode<T> >* stack;
};


//!  BST implements a binary search tree
template <typename T>
class BST {
	
	public:
		//!  Assignment operator.  Makes a complete copy of its argument
		//!  @return Reference to oneself
		BST& operator =(const BST<T> & other){
			if(&other == this)
				return *this;
			Clear();
			Copy(other.GetRoot());
			return *this;
		}
	
		//!  No-arg constructor.  Initializes an empty BST
		BST(){
		     root = NULL;
		     size = 0;
		}
		
		//!  Copy constructor.  Makes a complete copy of its argument
		BST(const BST<T> & other){
			size = 0;
			root = NULL;
			Copy(other.GetRoot());
		}
		
		BSTIterator<T> Iterator(){
			return BSTIterator<T>(this);
		}
		
		//! Entirely copies the Tree using recursive method Unfold
		void Copy(const BSTNode<T> * other){
			if(other == this->root)
				return;
			if(other == NULL){
				Clear();
				root = NULL;
				size = 0;
				return;
			}
			
			Clear();
			root = new BSTNode<T>(*other);
			size++;
			
			if(other->left != NULL){
				root->left = new BSTNode<T>(*other->left);
				Unfold(other->left,root->left);
			}
			
			if(other->right != NULL){
				root->right = new BSTNode<T>(*other->right);
				Unfold(other->right,root->right);
			}
		}

		//!recursive copy method called by Copy
		void Unfold(const BSTNode<T> * copyFrom, BSTNode<T> * copyTo){
			size++;
			if(copyFrom->left != NULL){
				copyTo->left = new BSTNode<T>(*copyFrom->left);
				Unfold(copyFrom->left,copyTo->left);
			}
			if(copyFrom->right != NULL){
				copyTo->right = new BSTNode<T>(*copyFrom->right);
				Unfold(copyFrom->right,copyTo->right);
			}
		}

		//!  Destructor
		~BST(){
			Clear();
		}


		//!  @return a pointer to the root node of the tree, or NULL if the tree is empty.
		//!  @note This is useful for BST clients that need to traverse the tree.)
		BSTNode<T> * GetRoot()const{
			return root;
		}
		
		
		//!  @return true if the BST is empty, or false if the BST is not empty
		bool IsEmpty() const{
		     return size == 0;
		}
		
		
		//!  Removes all values from the BST via the recursive Goodbye method
		void Clear(){
			Goodbye(root);
			root = NULL;
		}
		
		//! A recursive deleting method; deletes the argument node and every node it contains.
		void Goodbye(BSTNode<T> * root){
			if(root == NULL)
				return;
			Goodbye(root->GetLeft());
			Goodbye(root->GetRight());
			size--;
			delete root;
		}


		//!  @return the number of values in the BST
		int GetSize() const{
		     return size;
		}
		
		
		//!  Inserts value v into the BST via the recursive Trickle method
		//!  
		//!  @param v The new value being inserted
		//!
		//!  @return a pointer to the newly inserted node, or a pointer to the node which uses the
		//!   value already
		BSTNode<T> * Insert(const T & v){
			if(root == NULL){
				root = new BSTNode<T>(v);
				size++;
				return root;
			}
			return Trickle(v,root);
		}

		//! The recursive insert function controlled by Insert
		BSTNode<T> * Trickle(const T & v, BSTNode<T>* node){
		     int cmp = node->GetValue().compare(v);
		     if(cmp == 0)//already in tree
			  return node;
		     if(cmp < 0)
			  if(node->right != NULL)
			      return Trickle(v,node->right);
			  else {
			      node->right = new BSTNode<T>(v);
				  size++;
			      return node->right;
			  }
		     else {
			  if(node->left != NULL)
			      return Trickle(v,node->left);
			  else {
			      node->left = new BSTNode<T> (v);
				  size++;
			      return node->left;
			  }
		     }
		 }
		
		//!  Searches the tree for value v by starting a recursive search via Search
		//!  
		//!  @param v The new value being searched for
		//!
		//!  @return a pointer to the node containing v, or NULL if v is not in the tree
		BSTNode<T> * Find(const T & v) const{
			if(root == NULL)
				return NULL;
			return Search(v,root);
		 }
		
		//! The recursive method controlled by Find
		BSTNode<T> * Search(const T & v, BSTNode<T>* node) const{
			int cmp = node->GetValue().compare(v);
			if(cmp == 0)//already in tree
				return node;
			if(cmp < 0)
			  if(node->right != NULL)
				return Search(v,node->right);
			  else
				return NULL;
			else {
			  if(node->left != NULL)
				return Search(v,node->left);
			  else
				return NULL;
			}
	    }
		
		//! Writes an XML representation of the tree to out.
		void WriteXML(XML* xml) const{
			if(root == NULL)
				return;
			root->WriteXML(xml);
		}
	private:
		BSTNode<T> * root;
		int size;
};


#endif
