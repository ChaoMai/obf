#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "gtest/gtest.h"

#include "common.h"
#include "gtest/gtest.h"
#include "obf/ordinal_bloom_filter.h"

using obf::OrdinalBloomFilter;
using std::cout;
using std::endl;
using std::range_error;
using std::string;

TEST(OrdinalBloomFilter, default_constructor) {
  OrdinalBloomFilter<string> bf(false_positive, capacity);
  ASSERT_THROW(OrdinalBloomFilter<string>(10, 3), range_error);
}

TEST(OrdinalBloomFilter, move_constructor) {
  string vals[] = {"a", "b", "c"};
  OrdinalBloomFilter<string> bf(false_positive, capacity);

  for (const string& str : vals) {
    bf.add(str);
  }

  OrdinalBloomFilter<string> bf1(std::move(bf));

  for (const string& str : vals) {
    ASSERT_TRUE(bf1.contains(str));
  }
}

TEST(OrdinalBloomFilter, move_assign_constructor) {
  string vals[] = {"a", "b", "c"};
  OrdinalBloomFilter<string> bf(false_positive, capacity);

  for (const string& str : vals) {
    bf.add(str);
  }

  OrdinalBloomFilter<string> bf1 = std::move(bf);

  for (const string& str : vals) {
    ASSERT_TRUE(bf1.contains(str));
  }
}

TEST(OrdinalBloomFilter, clear) {
  string vals[] = {"a", "b", "c"};
  OrdinalBloomFilter<string> bf(false_positive, capacity);

  for (const string& str : vals) {
    bf.add(str);
  }

  bf.clear();

  for (const string& str : vals) {
    ASSERT_FALSE(bf.contains(str));
  }
}

TEST(OrdinalBloomFilter, basic_insert_and_contains_string) {
  string vals[] = {"a", "b", "c"};
  OrdinalBloomFilter<string> bf(false_positive, capacity);

  for (const string& str : vals) {
    bf.add(str);
  }

  for (const string& str : vals) {
    ASSERT_TRUE(bf.contains(str));
  }

  ASSERT_FALSE(bf.contains("d"));
}

TEST(OrdinalBloomFilter, basic_insert_and_contains_int) {
  OrdinalBloomFilter<int> bf(false_positive, capacity);

  for (int i = 0; i < 3; ++i) {
    bf.add(i);
  }

  for (int i = 0; i < 3; ++i) {
    ASSERT_TRUE(bf.contains(i));
  }
}

TEST(OrdinalBloomFilter, massive_insert_and_contains) {
  OrdinalBloomFilter<int> bf(false_positive, capacity);

  for (int i = 0; i < iteration_num; ++i) {
    bf.add(i);
  }

  for (int i = 0; i < iteration_num; ++i) {
    ASSERT_TRUE(bf.contains(i));
  }

  for (int i = 1; i < 5; ++i) {
    int true_count = 0;
    int false_positive_count = 0;

    for (int j = iteration_num * i; j < iteration_num * (i + 1); ++j) {
      if (bf.contains(j)) {
        ++false_positive_count;
      } else {
        ++true_count;
      }
    }

    cout << "true count: " << true_count << endl;
    cout << "false positive count:" << false_positive_count << endl;
    cout << "false positive rate:" << false_positive_count / iteration_num
         << endl;
  }
}

TEST(OrdinalBloomFilter, swap) {
  string vals[] = {"a", "b", "c"};
  OrdinalBloomFilter<string> bf(false_positive, capacity);

  for (const string& str : vals) {
    bf.add(str);
  }

  OrdinalBloomFilter<string> bf1(false_positive, capacity);
  bf.swap(bf1);

  for (const string& str : vals) {
    ASSERT_FALSE(bf.contains(str));
  }

  for (const string& str : vals) {
    ASSERT_TRUE(bf1.contains(str));
  }
}
