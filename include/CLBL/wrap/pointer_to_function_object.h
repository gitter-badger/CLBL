#ifndef CLBL_POINTER_TO_FUNCTION_OBJECT_H
#define CLBL_POINTER_TO_FUNCTION_OBJECT_H

#include <CLBL/utility.h>
#include <CLBL/tags.h>
#include <CLBL/qualify_flags.h>
#include <CLBL/member_function_decay.h>
#include <CLBL/wrappers/pmf_ptr_wrapper_slim.h>
#include <CLBL/wrappers/ambi_fn_obj_ptr_wrapper.h>
#include <CLBL/wrappers/casted_fn_obj_ptr_wrapper.h>

namespace clbl {

    struct pointer_to_function_object {

        template<qualify_flags Flags, typename TPtr>
        static inline constexpr auto 
        wrap(TPtr&& object_ptr) {
            constexpr auto cv_qualifiers = cv<TPtr> | Flags;
            using object_type = no_ref<decltype(*std::declval<TPtr>())>;
            using member_fn_type = decltype(&object_type::operator());
            using ptr_type = no_ref<TPtr>;
            using decayed_fn = member_function_decay<member_fn_type>;
            using wrapper = pmf_ptr_wrapper_slim<pointer_to_function_object,
                cv_qualifiers, object_type, ptr_type, member_fn_type, &object_type::operator(), decayed_fn>;
            return wrapper{ std::forward<TPtr>(object_ptr) };
        }

        template<qualify_flags Flags, typename Invocation>
        static inline constexpr auto
            wrap_data(Invocation&& data) {
            return wrap<Flags>(data.object_ptr);
        }

        struct ambiguous {

            template<qualify_flags Flags, typename T>
            static inline constexpr auto
            wrap(T&& t) {
                constexpr auto cv_qualifiers = cv<T> | Flags;
                using wrapper = ambi_fn_obj_ptr_wrapper<
                    pointer_to_function_object::ambiguous, cv_qualifiers, no_ref<T> >;
                return wrapper{std::forward<T>(t)};
            }

            template<qualify_flags Flags, typename Invocation>
            static inline constexpr auto
                wrap_data(Invocation&& data) {
                return wrap<Flags>(data.ptr);
            }
        };

        struct casted {

            template<qualify_flags Flags = qflags::default_, typename TMemberFnPtr, typename T>
            static inline constexpr auto
                wrap(T&& t) {
                constexpr auto cv_qualifiers = cv<T> | Flags;
                using object_type = no_ref<decltype(*std::declval<no_ref<T> >())>;
                using ptr_type = no_ref<T>;
                using decayed_fn = member_function_decay<TMemberFnPtr>;
                using wrapper = casted_fn_obj_ptr_wrapper<
                    typename pointer_to_function_object::casted,
                    cv_qualifiers, object_type, ptr_type, TMemberFnPtr, decayed_fn>;
                return wrapper{ std::forward<T>(t) };
            }

            template<qualify_flags Flags, typename Invocation>
            static inline constexpr auto
                wrap_data(Invocation&& data) {
                /*
                std::remove_const_t here removes constness from the 
                type of the constexpr pointer in Invocation, which is
                only const because of constexr. This has no affect
                whatsoever on const-correctness
                */
                using pmf_type = std::remove_const_t<decltype(no_ref<Invocation>::pmf)>;
                return wrap<Flags, pmf_type>(data.object_ptr);
            }
        };
    };

}

#endif