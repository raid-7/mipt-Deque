// https://github.com/raid-7/mipt-Deque

#include <iterator>
#include <algorithm>
#include <stdexcept>

namespace dequeNS {

	template <class DequeType, class ElementType, bool reversed>
	class DequeIndexIterator :public std::iterator<std::random_access_iterator_tag, ElementType, unsigned int> {
	public:
		DequeIndexIterator(DequeType* deque, unsigned int initialIndex)
			:deque(deque), index(initialIndex) {
		}

		DequeIndexIterator<DequeType, ElementType, reversed>& operator ++() {
			++index;
			return *this;
		}

		const DequeIndexIterator<DequeType, ElementType, reversed> operator ++(int) {
			DequeIndexIterator<DequeType, ElementType, reversed> res = *this;
			++(*this);
			return res;
		}

		DequeIndexIterator<DequeType, ElementType, reversed>& operator --() {
			--index;
			return *this;
		}

		const DequeIndexIterator<DequeType, ElementType, reversed> operator --(int) {
			DequeIndexIterator<DequeType, ElementType, reversed> res = *this;
			--(*this);
			return res;
		}

		DequeIndexIterator<DequeType, ElementType, reversed>& operator +=(int n) {
			index += n;
			return *this;
		}
		DequeIndexIterator<DequeType, ElementType, reversed>& operator -=(int n) {
			index -= n;
			return *this;
		}

		ElementType& operator *() const {
			return (*this)[0];
		}

		ElementType* operator ->() const {
			return &(*(*this));
		}

		ElementType& operator [](int x) const {
			if (reversed)
				return (*deque)[deque->size() - index - x - 1];
			return (*deque)[index + x];
		}

		template <class DstDeque, class DstElement>
		int distanceTo(const DequeIndexIterator<DstDeque, DstElement, reversed>& oth) const {
			if (oth.deque != deque)
				throw std::invalid_argument("Comparing iterators reference to different containers!");

			return oth.index - index;
		}

		template <class CmpDeque, class CmpElement, bool CmpReversed>
		int compareTo(const DequeIndexIterator<CmpDeque, CmpElement, CmpReversed>& oth) const {
			if (oth.deque != deque)
				throw std::invalid_argument("Comparing iterators reference to different containers!");

			if (reversed == CmpReversed)
				return index < oth.index ? -1 : index > oth.index ? +1 : 0;

			int indexA = reversed ? deque->size() - index : index;
			int indexB = CmpReversed ? oth.deque->size() - oth.index : oth.index;
			return indexA < indexB ? -1 : indexA > indexB ? +1 : 0;
		}


	protected:
		DequeType* deque;
		int index;
	};


	template <class DequeA, class ElementA, bool ReversedA, class DequeB, class ElementB, bool ReversedB>
	bool operator ==(const DequeIndexIterator<DequeA, ElementA, ReversedA>& a,
						const DequeIndexIterator<DequeB, ElementB, ReversedB>& b) {
		return a.compareTo(b) == 0;
	}

	template <class DequeA, class ElementA, bool ReversedA, class DequeB, class ElementB, bool ReversedB>
	bool operator !=(const DequeIndexIterator<DequeA, ElementA, ReversedA>& a,
			const DequeIndexIterator<DequeB, ElementB, ReversedB>& b) {
		return a.compareTo(b) != 0;
	}

	template <class DequeA, class ElementA, bool ReversedA, class DequeB, class ElementB, bool ReversedB>
	bool operator <(const DequeIndexIterator<DequeA, ElementA, ReversedA>& a,
			const DequeIndexIterator<DequeB, ElementB, ReversedB>& b) {
		return a.compareTo(b) < 0;
	}

	template <class DequeA, class ElementA, bool ReversedA, class DequeB, class ElementB, bool ReversedB>
	bool operator >(const DequeIndexIterator<DequeA, ElementA, ReversedA>& a,
			const DequeIndexIterator<DequeB, ElementB, ReversedB>& b) {
		return a.compareTo(b) > 0;
	}

	template <class DequeA, class ElementA, bool ReversedA, class DequeB, class ElementB, bool ReversedB>
	bool operator >=(const DequeIndexIterator<DequeA, ElementA, ReversedA>& a,
			const DequeIndexIterator<DequeB, ElementB, ReversedB>& b) {
		return a.compareTo(b) >= 0;
	}

	template <class DequeA, class ElementA, bool ReversedA, class DequeB, class ElementB, bool ReversedB>
	bool operator <=(const DequeIndexIterator<DequeA, ElementA, ReversedA>& a,
			const DequeIndexIterator<DequeB, ElementB, ReversedB>& b) {
		return a.compareTo(b) <= 0;
	}

	template <class DequeType, class ElementType, bool Reversed>
	DequeIndexIterator<DequeType, ElementType, Reversed> operator +(
			const DequeIndexIterator<DequeType, ElementType, Reversed>& it, int a) {
		DequeIndexIterator<DequeType, ElementType, Reversed> res = it;
		res += a;
		return res;
	}

	template <class DequeType, class ElementType, bool Reversed>
	DequeIndexIterator<DequeType, ElementType, Reversed> operator +(int a,
						const DequeIndexIterator<DequeType, ElementType, Reversed>& it) {
		return it + a;
	}

	template <class DequeType, class ElementType, bool Reversed>
	DequeIndexIterator<DequeType, ElementType, Reversed> operator -(
			const DequeIndexIterator<DequeType, ElementType, Reversed>& it, int a) {
		DequeIndexIterator<DequeType, ElementType, Reversed> res = it;
		res -= a;
		return res;
	}

	template <class DequeTypeA, class ElementTypeA, class DequeTypeB, class ElementTypeB, bool Reversed>
	int operator -(const DequeIndexIterator<DequeTypeA, ElementTypeA, Reversed>& a,
						const DequeIndexIterator<DequeTypeB, ElementTypeB, Reversed>& b) {
		return b.distanceTo(a);
	}


	template <class Value>
	class Deque {
	public:
		typedef DequeIndexIterator<Deque<Value>, Value, false> iterator;
		typedef DequeIndexIterator<const Deque<Value>, const Value, false> const_iterator;
		typedef DequeIndexIterator<Deque<Value>, Value, true> reverse_iterator;
		typedef DequeIndexIterator<const Deque<Value>, const Value, true> const_reverse_iterator;

		Deque()
			:data(new Value[MIN_CAPACITY]), head(0), tail(0), count(0), capacity(MIN_CAPACITY) {}
		Deque(const Deque<Value>& from)
			:data(new Value[from.capacity]), head(0), tail(from.count), count(from.count), capacity(from.capacity) {
			copyFrom(from.data, from.head, from.tail, from.count);
		}

		Deque<Value>& operator =(const Deque<Value>& from) {
			if (&from == this)
				return *this;

			delete[] data;
			data = new Value[from.capacity];
			head = 0;
			tail = from.count;
			count = from.count;
			capacity = from.capacity;
			copyFrom(from.data, from.head, from.tail, from.count);
			return *this;
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

		Value& operator[](int index) {
			return data[(head + index) % capacity];
		}
		const Value& operator[](int index) const {
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
			return iterator(this, 0);
		}
		iterator end() {
			return iterator(this, count);
		}

		const_iterator begin() const {
			return const_iterator(this, 0);
		}
		const_iterator end() const {
			return const_iterator(this, count);
		}

		const_iterator cbegin() const {
			return const_iterator(this, 0);
		}
		const_iterator cend() const {
			return const_iterator(this, count);
		}

		reverse_iterator rbegin() {
			return reverse_iterator(this, 0);
		}
		reverse_iterator rend() {
			return reverse_iterator(this, count);
		}

		const_reverse_iterator rbegin() const {
			return const_reverse_iterator(this, 0);
		}
		const_reverse_iterator rend() const {
			return const_reverse_iterator(this, count);
		}

		const_reverse_iterator crbegin() const {
			return const_reverse_iterator(this, 0);
		}
		const_reverse_iterator crend() const {
			return const_reverse_iterator(this, count);
		}

		~Deque() {
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
			if (count >= capacity)
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


using dequeNS::Deque;
