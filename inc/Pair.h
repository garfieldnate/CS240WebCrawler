#ifndef PAIR_H
#define PAIR_H

//! This class is a template for storing items in pairs. The items to be stored should have an
//! operator= function, and, if comparing is need, type T should have a compare method.
template <typename T, typename U>
class Pair{
	
	public:
	
		Pair(const Pair & other):first(other.getFirst()),second(other.getSecond()){}
		
		Pair(const T & t, U u):first(t),second(u){}
		
		T getFirst()const{
			return first;
		}
		
		U getSecond()const{
			return second;
		}
		
		void setFirst(T f){
			first = f;
		}
		
		void setSecond(U s){
			second = s;
		}
		
		//!only for use with integers, etc.
		void incrementSecond(){
			++second;
		}
	
		//!compare the pairs through their First elements
		int compare(const Pair<T,U> & other)const{
			return first.compare(other.getFirst());
		}
	
	private:
		T first;
		U second;
};

#endif
