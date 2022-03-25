#include <iostream>
#include <vector>
#include <list>
#include "test_runner.h"
#include "vl_vector.h"

using std::cin;
using std::cout;
using std::endl;
using std::size_t;
using std::string;

#define TESTS_DEFAULT_CAPACITY 16

class TestsType {
public:
    int a;
    int b;

    TestsType(int a = 0, int b = 0) : a(a), b(b) {}

    TestsType(const TestsType &other) = default;

    TestsType(std::pair<int, int> p) : a(p.first), b(p.second) {}

    bool operator==(const TestsType &rhs) const {
        return a == rhs.a && b == rhs.b;
    }

    bool operator!=(const TestsType &rhs) const {
        return !(*this == rhs);
    }

    TestsType &operator=(const TestsType &rhs) {
        if (&rhs != this) {
            a = rhs.a;
            b = rhs.b;
        }

        return *this;
    }
};


void test_default_ctor_with_static_capacity() {
    vl_vector<TestsType, 10> vec;
    ASSERT(vec.capacity() == 10
           && vec.size() == 0
           && vec.empty())
}

void test_default_ctor_default_static_capacity() {
    vl_vector<TestsType> vec;
    ASSERT(vec.capacity() == TESTS_DEFAULT_CAPACITY
           && vec.size() == 0
           && vec.empty())
}


void test_iterator_ctor_static() {
    std::vector<TestsType> real_vector{
            {1, 2},
            {3, 4},
            {5, 6},
    };
    vl_vector<TestsType, 5> vec(real_vector.begin(), real_vector.end());
    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(real_vector[i] == vec.at(i))
    }
    ASSERT(vec.capacity() == 5
           && vec.size() == 3
           && !vec.empty())
}


void test_iterator_ctor_dynamic() {
    std::vector<TestsType> real_vector{
            {1, 2},
            {3, 4},
            {5, 6},
    };
    vl_vector<TestsType, 2> vec(real_vector.begin(), real_vector.end());
    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(real_vector[i] == vec.at(i))
    }
    size_t expected_capacity = 4; // (3 * 3) / 2
    ASSERT(vec.capacity() == expected_capacity
           && vec.size() == 3
           && !vec.empty())
}

void test_iterator_ctor_zero_elements() {
    std::vector<TestsType> real_vector{
            {1, 2},
            {3, 4},
            {5, 6},
    };
    vl_vector<TestsType, 5> vec(real_vector.begin(), real_vector.begin());
    ASSERT(vec.capacity() == 5
           && vec.size() == 0
           && vec.empty())
}


void test_copy_ctor_static() {
    std::vector<TestsType> real_vector{
            {1, 2},
            {3, 4},
            {5, 6},
    };
    vl_vector<TestsType, 5> vec1(real_vector.begin(), real_vector.end());
    vl_vector<TestsType, 5> vec2 = vec1; // use copy ctor

    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(real_vector[i] == vec2.at(i))
    }
    ASSERT(vec2.capacity() == 5 && vec2.size() == 3 && !vec2.empty())
}

void test_copy_ctor_dynamic() {
    std::vector<TestsType> real_vector{
            {1, 2},
            {3, 4},
            {5, 6},
    };
    vl_vector<TestsType, 2> vec1(real_vector.begin(), real_vector.end());
    vl_vector<TestsType, 2> vec2 = vec1; // use copy ctor

    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(real_vector[i] == vec2.at(i))
    }
    size_t expected_capacity = 4; // (3 * 3) / 2
    ASSERT(vec2.capacity() == expected_capacity
           && vec2.size() == 3
           && !vec2.empty())
}

void test_copy_ctor_zero_elements() {
    vl_vector<TestsType, 5> vec1;
    vl_vector<TestsType, 5> vec2 = vec1; // use copy ctor

    ASSERT(vec2.capacity() == 5
           && vec2.size() == 0
           && vec2.empty())
}

void test_value_ctor_static() {
    TestsType value = TestsType(4, 7);
    const size_t amount = 3;
    vl_vector<TestsType, 5> vec(amount, value);

    for (size_t i = 0; i < amount; ++i) {
        ASSERT(vec.at(i) == value)
    }
    ASSERT(vec.capacity() == 5
           && vec.size() == amount
           && !vec.empty())
}

void test_value_ctor_dynamic() {
    TestsType value = TestsType(4, 7);
    const size_t amount = 3;
    vl_vector<TestsType, 2> vec(amount, value);

    for (size_t i = 0; i < amount; ++i) {
        ASSERT(vec.at(i) == value)
    }

    size_t expected_capacity = 4; // (3 * 3) / 2
    ASSERT(vec.capacity() == expected_capacity
           && vec.size() == amount
           && !vec.empty())
}

void test_value_ctor_zero_elements() {
    TestsType value = TestsType(4, 7);
    vl_vector<TestsType, 5> vec(0, value);

    ASSERT(vec.capacity() == 5
           && vec.size() == 0
           && vec.empty())
}

void test_at_invalid() {
    std::vector<TestsType> real_vector{
            {1, 2},
            {3, 4},
            {5, 6},
    };

    TestsType value_to_insert(7,8);

    vl_vector<TestsType, 5> vec_stat(real_vector.begin(), real_vector.end());
    vl_vector<TestsType, 2> vec_dyn(real_vector.begin(), real_vector.end());

    std::vector<int> vec_stat_test_values{3, 5, -1};
    std::vector<int> vec_dyn_test_values{3, 4, -1};

    for (const auto &test_value: vec_stat_test_values) {
        ASSERT_THROWS(vec_stat.at(test_value))
        ASSERT_THROWS(vec_stat.at(test_value) = value_to_insert)
    }

    for (const auto &test_value: vec_dyn_test_values) {
        ASSERT_THROWS(vec_dyn.at(test_value))
        ASSERT_THROWS(vec_dyn.at(test_value) = value_to_insert)
    }
}

void test_operator_at_static() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
    };
    vl_vector<TestsType, 4> vec1(real_vector.begin(), real_vector.end());
    const vl_vector<TestsType, 4> vec2(real_vector.begin(), real_vector.end());

    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(vec1.at(i) == real_vector.at(i))
        ASSERT(vec2.at(i) == real_vector.at(i))
    }

    std::vector<TestsType> new_values{
            TestsType(4, 4),
            TestsType(5, 5),
            TestsType(6, 6),
    };

    for (size_t i = 0; i < real_vector.size(); ++i) {
        vec1.at(i) = new_values.at(i);
        ASSERT(vec1.at(i) == new_values.at(i))
    }
}

void test_operator_at_dynamic() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
    };
    vl_vector<TestsType, 2> vec1(real_vector.begin(), real_vector.end());
    const vl_vector<TestsType, 2> vec2(real_vector.begin(), real_vector.end());

    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(vec1.at(i) == real_vector.at(i))
        ASSERT(vec2.at(i) == real_vector.at(i))
    }

    std::vector<TestsType> new_values{
            TestsType(4, 4),
            TestsType(5, 5),
            TestsType(6, 6),
    };

    for (size_t i = 0; i < real_vector.size(); ++i) {
        vec1.at(i) = new_values.at(i);
        ASSERT(vec1.at(i) == new_values.at(i))
    }
}


void test_push_back_static() {
    std::vector<TestsType> real_vector;
    vl_vector<TestsType, 5> vec;

    std::vector<TestsType> values_to_push{
            TestsType(1, 2),
            TestsType(3, 4),
            TestsType(5, 6),
    };

    size_t size = 0;
    size_t capacity = 5;

    for (const auto &value_to_push: values_to_push) {
        vec.push_back(value_to_push);
        real_vector.push_back(value_to_push);
        ++size;

        ASSERT(vec.size() == size
               && vec.capacity() == capacity
               && !vec.empty())

        for (size_t i = 0; i < real_vector.size(); ++i) {
            ASSERT(real_vector[i] == vec.at(i))
        }
    }
}

void test_push_back_dynamic() {
    std::vector<TestsType> real_vector{
            {1,  2},
            {3,  4},
            {5,  6},
            {7,  8},
            {9,  10},
            {11, 12},
    };

    vl_vector<TestsType, 2> vec(real_vector.begin(), real_vector.end());

    std::vector<TestsType> values_to_push{
            TestsType(-1, -1),
            TestsType(-2, -2),
            TestsType(-3, -3)
    };

    size_t size = real_vector.size();
    size_t capacity = 9; // (6 * 3) / 2 = 9

    for (const auto &value_to_push: values_to_push) {
        vec.push_back(value_to_push);
        real_vector.push_back(value_to_push);
        ++size;

        ASSERT(vec.size() == size
               && vec.capacity() == capacity
               && !vec.empty())

        for (size_t i = 0; i < real_vector.size(); ++i) {
            ASSERT(real_vector[i] == vec.at(i))
        }
    }
}


void test_push_back_static_to_dynamic() {
    std::vector<TestsType> real_vector{
            {1, 1},
            {2, 2},
    };

    vl_vector<TestsType, 2> vec(real_vector.begin(), real_vector.end());

    std::vector<TestsType> values_to_push{
            TestsType(3, 3),
            TestsType(4, 4),
            TestsType(5, 5),
    };

    std::vector<size_t> expected_capacities{
            4,
            4,
            7
    };

    size_t size = real_vector.size();

    for (size_t i = 0; i < values_to_push.size(); ++i) {
        vec.push_back(values_to_push[i]);
        real_vector.push_back(values_to_push[i]);
        ++size;

        ASSERT(vec.size() == size
               && vec.capacity() == expected_capacities[i]
               && !vec.empty())

        for (size_t j = 0; j < real_vector.size(); ++j) {
            ASSERT(real_vector[j] == vec.at(j))
        }
    }
}

void test_push_back_dynamic_increase_capacity() {
    // initial capacity is 4
    std::vector<TestsType> real_vector{
            {1, 1},
            {2, 2},
            {3, 3},
    };

    vl_vector<TestsType, 2> vec(real_vector.begin(), real_vector.end());

    std::vector<TestsType> values_to_push{
            TestsType(4, 4),
            TestsType(5, 5),
            TestsType(6, 6),
            TestsType(7, 7),
            TestsType(8, 8),
            TestsType(9, 9),
            TestsType(10, 10),
    };

    std::vector<size_t> expected_capacities{
            4,
            7,
            7,
            7,
            12,
            12,
            12
    };

    size_t size = real_vector.size();

    for (size_t i = 0; i < values_to_push.size(); ++i) {
        vec.push_back(values_to_push[i]);
        real_vector.push_back(values_to_push[i]);
        ++size;

        ASSERT(vec.size() == size
               && vec.capacity() == expected_capacities[i]
               && !vec.empty())

        for (size_t j = 0; j < real_vector.size(); ++j) {
            ASSERT(real_vector[j] == vec.at(j))
        }
    }
}

void test_pop_back_static() {
    std::vector<TestsType> real_vector{
            TestsType(1, 2),
            TestsType(3, 4),
            TestsType(5, 6),
    };
    vl_vector<TestsType, 5> vec(real_vector.begin(), real_vector.end());

    vec.pop_back();
    ASSERT(vec.size() == 2
           && !vec.empty()
           && vec.capacity() == 5
           && vec.at(0) == real_vector[0]
           && vec.at(1) == real_vector[1])

    ASSERT_THROWS(vec.at(2))

    vec.pop_back();
    ASSERT(vec.size() == 1
           && !vec.empty()
           && vec.capacity() == 5
           && vec.at(0) == real_vector[0])
    ASSERT_THROWS(vec.at(1))

    vec.pop_back();

    ASSERT(vec.size() == 0)
    ASSERT(vec.empty())
    ASSERT(vec.capacity() == 5)
    ASSERT_THROWS(vec.at(0))
}

void test_pop_back_dynamic() {
    std::vector<TestsType> real_vector{
            TestsType(1, 2),
            TestsType(3, 4),
            TestsType(5, 6),
            TestsType(7, 8),
    };
    vl_vector<TestsType, 1> vec(real_vector.begin(), real_vector.end());
    const size_t expected_capacity = 6; // (4 * 3) / 2

    vec.pop_back();
    ASSERT(vec.size() == 3
           && !vec.empty()
           && vec.capacity() == expected_capacity
           && vec.at(0) == real_vector[0]
           && vec.at(1) == real_vector[1]
           && vec.at(2) == real_vector[2])
    ASSERT_THROWS(vec.at(3))

    vec.pop_back();
    ASSERT(vec.size() == 2
           && !vec.empty()
           && vec.capacity() == expected_capacity
           && vec.at(0) == real_vector[0]
           && vec.at(1) == real_vector[1])
    ASSERT_THROWS(vec.at(2))
}

void test_pop_back_dynamic_to_static() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
    };
    vl_vector<TestsType, 2> vec(real_vector.begin(), real_vector.end());


    vec.pop_back();
    ASSERT(vec.size() == 2
           && !vec.empty()
           && vec.capacity() == 2 // static capacity
           && vec.at(0) == real_vector[0]
           && vec.at(1) == real_vector[1])
    ASSERT_THROWS(vec.at(2))

    vec.pop_back();
    ASSERT(vec.size() == 1
           && !vec.empty()
           && vec.capacity() == 2 // static capacity
           && vec.at(0) == real_vector[0])

    ASSERT_THROWS(vec.at(1))

}

void test_pop_back_empty() {
    vl_vector<TestsType, 2> vec;

    vec.pop_back(); // should not have any effect
    ASSERT(vec.size() == 0
           && vec.empty()
           && vec.capacity() == 2)
}

void test_clear_static() {
    vl_vector<TestsType, 2> vec;
    vec.push_back(TestsType(1, 2));
    vec.clear();

    ASSERT(vec.empty()
           && vec.size() == 0
           && vec.capacity() == 2)
}

void test_clear_dynamic() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
    };
    vl_vector<TestsType, 2> vec(real_vector.begin(), real_vector.end());
    vec.clear();

    ASSERT(vec.empty()
           && vec.size() == 0
           && vec.capacity() == 2)
}

void test_clear_and_operations() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
    };
    vl_vector<TestsType, 2> vec(real_vector.begin(), real_vector.end());
    vec.clear();

    TestsType val0(4, 4);
    TestsType val1(5, 5);
    TestsType val2(6, 6);

    vec.push_back(val0);
    ASSERT(!vec.empty() && vec.size() == 1 && vec.capacity() == 2
           && vec.at(0) == val0)

    vec.push_back(val1);
    ASSERT(!vec.empty() && vec.size() == 2 && vec.capacity() == 2
           && vec.at(0) == val0
           && vec.at(1) == val1)

    vec.push_back(val2);
    ASSERT(!vec.empty() && vec.size() == 3 && vec.capacity() == 4
           && vec.at(0) == val0
           && vec.at(1) == val1
           && vec.at(2) == val2)
}

void test_data_static() {
    vl_vector<TestsType, 2> vec;
    TestsType value1(1, 2);
    vec.push_back(value1);

    ASSERT(vec.data()[0] == value1)
    vec.clear();
    vec.push_back(value1);
    ASSERT(vec.data()[0] == value1)
}

void test_data_dynamic() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
    };
    vl_vector<TestsType, 2> vec(real_vector.begin(), real_vector.end());
    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(vec.data()[i] == real_vector[i])
    }
}

void test_contains_static() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
    };
    vl_vector<TestsType, 4> vec(real_vector.begin(), real_vector.end());
    for (auto &value: real_vector) {
        ASSERT(vec.contains(value))
    }

    TestsType not_contained_value = TestsType(4, 4);
    ASSERT(!vec.contains(not_contained_value))
}

void test_contains_dynamic() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
    };
    vl_vector<TestsType, 2> vec(real_vector.begin(), real_vector.end());
    for (auto &value: real_vector) {
        ASSERT(vec.contains(value))
    }

    TestsType not_contained_value = TestsType(4, 4);
    ASSERT(!vec.contains(not_contained_value))
}

void test_insert_single_static() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
    };

    vl_vector<TestsType, 10> vec(real_vector.begin(), real_vector.end());

    TestsType new_value(4, 4);
    auto vec_it = vec.begin();
    vec_it++;
    auto real_vec_it = real_vector.begin();
    real_vec_it++;

    auto real_vector_insert_it = real_vector.insert(real_vec_it, new_value);
    auto insert_it = vec.insert(vec_it, new_value);

    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(real_vector[i] == vec[i])
    }

    ASSERT(*insert_it == new_value)
    while (real_vector_insert_it != real_vector.end()) {
        ASSERT(*real_vector_insert_it == *insert_it)
        ++real_vector_insert_it;
        ++insert_it;
    }

    ASSERT(real_vector.size() == vec.size())
    ASSERT(vec.capacity() == 10)
}

void test_insert_single_dynamic() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
    };

    vl_vector<TestsType, 2> vec(real_vector.begin(), real_vector.end());

    TestsType new_value(4, 4);
    auto vec_it = vec.begin();
    vec_it++;
    auto real_vec_it = real_vector.begin();
    real_vec_it++;

    real_vector.insert(real_vec_it, new_value);
    vec.insert(vec_it, new_value);

    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(real_vector[i] == vec[i])
    }

    ASSERT(real_vector.size() == vec.size())
    ASSERT(vec.capacity() == 4)
}

void test_insert_single_static_to_dynamic() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
    };

    vl_vector<TestsType, 3> vec(real_vector.begin(), real_vector.end());

    TestsType new_value(4, 4);
    auto vec_it = vec.begin();
    vec_it++;
    auto real_vec_it = real_vector.begin();
    real_vec_it++;

    real_vector.insert(real_vec_it, new_value);
    vec.insert(vec_it, new_value);

    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(real_vector[i] == vec[i])
    }

    ASSERT(real_vector.size() == vec.size())
    ASSERT(vec.capacity() == 6)
}

void test_insert_single_dynamic_increase_capacity() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
    };

    vl_vector<TestsType, 2> vec(real_vector.begin(), real_vector.end());

    real_vector.push_back(TestsType(4, 4));
    vec.push_back(TestsType(4, 4));

    TestsType new_value(5, 5);
    auto vec_it = vec.begin();
    vec_it++;
    auto real_vec_it = real_vector.begin();
    real_vec_it++;

    real_vector.insert(real_vec_it, new_value);
    vec.insert(vec_it, new_value);

    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(real_vector[i] == vec[i])
    }

    ASSERT(real_vector.size() == vec.size())
    ASSERT(vec.capacity() == 7)
}

void test_insert_iterator_static() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
    };

    vl_vector<TestsType, 10> vec(real_vector.begin(), real_vector.end());

    std::vector<TestsType> new_values{
            TestsType(4, 4),
            TestsType(5, 5),
            TestsType(6, 6),
    };

    auto vec_it = vec.begin();
    vec_it += 2;
    auto real_vec_it = real_vector.begin();
    real_vec_it += 2;

    auto real_vector_insert_it = real_vector.insert(real_vec_it,
                                                    new_values.begin(),
                                                    new_values.end());
    auto insert_it = vec.insert(vec_it, new_values.begin(), new_values.end());

    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(real_vector[i] == vec[i])
    }

    ASSERT(*insert_it == new_values[0])
    while (real_vector_insert_it != real_vector.end()) {
        ASSERT(*real_vector_insert_it == *insert_it)
        ++real_vector_insert_it;
        ++insert_it;
    }

    ASSERT(real_vector.size() == vec.size())
    ASSERT(vec.capacity() == 10)
}

void test_insert_iterator_dynamic() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
            TestsType(4, 4),
    };

    vl_vector<TestsType, 2> vec(real_vector.begin(), real_vector.end());

    std::vector<TestsType> new_values{
            TestsType(5, 5),
            TestsType(6, 6),
    };

    auto vec_it = vec.begin();
    vec_it += 2;
    auto real_vec_it = real_vector.begin();
    real_vec_it += 2;

    auto real_vector_insert_it = real_vector.insert(real_vec_it,
                                                    new_values.begin(),
                                                    new_values.end());
    auto insert_it = vec.insert(vec_it, new_values.begin(), new_values.end());

    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(real_vector[i] == vec[i])
    }

    ASSERT(*insert_it == new_values[0])
    while (real_vector_insert_it != real_vector.end()) {
        ASSERT(*real_vector_insert_it == *insert_it)
        ++real_vector_insert_it;
        ++insert_it;
    }


    ASSERT(real_vector.size() == vec.size())
    ASSERT(vec.capacity() == 6)
}

void test_insert_iterator_static_to_dynamic() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
            TestsType(4, 4),
    };

    vl_vector<TestsType, 4> vec(real_vector.begin(), real_vector.end());

    std::vector<TestsType> new_values{
            TestsType(5, 5),
            TestsType(6, 6),
    };

    auto vec_it = vec.begin();
    vec_it += 2;
    auto real_vec_it = real_vector.begin();
    real_vec_it += 2;

    auto real_vector_insert_it = real_vector.insert(real_vec_it,
                                                    new_values.begin(),
                                                    new_values.end());
    auto insert_it = vec.insert(vec_it, new_values.begin(), new_values.end());

    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(real_vector[i] == vec[i])
    }

    ASSERT(*insert_it == new_values[0])
    while (real_vector_insert_it != real_vector.end()) {
        ASSERT(*real_vector_insert_it == *insert_it)
        ++real_vector_insert_it;
        ++insert_it;
    }

    ASSERT(real_vector.size() == vec.size())
    ASSERT(vec.capacity() == 9)
}

void test_insert_iterator_dynamic_increase_capacity() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
    };

    vl_vector<TestsType, 2> vec(real_vector.begin(), real_vector.end());

    std::vector<TestsType> new_values{
            TestsType(4, 4),
            TestsType(5, 5),
    };

    auto vec_it = vec.begin();
    vec_it += 2;
    auto real_vec_it = real_vector.begin();
    real_vec_it += 2;

    auto real_vector_insert_it = real_vector.insert(real_vec_it,
                                                    new_values.begin(),
                                                    new_values.end());
    auto insert_it = vec.insert(vec_it, new_values.begin(), new_values.end());

    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(real_vector[i] == vec[i])
    }

    ASSERT(*insert_it == new_values[0])
    while (real_vector_insert_it != real_vector.end()) {
        ASSERT(*real_vector_insert_it == *insert_it)
        ++real_vector_insert_it;
        ++insert_it;
    }

    ASSERT(real_vector.size() == vec.size())
    ASSERT(vec.capacity() == 7)
}

void test_erase_single_static() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
    };

    vl_vector<TestsType, 10> vec(real_vector.begin(), real_vector.end());

    auto vec_it = vec.begin();
    vec_it++;
    auto real_vec_it = real_vector.begin();
    real_vec_it++;

    auto real_vector_erase_it = real_vector.erase(real_vec_it);
    auto erase_it = vec.erase(vec_it);

    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(real_vector[i] == vec[i])
    }

    ASSERT(*erase_it == TestsType(3, 3))
    while (real_vector_erase_it != real_vector.end()) {
        ASSERT(*real_vector_erase_it == *erase_it)
        ++real_vector_erase_it;
        ++erase_it;
    }

    ASSERT(real_vector.size() == vec.size())
    ASSERT(vec.capacity() == 10)
}

void test_erase_single_dynamic() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
    };

    vl_vector<TestsType, 1> vec(real_vector.begin(), real_vector.end());

    auto vec_it = vec.begin();
    vec_it++;
    auto real_vec_it = real_vector.begin();
    real_vec_it++;

    auto real_vector_erase_it = real_vector.erase(real_vec_it);
    auto erase_it = vec.erase(vec_it);

    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(real_vector[i] == vec[i])
    }

    ASSERT(*erase_it == TestsType(3, 3))
    while (real_vector_erase_it != real_vector.end()) {
        ASSERT(*real_vector_erase_it == *erase_it)
        ++real_vector_erase_it;
        ++erase_it;
    }

    ASSERT(real_vector.size() == vec.size())
    ASSERT(vec.capacity() == 4)
}

void test_erase_single_dynamic_to_static() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
    };

    vl_vector<TestsType, 2> vec(real_vector.begin(), real_vector.end());

    auto vec_it = vec.begin();
    vec_it++;
    auto real_vec_it = real_vector.begin();
    real_vec_it++;

    auto real_vector_erase_it = real_vector.erase(real_vec_it);
    auto erase_it = vec.erase(vec_it);

    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(real_vector[i] == vec[i])
    }

    ASSERT(*erase_it == TestsType(3, 3))
    while (real_vector_erase_it != real_vector.end()) {
        ASSERT(*real_vector_erase_it == *erase_it)
        ++real_vector_erase_it;
        ++erase_it;
    }

    ASSERT(real_vector.size() == vec.size())
    ASSERT(vec.capacity() == 2)
}

void test_erase_iterator_static() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
            TestsType(4, 4),
            TestsType(5, 5),
    };

    vl_vector<TestsType, 6> vec(real_vector.begin(), real_vector.end());

    auto first_it = vec.begin();
    ++first_it;
    auto real_vec_first_it = real_vector.begin();
    ++real_vec_first_it;

    auto last_it = vec.end();
    --last_it;
    auto real_vec_last_it = real_vector.end();
    --real_vec_last_it;

    // delete 2,3,4
    auto real_vector_erase_it = real_vector.erase(real_vec_first_it,
                                                  real_vec_last_it);
    auto erase_it = vec.erase(first_it, last_it);

    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(real_vector[i] == vec[i])
    }

    ASSERT(*erase_it == TestsType(5, 5))
    while (real_vector_erase_it != real_vector.end()) {
        ASSERT(*real_vector_erase_it == *erase_it)
        ++real_vector_erase_it;
        ++erase_it;
    }

    ASSERT(real_vector.size() == vec.size())
    ASSERT(vec.capacity() == 6)
}

void test_erase_iterator_dynamic() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
            TestsType(4, 4),
            TestsType(5, 5),
    };

    vl_vector<TestsType, 1> vec(real_vector.begin(), real_vector.end());

    auto first_it = vec.begin();
    ++first_it;
    auto real_vec_first_it = real_vector.begin();
    ++real_vec_first_it;

    auto last_it = vec.end();
    --last_it;
    auto real_vec_last_it = real_vector.end();
    --real_vec_last_it;

    // delete 2,3,4
    auto real_vector_erase_it = real_vector.erase(real_vec_first_it,
                                                  real_vec_last_it);
    auto erase_it = vec.erase(first_it, last_it);

    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(real_vector[i] == vec[i])
    }

    ASSERT_M(*erase_it == TestsType(5, 5), "return iterator does not match")
    while (real_vector_erase_it != real_vector.end()) {
        ASSERT(*real_vector_erase_it == *erase_it)
        ++real_vector_erase_it;
        ++erase_it;
    }

    ASSERT_M(real_vector.size() == vec.size(), "size does not match")
    ASSERT_M(vec.capacity() == 7,
             "capacity does not match. expected: 7, actual: " +
             std::to_string(vec.size()))
}

void test_erase_iterator_dynamic_to_static() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
            TestsType(4, 4),
            TestsType(5, 5),
    };

    vl_vector<TestsType, 2> vec(real_vector.begin(), real_vector.end());

    auto first_it = vec.begin();
    ++first_it;
    auto real_vec_first_it = real_vector.begin();
    ++real_vec_first_it;

    auto last_it = vec.end();
    --last_it;
    auto real_vec_last_it = real_vector.end();
    --real_vec_last_it;

    // delete 2,3,4
    auto real_vector_erase_it = real_vector.erase(real_vec_first_it,
                                                  real_vec_last_it);
    auto erase_it = vec.erase(first_it, last_it);

    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(real_vector[i] == vec[i])
    }

    ASSERT_M(*erase_it == TestsType(5, 5), "return iterator does not match")
    while (real_vector_erase_it != real_vector.end()) {
        ASSERT(*real_vector_erase_it == *erase_it)
        ++real_vector_erase_it;
        ++erase_it;
    }

    ASSERT_M(real_vector.size() == vec.size(), "size does not match")
    ASSERT_M(vec.capacity() == 2,
             "capacity does not match. expected: 2, actual: " +
             std::to_string(vec.size()))
}


void test_operator_square_brackets_static() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
    };
    vl_vector<TestsType, 4> vec1(real_vector.begin(), real_vector.end());
    const vl_vector<TestsType, 4> vec2(real_vector.begin(), real_vector.end());

    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(vec1[i] == real_vector[i])
        ASSERT(vec2[i] == real_vector[i])
    }

    vec1[3]; // should not throw
    vec2[3]; // should not throw

    std::vector<TestsType> new_values{
            TestsType(4, 4),
            TestsType(5, 5),
            TestsType(6, 6),
    };

    for (size_t i = 0; i < real_vector.size(); ++i) {
        vec1[i] = new_values[i];
        ASSERT(vec1[i] == new_values[i])
    }
}

void test_operator_square_brackets_dynamic() {
    std::vector<TestsType> real_vector{
            TestsType(1, 1),
            TestsType(2, 2),
            TestsType(3, 3),
    };
    vl_vector<TestsType, 2> vec1(real_vector.begin(), real_vector.end());
    const vl_vector<TestsType, 2> vec2(real_vector.begin(), real_vector.end());

    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(vec1[i] == real_vector[i])
        ASSERT(vec2[i] == real_vector[i])
    }

    vec1[3]; // should not throw, capacity is 4
    vec2[3]; // should not throw, capacity is 4

    std::vector<TestsType> new_values{
            TestsType(4, 4),
            TestsType(5, 5),
            TestsType(6, 6),
    };

    for (size_t i = 0; i < real_vector.size(); ++i) {
        vec1[i] = new_values[i];
        ASSERT(vec1[i] == new_values[i])
    }
}

void test_operator_assignment_static() {
    std::vector<TestsType> real_vector{
            {1, 2},
            {3, 4},
            {5, 6},
    };
    vl_vector<TestsType, 5> vec1(real_vector.begin(), real_vector.end());
    vl_vector<TestsType, 5> vec2;
    vec2 = vec1; // use assignment operator

    for (size_t i = 0; i < real_vector.size(); ++i) {
        ASSERT(real_vector[i] == vec2.at(i))
    }
    ASSERT(vec2.capacity() == 5 && vec2.size() == 3 && !vec2.empty())
}

void test_operator_assignment_dynamic() {
    std::vector<TestsType> real_vector1{
            {1, 2},
            {3, 4},
            {5, 6},
    };
    std::vector<TestsType> real_vector2{
            {4, 4},
            {5, 5},
            {6, 6},
            {7, 7},
            {8, 8},
    };
    vl_vector<TestsType, 2> vec1(real_vector1.begin(), real_vector1.end());
    vl_vector<TestsType, 2> vec2(real_vector2.begin(), real_vector2.end());
    vec2 = vec1; // use assignment operator

    for (size_t i = 0; i < real_vector1.size(); ++i) {
        ASSERT(real_vector1[i] == vec2.at(i))
    }

    size_t expected_capacity = 4; // (3 * 3) / 2
    ASSERT(vec2.capacity() == expected_capacity
           && vec2.size() == 3
           && !vec2.empty())
}

void test_operator_assignment_static_to_dynamic() {
    std::vector<TestsType> real_vector1{
            {1, 1},
            {2, 2},
            {3, 3},
    };
    std::vector<TestsType> real_vector2{
            {4, 4},
            {5, 5},
            {6, 6},
            {7, 7},
            {8, 8},
    };
    vl_vector<TestsType, 4> static_vec(real_vector1.begin(),
                                       real_vector1.end());
    vl_vector<TestsType, 4> dynamic_vec(real_vector2.begin(),
                                        real_vector2.end());
    dynamic_vec = static_vec; // use assignment operator

    for (size_t i = 0; i < real_vector1.size(); ++i) {
        ASSERT(real_vector1[i] == dynamic_vec.at(i))
    }

    size_t expected_capacity = 4; // (3 * 3) / 2
    ASSERT(dynamic_vec.capacity() == expected_capacity
           && dynamic_vec.size() == 3
           && !dynamic_vec.empty())
}

void test_operator_assignment_dynamic_to_static() {
    std::vector<TestsType> real_vector1{
            {1, 1},
            {2, 2},
            {3, 3},
            {4, 4},
            {5, 5},
    };
    std::vector<TestsType> real_vector2{
            {6, 6},
            {7, 7},
            {8, 8},
    };

    vl_vector<TestsType, 4> dynamic_vec(real_vector1.begin(),
                                        real_vector1.end());
    vl_vector<TestsType, 4> static_vec(real_vector2.begin(),
                                       real_vector2.end());
    static_vec = dynamic_vec; // use assignment operator

    for (size_t i = 0; i < real_vector1.size(); ++i) {
        ASSERT(real_vector1[i] == static_vec.at(i))
    }

    size_t expected_capacity = 7; // (5 * 3) / 2
    ASSERT(static_vec.capacity() == expected_capacity
           && static_vec.size() == 5
           && !static_vec.empty())
}

void test_operator_equals_vectors_are_equal() {
    std::vector<TestsType> real_vector{
            {1, 1},
            {2, 2},
            {3, 3},
            {4, 4},
            {5, 5},
    };

    vl_vector<TestsType, 4> vec1(real_vector.begin(), real_vector.end());
    vl_vector<TestsType, 4> vec2(real_vector.begin(), real_vector.end());

    ASSERT(vec1 == vec2)
}

void test_operator_equals_vectors_are_not_equal() {
    std::vector<TestsType> real_vector{
            {1, 1},
            {2, 2},
            {3, 3},
            {4, 4},
            {5, 5},
    };

    vl_vector<TestsType, 3> vec1(real_vector.begin(), real_vector.end());
    vl_vector<TestsType, 3> vec2(real_vector.begin(), real_vector.end());
    vec2.pop_back();

    ASSERT(!(vec1 == vec2))
    ASSERT(!(vec2 == vec1))
}

void test_operator_not_equals_vectors_are_equal() {
    std::vector<TestsType> real_vector{
            {1, 1},
            {2, 2},
            {3, 3},
            {4, 4},
            {5, 5},
    };

    vl_vector<TestsType, 4> vec1(real_vector.begin(), real_vector.end());
    vl_vector<TestsType, 4> vec2(real_vector.begin(), real_vector.end());

    ASSERT(!(vec1 != vec2))
}

void test_operator_not_equals_vectors_are_not_equal() {
    std::vector<TestsType> real_vector{
            {1, 1},
            {2, 2},
            {3, 3},
            {4, 4},
            {5, 5},
    };

    vl_vector<TestsType, 3> vec1(real_vector.begin(), real_vector.end());
    vl_vector<TestsType, 3> vec2(real_vector.begin(), real_vector.end());
    vec2.pop_back();

    ASSERT(vec1 != vec2)
    ASSERT(vec2 != vec1)
}

void test_iterator_forward_static() {
    std::vector<TestsType> real_vector{
            {1, 1},
            {2, 2},
            {3, 3},
            {4, 4},
            {5, 5},
    };

    vl_vector<TestsType, 6> vec(real_vector.begin(), real_vector.end());
    const vl_vector<TestsType, 6> const_vec(real_vector.begin(),
                                            real_vector.end());

    auto it = real_vector.begin();
    for (const auto &value: vec) {
        ASSERT(value == *it)
        it++;
    }

    it = real_vector.begin();
    for (const auto &value: const_vec) {
        ASSERT(value == *it)
        it++;
    }
}


void test_iterator_forward_dynamic() {
    std::vector<TestsType> real_vector{
            {1, 1},
            {2, 2},
            {3, 3},
            {4, 4},
            {5, 5},
    };

    vl_vector<TestsType, 3> vec(real_vector.begin(), real_vector.end());
    const vl_vector<TestsType, 3> const_vec(real_vector.begin(),
                                            real_vector.end());

    auto it = real_vector.begin();
    for (const auto &value: vec) {
        ASSERT(value == *it)
        it++;
    }

    it = real_vector.begin();
    for (const auto &value: const_vec) {
        ASSERT(value == *it)
        it++;
    }
}

void test_iterator_reverse_static() {
    std::vector<TestsType> real_vector{
            {1, 1},
            {2, 2},
            {3, 3},
            {4, 4},
            {5, 5},
    };

    vl_vector<TestsType, 6> vec(real_vector.begin(), real_vector.end());
    const vl_vector<TestsType, 6> const_vec(real_vector.begin(),
                                            real_vector.end());

    auto real_it = real_vector.rbegin();
    auto vec_it = vec.rbegin();
    auto const_vec_it = const_vec.rbegin();
    while (real_it != real_vector.rend()) {
        ASSERT(*vec_it == *real_it)
        ASSERT(*const_vec_it == *real_it)

        real_it++;
        vec_it++;
        const_vec_it++;
    }

    ASSERT(vec_it == vec.rend())
    ASSERT(const_vec_it == const_vec.rend())
}


void test_iterator_reverse_dynamic() {
    std::vector<TestsType> real_vector{
            {1, 1},
            {2, 2},
            {3, 3},
            {4, 4},
            {5, 5},
    };

    vl_vector<TestsType, 3> vec(real_vector.begin(), real_vector.end());
    const vl_vector<TestsType, 3> const_vec(real_vector.begin(),
                                            real_vector.end());

    auto real_it = real_vector.rbegin();
    auto vec_it = vec.rbegin();
    auto const_vec_it = const_vec.rbegin();
    while (real_it != real_vector.rend()) {
        ASSERT(*vec_it == *real_it)
        ASSERT(*const_vec_it == *real_it)

        real_it++;
        vec_it++;
        const_vec_it++;
    }

    ASSERT(vec_it == vec.rend())
    ASSERT(const_vec_it == const_vec.rend())
}


void test_iterator_random_access() {
    std::vector<TestsType> real_vector{
            {1, 1},
            {2, 2},
            {3, 3},
            {4, 4},
            {5, 5},
    };

    const int forward_offset = 3;
    const int backward_offset = -2;

    vl_vector<TestsType, 3> vec(real_vector.begin(), real_vector.end());
    const vl_vector<TestsType, 3> const_vec(real_vector.begin(),
                                            real_vector.end());

    auto real_vector_it = real_vector.begin();
    auto vec_it = vec.begin();
    auto const_vec_it = const_vec.begin();

    real_vector_it += forward_offset;
    vec_it += forward_offset;
    const_vec_it += forward_offset;

    ASSERT(*vec_it == *real_vector_it)
    ASSERT(*const_vec_it == *real_vector_it)

    real_vector_it += backward_offset;
    vec_it += backward_offset;
    const_vec_it += backward_offset;

    ASSERT(*vec_it == *real_vector_it)
    ASSERT(*const_vec_it == *real_vector_it)
}

void test_iterator_write() {
    std::vector<TestsType> real_vector{
            {1, 1},
            {2, 2},
            {3, 3},
            {4, 4},
            {5, 5},
    };

    TestsType new_value = TestsType(6, 6);
    const size_t offset = 2;

    vl_vector<TestsType, 3> vec(real_vector.begin(), real_vector.end());

    auto real_vector_it = real_vector.begin();
    auto vec_it = vec.begin();

    real_vector_it += offset;
    vec_it += offset;

    *real_vector_it = new_value;
    *vec_it = new_value;

    auto it = real_vector.begin();
    for (const auto &value: vec) {
        ASSERT(value == *it)
        it++;
    }
}


int main() {
    std::vector<TestInfo> constructors_tests = {
            TEST_CASE(test_default_ctor_with_static_capacity),
            TEST_CASE(test_default_ctor_default_static_capacity),
            TEST_CASE(test_iterator_ctor_static),
            TEST_CASE(test_iterator_ctor_dynamic),
            TEST_CASE(test_iterator_ctor_zero_elements),
            TEST_CASE(test_copy_ctor_static),
            TEST_CASE(test_copy_ctor_dynamic),
            TEST_CASE(test_copy_ctor_zero_elements),
            TEST_CASE(test_value_ctor_dynamic),
            TEST_CASE(test_value_ctor_static),
            TEST_CASE(test_value_ctor_zero_elements),
    };

    std::vector<TestInfo> methods_tests = {
            TEST_CASE(test_at_invalid),
            TEST_CASE(test_operator_at_static),
            TEST_CASE(test_operator_at_dynamic),
            TEST_CASE(test_push_back_static),
            TEST_CASE(test_push_back_dynamic),
            TEST_CASE(test_push_back_static_to_dynamic),
            TEST_CASE(test_push_back_dynamic_increase_capacity),
            TEST_CASE(test_pop_back_static),
            TEST_CASE(test_pop_back_dynamic),
            TEST_CASE(test_pop_back_dynamic_to_static),
            TEST_CASE(test_pop_back_empty),
            TEST_CASE(test_clear_static),
            TEST_CASE(test_clear_dynamic),
            TEST_CASE(test_clear_and_operations),
            TEST_CASE(test_data_static),
            TEST_CASE(test_data_dynamic),
            TEST_CASE(test_contains_static),
            TEST_CASE(test_contains_dynamic),
            TEST_CASE(test_insert_single_static),
            TEST_CASE(test_insert_single_dynamic),
            TEST_CASE(test_insert_single_static_to_dynamic),
            TEST_CASE(test_insert_single_dynamic_increase_capacity),
            TEST_CASE(test_insert_iterator_static),
            TEST_CASE(test_insert_iterator_dynamic),
            TEST_CASE(test_insert_iterator_static_to_dynamic),
            TEST_CASE(test_insert_iterator_dynamic_increase_capacity),
            TEST_CASE(test_erase_single_static),
            TEST_CASE(test_erase_single_dynamic),
            TEST_CASE(test_erase_single_dynamic_to_static),
            TEST_CASE(test_erase_iterator_static),
            TEST_CASE(test_erase_iterator_dynamic),
            TEST_CASE(test_erase_iterator_dynamic_to_static),
    };

    std::vector<TestInfo> operators_tests = {
            TEST_CASE(test_operator_square_brackets_static),
            TEST_CASE(test_operator_square_brackets_dynamic),
            TEST_CASE(test_operator_assignment_static),
            TEST_CASE(test_operator_assignment_dynamic),
            TEST_CASE(test_operator_assignment_static_to_dynamic),
            TEST_CASE(test_operator_assignment_dynamic_to_static),
            TEST_CASE(test_operator_equals_vectors_are_equal),
            TEST_CASE(test_operator_equals_vectors_are_not_equal),
            TEST_CASE(test_operator_not_equals_vectors_are_equal),
            TEST_CASE(test_operator_not_equals_vectors_are_not_equal),
    };

    std::vector<TestInfo> iterator_tests = {
            TEST_CASE(test_iterator_forward_static),
            TEST_CASE(test_iterator_forward_dynamic),
            TEST_CASE(test_iterator_random_access),
            TEST_CASE(test_iterator_write),
            TEST_CASE(test_iterator_reverse_static),
            TEST_CASE(test_iterator_reverse_dynamic),
    };

    std::vector<TestSuite> test_suites{
            {"Constructors", constructors_tests},
            {"Methods",      methods_tests},
            {"Operators",    operators_tests},
            {"Iterator",     iterator_tests},
    };

    run_test_suites(test_suites);
}