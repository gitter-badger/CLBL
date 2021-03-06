#ifndef CLBL_PMF_WRAPPER_H
#define CLBL_PMF_WRAPPER_H

#include <type_traits>
#include <tuple>

#include <CLBL/cv.h>
#include <CLBL/utility.h>
#include <CLBL/forward.h>
#include <CLBL/forwardable.h>
#include <CLBL/harden_cast.h>
#include <CLBL/invocation_macros.h>
#include <CLBL/invocation_data.h>

namespace clbl {

    /*
    pmf_wrapper wraps a PMF and an object with which to call it.
    */
    template<typename, qualify_flags, typename,
        typename, typename Failure>
    struct pmf_wrapper {static_assert(sizeof(Failure) < 0, "Not a member function."); };

    template<typename Creator, qualify_flags CvFlags, typename T, 
        typename TMemberFnPtr, typename Return, typename... Args>
    struct pmf_wrapper<Creator, CvFlags, T,
        TMemberFnPtr, Return(std::remove_cv_t<T>::*)(Args...)> {

        using decayed_member_fn_ptr = Return(std::remove_cv_t<T>::*)(Args...);

        using arg_types = std::tuple<Args...>;
        using clbl_tag = pmf_tag;
        using creator = Creator;
        using forwarding_glue = Return(forward<Args>...);
        using invocation_data_type = pmf_invocation_data<apply_qualifiers<T, CvFlags>, TMemberFnPtr>;
        using my_type = pmf_wrapper<Creator, CvFlags, T, TMemberFnPtr, decayed_member_fn_ptr>;
        using return_type = Return;
        using type = Return(Args...);
        using underlying_type = apply_qualifiers<T, CvFlags>;

        template<qualify_flags Flags>
        using apply_cv = pmf_wrapper<Creator, CvFlags | Flags, T, TMemberFnPtr, decayed_member_fn_ptr>;

        static constexpr auto cv_flags = CvFlags;
        static constexpr auto is_ambiguous = std::is_same<Return(Args...), ambiguous_return(ambiguous_args)>::value;

        invocation_data_type data;

        inline pmf_wrapper(TMemberFnPtr f_ptr, T&& o)
            : data{ f_ptr, std::forward<T>(o) }
        {}

        inline pmf_wrapper(TMemberFnPtr f_ptr, T& o)
            : data{ f_ptr, o }
        {}

        inline pmf_wrapper(my_type& other) = default;
        inline pmf_wrapper(const my_type& other) = default;
        inline pmf_wrapper(my_type&& other) = default;

        inline pmf_wrapper(volatile my_type& other)
            : data(other.data)
        {}

        inline pmf_wrapper(const volatile my_type& other)
            : data(other.data)
        {}

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) {
            return CLBL_UPCAST_AND_CALL_MEMBER_VAL(__CLBL_NO_CV,
                data.object, data.pmf, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const {
            return CLBL_UPCAST_AND_CALL_MEMBER_VAL(const,
                data.object, data.pmf, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) volatile {
            return CLBL_UPCAST_AND_CALL_MEMBER_VAL(volatile,
                data.object, data.pmf, std::forward<Fargs>(a)...);
        }

        template<typename... Fargs>
        inline Return operator()(Fargs&&... a) const volatile {
            return CLBL_UPCAST_AND_CALL_MEMBER_VAL(const volatile,
                data.object, data.pmf, std::forward<Fargs>(a)...);
        }

        template<typename T = underlying_type, std::enable_if_t<is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto
        copy_invocation(T& c) {
            return no_ref<decltype(c.data.object)>::copy_invocation(
                harden_cast<cv_flags>(c.data.object)
            );
        }

        template<typename T = underlying_type, std::enable_if_t<is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto
        copy_invocation(const T& c) {
            return no_ref<decltype(c.data.object)>::copy_invocation(
                harden_cast<qflags::const_ | cv_flags>(c.data.object)
            );
        }

        template<typename T = underlying_type, std::enable_if_t<is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto
        copy_invocation(volatile T& c) {
            return no_ref<decltype(c.data.object)>::copy_invocation(
                harden_cast<qflags::volatile_ | cv_flags>(c.data.object)
            );
        }

        template<typename T = underlying_type, std::enable_if_t<is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto
        copy_invocation(const volatile T& c) {
            return no_ref<decltype(data.object)>::copy_invocation(
                harden_cast<qflags::const_ | qflags::volatile_ | cv_flags>(c.data.object)
            );
        }

        template<typename T = underlying_type, std::enable_if_t<!is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto
        copy_invocation(my_type& c) {
            return[d = c.data](auto&&... args) mutable {
                return CLBL_UPCAST_AND_CALL_MEMBER_VAL(__CLBL_NO_CV,
                    d.object, d.pmf, args...);
            };
        }

        template<typename T = underlying_type, std::enable_if_t<!is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(const my_type& c) {
            return[d = c.data](auto&&... args){
                return CLBL_UPCAST_AND_CALL_MEMBER_VAL(const,
                    d.object, d.pmf, args...);
            };
        }

        template<typename T = underlying_type, std::enable_if_t<!is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(volatile my_type& c) {
            return[d = c.data](auto&&... args) mutable {
                return CLBL_UPCAST_AND_CALL_MEMBER_VAL(volatile,
                    d.object, d.pmf, args...);
            };
        }

        template<typename T = underlying_type, std::enable_if_t<!is_clbl<T>, dummy>* = nullptr>
        static inline constexpr auto copy_invocation(const volatile my_type& c) {
            return[d = c.data](auto&&... args){
                return CLBL_UPCAST_AND_CALL_MEMBER_VAL(const volatile,
                    d.object, d.pmf, args...);
            };
        }
    };
}

#endif