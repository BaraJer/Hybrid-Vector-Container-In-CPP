#include <iostream>
#include <string>
#include "test_runner.h"

#define PASS_COLOR "\033[0;32m"
#define FAIL_COLOR "\033[0;31m"
#define PARTIAL_FAIL_COLOR "\e[0;33m"
#define DEFAULT_COLOR "\033[0m"

#define PASS_STR "PASSED"
#define FAIL_STR "FAILED"

using std::cout;
using std::string;
using std::endl;
using std::size_t;


void print_passing_str(bool result);

bool run_test(TestInfo test_info, int number, size_t amount_of_tests);

void setup() {
    setvbuf(stdout, NULL, _IONBF, 0);
}

void set_color(std::string color) {
    cout << color;
}

void print_in_color(string text, string color) {
    set_color(color);
    cout << text;
    set_color(DEFAULT_COLOR);
}

void print_test_passed(size_t passed, size_t amount) {
    set_color(passed == amount ? PASS_COLOR : PARTIAL_FAIL_COLOR);
    cout << passed << "\\" << amount << " tests passed " << endl;
    set_color(DEFAULT_COLOR);
}

void
print_suite_result(TestSuite test_suite, TestSuiteResult testSuiteResult) {
    cout << test_suite.name << ": ";
    print_test_passed(testSuiteResult.passed_count,
                      test_suite.tests.size());
}

void print_summary(const std::vector<TestSuite> &test_suites,
                   const std::vector<TestSuiteResult> &test_results) {
    cout << "************************************************" << endl;
    cout << "Finished running all tests !" << endl;
    cout << "Summary:" << endl;

    size_t total_tests = 0;
    size_t total_passed = 0;

    for (size_t i = 0; i < test_suites.size(); i++) {
        print_suite_result(test_suites[i], test_results[i]);
        total_tests += test_suites[i].tests.size();
        total_passed += test_results[i].passed_count;
    }

    cout << endl << "Total: ";
    print_test_passed(total_passed, total_tests);
    cout << "************************************************" << endl;
}

TestSuiteResult run_test_suite(TestSuite test_suite) {
    size_t passed_count = 0;
    for (size_t i = 0; i < test_suite.tests.size(); i++) {
        if (run_test(test_suite.tests[i], i + 1, test_suite.tests.size())) {
            passed_count++;
        }
    }

    return (TestSuiteResult) {passed_count};
}


void run_test_suites(const std::vector<TestSuite> &test_suites) {
    setup();
    cout << "-------------------------------------------------------" << endl;
    cout << "Running Tests:" << endl;
    cout << "\"=============" << endl;

    std::vector<TestSuiteResult> test_results;

    for (size_t i = 0; i < test_suites.size(); i++) {
        cout << "(" << i + 1 << "\\" << test_suites.size()
             << ") Running Test Suite - " << test_suites[i].name << ":"
             << endl;
        test_results.push_back(run_test_suite(test_suites[i]));
        cout << endl;
        print_suite_result(test_suites[i], test_results[i]);
        cout << "====================" << endl;
    }

    print_summary(test_suites, test_results);
}

bool run_test(TestInfo test_info, int number, size_t amount_of_tests) {
    bool result = true;
    string extra_info = "";
    cout << "    (" << number << "\\" << amount_of_tests << ") " <<
         test_info.description << " - ";
    try {
        test_info.func_ptr();
    } catch (const std::exception &err) {
        result = false;
        extra_info = err.what();
    }

    print_passing_str(result);
    if (!extra_info.empty()) {
        cout << " extra info: " << extra_info;
    }
    cout << endl;
    return result;
}

void print_passing_str(bool result) {
    print_in_color(result ? PASS_STR : FAIL_STR,
                   result ? PASS_COLOR : FAIL_COLOR);
}
