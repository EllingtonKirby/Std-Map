#include <iostream>
#include <utility>
#include <random>
#include <limits.h>
#include <memory>

namespace cs540{
	template<typename Key_T, typename Mapped_T> class Map{
		public:
			struct Node;
		private:
			int max_height;
			int current_size;
			int rheight();
			Node * head;
			Node * tail;
			int curr_height;
			std::pair<Node*, bool> skip_list_insert(Key_T key, Mapped_T val, bool searchFlag);
			bool skip_list_erase(const Key_T key);
		public:
			class Iterator;
			class ConstIterator;
			class ReverseIterator;
			struct Node{
				int height;
				Key_T& key;
				Mapped_T& value;
				ValueType vtype;
				Node **next;
				Node *prev;
				int width;
				Node(Key_T keyed, const Mapped_T &valued, int applied_height = rheight()){
					vtype = std::pair<Key_T, Mapped_T>(key, value);
					value = vtype.second;
					key = vtype.first;
					height = applied_height;
					next = new Node*[max_height];
					for(int i = 0; i < max_height; i++){
						this->next[i] = nullptr;
					}
					prev = nullptr;
				};
				~Node(){
					delete[] next;
				};
				Node(const Node& rhs){
					this->key = rhs->key;
					this->value = rhs->value;
					this->height = rhs->height;
					this->width = rhs->width;
					//What to do about prev? Is this a shallow copy? Confused about this
					this->prev = rhs->prev;
					
					this->next = new Node*[max_height];
					for(int i = 0; i < max_height; i++){
						//Is this an issue? Are you pointing to the object in RHS now? Do you need to define the assignment operator here?
						//I think this is a shallow copy, where next[i] is now pointing to the object pointed to by rhs->next[i] rather than
						//a new instance of that node
						this->next[i] = rhs->next[i];
					}
				};
			};
			//Member Type
			typedef std::pair<const Key_T, Mapped_T> ValueType;

			//Constructors and Assignment
			Map();
			Map(const Map&);
			Map &operator=(const Map&);
			Map(std::initializer_list<std::pair<const Key_T, Mapped_T>>);
			~Map();
			//Size
			size_t size() const;
			bool empty() const;
			//Iterators
			Iterator begin();
			Iterator end();
			ConstIterator begin() const;
			ConstIterator end() const;
			ReverseIterator rbegin();
			ReverseIterator rend();
			//Element Access
			Iterator find(const Key_T &);
			Mapped_T &at(const Key_T &);
			const Mapped_T &at(const Key_T &) const;
			Mapped_T &operator[](const Key_T &);
			//Modifiers
			std::pair<Iterator, bool> insert(const ValueType &);
			template<typename IT_T> void insert(IT_T range_beg, IT_T range_end);
			void erase(Iterator pos);
			void erase(const Key_T &);
			void clear();

			class Base_Iterator{
				protected:
					std::shared_ptr<Node> target;
			};
			
			class Iterator: public Base_Iterator{	
				private:
					Iterator(Node* pos);
				public:
					Iterator(const Iterator &);
					Iterator& operator=(const Iterator &);
					Iterator& operator++();
					Iterator operator++(int);
					Iterator& operator--();
					Iterator operator--(int);
					ValueType *operator->() const;
					ValueType &operator*() const;
			};

			class ConstIterator: public Base_Iterator{
				private:
					ConstIterator(const Node* pos);
				public:
					ConstIterator(const Iterator &);
					ConstIterator& operator=(const Iterator &);
					ConstIterator& operator++();
					ConstIterator operator++(int);
					ConstIterator& operator--();
					ConstIterator operator--(int);
					const ValueType *operator->() const;
					const ValueType &operator*() const;
			};

			class ReverseIterator: public Base_Iterator{
				private:
					ReverseIterator(Node* pos);
				public:
					ReverseIterator(const Iterator &);
					ReverseIterator& operator=(const Iterator &);
					ReverseIterator& operator++();
					ReverseIterator operator++(int);
					ReverseIterator& operator--();
					ReverseIterator operator--(int);
					ValueType *operator->() const;
					ValueType &operator*() const;
			};


	};	
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::Map(){
		int max_height = 32; //TODO find out what to do with max height
		Node * head = new Node(NULL, NULL, max_height);
		Node * tail = new Node(NULL, NULL, max_height);
		for(int i = 0; i < max_height; i++){
			head->next[i] = tail;
			tail->next[i] = nullptr;
		}
		head->prev = nullptr;
		tail->prev = head;
		size = 0;
		//What's next?
	}

	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::Map(const Map &rhs){
		//What is the best way to do this? 
		//Iterate through using iterators, dereference the iterator and call insert?
		//Iterate through using iterators, dereference the iterator and copy the node? Seems to be the best, but need a way to also copy head
		//Begin skips head, but TA suggests this is best way. Oh dont need to copy head... just call insert and the head will be properly formed
		int max_height = 32;
		Node * head = new Node(NULL, NULL, max_height);
		Node * tail = new Node(NULL, NULL, max_height);
		for(int i = 0; i < max_height; i++){
			head->next[i] = tail;
			tail->next[i] = nullptr;
		}
		head->prev = nullptr;
		tail->prev = head;
		size = 0;
		
		auto iter = rhs.begin();
		while(iter != rhs.end()){
			//For the insert function, do we insert the keys and mapped objects by value??? Or references??
			this->skip_list_insert(iter->first, iter->second);
		}
			

	}
	//operator- 
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>& Map<Key_T, Mapped_T>::operator=(const Map&){}

	//initializer list construction
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::Map(std::initializer_list<std::pair<const Key_T, Mapped_T>>){}
	
	//destructor
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::~Map(){
		//Ask about this function
		Node * it = tail->prev;
		while(it->prev != nullptr){
			it = it->prev;
			delete it->next[0];
		}
		delete head;
		delete tail;
		curr_height = 0;
		size = 0;
	}

	//Iterator constructor
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::Iterator::Iterator(Node* pos){
		this->target = std::make_shared<Node>(pos);
	}
	
	//Iterator ++ operator returns reference post increment
	//the ampersand at the end takes care of the return by reference 
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator& Map<Key_T, Mapped_T>::Iterator::operator++(){
		this->target = target->next[0];
		return *this;
	}
	
	//Iterator ++ operator returns iterator preincrement
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::Iterator::operator++(int){
		this->target = target->next[0];
		return *this;
	}
	
	//Iterator -- operator returns reference postdecremet
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator& Map<Key_T, Mapped_T>::Iterator::operator--(){
		this->target = target->prev;
		return *this;
	}
	
	//Iterator -- operator returns iterator predecrement
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::Iterator::operator--(int){
		this->target = target->prev;
		return this;
	}
	
	//Iterator -> access	
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ValueType* Map<Key_T, Mapped_T>::Iterator::operator->() const{
		//Does this return a pointer???
		return this->target->vtype;	
	}
	
	//Iterator dereference operator 
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ValueType& Map<Key_T, Mapped_T>::Iterator::operator*() const{
		//Does this pair object copy the elements by value? How do I make it possible to modify the members of the iterator via this function
		return this->target->vtype; 
	}

	//ConstIterator constructor
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::ConstIterator::ConstIterator(const Node* pos){
		this->target = std::make_shared<Node>(pos);
	}

	//ReverseIterator constructor
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::ReverseIterator::ReverseIterator(Node* pos){
		this->target = std::make_shared<Node>(pos);
	}

	//Iterator begin()
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::begin(){
		auto it = new Iterator(head->next[0]);
		return it;
	}
	
	//Iterator end()
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::end(){
		auto it = new Iterator(tail);
		return it;
	}
	
	//ConstIterator begin()
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::begin() const{
		auto it = new ConstIterator(head->next[0]);
		return it;
	}
	
	//ConstIterator end()
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::end() const{
		auto it = new ConstIterator(tail);
		return it;
	}
	
	//ReverseIterator rbegin()
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::rbegin(){
		auto it = new ReverseIterator(tail->prev);
		return it;
	}
	
	//ReverseIterator rend()
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::rend(){
		auto it = new ReverseIterator(head);
		return it;
	}
	
	//Function that determines the random height of a column. All credit to Eternally Confuzzled at eternallyconfuzzled.com/tuts/datastructures/jsw_tut_skip.aspx
	template<typename Key_T, typename Mapped_T>
	int Map<Key_T, Mapped_T>::rheight(){
		static int bits = 0;
		static int reset = 0;
		int h, found = 0;
		std::random_device rd;
		for(h = 0; !found; h++){
			if(reset == 0){
				bits = rd();
				reset = sizeof(int) * CHAR_BIT;
			}
			found  = bits & 1;
			bits = bits >> 1;
			--reset;
		}

		if(h >= max_height){
			h = max_height - 1;
		}
		return h;
	}

	//All credit to Eternally Confuzzled at eternallyconfuzzled.com/tuts/datastructures/jsw_tut_skip.aspx
	//Skip List insert
	template<typename Key_T, typename Mapped_T>
	std::pair<typename Map<Key_T, Mapped_T>::Node*, bool> Map<Key_T, Mapped_T>::skip_list_insert(Key_T key, Mapped_T value, bool searchFlag = false){
		Node *it = this->head;
		Node *item  = new Node(key, value);
		Node **fix = new Node*[max_height];

		for(int i = curr_height - 1; i >= 0; i--){
			
			while(it->next[i] != nullptr && it->next[i]->key < key){
				it = it->next[i];
			}
			fix[i] = it;
		}
		//Check if key already is in map
		if(it->next[0] != nullptr && it->next[0]->key == key){
			delete[] fix;
			delete item;
			return std::pair<Node*, bool>(&it->next[0], false);
		}
		else if(searchFlag){
			//Key is not in map, but we are just here to search
			delete[] fix;
			delete item;
			return std::pair<Node*, bool>(nullptr, false);
		}
		else{
			//key is not in map but we are here to insert
			while(item->height > curr_height){
				fix[curr_height++] = this->head;
			}
			
			int h = item->height;
			while(--h >= 0){
				item->next[h] = fix[h]->next[h];
				fix[h]->next[h] = item;
				if(h == 0){
					item->prev = fix[h]->next[h];
				}
			}
			item->next[0]->prev = item;
			current_size++;
		}
		delete[] fix;
		return std::pair<Node*, bool>(&item, true);
	}			

	
	//All credit to Eternally Confuzzled at eternallyconfuzzled.com/tuts/datastructures/jsw_tut_skip.aspx
	//Skip List erase
	template<typename Key_T, typename Mapped_T>
	bool Map<Key_T, Mapped_T>::skip_list_erase(const Key_T key){
		Node* it = head;
		Node* to_del;
		Node **fix = new Node*[max_height];
		for(int i = curr_height - 1; i >= 0; i--){
			while(it->next[i] != nullptr && it->next[i]->key < key){
				it = it->next[i];
			}
			fix[i] = it;
		}
		if(it->next[0] == nullptr || it->next[0]->key != key){
			return false;
		}
		else{
			to_del = fix[0]->next[0];
			for(int i = 0; i < curr_height; i++){
				if(fix[i]->next[i] != nullptr){
					fix[i]->next[i] = fix[i]->next[i]->next[i];
					fix[i]->next[i]->prev = fix[i];
				}
			}
			while(curr_height > 0){
				if(head->next[curr_height-1] != nullptr){
					break;
				}
				head->next[--curr_height] = nullptr;
			}
		}
		current_size--;
		delete to_del;
		delete[] fix;
		return true;
	}

}
//Comparison
//bool operator==(const Map &, const Map &);
//bool operator!=(const Map &, const Map &);
//bool operator<(const Map &, const Map &);
//Implement all comparisons between iterators

