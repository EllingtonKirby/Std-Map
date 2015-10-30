#include <iostream>
#include <utility>
#include <random>
#include <limits.h>
#include <memory>
#include <array>
#include <stdexcept>
#include <vector>

namespace cs540{
	template<typename Key_T, typename Mapped_T> class Map{
		public:
			class Node;
		       	class Base_Node;
			class Link{
				public:
					friend class Map;
					Base_Node *right;
				       	Base_Node *left;
					Link(Base_Node *nb) : right(nb), left(nb){}
			};	
		private:
			int max_height;
			int current_size;
			static int rheight();
			Base_Node head;
			Base_Node tail;
			int curr_height;
			std::pair<Node*, bool> skip_list_insert(Key_T key, Mapped_T val, bool searchFlag);
			bool skip_list_erase(const Key_T key);
		public:	
			//Member Type
			typedef std::pair<const Key_T, Mapped_T> ValueType;
			
			class Iterator;
			class ConstIterator;
			class ReverseIterator;
			class Base_Node{
				public:
					friend class Map;
					int height;
					std::vector<Link> next;
					Base_Node(int applied_height = rheight()) : height(applied_height), next(applied_height, Link(nullptr)){};
			};
			class Node : public Base_Node{
				public:
					friend class Map;
					Key_T key;
					Mapped_T value;
					ValueType vtype;
					int width;
					Node(Key_T &keyed, Mapped_T &valued, int applied_height = rheight()) : Base_Node(applied_height), vtype(keyed, valued){
						value = vtype.second;
						key = vtype.first;
					};
					Node(const Node& rhs) : Base_Node(rhs.height){
						this->vtype = rhs.vtype;
						this->key = vtype.first;
						this->value = vtype.second;
					};
					~Node(){

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
					friend class Map;
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
					ConstIterator(const Base_Node* pos);
					std::shared_ptr<Base_Node> target;
				public:
					friend class Map;
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
					ReverseIterator(Base_Node* pos);
					std::shared_ptr<Base_Node> target;
				public:
					friend class Map;
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
	Map<Key_T, Mapped_T>::Map() : max_height(32), head(32), tail(32){
		//Lets steal his implementation... 
		//Key_T sent_key;
		//Mapped_T sent_map;
		//Node * head = new Node(sent_key, sent_map, max_height);
		//Node * tail = new Node(sent_key, sent_map, max_height);
		for(int i = 0; i < max_height; i++){
			head.next[i].right = &tail;
			tail.next[i].left = &head;
		}
		current_size = 0;
	}
	//Map Copy Constructor
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::Map(const Map &rhs) : max_height(32), head(32), tail(32){
		for(int i = 0; i < max_height; i++){
			head.next[i].right = &tail;
			tail.next[i].left = &head;
		}
		current_size = 0;
		std::array<Base_Node*, 32> last;
		last.fill(&head);
		
		for(Node *n = rhs.head.next[0]; n != rhs.tail; n = n->next[0]){
			Node *nn = new Node(*n);
			for(int i = 0; i < nn->height; i++){
				//if(i == 0){
				//	nn->prev = last[i];
				//}
				//insert nn into corresponding row for each height it is a part of
				//insertSpecificNode(i, last.at(i), nn);
				//Is this all we need to do?
				nn->next[i].left = last.at(i);
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
		std::array<Base_Node*, max_height> last;
		last.fill(&head);
		
		for(Node *n = rhs.head.next[0]; n != rhs.tail; n = n->next[0]){
			Node *nn = new Node(*n);
			for(int i = 0; i < nn->height; i++){
				//if(i == 0){
				//	nn->prev = last[i];
				//}
				//insert nn into corresponding row for each height it is a part of
				//insertSpecificNode(i, last.at(i), nn);
				//Is this all we need to do?
				nn->next[i].left = last.at(i);
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
		if(current_size > 0){
			Base_Node * it = tail.next[0].left;
			while(it->next[0].left != nullptr){
				it = it->next[0].left;
				delete it->next[0].right;
			}
		}
		//delete head;
		//delete tail;
	}

	//Iterator constructor
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::Iterator::Iterator(Map<Key_T, Mapped_T>::Node* pos) : target(std::shared_ptr<Node>(pos)){}
	
	//Iterator ++ operator returns reference post increment
	//the ampersand at the end takes care of the return by reference 
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator& Map<Key_T, Mapped_T>::Iterator::operator++(){
		this->target = target->next[0].right;
		return *this;
	}
	
	//Iterator ++ operator returns iterator preincrement
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::Iterator::operator++(int){
		Iterator retval = *this;
		this->target = target->next[0].right;
		return retval;
	}
	
	//Iterator -- operator returns reference postdecremet
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator& Map<Key_T, Mapped_T>::Iterator::operator--(){
		this->target = target->next[0].left;
		return *this;
	}
	
	//Iterator -- operator returns iterator predecrement
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::Iterator::operator--(int){
		Iterator retval = *this;
		this->target = target->next[0].left;
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
	Map<Key_T, Mapped_T>::ConstIterator::ConstIterator(const Map<Key_T, Mapped_T>::Base_Node* pos){
		this->target = std::make_shared<Base_Node>(pos);
	}

	//ReverseIterator constructor
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::ReverseIterator::ReverseIterator(Map<Key_T, Mapped_T>::Base_Node* pos){
		this->target = std::make_shared<Base_Node>(pos);
	}

	//Iterator begin()
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::begin(){
		auto it = new Iterator(head.next[0].right);
		return it;
	}
	
	//Iterator end()
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::end(){
		auto it = new Iterator(&tail);
		return it;
	}
	
	//ConstIterator begin()
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::begin() const{
		auto it = new ConstIterator(head.next[0].right);
		return it;
	}
	
	//ConstIterator end()
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::end() const{
		auto it = new ConstIterator(&tail);
		return it;
	}
	
	//ReverseIterator rbegin()
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::rbegin(){
		auto it = new ReverseIterator(tail.next[0].left);
		return it;
	}
	
	//ReverseIterator rend()
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::rend(){
		auto it = new ReverseIterator(&head);
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

		if(h >= 32){
			h = h % 32;
		}
		return h;
	}

	//All credit to Eternally Confuzzled at eternallyconfuzzled.com/tuts/datastructures/jsw_tut_skip.aspx
	//Skip List insert
	template<typename Key_T, typename Mapped_T>
	std::pair<typename Map<Key_T, Mapped_T>::Node*, bool> Map<Key_T, Mapped_T>::skip_list_insert(Key_T key, Mapped_T value, bool searchFlag = false){
		Node *it = static_cast<Node*>(&head);
		Node *item  = new Node(key, value);
		std::vector<Base_Node*> fix(max_height, nullptr);

		for(int i = curr_height - 1; i >= 0; i--){	
			while(it->next[i].right != nullptr){
				Node* right = static_cast<Node*>(it->next[i].right);
				if(right->key > key){
					break;
				}
				it = right;
			}
			fix[i] = it;
		}
		//Check if key already is in map
		if(it->next[0].right != nullptr && static_cast<Node*>(it->next[0].right)->key == key){
			delete item;
			return std::pair<Node*, bool>(static_cast<Node*>(it->next[0].right), false);
		}
		else if(searchFlag){
			//Key is not in map, but we are just here to search
			delete item;
			return std::pair<Node*, bool>(nullptr, false);
		}
		else{
			//key is not in map but we are here to insert
			while(item->height > curr_height){
				fix[curr_height++] = &head;
			}
			
			int h = item->height;
			while(--h >= 0){
				item->next[h].right = fix[h]->next[h].right->next[h].right;
				fix[h]->next[h].right->next[h].right = item;
				item->next[h].left = fix[h]->next[h].right;
			}
			item->next[0].right->next[0].left = item;
			current_size++;
		}
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
			while(it->next[i].right != nullptr && it->next[i].right->key < key){
				it = it->next[i].right;
			}
			fix[i] = it;
		}
		if(it->next[0].right == nullptr || it->next[0].right->key != key){
			return false;
		}
		else{
			to_del = fix[0]->next[0].right;
			for(int i = 0; i < curr_height; i++){
				if(fix[i]->next[i].right != nullptr){
					fix[i]->next[i].right = fix[i]->next[i].right->next[i].right;
					fix[i]->next[i].right->next[i].left = fix[i];
				}
			}
			while(curr_height > 0){
				if(head.next[curr_height-1].right != nullptr){
					break;
				}
				head.next[--curr_height].right = nullptr;
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
