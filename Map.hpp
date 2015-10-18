#include <iostream>
#include <utility>
#include <random>

namespace cs540{
template<typename Key_T, typename Mapped_T> class Map{
	private:
		int max_height;
		int rheight();
		struct Node{
			int height;
			Key_T key;
			Mapped_T value;
			Node **next;
			Node::Node(Key_T keyed, Mapped_T valued, int applied_height = rheight()){
				key = keyed;
				value = valued;
				height = applied_height;
				Node **next = new Node[max_height];
			};
			Node::~Node(){
				delete[] next;
			}
		};
		Node * head;
		int curr_height;
		std::pair<Key_T, Mapped_T> search(Key_T key);	
		int skip_list_insert(Key_T key, Mapped_T val);
	public:
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
				Iterator(const Iterator &);
				~Iterator();
				Iterator& operator=(const Iterator &);
				Iterator& operator++();
				Iterator& operator++(int);
				Iterator& operator--();
				Iterator& operator--(int);
				
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
}
//Comparison
bool operator==(const Map &, const Map &);
bool operator!=(const Map &, const Map &);
bool operator<(const Map &, const Map &);
//Implement all comparisons between iterators

Map::Map(){
	//Need to construct sentinel Nodes head and tail. All next in tail will point to null, head will point to tail at first
	int max_height = 16; //TODO find out what to do with max height
	Node * head = new Node(NULL, NULL, max_height);
	Node * tail = new Node(NULL, NULL, max_height);
	for(int i = 0; i < max_height; i++){
		head->next[i] = tail;
		tail->next[i] = NULL;
	}

	//What's next?
}
//Function that determines the random height of a column. All credit to Eternally Confuzzled at eternallyconfuzzled.com/tuts/datastructures/jsw_tut_skip.aspx
int Map::rheight(){
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

std::pair<Key_T, Mapped_T> Map::search(Key_T key){
	Node * it = this->head;
	int i;

	for(i = this->curr_height - 1; i >= 0; i--){
		while(it->next[i] != NULL && next[i]->key < key){
			it = it->next[i];
		}
	}

	return it->next[0] ? pair<Key_T, Mapped_T>(it->next[0]->key, it->next[0]->value) : NULL;
}

int Map::skip_list_insert(Key_T key, Mapped_T value){
	Node *it = this->head;
	Node *item  = new Node(key, value);
	Node **fix = new Node[max_height];
	int i;

	for(i = curr_height - 1; i >= 0; i--){
		while(it->next[i] != NULL && it->next[i]->key < key){
			it = it->next[i];
		}
		fix[i] = it;
	}

	if(it->next[0] != NULL && it->next[0]->key == key){
		return 0;
	}
	else{
		int h;
		while(item->height > curr_height){
			fix[curr_height++] = this->head;
		}
		
		h = item->height;
		while(--h >= 0){
			item->next[h] = fix[h]->next[h];
			fix[h]->next[h] = item;
		}
	}
	delete[] fix;
	return 1;
}
		


