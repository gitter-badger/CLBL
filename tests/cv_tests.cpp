#include "test.h"
#include <CLBL/clbl.h>
#include "int_char_definitions.h"

#include<functional>
#include<iostream>

#include<iostream>

using namespace clbl::tests;
using namespace clbl;

auto int_char_std_function_result(std::function<const char*(int, char)> f) {
    return f(1, 'c');
}

void call_std_function(std::function<void(int)> f) {
    f(0);
}

struct mutable_struct {
    int value;
    static int copy_count;

    mutable_struct(int i) : value(i) {}

    void increment(int i) {
        ++value;
    }

    mutable_struct(mutable_struct&&) = default;
    mutable_struct(const mutable_struct&) { ++copy_count; }
    mutable_struct(mutable_struct&) { ++copy_count; }
};

int mutable_struct::copy_count = 0;

void conversion_tests() {

#ifdef CLBL_CV_TESTS

#define CLBL_CV_TESTS_1
#define CLBL_CV_TESTS_2
#define CLBL_CV_TESTS_3
#define CLBL_CV_TESTS_4
#define CLBL_CV_TESTS_5
#define CLBL_CV_TESTS_6
#define CLBL_CV_TESTS_7
#define CLBL_CV_TESTS_8
#define CLBL_CV_TESTS_9
#define CLBL_CV_TESTS_10
#define CLBL_CV_TESTS_11

    std::cout << "running CLBL_CV_TESTS" << std::endl;

    #ifdef CLBL_CV_TESTS_1
    {
        //testing object reference preservation when converted to std::function
        auto mutable_object = mutable_struct{ 0 };
        auto f = fwrap(&mutable_object, &mutable_struct::increment);
        auto stdf = convert_to<std::function>(f);

        f(0);
        stdf(0);
        call_std_function(stdf);

        TEST(mutable_object.value == 3);
    }
    #endif
    #ifdef CLBL_CV_TESTS_2
    {
        //testing cv-correctness of clbl callables converted to std::function
        auto overloaded_object = overloaded_int_char_struct{};

        auto normal = fwrap(&overloaded_object);
        auto c = fwrap(&overloaded_object);
        auto v = fwrap(&overloaded_object);
        auto cv = fwrap(&overloaded_object);

        auto hnormal = harden<const char*(int, char)>(normal);
        auto hc = harden<const char*(int, char) const>(c);
        auto hv = harden<const char*(int, char) volatile>(v);
        auto hcv = harden<const char*(int, char) const volatile>(cv);

        TEST(hnormal(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(hc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(hv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(hcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        auto stdn = convert_to<std::function>(hnormal);
        auto stdc = convert_to<std::function>(hc);
        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        TEST(stdn(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(stdc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(stdv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(stdcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        TEST(int_char_std_function_result(stdn) == test_id::overloaded_int_char_struct_op);
        TEST(int_char_std_function_result(stdc) == test_id::overloaded_int_char_struct_op_c);
        TEST(int_char_std_function_result(stdv) == test_id::overloaded_int_char_struct_op_v);
        TEST(int_char_std_function_result(stdcv) == test_id::overloaded_int_char_struct_op_cv);

        auto rehardened = harden<const char*(int, char) const>(hnormal);
        TEST(rehardened(1, 'c') == test_id::overloaded_int_char_struct_op_c);

        auto rehardened_again = harden<auto_(int, char) const volatile>(rehardened);
        TEST(rehardened_again(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        auto rehardened_v = harden<const char*(int, char) volatile>(hnormal);
        TEST(rehardened_v(1, 'c') == test_id::overloaded_int_char_struct_op_v);

        auto rehardened_again_v = harden<auto_(int, char) const volatile>(rehardened_v);
        TEST(rehardened_again_v(1, 'c') == test_id::overloaded_int_char_struct_op_cv);
    }
    #endif
#ifdef CLBL_CV_TESTS_3
    {
        auto overloaded_object = overloaded_int_char_struct{};

        auto normal = fwrap(&overloaded_object);
        auto c = fwrap(&overloaded_object);
        auto v = fwrap(&overloaded_object);
        auto cv = fwrap(&overloaded_object);

        /*CLBL_PRINT_NAME_AND_TYPE(normal);
        CLBL_PRINT_NAME_AND_TYPE(c);
        CLBL_PRINT_NAME_AND_TYPE(v);
        CLBL_PRINT_NAME_AND_TYPE(cv);*/

        auto nested_normal = fwrap(&normal);
        auto nested_c = fwrap(&c);
        auto nested_v = fwrap(&v);
        auto nested_cv = fwrap(&cv);

        /*CLBL_PRINT_NAME_AND_TYPE(nested_normal);
        CLBL_PRINT_NAME_AND_TYPE(nested_c);
        CLBL_PRINT_NAME_AND_TYPE(nested_v);
        CLBL_PRINT_NAME_AND_TYPE(nested_cv);*/

        TEST(nested_normal(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(nested_c(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(nested_v(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(nested_cv(1, 'c') == test_id::overloaded_int_char_struct_op);

        auto hnormal = harden<auto_(int, char)>(nested_normal);
        auto hc = harden<auto_(int, char) const>(nested_c);
        auto hv = harden<auto_(int, char) volatile>(nested_v);
        auto hcv = harden<auto_(int, char) const volatile>(nested_cv);

        /*CLBL_PRINT_NAME_AND_TYPE(hnormal);
        CLBL_PRINT_NAME_AND_TYPE(hc);
        CLBL_PRINT_NAME_AND_TYPE(hv);
        CLBL_PRINT_NAME_AND_TYPE(hcv);*/

        TEST(hnormal(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(hc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(hv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(hcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        auto stdn = convert_to<std::function>(hnormal);
        auto stdc = convert_to<std::function>(hc);
        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        /*CLBL_PRINT_NAME_AND_TYPE(stdn);
        CLBL_PRINT_NAME_AND_TYPE(stdc);
        CLBL_PRINT_NAME_AND_TYPE(stdv);
        CLBL_PRINT_NAME_AND_TYPE(stdcv);*/

        TEST(stdn(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(stdc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(stdv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(stdcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        TEST(int_char_std_function_result(stdn) == test_id::overloaded_int_char_struct_op);
        TEST(int_char_std_function_result(stdc) == test_id::overloaded_int_char_struct_op_c);
        TEST(int_char_std_function_result(stdv) == test_id::overloaded_int_char_struct_op_v);
        TEST(int_char_std_function_result(stdcv) == test_id::overloaded_int_char_struct_op_cv);

        auto rehardened = harden<const char*(int, char) const>(hnormal);
        TEST(rehardened(1, 'c') == test_id::overloaded_int_char_struct_op_c);

        auto rehardened_again = harden<auto_(int, char) const volatile>(rehardened);
        TEST(rehardened_again(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        auto rehardened_v = harden<const char*(int, char) volatile>(hnormal);
        TEST(rehardened_v(1, 'c') == test_id::overloaded_int_char_struct_op_v);

        auto rehardened_again_v = harden<auto_(int, char) const volatile>(rehardened_v);
        TEST(rehardened_again_v(1, 'c') == test_id::overloaded_int_char_struct_op_cv);
    }
    {
        auto overloaded_object = overloaded_int_char_struct{};

        auto normal = fwrap(&overloaded_object);
        auto c = fwrap(&overloaded_object);
        auto v = fwrap(&overloaded_object);
        auto cv = fwrap(&overloaded_object);

        /*CLBL_PRINT_NAME_AND_TYPE(normal);
        CLBL_PRINT_NAME_AND_TYPE(c);
        CLBL_PRINT_NAME_AND_TYPE(v);
        CLBL_PRINT_NAME_AND_TYPE(cv);*/

        auto nested_normal_intermediate = fwrap(&normal);
        auto nested_c_intermediate = fwrap(&c);
        auto nested_v_intermediate = fwrap(&v);
        auto nested_cv_intermediate = fwrap(&cv);

        auto nested_normal = fwrap(&nested_normal_intermediate);
        auto nested_c = fwrap(&nested_c_intermediate);
        auto nested_v = fwrap(&nested_v_intermediate);
        auto nested_cv = fwrap(&nested_cv_intermediate);

        /*CLBL_PRINT_NAME_AND_TYPE(nested_normal);
        CLBL_PRINT_NAME_AND_TYPE(nested_c);
        CLBL_PRINT_NAME_AND_TYPE(nested_v);
        CLBL_PRINT_NAME_AND_TYPE(nested_cv);*/

        TEST(nested_normal(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(nested_c(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(nested_v(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(nested_cv(1, 'c') == test_id::overloaded_int_char_struct_op);

        auto hnormal = harden<auto_(int, char)>(nested_normal);
        auto hc = harden<auto_(int, char) const>(nested_c);
        auto hv = harden<auto_(int, char) volatile>(nested_v);
        auto hcv = harden<auto_(int, char) const volatile>(nested_cv);

        /*CLBL_PRINT_NAME_AND_TYPE(hnormal);
        CLBL_PRINT_NAME_AND_TYPE(hc);
        CLBL_PRINT_NAME_AND_TYPE(hv);
        CLBL_PRINT_NAME_AND_TYPE(hcv);*/

        TEST(hnormal(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(hc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(hv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(hcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        auto stdn = convert_to<std::function>(hnormal);
        auto stdc = convert_to<std::function>(hc);
        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        /*CLBL_PRINT_NAME_AND_TYPE(stdn);
        CLBL_PRINT_NAME_AND_TYPE(stdc);
        CLBL_PRINT_NAME_AND_TYPE(stdv);
        CLBL_PRINT_NAME_AND_TYPE(stdcv);*/

        TEST(stdn(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(stdc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(stdv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(stdcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        TEST(int_char_std_function_result(stdn) == test_id::overloaded_int_char_struct_op);
        TEST(int_char_std_function_result(stdc) == test_id::overloaded_int_char_struct_op_c);
        TEST(int_char_std_function_result(stdv) == test_id::overloaded_int_char_struct_op_v);
        TEST(int_char_std_function_result(stdcv) == test_id::overloaded_int_char_struct_op_cv);

        auto rehardened = harden<const char*(int, char) const>(hnormal);
        TEST(rehardened(1, 'c') == test_id::overloaded_int_char_struct_op_c);

        auto rehardened_again = harden<auto_(int, char) const volatile>(rehardened);
        TEST(rehardened_again(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        auto rehardened_v = harden<const char*(int, char) volatile>(hnormal);
        TEST(rehardened_v(1, 'c') == test_id::overloaded_int_char_struct_op_v);

        auto rehardened_again_v = harden<auto_(int, char) const volatile>(rehardened_v);
        TEST(rehardened_again_v(1, 'c') == test_id::overloaded_int_char_struct_op_cv);
    }
#endif
#ifdef CLBL_CV_TESTS_4
    {
        auto overloaded_object = overloaded_int_char_struct{};

        auto normal = fwrap(&overloaded_object);
        const auto c = fwrap(&overloaded_object);
        volatile auto v = fwrap(&overloaded_object);
        const volatile auto cv = fwrap(&overloaded_object);

        auto hnormal = harden<const char*(int, char)>(normal);
        auto hc = harden<const char*(int, char) const>(c);
        auto hv = harden<const char*(int, char) volatile>(v);
        auto hcv = harden<const char*(int, char) const volatile>(cv);

        auto stdn = convert_to<std::function>(hnormal);
        auto stdc = convert_to<std::function>(hc);
        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        static_assert(std::is_same<
            decltype(stdn), 
            std::function<const char* (forward<int>, forward<char>) > 
        >::value, "");

        TEST(stdn(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(stdc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(stdv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(stdcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        TEST(int_char_std_function_result(stdn) == test_id::overloaded_int_char_struct_op);
        TEST(int_char_std_function_result(stdc) == test_id::overloaded_int_char_struct_op_c);
        TEST(int_char_std_function_result(stdv) == test_id::overloaded_int_char_struct_op_v);
        TEST(int_char_std_function_result(stdcv) == test_id::overloaded_int_char_struct_op_cv);

        auto rehardened = harden<const char*(int, char) const>(hnormal);
        TEST(rehardened(1, 'c') == test_id::overloaded_int_char_struct_op_c);

        auto rehardened_again = harden<auto_(int, char) const volatile>(rehardened);
        TEST(rehardened_again(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        auto rehardened_v = harden<const char*(int, char) volatile>(hnormal);
        TEST(rehardened_v(1, 'c') == test_id::overloaded_int_char_struct_op_v);

        auto rehardened_again_v = harden<auto_(int, char) const volatile>(rehardened_v);
        TEST(rehardened_again_v(1, 'c') == test_id::overloaded_int_char_struct_op_cv);
    }
#endif
#ifdef CLBL_CV_TESTS_5
    {
        const auto overloaded_object = overloaded_int_char_struct{};

        //auto normal = fwrap(&overloaded_object);
        const auto c = fwrap(&overloaded_object);
        //volatile auto v = fwrap(&overloaded_object);
        const volatile auto cv = fwrap(&overloaded_object);

        //should cause compile error if called: auto hnormal = harden<const char*(int, char)>(normal);
        const auto hc = harden<auto_(int, char) const>(c);
        //should cause compile error if called: volatile auto hv = harden<const char*(int, char) volatile>(v);
        const volatile auto hcv = harden<auto_(int, char) const volatile>(cv);

        auto stdc = convert_to<std::function>(hc);
        auto stdcv = convert_to<std::function>(hcv);

        TEST(stdc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(stdcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        TEST(int_char_std_function_result(stdc) == test_id::overloaded_int_char_struct_op_c);
        TEST(int_char_std_function_result(stdcv) == test_id::overloaded_int_char_struct_op_cv);
    }
#endif
#ifdef CLBL_CV_TESTS_6
    {
        volatile auto overloaded_object = overloaded_int_char_struct{};

        //auto normal = fwrap(&overloaded_object);
        //const auto c = fwrap(&overloaded_object);
        volatile auto v = fwrap(&overloaded_object);
        const volatile auto cv = fwrap(&overloaded_object);

        //should cause compile error if called: auto hnormal = harden<const char*(int, char)>(normal);
        //should cause compile error if called: const auto hc = harden<const char*(int, char) const>(c);
        volatile auto hv = harden<const char*(int, char) volatile>(v);
        const volatile auto hcv = harden<const char*(int, char) const volatile>(cv);

        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        TEST(stdv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(stdcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        TEST(int_char_std_function_result(stdv) == test_id::overloaded_int_char_struct_op_v);
        TEST(int_char_std_function_result(stdcv) == test_id::overloaded_int_char_struct_op_cv);
    }
#endif
#ifdef CLBL_CV_TESTS_7
    {
        //testing cv-correctness of clbl callables converted to std::function
        auto overloaded_object = overloaded_int_char_struct{};

        auto normal = fwrap(overloaded_object);
        auto c = fwrap(overloaded_object);
        auto v = fwrap(overloaded_object);
        auto cv = fwrap(overloaded_object);

        auto hnormal = harden<const char*(int, char)>(normal);
        auto hc = harden<const char*(int, char) const>(c);
        auto hv = harden<const char*(int, char) volatile>(v);
        auto hcv = harden<const char*(int, char) const volatile>(cv);

        TEST(hnormal(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(hc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(hv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(hcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        auto stdn = convert_to<std::function>(hnormal);
        auto stdc = convert_to<std::function>(hc);
        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        TEST(stdn(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(stdc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(stdv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(stdcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        TEST(int_char_std_function_result(stdn) == test_id::overloaded_int_char_struct_op);
        TEST(int_char_std_function_result(stdc) == test_id::overloaded_int_char_struct_op_c);
        TEST(int_char_std_function_result(stdv) == test_id::overloaded_int_char_struct_op_v);
        TEST(int_char_std_function_result(stdcv) == test_id::overloaded_int_char_struct_op_cv);

        auto rehardened = harden<const char*(int, char) const>(hnormal);
        TEST(rehardened(1, 'c') == test_id::overloaded_int_char_struct_op_c);

        auto rehardened_again = harden<auto_(int, char) const volatile>(rehardened);
        TEST(rehardened_again(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        auto rehardened_v = harden<const char*(int, char) volatile>(hnormal);
        TEST(rehardened_v(1, 'c') == test_id::overloaded_int_char_struct_op_v);

        auto rehardened_again_v = harden<auto_(int, char) const volatile>(rehardened_v);
        TEST(rehardened_again_v(1, 'c') == test_id::overloaded_int_char_struct_op_cv);
    }
#endif
#ifdef CLBL_CV_TESTS_8
    {
        auto overloaded_object = overloaded_int_char_struct{};

        auto normal = fwrap(overloaded_object);
        auto c = fwrap(overloaded_object);
        auto v = fwrap(overloaded_object);
        auto cv = fwrap(overloaded_object);

        /*CLBL_PRINT_NAME_AND_TYPE(normal);
        CLBL_PRINT_NAME_AND_TYPE(c);
        CLBL_PRINT_NAME_AND_TYPE(v);
        CLBL_PRINT_NAME_AND_TYPE(cv);*/

        auto nested_normal = fwrap(normal);
        auto nested_c = fwrap(c);
        auto nested_v = fwrap(v);
        auto nested_cv = fwrap(cv);

        /*CLBL_PRINT_NAME_AND_TYPE(nested_normal);
        CLBL_PRINT_NAME_AND_TYPE(nested_c);
        CLBL_PRINT_NAME_AND_TYPE(nested_v);
        CLBL_PRINT_NAME_AND_TYPE(nested_cv);*/

        TEST(nested_normal(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(nested_c(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(nested_v(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(nested_cv(1, 'c') == test_id::overloaded_int_char_struct_op);

        auto hnormal = harden<const char*(int, char)>(nested_normal);
        auto hc = harden<const char*(int, char) const>(nested_c);
        auto hv = harden<const char*(int, char) volatile>(nested_v);
        auto hcv = harden<const char*(int, char) const volatile>(nested_cv);

        /*CLBL_PRINT_NAME_AND_TYPE(hnormal);
        CLBL_PRINT_NAME_AND_TYPE(hc);
        CLBL_PRINT_NAME_AND_TYPE(hv);
        CLBL_PRINT_NAME_AND_TYPE(hcv);*/

        TEST(hnormal(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(hc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(hv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(hcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        auto stdn = convert_to<std::function>(hnormal);
        auto stdc = convert_to<std::function>(hc);
        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        /*CLBL_PRINT_NAME_AND_TYPE(stdn);
        CLBL_PRINT_NAME_AND_TYPE(stdc);
        CLBL_PRINT_NAME_AND_TYPE(stdv);
        CLBL_PRINT_NAME_AND_TYPE(stdcv);*/

        TEST(stdn(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(stdc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(stdv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(stdcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        TEST(int_char_std_function_result(stdn) == test_id::overloaded_int_char_struct_op);
        TEST(int_char_std_function_result(stdc) == test_id::overloaded_int_char_struct_op_c);
        TEST(int_char_std_function_result(stdv) == test_id::overloaded_int_char_struct_op_v);
        TEST(int_char_std_function_result(stdcv) == test_id::overloaded_int_char_struct_op_cv);

        auto rehardened = harden<const char*(int, char) const>(hnormal);
        TEST(rehardened(1, 'c') == test_id::overloaded_int_char_struct_op_c);

        auto rehardened_again = harden<auto_(int, char) const volatile>(rehardened);
        TEST(rehardened_again(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        auto rehardened_v = harden<const char*(int, char) volatile>(hnormal);
        TEST(rehardened_v(1, 'c') == test_id::overloaded_int_char_struct_op_v);

        auto rehardened_again_v = harden<auto_(int, char) const volatile>(rehardened_v);
        TEST(rehardened_again_v(1, 'c') == test_id::overloaded_int_char_struct_op_cv);
    }
    {
        auto overloaded_object = overloaded_int_char_struct{};

        auto normal = fwrap(overloaded_object);
        auto c = fwrap(overloaded_object);
        auto v = fwrap(overloaded_object);
        auto cv = fwrap(overloaded_object);

        /*CLBL_PRINT_NAME_AND_TYPE(normal);
        CLBL_PRINT_NAME_AND_TYPE(c);
        CLBL_PRINT_NAME_AND_TYPE(v);
        CLBL_PRINT_NAME_AND_TYPE(cv);*/

        auto nested_normal_intermediate = fwrap(normal);
        auto nested_c_intermediate = fwrap(c);
        auto nested_v_intermediate = fwrap(v);
        auto nested_cv_intermediate = fwrap(cv);

        auto nested_normal = fwrap(nested_normal_intermediate);
        auto nested_c = fwrap(nested_c_intermediate);
        auto nested_v = fwrap(nested_v_intermediate);
        auto nested_cv = fwrap(nested_cv_intermediate);

        /*CLBL_PRINT_NAME_AND_TYPE(nested_normal);
        CLBL_PRINT_NAME_AND_TYPE(nested_c);
        CLBL_PRINT_NAME_AND_TYPE(nested_v);
        CLBL_PRINT_NAME_AND_TYPE(nested_cv);*/

        TEST(nested_normal(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(nested_c(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(nested_v(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(nested_cv(1, 'c') == test_id::overloaded_int_char_struct_op);

        auto hnormal = harden<const char*(int, char)>(nested_normal);
        auto hc = harden<const char*(int, char) const>(nested_c);
        auto hv = harden<const char*(int, char) volatile>(nested_v);
        auto hcv = harden<const char*(int, char) const volatile>(nested_cv);

        /*CLBL_PRINT_NAME_AND_TYPE(hnormal);
        CLBL_PRINT_NAME_AND_TYPE(hc);
        CLBL_PRINT_NAME_AND_TYPE(hv);
        CLBL_PRINT_NAME_AND_TYPE(hcv);*/

        TEST(hnormal(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(hc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(hv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(hcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        auto stdn = convert_to<std::function>(hnormal);
        auto stdc = convert_to<std::function>(hc);
        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        /*CLBL_PRINT_NAME_AND_TYPE(stdn);
        CLBL_PRINT_NAME_AND_TYPE(stdc);
        CLBL_PRINT_NAME_AND_TYPE(stdv);
        CLBL_PRINT_NAME_AND_TYPE(stdcv);*/

        TEST(stdn(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(stdc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(stdv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(stdcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        TEST(int_char_std_function_result(stdn) == test_id::overloaded_int_char_struct_op);
        TEST(int_char_std_function_result(stdc) == test_id::overloaded_int_char_struct_op_c);
        TEST(int_char_std_function_result(stdv) == test_id::overloaded_int_char_struct_op_v);
        TEST(int_char_std_function_result(stdcv) == test_id::overloaded_int_char_struct_op_cv);

        auto rehardened = harden<const char*(int, char) const>(hnormal);
        TEST(rehardened(1, 'c') == test_id::overloaded_int_char_struct_op_c);

        auto rehardened_again = harden<auto_(int, char) const volatile>(rehardened);
        TEST(rehardened_again(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        auto rehardened_v = harden<const char*(int, char) volatile>(hnormal);
        TEST(rehardened_v(1, 'c') == test_id::overloaded_int_char_struct_op_v);

        auto rehardened_again_v = harden<auto_(int, char) const volatile>(rehardened_v);
        TEST(rehardened_again_v(1, 'c') == test_id::overloaded_int_char_struct_op_cv);
    }
#endif
#ifdef CLBL_CV_TESTS_9
    {
        auto overloaded_object = overloaded_int_char_struct{};

        auto normal = fwrap(overloaded_object);
        const auto c = fwrap(overloaded_object);
        volatile auto v = fwrap(overloaded_object);
        const volatile auto cv = fwrap(overloaded_object);

        auto hnormal = harden<const char*(int, char)>(normal);
        auto hc = harden<const char*(int, char) const>(c);
        auto hv = harden<const char*(int, char) volatile>(v);
        auto hcv = harden<const char*(int, char) const volatile>(cv);

        auto stdn = convert_to<std::function>(hnormal);
        auto stdc = convert_to<std::function>(hc);
        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        static_assert(std::is_same<
            decltype(stdn),
            std::function<const char* (forward<int>, forward<char>) >
        >::value, "");

        TEST(stdn(1, 'c') == test_id::overloaded_int_char_struct_op);
        TEST(stdc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(stdv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(stdcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        TEST(int_char_std_function_result(stdn) == test_id::overloaded_int_char_struct_op);
        TEST(int_char_std_function_result(stdc) == test_id::overloaded_int_char_struct_op_c);
        TEST(int_char_std_function_result(stdv) == test_id::overloaded_int_char_struct_op_v);
        TEST(int_char_std_function_result(stdcv) == test_id::overloaded_int_char_struct_op_cv);

        auto rehardened = harden<const char*(int, char) const>(hnormal);
        TEST(rehardened(1, 'c') == test_id::overloaded_int_char_struct_op_c);

        auto rehardened_again = harden<auto_(int, char) const volatile>(rehardened);
        TEST(rehardened_again(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        auto rehardened_v = harden<const char*(int, char) volatile>(hnormal);
        TEST(rehardened_v(1, 'c') == test_id::overloaded_int_char_struct_op_v);

        auto rehardened_again_v = harden<auto_(int, char) const volatile>(rehardened_v);
        TEST(rehardened_again_v(1, 'c') == test_id::overloaded_int_char_struct_op_cv);
    }
#endif
#ifdef CLBL_CV_TESTS_10
    {
        const auto overloaded_object = overloaded_int_char_struct{};

        //auto normal = fwrap(overloaded_object);
        const auto c = fwrap(overloaded_object);
        //volatile auto v = fwrap(overloaded_object);
        const volatile auto cv = fwrap(overloaded_object);

        //should cause compile error if called: auto hnormal = harden<const char*(int, char)>(normal);
        const auto hc = harden<const char*(int, char) const>(c);
        //should cause compile error if called: volatile auto hv = harden<const char*(int, char) volatile>(v);
        const volatile auto hcv = harden<const char*(int, char) const volatile>(cv);

        auto stdc = convert_to<std::function>(hc);
        auto stdcv = convert_to<std::function>(hcv);

        TEST(stdc(1, 'c') == test_id::overloaded_int_char_struct_op_c);
        TEST(stdcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        TEST(int_char_std_function_result(stdc) == test_id::overloaded_int_char_struct_op_c);
        TEST(int_char_std_function_result(stdcv) == test_id::overloaded_int_char_struct_op_cv);
    }
#endif
#ifdef CLBL_CV_TESTS_11
    {
        volatile auto overloaded_object = overloaded_int_char_struct{};

        //auto normal = fwrap(&overloaded_object);
        //const auto c = fwrap(&overloaded_object);
        volatile auto v = fwrap(overloaded_object);
        const volatile auto cv = fwrap(overloaded_object);

        //should cause compile error if called: auto hnormal = harden<const char*(int, char)>(normal);
        //should cause compile error if called: const auto hc = harden<const char*(int, char) const>(c);
        volatile auto hv = harden<const char*(int, char) volatile>(v);
        const volatile auto hcv = harden<const char*(int, char) const volatile>(cv);

        auto stdv = convert_to<std::function>(hv);
        auto stdcv = convert_to<std::function>(hcv);

        TEST(stdv(1, 'c') == test_id::overloaded_int_char_struct_op_v);
        TEST(stdcv(1, 'c') == test_id::overloaded_int_char_struct_op_cv);

        TEST(int_char_std_function_result(stdv) == test_id::overloaded_int_char_struct_op_v);
        TEST(int_char_std_function_result(stdcv) == test_id::overloaded_int_char_struct_op_cv);
    }
#endif
#endif
}