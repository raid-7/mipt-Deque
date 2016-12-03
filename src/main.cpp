#include <deque>
#include <iostream>
#include <utility>
#include <ctime>
#include <vector>
#include <gtest/gtest.h>
#include "deque.h"


const unsigned long long lcgA = 211, lcgC = 25731, lcgM = 1000000000000037;
unsigned long long lcgX = 7312ULL;
unsigned long long nextRandom() {
	return lcgX = (lcgX * lcgA + lcgC) % lcgM;
}


const unsigned int DEF_TEST_SIZE = 2700000;


template <class DequeType, class ValType>
inline void dequeSimpleAction(DequeType& testingDeque, ValType& val, int queryType) {
	if (queryType == 0) { // push back
		testingDeque.push_back(val);
	}
	if (queryType == 1) { // push front
		testingDeque.push_front(val);
	}
	if (queryType == 2) { // pop back
		bool empty = testingDeque.empty();
		ASSERT_EQ(testingDeque.size() == 0, empty);

		if (!empty) {
			val = testingDeque.back();
			testingDeque.pop_back();
		}
	}
	if (queryType == 3) { // pop front
		bool empty = testingDeque.empty();
		ASSERT_EQ(testingDeque.size() == 0, empty);

		if (!empty) {
			val = testingDeque.front();
			testingDeque.pop_front();
		}
	}
}

template <class TestIt, class StdIt>
inline void tortureIterators(TestIt testBegin, TestIt testEnd, TestIt test,
			StdIt stdBegin, StdIt stdEnd, StdIt std) {
	int size = testEnd - testBegin;
	ASSERT_EQ(size, stdEnd - stdBegin);

	while (test < testEnd && std < stdEnd) {
		int offset = static_cast<int>(nextRandom() & size) - static_cast<int>(size / 2);
		std += offset;
		test += offset;

		ASSERT_EQ(std - stdBegin, test - testBegin);

		if (test < testBegin) {
			test = testBegin + size / 2;
			std = stdBegin + size / 2;
		}

		if (test < testEnd) {
			ASSERT_EQ(*test, *std);
		}
	}
}


TEST(DEQUE_TEST_MAIN_FUNCTIONALITY, Test1) {
	deque<unsigned long long> victim;
	std::deque<unsigned long long> standard;

	for (unsigned int i = 0; i < DEF_TEST_SIZE; ++i) {
		int queryType = static_cast<int>(nextRandom() % 6);
		queryType = queryType == 4 ? 0 : queryType == 5 ? 1 : queryType;

		unsigned long long val = nextRandom();
		dequeSimpleAction(victim, val, queryType);

		if (queryType == 0) { // push back
			standard.push_back(val);
		}
		if (queryType == 1) { // push front
			standard.push_front(val);
		}
		if (queryType == 2) { // pop back
			if (!standard.empty()) {
				ASSERT_EQ(standard.back(), val);
				standard.pop_back();
			}
		}
		if (queryType == 3) { // pop front
			if (!standard.empty()) {
				ASSERT_EQ(standard.front(), val);
				standard.pop_front();
			}
		}

		unsigned int size = victim.size();
		ASSERT_EQ(standard.size(), size);

		if (size != 0) {
			unsigned int randomIndex = static_cast<unsigned int>(nextRandom() % size);
			ASSERT_EQ(standard[randomIndex], victim[randomIndex]);
		}
	}
}

TEST(DEQUE_TEST_ITERATORS, Test1) {
	typedef std::pair<unsigned long long, unsigned long long> Val;

	deque<Val> victim;
	std::deque<Val> standard;

	for (unsigned int i = 0; i < DEF_TEST_SIZE; ++i) {
		int queryType = static_cast<int>(nextRandom() & 7);
		queryType = queryType >= 6 ? 0 : queryType >= 4 ? 1 : queryType;

		Val val = std::make_pair(nextRandom(), nextRandom());
		dequeSimpleAction(victim, val, queryType);
		dequeSimpleAction(standard, val, queryType);

		tortureIterators(victim.begin(), victim.end(), victim.begin(),
							standard.begin(), standard.end(), standard.begin());
		tortureIterators(victim.rbegin(), victim.rend(), victim.rbegin(),
							standard.rbegin(), standard.rend(), standard.rbegin());
		tortureIterators(victim.crbegin(), victim.crend(), victim.crbegin(),
							standard.crbegin(), standard.crend(), standard.crbegin());
	}

	std::sort(victim.begin(), victim.end());
	std::sort(standard.begin(), standard.end());

	for (unsigned int i = 0; i < victim.size(); ++i) {
		ASSERT_EQ(victim[i], standard[i]) << i;
	}
}

TEST(DEQUE_TEST_COMPLEXITY, Test1) {
	deque<unsigned long long> victim;

	unsigned long long startTime = clock();
	std::vector<unsigned long long> times;

	for (unsigned int i = 0; i < DEF_TEST_SIZE * 5; ++i) {
		int queryType = static_cast<int>(nextRandom() % 6);
		queryType = queryType >= 6 ? 0 : queryType >= 4 ? 1 : queryType;

		unsigned long long val = nextRandom();
		dequeSimpleAction(victim, val, queryType);

		times.push_back(clock() - startTime);
	}

	double maxK = -1.0, minK = -1.0;
	for (unsigned int i = times.size() / 5; i < times.size(); ++i) {
		double k = static_cast<double>(times[i]) / static_cast<double>(i + 1);
		if (maxK < k || maxK < 0.0) {
			maxK = k;
		}
		if (minK > k || minK < 0.0) {
			minK = k;
		}
	}

	ASSERT_NEAR(minK, maxK, maxK * 0.5);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}