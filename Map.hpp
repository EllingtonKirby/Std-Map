#include <iostream>
#include <utility>
#include <random>
#include <limits.h>
#include <memory>
#include <array>
#include <stdexcept>

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
			void insertSpecificNode(int level, Node* last, Node* n);
		public:	
			//Member Type
			typedef std::pair<const Key_T, Mapped_T> ValueType;
			
			class Iterator;
			class ConstIterator;
			class ReverseIterator;
			struct Node{
				int height;
				Key_T key;
				Mapped_T value;
				ValueType vtype;
				Node **next;
				Node *prev;
				int width;
				Node(Key_T &keyed, Mapped_T &valued, int applied_height = rheight()) : vtype(keyed, valued){
					value = vtype.second;
					key = vtype.first;
					height = applied_height;
					next = new Node*[height];
					for(int i = 0; i < height; i++){
						this->next[i] = nullptr;
					}
					prev = nullptr;
				};
				~Node(){
					delete[] next;
				};
				Node(const Node& rhs){
					this->vtype = rhs.vtype;
					this->key = vtype.first;
					this->value = vtype.second;
					this->height = rhs.height;
					//What to do about prev? Is this a shallow copy? Confused about this
					//this->prev = rhs->prev;
					
					//this->next = new Node*[max_height];
					//for(int i = 0; i < max_height; i++){
						//Is this an issue? Are you pointing to the object in RHS now? Do you need to define the assignment operator here?
						//I think this is a shallow copy, where next[i] is now pointing to the object pointed to by rhs->next[i] rather than
						//a new instance of that node
					//	this->next[i] = rhs->next[i];
					//}
				};
			};


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
			template<typename IT_T> 
			void insert(IT_T range_beg, IT_T range_end){
				while(begin != end){
					skip_list_insert(*begin.first, *begin.second);
				}
				
			}
			void erase(Iterator pos);
			void erase(const Key_T &);
			void clear();

			class Iterator{	
				private:
					Iterator(Node* pos);
					std::shared_ptr<Node> target;
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

			class ConstIterator{
				private:
					ConstIterator(const Node* pos);
					std::shared_ptr<Node> target;
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

			class ReverseIterator{
				private:
					ReverseIterator(Node* pos);
					std::shared_ptr<Node> target;
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
	//Map Default Constructor	
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::Map(){
		int max_height = 32; 
		Key_T sent_key;
		Mapped_T sent_map;
		Node * head = new Node(sent_key, sent_map, max_height);
		Node * tail = new Node(sent_key, sent_map, max_height);
		for(int i = 0; i < max_height; i++){
			head->next[i] = tail;
			tail->next[i] = nullptr;
		}
		head->prev = nullptr;
		tail->prev = head;
		current_size = 0;
		//What's next?
	}
	//Map Copy Constructor
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::Map(const Map &rhs){
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
		std::array<Node*, 32> last;
		last.fill(&head);
		
		for(Node *n = rhs.head.next[0]; n != rhs.tail; n = n->next[0]){
			Node *nn = new Node(*n);
			for(int i = 0; i < nn->height; i++){
				if(i == 0){
					nn->prev = last[i];
				}
				//insert nn into corresponding row for each height it is a part of
				//insertSpecificNode(i, last.at(i), nn);
				//Is this all we need to do?
				last.at(i).next[i] = nn;
				last.at(i) = nn;
			}
			current_size++;
		}
	}
	//Map operator= 
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>& Map<Key_T, Mapped_T>::operator=(const Map& rhs){
		if(*this == rhs){
			return *this;
		}
		std::array<Node*, max_height> last;
		last.fill(&head);
		
		for(Node *n = rhs.head.next[0]; n != rhs.tail; n = n->next[0]){
			Node *nn = new Node(*n);
			for(int i = 0; i < nn->height; i++){
				if(i == 0){
					nn->prev = last[i];
				}
				//insert nn into corresponding row for each height it is a part of
				//insertSpecificNode(i, last.at(i), nn);
				//Is this all we need to do?
				last.at(i).next[i] = nn;
				last.at(i) = nn;
			}
			current_size++;
		}	
		return *this;
	}

	//Map initializer list construction
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::Map(std::initializer_list<std::pair<const Key_T, Mapped_T>> list){
		auto iter = list.begin();
		while(iter != list.end()){
			this->skip_list_insert(iter->first, iter->second);
			iter++;
		}
	}
	
	//Map destructor
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
		Iterator retval = *this;
		this->target = target->next[0];
		return retval;
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
		Iterator retval = *this;
		this->target = target->prev;
		return retval;
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
			h = h % max_height;
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
			return std::pair<Node*, bool>(it->next[0], false);
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
		return std::pair<Node*, bool>(item, true);
	}			

	//iterator find
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::find(const Key_T &search){
		std::pair<Node*, bool> result = skip_list_insert(search, NULL, true);
		if(result.first == nullptr){
			return end();
		}

		return Iterator(result.first);
	}		
	//At, non const
	template<typename Key_T, typename Mapped_T>
	Mapped_T& Map<Key_T, Mapped_T>::at(const Key_T &search){
		std::pair<Node*, bool> result = skip_list_insert(search, NULL, true);
		if(result.first == nullptr){
			throw std::out_of_range("Key is not found in map");
		}

		return result.first->key;
	}
	
	//At, const
	template<typename Key_T, typename Mapped_T>	
	const Mapped_T& Map<Key_T, Mapped_T>::at(const Key_T &search) const{
		std::pair<Node*, bool> result = skip_list_insert(search, NULL, true);
		if(result.first == nullptr){
			throw std::out_of_range("Key is not found in map");
		}

		return result.first->key;
	}
		
	//Map Operator[]	
	template<typename Key_T, typename Mapped_T>
	Mapped_T& Map<Key_T, Mapped_T>::operator[](const Key_T &search){
		std::pair<Node*, bool> result  = skip_list_insert(search, NULL, true);
		if(result.first == nullptr){
			Mapped_T mapped;
			skip_list_insert(search, mapped);
			return mapped;
		}
		else{
			return result.first->value;
		}
	}	
	
	//ValueType insert
	template<typename Key_T, typename Mapped_T>
	std::pair<typename Map<Key_T, Mapped_T>::Iterator, bool> Map<Key_T, Mapped_T>::insert(const Map<Key_T, Mapped_T>::ValueType &vt){
		std::pair<Node*, bool> insert_result = skip_list_insert(vt.first, vt.second);
		Iterator iter(insert_result.first);			
		return insert_result.second ? std::pair<Iterator, bool>(iter, true) : std::pair<Iterator, bool>(iter, false);
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
	
	//Iterator erase
	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::erase(Map<Key_T, Mapped_T>::Iterator pos){
		skip_list_erase(*pos.first);
	}

	//Key_T erase
	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::erase(const Key_T &search){
		bool result = skip_list_erase(search);
		if(!result){
			throw std::out_of_range("Key not found in map on erase attempt");
		}
	}

	//Clear
	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::clear(){
		auto it = begin();
		while(it != end()){
			Key_T to_del = *it.first;
			it++;
			skip_list_erase(to_del);
		}	
	}

	//Size
	template<typename Key_T, typename Mapped_T>
	size_t Map<Key_T, Mapped_T>::size() const{
		return current_size;
	}	

	//Empty
	template<typename Key_T, typename Mapped_T>
	bool Map<Key_T, Mapped_T>::empty() const {
		return current_size == 0;
	}

	//Comparison
	template<typename Key_T, typename Mapped_T>
	bool operator==(const Map<Key_T, Mapped_T> &lhs, const Map<Key_T, Mapped_T> &rhs){
		if(lhs.size() != rhs.size()){
			return false;
		}
		
		auto L = lhs.begin();
		auto R = rhs.begin();

		while(L != lhs.end()){
			if(*L != *R){
				return false;
			}
			++L;
			++R;
		}

		return true;
	}

	template<typename Key_T, typename Mapped_T>
	bool operator!=(const Map<Key_T, Mapped_T> &lhs, const Map<Key_T, Mapped_T> &rhs){
		return !(lhs == rhs);
	}

	template<typename Key_T, typename Mapped_T>
	bool operator<(const Map<Key_T, Mapped_T> &lhs, const Map<Key_T, Mapped_T> &rhs){
		if(lhs.size() > rhs.size()){
			return false;
		}
		
		auto L = lhs.begin();
		auto R = rhs.begin();

		while(L != lhs.end()){
			if(*L < *R){
				return true;
			}
			++L;
			++R;
		}
		
		if(lhs.size() < rhs.size()){
			return true;
		}

		return false;
	}
//Implement all comparisons between iterators
}
