#include <iterator>
#include <algorithm>
#include <stdexcept>

namespace dequeNS {

	template <class DequeType, class ElementType>
	class DequeIndexIterator :public std::iterator<std::random_access_iterator_tag, ElementType, unsigned int> {
	public:
		DequeIndexIterator(DequeType* deque, unsigned int initialIndex, bool reversed)
			:deque(deque), index(initialIndex), reversed(reversed) {}

		DequeIndexIterator<DequeType, ElementType>& operator ++() {
			++index;
			return *this;
		}

		const DequeIndexIterator<DequeType, ElementType> operator ++(int) {
			DequeIndexIterator<DequeType, ElementType> res = *this;
			++(*this);
			return res;
		}

		DequeIndexIterator<DequeType, ElementType>& operator --() {
			--index;
			return *this;
		}

		const DequeIndexIterator<DequeType, ElementType> operator --(int) {
			DequeIndexIterator<DequeType, ElementType> res = *this;
			--(*this);
			return res;
		}

		DequeIndexIterator<DequeType, ElementType>& operator +=(int n) {
			index += n;
			return *this;
		}
		DequeIndexIterator<DequeType, ElementType>& operator -=(int n) {
			index -= n;
			return *this;
		}

		const ElementType& operator *() const {
			return (*this)[0];
		}

		const ElementType& operator ->() const {
			return *(*this);
		}

		const ElementType& operator [](int x) const {
			if (reversed)
				return (*deque)[deque->size() - index - x - 1];
			return (*deque)[index + x];
		}

		template <class DstDeque, class DstElement>
		int distanceTo(const DequeIndexIterator<DstDeque, DstElement>& oth) const {
			if (oth.deque != deque)
				throw std::invalid_argument("Comparing iterators reference to different containers!");

			return oth.index - index;
		}

		template <class CmpDeque, class CmpElement>
		int compareTo(const DequeIndexIterator<CmpDeque, CmpElement>& oth) const {
			if (oth.deque != deque)
				throw std::invalid_argument("Comparing iterators reference to different containers!");

			return index < oth.index ? -1 : index > oth.index ? +1 : 0;
		}


	protected:
		DequeType* deque;
		int index;
		bool reversed;
	};

	template <class DequeType, class ElementType>
	class MutableDequeIndexIterator: public DequeIndexIterator<DequeType, ElementType> {
	public:
		MutableDequeIndexIterator(DequeType* deque, unsigned int initialIndex, bool reversed)
			:DequeIndexIterator<DequeType, ElementType>(deque, initialIndex, reversed) {}

		ElementType& operator [](int x) {
			if (DequeIndexIterator<DequeType, ElementType>::reversed)
				return (*DequeIndexIterator<DequeType, ElementType>::deque)
						[DequeIndexIterator<DequeType, ElementType>::deque->size()
						 	 - DequeIndexIterator<DequeType, ElementType>::index - x - 1];
			return (*DequeIndexIterator<DequeType, ElementType>::deque)
					[DequeIndexIterator<DequeType, ElementType>::index + x];
		}

		ElementType& operator *() {
			return (*this)[0];
		}

		ElementType& operator ->() {
			return *(*this);
		}

		const MutableDequeIndexIterator<DequeType, ElementType> operator ++(int) {
			MutableDequeIndexIterator<DequeType, ElementType> res = *this;
			++(*this);
			return res;
		}

		const MutableDequeIndexIterator<DequeType, ElementType> operator --(int) {
			MutableDequeIndexIterator<DequeType, ElementType> res = *this;
			--(*this);
			return res;
		}

		MutableDequeIndexIterator<DequeType, ElementType>& operator ++() {
			DequeIndexIterator<DequeType, ElementType>::operator++();
			return *this;
		}
		MutableDequeIndexIterator<DequeType, ElementType>& operator --() {
			DequeIndexIterator<DequeType, ElementType>::operator--();
			return *this;
		}

		MutableDequeIndexIterator<DequeType, ElementType>& operator +=(int n) {
			DequeIndexIterator<DequeType, ElementType>::operator+=(n);
			return *this;
		}
		MutableDequeIndexIterator<DequeType, ElementType>& operator -=(int n) {
			DequeIndexIterator<DequeType, ElementType>::operator-=(n);
			return *this;
		}
	};



	template <class DequeTypeA, class ElementTypeA, class DequeTypeB, class ElementTypeB>
	bool operator ==(const DequeIndexIterator<DequeTypeA, ElementTypeA>& a,
						const DequeIndexIterator<DequeTypeB, ElementTypeB>& b) {
		return a.compareTo(b) == 0;
	}

	template <class DequeTypeA, class ElementTypeA, class DequeTypeB, class ElementTypeB>
	bool operator !=(const DequeIndexIterator<DequeTypeA, ElementTypeA>& a,
						const DequeIndexIterator<DequeTypeB, ElementTypeB>& b) {
		return a.compareTo(b) != 0;
	}

	template <class DequeTypeA, class ElementTypeA, class DequeTypeB, class ElementTypeB>
	bool operator <(const DequeIndexIterator<DequeTypeA, ElementTypeA>& a,
						const DequeIndexIterator<DequeTypeB, ElementTypeB>& b) {
		return a.compareTo(b) < 0;
	}

	template <class DequeTypeA, class ElementTypeA, class DequeTypeB, class ElementTypeB>
	bool operator >(const DequeIndexIterator<DequeTypeA, ElementTypeA>& a,
						const DequeIndexIterator<DequeTypeB, ElementTypeB>& b) {
		return a.compareTo(b) > 0;
	}

	template <class DequeTypeA, class ElementTypeA, class DequeTypeB, class ElementTypeB>
	bool operator >=(const DequeIndexIterator<DequeTypeA, ElementTypeA>& a,
						const DequeIndexIterator<DequeTypeB, ElementTypeB>& b) {
		return a.compareTo(b) >= 0;
	}

	template <class DequeTypeA, class ElementTypeA, class DequeTypeB, class ElementTypeB>
	bool operator <=(const DequeIndexIterator<DequeTypeA, ElementTypeA>& a,
						const DequeIndexIterator<DequeTypeB, ElementTypeB>& b) {
		return a.compareTo(b) <= 0;
	}

	template <class DequeType, class ElementType>
	DequeIndexIterator<DequeType, ElementType> operator +(
			const DequeIndexIterator<DequeType, ElementType>& it, int a) {
		DequeIndexIterator<DequeType, ElementType> res = it;
		res += a;
		return res;
	}

	template <class DequeType, class ElementType>
	DequeIndexIterator<DequeType, ElementType> operator +(int a,
						const DequeIndexIterator<DequeType, ElementType>& it) {
		return it + a;
	}

	template <class DequeType, class ElementType>
	DequeIndexIterator<DequeType, ElementType> operator -(
			const DequeIndexIterator<DequeType, ElementType>& it, int a) {
		DequeIndexIterator<DequeType, ElementType> res = it;
		res -= a;
		return res;
	}

	template <class DequeTypeA, class ElementTypeA, class DequeTypeB, class ElementTypeB>
	int operator -(const DequeIndexIterator<DequeTypeA, ElementTypeA>& a,
						const DequeIndexIterator<DequeTypeB, ElementTypeB>& b) {
		return b.distanceTo(a);
	}

	template <class DequeType, class ElementType>
	MutableDequeIndexIterator<DequeType, ElementType> operator +(
			const MutableDequeIndexIterator<DequeType, ElementType>& it, int a) {
		MutableDequeIndexIterator<DequeType, ElementType> res = it;
		res += a;
		return res;
	}

	template <class DequeType, class ElementType>
	MutableDequeIndexIterator<DequeType, ElementType> operator +(int a,
						const MutableDequeIndexIterator<DequeType, ElementType>& it) {
		return it + a;
	}

	template <class DequeType, class ElementType>
	MutableDequeIndexIterator<DequeType, ElementType> operator -(
			const MutableDequeIndexIterator<DequeType, ElementType>& it, int a) {
		MutableDequeIndexIterator<DequeType, ElementType> res = it;
		res -= a;
		return res;
	}



	template <class Value>
	class deque {
	public:
		typedef MutableDequeIndexIterator<deque<Value>, Value> iterator;
		typedef DequeIndexIterator<const deque<Value>, Value> const_iterator;
		typedef iterator reverse_iterator;
		typedef const_iterator const_reverse_iterator;

		deque()
			:data(new Value[MIN_CAPACITY]), head(0), tail(0), count(0), capacity(MIN_CAPACITY) {}
		deque(const deque<Value>& from)
			:data(new Value[from.capacity]), head(0), tail(from.count), count(from.count), capacity(from.capacity) {
			copyFrom(from.data, from.head, from.tail, from.count);
		}

		void push_back(const Value& v) {
			increaseCapacity();
			data[tail] = v;
			tail = (tail + 1) % capacity;
			++count;
		}

		const Value pop_back() {
			tail = (tail + capacity - 1) % capacity;
			Value res = data[tail];
			--count;
			decreaseCapacity();
			return res;
		}

		void push_front(const Value& v) {
			increaseCapacity();
			head = (head + capacity - 1) % capacity;
			data[head] = v;
			++count;
		}

		const Value pop_front() {
			Value res = data[head];
			head = (head + 1) % capacity;
			--count;
			decreaseCapacity();
			return res;
		}

		Value& operator[](unsigned int index) {
			return data[(head + index) % capacity];
		}
		const Value& operator[](unsigned int index) const {
			return data[(head + index) % capacity];
		}

		Value& front() {
			return data[head];
		}
		const Value& front() const {
			return data[head];
		}

		Value& back() {
			return (*this)[count - 1];
		}
		const Value& back() const {
			return (*this)[count - 1];
		}

		bool empty() const {
			return count == 0;
		}
		unsigned int size() const {
			return count;
		}

		iterator begin() {
			return iterator(this, 0, false);
		}
		iterator end() {
			return iterator(this, count, false);
		}

		const_iterator begin() const {
			return const_iterator(this, 0, false);
		}
		const_iterator end() const {
			return const_iterator(this, count, false);
		}

		const_iterator cbegin() const {
			return const_iterator(this, 0, false);
		}
		const_iterator cend() const {
			return const_iterator(this, count, false);
		}

		reverse_iterator rbegin() {
			return reverse_iterator(this, 0, true);
		}
		reverse_iterator rend() {
			return reverse_iterator(this, count, true);
		}

		const_reverse_iterator rbegin() const {
			return const_reverse_iterator(this, 0, true);
		}
		const_reverse_iterator rend() const {
			return const_reverse_iterator(this, count, true);
		}

		const_reverse_iterator crbegin() const {
			return const_reverse_iterator(this, 0, true);
		}
		const_reverse_iterator crend() const {
			return const_reverse_iterator(this, count, true);
		}

		~deque() {
			delete[] data;
			data = nullptr;
		}

	private:
		static const unsigned int MIN_CAPACITY = 16;

		Value* data;
		unsigned int head, tail;
		unsigned int count;
		unsigned int capacity;


		void increaseCapacity() {
			if (count == capacity)
				changeCapacity(capacity * 2);
		}
		void decreaseCapacity() {
			if (count * 4 <= capacity)
				changeCapacity(count * 2);

		}

		void changeCapacity(unsigned int newCapacity) {
			if (newCapacity < MIN_CAPACITY)
				newCapacity = MIN_CAPACITY;
			if (capacity == newCapacity)
				return;

			Value* oldData = data;
			data = new Value[newCapacity];
			capacity = newCapacity;

			copyFrom(oldData, head, tail, count);
			delete[] oldData;

			head = 0;
			tail = count;
		}

		void copyFrom(const Value* const from, unsigned int head, unsigned int tail, unsigned int size) {
			if (head < tail) {
				std::copy(from + head, from + tail, data);
			} else {
				std::copy(from + head, from + size, data);
				std::copy(from, from + tail, data + (size - head));
			}
		}
	};

}


using dequeNS::deque;
