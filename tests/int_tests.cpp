#include "test.h"
#include <CLBL/clbl.h>
#include "int_definitions.h"

#include <iostream>
#include <functional>

using namespace clbl::tests;
using namespace clbl;

void int_tests() {

#ifdef CLBL_INT_TESTS
    std::cout << "running CLBL_INT_TESTS" << std::endl;

    int_struct int_object{};

    auto f = fwrap(&int_object);
    auto g = fwrap(&int_func);
    auto h = fwrap(&int_object, &int_struct::func);
    auto hardened_f = harden<const char*(int)>(f);
    auto hardened_g = harden<const char*(int)>(g);
    auto hardened_h = harden<const char*(int)>(h);
    auto default_hardened_f = harden(f);
    auto default_hardened_g = harden(g);
    auto default_hardened_h = harden(h);

    /*todo static_assert(!can_call(f, 1, 2), "");
    static_assert(can_call(f, 1), "");
    static_assert(!can_call(f, "illegal argument"), "");*/

    static_assert(std::is_same<decltype(f)::type, const char*(int)>::value, "std::is_same<decltype(f)::type, const char*(int)>::value");

    run_tests(
        f, test_id::int_struct_op,
        g, test_id::int_func,
        h, test_id::int_struct_func,
        1
        );

    run_tests(
        hardened_f, test_id::int_struct_op,
        hardened_g, test_id::int_func,
        hardened_h, test_id::int_struct_func,
        1
        );

    run_tests(
        default_hardened_f, test_id::int_struct_op,
        default_hardened_g, test_id::int_func,
        default_hardened_h, test_id::int_struct_func,
        1
        );

    auto j = fwrap(int_object);
    auto k = fwrap(&int_func);
    auto l = fwrap(int_object, &int_struct::func);
    auto hardened_j = harden<const char*(int)>(j);
    auto hardened_k = harden<const char*(int)>(k);
    auto hardened_l = harden<const char*(int)>(l);
    auto default_hardened_j = harden(j);
    auto default_hardened_k = harden(k);
    auto default_hardened_l = harden(l);

    run_tests(
        j, test_id::int_struct_op,
        k, test_id::int_func,
        l, test_id::int_struct_func,
        1
        );

    run_tests(
        hardened_j, test_id::int_struct_op,
        hardened_k, test_id::int_func,
        hardened_l, test_id::int_struct_func,
        1
        );

    run_tests(
        default_hardened_j, test_id::int_struct_op,
        default_hardened_k, test_id::int_func,
        default_hardened_l, test_id::int_struct_func,
        1
        );

    auto m = fwrap(std::ref(int_object));
    auto n = fwrap(&int_func);
    auto o = fwrap(std::ref(int_object), &int_struct::func);
    auto hardened_m = harden<const char*(int)>(m);
    auto hardened_n = harden<const char*(int)>(n);
    auto hardened_o = harden<const char*(int)>(o);
    auto default_hardened_m = harden(m);
    auto default_hardened_n = harden(n);
    auto default_hardened_o = harden(o);

    run_tests(
        m, test_id::int_struct_op,
        n, test_id::int_func,
        o, test_id::int_struct_func,
        1
        );

    run_tests(
        hardened_m, test_id::int_struct_op,
        hardened_n, test_id::int_func,
        hardened_o, test_id::int_struct_func,
        1
        );

    run_tests(
        default_hardened_m, test_id::int_struct_op,
        default_hardened_n, test_id::int_func,
        default_hardened_o, test_id::int_struct_func,
        1
        );
#endif
}