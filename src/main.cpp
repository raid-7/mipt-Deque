#include <deque>
#include <iostream>
#include <utility>
#include <ctime>
#include <vector>
#include <gtest/gtest.h>

#include "deque.h"


const unsigned long long lcgA = 211, lcgC = 25731, lcgM = 1000000000000037;
unsigned long long lcgX = 71312ULL;
unsigned long long nextRandom() {
	return lcgX = (lcgX * lcgA + lcgC) % lcgM;
}


const unsigned int DEF_TEST_SIZE = 2700000;

enum class DequeSimpleAction {
	PUSH_BACK, PUSH_FRONT, POP_BACK, POP_FRONT
};

DequeSimpleAction actionFromInt(unsigned int type) {
	return type == 0 ? DequeSimpleAction::PUSH_BACK :
			type == 1 ? DequeSimpleAction::PUSH_FRONT :
			type == 2 ? DequeSimpleAction::POP_BACK :
						DequeSimpleAction::POP_FRONT;
}

template <class DequeType, class ValType>
inline void dequeSimpleAction(DequeType& testingDeque, ValType& val, DequeSimpleAction queryType) {
	if (queryType == DequeSimpleAction::PUSH_BACK) {
		testingDeque.push_back(val);
	}
	if (queryType == DequeSimpleAction::PUSH_FRONT) {
		testingDeque.push_front(val);
	}
	if (queryType == DequeSimpleAction::POP_BACK) {
		bool empty = testingDeque.empty();
		ASSERT_EQ(testingDeque.size() == 0, empty);

		if (!empty) {
			val = testingDeque.back();
			testingDeque.pop_back();
		}
	}
	if (queryType == DequeSimpleAction::POP_FRONT) {
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
	Deque<unsigned long long> victim;
	std::deque<unsigned long long> standard;

	for (unsigned int i = 0; i < DEF_TEST_SIZE; ++i) {
		int queryType = static_cast<int>(nextRandom() % 6);
		queryType = queryType == 4 ? 0 : queryType == 5 ? 1 : queryType;

		unsigned long long val = nextRandom();
		dequeSimpleAction(victim, val, actionFromInt(queryType));

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

	Deque<Val> victim;
	std::deque<Val> standard;

	for (unsigned int i = 0; i < DEF_TEST_SIZE; ++i) {
		int queryTypeInt = static_cast<int>(nextRandom() & 7);
		queryTypeInt = queryTypeInt >= 6 ? 0 : queryTypeInt >= 4 ? 1 : queryTypeInt;
		DequeSimpleAction queryType = actionFromInt(queryTypeInt);

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

void assertLinearComplexity(const std::vector<unsigned long long>& times) {
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

	ASSERT_NEAR(minK, maxK, maxK * 0.2);
}

TEST(DEQUE_TEST_COMPLEXITY, Test1) {
	Deque<unsigned long long> victim;

	unsigned long long startTime = clock();
	std::vector<unsigned long long> times;

	for (unsigned int i = 0; i < DEF_TEST_SIZE * 5; ++i) {
		int queryType = static_cast<int>(nextRandom() % 6);
		queryType = queryType >= 6 ? 0 : queryType >= 4 ? 1 : queryType;

		unsigned long long val = nextRandom();
		dequeSimpleAction(victim, val, actionFromInt(queryType));

		times.push_back(clock() - startTime);
	}

	assertLinearComplexity(times);
}

TEST(DEQUE_TEST_COMPLEXITY, Test2) {
	Deque<unsigned long long> victim;

	unsigned long long startTime = clock();
	std::vector<unsigned long long> times;

	const unsigned int SERIES_COUNT = 4;
	for (unsigned int series = 0; series < SERIES_COUNT; ++series) {
		for (unsigned int i = 0; i < DEF_TEST_SIZE * (SERIES_COUNT - series); ++i) {
			int rand = static_cast<int>(nextRandom() % 2);
			DequeSimpleAction queryType;
			if (series & 1) {
				queryType = rand == 0 ? DequeSimpleAction::POP_BACK : DequeSimpleAction::POP_FRONT;
			} else {
				queryType = rand == 0 ? DequeSimpleAction::PUSH_BACK : DequeSimpleAction::PUSH_FRONT;
			}

			unsigned long long val = nextRandom();
			dequeSimpleAction(victim, val, queryType);

			times.push_back(clock() - startTime);
		}
	}

	assertLinearComplexity(times);
}

TEST(DEQUE_MANUAL_TEST, TestBrackets) {
	Deque<double> victim;

	victim.push_back(1.0);
	victim.push_front(2.0);
	ASSERT_EQ(victim[0], 2.0);
	ASSERT_EQ(victim[1], 1.0);

	victim.rbegin()[1] = 5.0;
	ASSERT_EQ(victim[0], 5.0);

	victim.end()[-1] = 4.0;
	ASSERT_EQ(victim[1], 4.0);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
