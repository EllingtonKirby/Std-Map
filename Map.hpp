#include <iostream>
#include <utility>
#include <random>
#include <limits.h>

namespace cs540{
	template<typename Key_T, typename Mapped_T> class Map{
		public:
			struct Node;
		private:
			int max_height;
			int rheight();
			Node * head;
			Node * tail;
			int curr_height;
			std::pair<Node*, bool> skip_list_insert(Key_T key, Mapped_T val, bool searchFlag);
		public:
			class Iterator;
			class ConstIterator;
			class ReverseIterator;
			struct Node{
				int height;
				Key_T key;
				Mapped_T value;
				Node **next;
				Node *prev;
				int width;
				Node(Key_T &keyed, Mapped_T &valued, int applied_height = rheight()){
					key = keyed;
					value = valued;
					height = applied_height;
					next = new Node*[max_height];
					for(int i = 0; i < max_height; i++){
						this->next[i] = nullptr;
					}
					prev = nullptr;
				};
				~Node(){
					delete[] next;
				}
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
				public:
					Node * target;
			};
			
			class Iterator: public Base_Iterator{	
				public:
					Iterator(const Iterator &);
					~Iterator();
					Iterator& operator=(const Iterator &);
					Iterator& operator++();
					Iterator& operator++(int);
					Iterator& operator--();
					Iterator& operator--(int);
					ValueType *operator->() const;
					ValueType &operator*() const;
			};

			class ConstIterator: public Base_Iterator{
				public:
					ConstIterator(const Iterator &);
					~ConstIterator();
					ConstIterator& operator=(const Iterator &);
					ConstIterator& operator++();
					ConstIterator& operator++(int);
					ConstIterator& operator--();
					ConstIterator& operator--(int);
					const ValueType *operator->() const;
					const ValueType &operator*() const;
			};

			class ReverseIterator: public Base_Iterator{
				public:
					ReverseIterator(const Iterator &);
					~ReverseIterator();
					ReverseIterator& operator=(const Iterator &);
					ReverseIterator& operator++();
					ReverseIterator& operator++(int);
					ReverseIterator& operator--();
					ReverseIterator& operator--(int);
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
		//What's next?
	}

	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::Map(const Map &rhs){
		if(this == rhs){

		}

	}

	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>& Map<Key_T, Mapped_T>::operator=(const Map&){}

	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::Map(std::initializer_list<std::pair<const Key_T, Mapped_T>>){}
	
	template<typename Key_T, typename Mapped_T>
	Map<Key_T, Mapped_T>::~Map(){}

	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::begin(){}
	
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::end(){}
	
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::begin() const{}
	
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::end() const{}
	
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::rbegin(){}
	
	template<typename Key_T, typename Mapped_T>
	typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::rend(){}
	
	template<typename Key_T, typename Mapped_T>
	//Function that determines the random height of a column. All credit to Eternally Confuzzled at eternallyconfuzzled.com/tuts/datastructures/jsw_tut_skip.aspx
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
		}
		delete[] fix;
		return std::pair<Node*, bool>(&item, true);
	}			
}
//Comparison
//bool operator==(const Map &, const Map &);
//bool operator!=(const Map &, const Map &);
//bool operator<(const Map &, const Map &);
//Implement all comparisons between iterators

