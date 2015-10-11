#include <iostream>
#include <utility>

template<typename Key_T, typename Mapped_T> class Map{
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

//Comparison
bool operator==(const Map &, const Map &);
bool operator!=(const Map &, const Map &);
bool operator<(const Map &, const Map &);
//Implement all comparisons between iterators











