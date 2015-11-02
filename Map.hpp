#ifndef XXX_MAP_XXX
#define XXX_MAP_XXX
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
			bool skip_list_erase(const Key_T);
			Node* skip_list_find(const Key_T &);
			const Node* skip_list_find(const Key_T &) const;
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
					ValueType vtype;
					Key_T key;
					Mapped_T value;
					Node(Key_T &keyed, Mapped_T &valued, int applied_height = rheight()) : Base_Node(applied_height), vtype(keyed, valued){
						value = vtype.second;
						key = vtype.first;
					};
					Node(const Node& rhs) : Base_Node(rhs.height), vtype(rhs.vtype), key(vtype.first), value(vtype.second){};
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
			ConstIterator find(const Key_T&) const;
			Mapped_T &at(const Key_T &);
			const Mapped_T &at(const Key_T &) const;
			Mapped_T &operator[](const Key_T &);
			//Modifiers
			std::pair<Iterator, bool> insert(const ValueType &);
			template<typename IT_T> 
			void insert(IT_T range_beg, IT_T range_end){
				IT_T iter = range_beg;
				while(iter != range_end){
					insert(*iter);
					iter++;
				}	
			};
			void erase(Iterator pos);
			void erase(const Key_T &);
			void clear();
			int current_height() const{return curr_height;};
			//Extra Credit
			Node* fingerSearch(const Key_T &);
			//Comparison
			friend bool operator<(const Map &lhs, const Map &rhs){
				if(lhs.size() > rhs.size()){
					return false;
				}
				ConstIterator L = lhs.begin();
				ConstIterator R = rhs.begin();
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
			};
			friend bool operator!=(const Map &lhs, const Map &rhs){
				return !(lhs == rhs);
			};
			friend bool operator==(const Map &lhs, const Map &rhs){
				if(lhs.size() != rhs.size()){
					return false;
				}
				else{
					ConstIterator L = lhs.begin();
					for(ConstIterator R = rhs.begin(); R != rhs.end(); R++){
						if(*L != *R){
							return false;
						}
						L++;
					}
					return true;
				}
			};
			class Iterator{	
				private:
					Iterator(Base_Node* pos) : target(pos){};
					Base_Node* target;
				public:
					friend class Map;
					friend class ConstIterator;
					friend bool operator==(const Iterator &lhs, const Iterator &rhs){
						return (lhs.target == rhs.target);
					};
					friend bool operator!=(const Iterator &lhs, const Iterator &rhs){
						return !(lhs == rhs);
					};
					Iterator(const Iterator &iter) : target(iter.target){};
					Iterator& operator++(){			
						target = target->next[0].right;
						return *this;
					};
					Iterator operator++(int){
						Iterator retval(*this);
						target = target->next[0].right;
						return retval;	
					};
					Iterator& operator--(){
						target = target->next[0].left;
						return *this;	
					};
					Iterator operator--(int){
						Iterator retval(*this);
						target = target->next[0].left;
						return retval;
					};
					ValueType *operator->() const{
						ValueType *retval = &static_cast<Node*>(target)->vtype;	
						return retval;
					};
					ValueType &operator*() const{
						return static_cast<Node*>(target)->vtype;	
					};
					friend bool operator==(const Iterator &rhs, const ConstIterator &lhs){
						return rhs.target == lhs.target;			
					};
					friend bool operator==(const ConstIterator &rhs, const Iterator &lhs){
						return rhs.target == lhs.target;
					};
					friend bool operator!=(const Iterator &rhs, const ConstIterator &lhs){
						return !(rhs == lhs); 
					};
					friend bool operator!=(const ConstIterator &rhs, const Iterator &lhs){
						return !(rhs == lhs);
					};
					
			};

			class ConstIterator{
				private:
					ConstIterator(const Base_Node* pos) : target(pos){};
					const Base_Node* target;
				public:
					friend class Map;
					friend class Iterator;
					friend bool operator==(const ConstIterator &lhs, const ConstIterator &rhs){
						return (lhs.target == rhs.target);
					};
					friend bool operator!=(const ConstIterator &lhs, const ConstIterator &rhs){
						return !(lhs == rhs);
					};
					ConstIterator(const Iterator &iter) : target(iter.target){};
					ConstIterator& operator++(){
						target = target->next[0].right;
						return *this;			
					};
					ConstIterator operator++(int){
						ConstIterator retval(*this);
						target = target->next[0].right;	
						return retval;
					};
					ConstIterator& operator--(){
						target = target->next[0].left;
						return *this;	
					};
					ConstIterator operator--(int){
						ConstIterator retval(*this);
						target = target->next[0].left;
						return retval;

					};
					const ValueType *operator->() const{	
						return &static_cast<const Node*>(target)->vtype;	
					};
					const ValueType &operator*() const{
						return static_cast<const Node*>(target)->vtype;	
					};
			};

			class ReverseIterator{
				private:
					ReverseIterator(Base_Node* pos) : target(pos){};
					Base_Node* target;
				public:
					friend class Map;
					friend bool operator==(const ReverseIterator &lhs, const ReverseIterator &rhs){
						return (lhs.target == rhs.target);
					};
					friend bool operator!=(const ReverseIterator &lhs, const ReverseIterator &rhs){
						return !(lhs == rhs);
					};
					ReverseIterator(const ReverseIterator &iter) : target(iter.target){};
					ReverseIterator& operator++(){
						target = target->next[0].left;
						return *this;	
					};
					ReverseIterator operator++(int){
						ReverseIterator retval(*this);
						target = target->next[0].left;
						return retval;	
					};
					ReverseIterator& operator--(){
						target = target->next[0].right;
						return *this;	
					};
					ReverseIterator operator--(int){
						ReverseIterator retval(*this);
						target = target->next[0].right;
						return retval;	
					};
					ValueType *operator->() const{
						return &static_cast<Node*>(target)->vtype;	
					};
					ValueType &operator*() const{
						return static_cast<Node*>(target)->vtype;	
					};
			};


	};
	//Map Default Constructor	
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::Map() : max_height(32), head(32), tail(32){
		for(int i = 0; i < max_height; i++){
			head.next[i].right = &tail;
			tail.next[i].left = &head;
		}
		current_size = 0;
		curr_height = 1;
	}
	//Map Copy Constructor
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::Map(const Map &rhs) : max_height(32), head(32), tail(32){
		for(int i = 0; i < max_height; i++){
			head.next[i].right = &tail;
			tail.next[i].left = &head;
		}
		current_size = 0;
		this->curr_height = rhs.current_height();
		std::array<Base_Node*, 32> last;
		last.fill(&head);
		
		for(Base_Node *n = rhs.head.next[0].right; n != &rhs.tail; n = n->next[0].right){
			Node *casted = static_cast<Node*>(n);
			Node *nn = new Node(*casted);
			for(int i = 0; i < nn->height; i++){
				nn->next[i].left = last.at(i);
				nn->next[i].right = last.at(i)->next[i].right;
				last.at(i)->next[i].right = nn;
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
		clear();
		this->curr_height = rhs.current_height();
		this->current_size = rhs.size();
		std::array<Base_Node*, 32> last;
		last.fill(&head);
		
		for(Base_Node *n = rhs.head.next[0].right; n != &rhs.tail; n = n->next[0].right){
			Node *casted = static_cast<Node*>(n);
			Node *nn = new Node(*casted);
			for(int i = 0; i < nn->height; i++){
				nn->next[i].left = last.at(i);
				nn->next[i].right = last.at(i)->next[i].right;
				last.at(i)->next[i].right = nn;
				last.at(i) = nn;
			}
		}
		return *this;
	}

	//Map initializer list construction
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::Map(std::initializer_list<std::pair<const Key_T, Mapped_T>> list) : max_height(32), head(32), tail(32) {
		for(int i = 0; i < max_height; i++){
			head.next[i].right = &tail;
			tail.next[i].left = &head;
		}
		current_size = 0;
		curr_height = 1;
		
		insert(list.begin(), list.end());
	}
	
	//Map destructor
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::~Map(){
		if(current_size > 0){
			clear();
		}
	}

	//Iterator begin()
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::begin(){
		return Iterator(head.next[0].right);
	}
	
	//Iterator end()
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::end(){
		return Iterator(&tail);;
	}
	
	//ConstIterator begin()
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::begin() const{
		return ConstIterator(head.next[0].right);
	}
	
	//ConstIterator end()
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::end() const{
		return ConstIterator(&tail);
	}
	
	//ReverseIterator rbegin()
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::rbegin(){
		return ReverseIterator(tail.next[0].left);
	}
	
	//ReverseIterator rend()
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::rend(){
		return ReverseIterator(&head);
	}
	
	//Function that determines the random height of a column. All credit to Eternally Confuzzled at eternallyconfuzzled.com/tuts/datastructures/jsw_tut_skip.aspx
	template<typename Key_T, typename Mapped_T>
	int Map<Key_T, Mapped_T>::rheight(){
		static int bits = 0;
		static int reset = 0;
		int h, found = 0;
		static std::random_device rd;
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
		Base_Node* it = &head;
		Node *item  = new Node(key, value);
		std::vector<Base_Node*> fix(max_height, &head);

		for(int i = curr_height - 1; i >= 0; i--){	
			while(it->next[i].right != &tail && static_cast<Node*>(it->next[i].right)->key < key){
				it = it->next[i].right;
			}
			fix[i] = it;
		}
		//Check if key already is in map
		if(it->next[0].right != &tail && static_cast<Node*>(it->next[0].right)->key == key){
			delete item;
			return std::pair<Node*, bool>(static_cast<Node*>(it->next[0].right), false);
		}
		else{
			//key is not in map, insert
			while(item->height > curr_height){
				fix[curr_height++] = &head;
			}
			
			int h = item->height;
			while(--h >= 0){
				item->next[h].right = fix[h]->next[h].right;
				item->next[h].left = fix[h]->next[h].right->next[h].left;
				fix[h]->next[h].right->next[h].left = item;
				fix[h]->next[h].right = item;
			}
			current_size++;
		}
		return std::pair<Node*, bool>(item, true);
	}			
	//skip list find
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Node* Map<Key_T, Mapped_T>::skip_list_find(const Key_T &search){
		Base_Node* it = &head;
		for(int i = curr_height - 1; i >= 0; i--){	
			while(it->next[i].right != &tail && static_cast<Node*>(it->next[i].right)->key < search){
				it = it->next[i].right;
			}
		}
		//Check if key already is in map
		if(it->next[0].right != &tail && static_cast<Node*>(it->next[0].right)->key == search){
			return static_cast<Node*>(it->next[0].right);
		}
		else{
			//Key is not in map
			return nullptr;
		}
	}

	//const skip list find
	template<typename Key_T, typename Mapped_T>
	const typename Map<Key_T, Mapped_T>::Node* Map<Key_T, Mapped_T>::skip_list_find(const Key_T &search) const{
		const Base_Node* it = &head;
		for(int i = curr_height - 1; i >= 0; i--){	
			while(it->next[i].right != &tail && static_cast<Node*>(it->next[i].right)->key < search){
				it = it->next[i].right;
			}
		}
		//Check if key already is in map
		if(it->next[0].right != nullptr && static_cast<Node*>(it->next[0].right)->key == search){
			return static_cast<Node*>(it->next[0].right);
		}
		else{
			//Key is not in map
			return nullptr;
		}
	}

	//iterator find
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::find(const Key_T &search){
		Node* result = skip_list_find(search);
		if(result == nullptr){
			return end();
		}
		return Iterator(result);
	}		

	//ConstIterator find
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::find(const Key_T &search) const{
		const Node * result = skip_list_find(search);
		if(result == nullptr){
			ConstIterator retval = end();
			return retval;
		}
		return ConstIterator(result);
	}
	//At, non const
	template<typename Key_T, typename Mapped_T>
	Mapped_T& Map<Key_T, Mapped_T>::at(const Key_T &search){
		Node* result = skip_list_find(search);
		if(result == nullptr){
			throw std::out_of_range("Key is not found in map");
		}

		return result->value;
	}
	
	//At, const
	template<typename Key_T, typename Mapped_T>	
	const Mapped_T& Map<Key_T, Mapped_T>::at(const Key_T &search) const{
		const Node* result = skip_list_find(search);
		if(result == nullptr){
			throw std::out_of_range("Key is not found in map");
		}

		return result->value;
	}
		
	//Map Operator[]	
	template<typename Key_T, typename Mapped_T>
	Mapped_T& Map<Key_T, Mapped_T>::operator[](const Key_T &search){
		Node* result  = skip_list_find(search);
		if(result == nullptr){
			skip_list_insert(search, Mapped_T());
			return skip_list_find(search)->vtype.second;
		}
		return result->vtype.second;
	}	
	
	//ValueType insert
	template<typename Key_T, typename Mapped_T>
	std::pair<typename Map<Key_T, Mapped_T>::Iterator, bool> Map<Key_T, Mapped_T>::insert(const Map<Key_T, Mapped_T>::ValueType &vt){
		std::pair<Node*, bool> insert_result = skip_list_insert(vt.first, vt.second);
		Iterator iter(insert_result.first);
		std::pair<Iterator, bool> retval = std::make_pair(iter, insert_result.second);		
		return retval;
	}

	//All credit to Eternally Confuzzled at eternallyconfuzzled.com/tuts/datastructures/jsw_tut_skip.aspx
	//Skip List erase
	template<typename Key_T, typename Mapped_T>
	bool Map<Key_T, Mapped_T>::skip_list_erase(const Key_T key){
		Node* to_del;
		Base_Node* it = &head;
		std::vector<Base_Node*> fix(max_height, nullptr);

		for(int i = curr_height - 1; i >= 0; i--){
			while(it->next[i].right != &tail && static_cast<Node*>(it->next[i].right)->key < key){
				it = it->next[i].right;
			}
			fix[i] = it;
		}
		if(it->next[0].right == &tail || static_cast<Node*>(it->next[0].right)->key != key){
			return false;
		}
		else{
			it = it->next[0].right;
			to_del = static_cast<Node*>(it);
			for(int i = 0; i < curr_height; i++){
				if(fix[i]->next[i].right != it){
					break;
				}
				fix[i]->next[i].right = it->next[i].right;
				it->next[i].right->next[i].left = fix[i];
			}
			while(curr_height > 0){
				if(head.next[curr_height-1].right != &tail){
					break;
				}
				head.next[--curr_height].right = &tail;
				tail.next[curr_height].left = &head;
			}
		}
		current_size--;
		delete to_del;
		return true;
	}
	
	//Iterator erase
	template<typename Key_T, typename Mapped_T>
	void Map<Key_T, Mapped_T>::erase(Map<Key_T, Mapped_T>::Iterator pos){
		skip_list_erase(pos->first);
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
		while(begin().target != &tail){
			skip_list_erase(begin()->first);
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
}
#endif
