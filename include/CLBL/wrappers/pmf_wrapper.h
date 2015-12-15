#ifndef CLBL_MEMBER_FUNCTION_H
#define CLBL_MEMBER_FUNCTION_H

#include <type_traits>

#include "CLBL/cv_checks.h"
#include "CLBL/callable.h"
#include "CLBL/utility.h"

namespace clbl {

    template<typename, qualify_flags, typename, typename, typename Failure>
    struct pmf_wrapper {static_assert(sizeof(Failure) < 0, "Not a member function."); };

    template<typename Creator, qualify_flags CvFlags, typename T, typename TMemberFnPtr, typename Return, typename... Args>
    struct pmf_wrapper<Creator, CvFlags, T, TMemberFnPtr, Return(std::remove_cv_t<T>::*)(Args...)> {

        static constexpr qualify_flags cv_flags = CvFlags;
        using creator = Creator;
        using clbl_tag = pmf_tag;
        using type = Return(Args...);
        using args_t = hana::tuple<Args...>;
        using return_t = Return;
        using my_type = pmf_wrapper<Creator, cv_flags, T, TMemberFnPtr, Return(std::remove_cv_t<T>::*)(Args...)>;
        using qualified_object = apply_qualifiers<T, cv_flags>;
        using underlying_type = qualified_object;

        TMemberFnPtr _value;
        qualified_object _object;

        template<qualify_flags Flags>
        using apply_cv = pmf_wrapper<Creator, cv_flags | Flags, T, TMemberFnPtr, Return(std::remove_cv_t<T>::*)(Args...)>;

        inline pmf_wrapper(TMemberFnPtr f_ptr, T&& o)
            : _value(f_ptr), _object(o)
        {}

        inline pmf_wrapper(TMemberFnPtr f_ptr, T& o)
            :_value(f_ptr), _object(o)
        {}

        inline pmf_wrapper(my_type& other) = default;
        inline pmf_wrapper(const my_type& other) = default;
        inline pmf_wrapper(my_type&& other) = default;

        inline pmf_wrapper(volatile my_type& other)
            : _object(other._object), _value(other._value)
        {}

        inline pmf_wrapper(const volatile my_type& other)
            : _object(other._object), _value(other._value)
        {}

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) {
            return CLBL_UPCAST_AND_CALL_MEMBER_VAL(CLBL_NOTHING, _object, _value, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const {
            return CLBL_UPCAST_AND_CALL_MEMBER_VAL(const, _object, _value, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) volatile {
            return CLBL_UPCAST_AND_CALL_MEMBER_VAL(volatile, _object, _value, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const volatile {
            return CLBL_UPCAST_AND_CALL_MEMBER_VAL(const volatile, _object, _value, std::forward<Fargs>(a)...);
        }

        static inline constexpr auto copy_invocation(my_type& c) {
            return[v = c._value, o = c._object](auto&&... args){ 
                return CLBL_UPCAST_AND_CALL_MEMBER_VAL(CLBL_NOTHING, o, v, args...);
            };
        }

        static inline constexpr auto copy_invocation(const my_type& c) {
            return[v = c._value, o = c._object](auto&&... args){ 
                return CLBL_UPCAST_AND_CALL_MEMBER_VAL(const, o, v, args...);
            };
        }

        static inline constexpr auto copy_invocation(volatile my_type& c) {
            return[v = c._value, o = c._object](auto&&... args){ 
                return CLBL_UPCAST_AND_CALL_MEMBER_VAL(volatile, o, v, args...);
            };
        }

        static inline constexpr auto copy_invocation(const volatile my_type& c) {
            return[v = c._value, o = c._object](auto&&... args){
                return CLBL_UPCAST_AND_CALL_MEMBER_VAL(const volatile, o, v, args...);};
        }
    };


}

#endif