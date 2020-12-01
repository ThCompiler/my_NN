#pragma once
#include "contexts.h"
#include <numeric>
#include <stack>

namespace binary_local {
    template<typename T>
    std::size_t size_of(const T& value);

    template <typename T, typename U = void>
    class stream {
    public:
         static std::size_t size_of(T);

         using value_type = T;
    };

    template<typename T, typename U>
    std::size_t stream<T, U>::size_of(T)
    {
        return sizeof(T);
    }

    template <typename T>
    class stream<T, only_if_boolean<T>> {
    public:
         static std::size_t size_of(T);

         using value_type = bool;
         using proxy_type = std::uint8_t;

         static constexpr proxy_type t_value = 'T';
         static constexpr proxy_type f_value = 'F';
    };

    template<typename T>
    inline std::size_t stream<T, only_if_boolean<T>>::size_of(T)
    {
        return sizeof(proxy_type);
    }

    template <typename T>
    class stream<T, only_if_string<T>> {
    public:
         static std::size_t size_of(const T& value);

         using size_type = std::uint16_t;
    };

    template<typename T>
    inline std::size_t stream<T, only_if_string<T>>::size_of(const T& value)
    {
        return sizeof(size_type) + value.size();
    }

#if _HAS_CXX20
    template <sequence_container T>
        class stream<T, void> {
#else
    template <typename T>
    class stream<T, only_if_sequence_container<T>> {
#endif  // _HAS_CXX20

    public:
         static std::size_t size_of(const T& value);

         using value_type = typename T::value_type;
         using size_type = std::uint16_t;
    };

#if _HAS_CXX20
    template<sequence_container T>
    inline std::size_t stream<T, void>::size_of(const T& value) {
#else
    template<typename T>
    inline std::size_t stream<T, only_if_sequence_container<T>>::size_of(const T & value) {
#endif  // _HAS_CXX20

        return std::accumulate(value.cbegin(), value.cend(), sizeof(size_type),
            [](const auto& init, const auto& item)
        { return init + stream<value_type>::size_of(item); });
    }

#if _HAS_CXX20
    template <associative_container T>
        class stream<T, void> {
#else
    template <typename T>
    class stream<T, only_if_associative_container<T>> {
#endif  // _HAS_CXX20

    public:
         static std::size_t size_of(const T& value);

         using value_type = typename T::value_type;
         using key_type = typename T::key_type;
         using size_type = std::uint16_t;
    };

#if _HAS_CXX20
    template<associative_container T>
    inline std::size_t stream<T, void>::size_of(const T& value) {
#else
    template<typename T>
    inline std::size_t stream<T, only_if_associative_container<T>>::size_of(const T & value) {
#endif  // _HAS_CXX20

        return std::accumulate(value.cbegin(), value.cend(), sizeof(size_type),
            [](const auto& init, const auto& item)
        { return init + stream<value_type>::size_of(item); });
    }

    template <typename T>
    class stream<T, only_if_pair<T>> {
    public:
         static std::size_t size_of(const T& value);

         using f_type = typename T::first_type;
         using s_type = typename T::second_type;
    };

    template<typename T>
    inline std::size_t stream<T, only_if_pair<T>>::size_of(const T& value)
    {
        return stream<f_type>::size_of(value.first) + stream<s_type>::size_of(value.second);
    }

    template<typename T>
    class stream<T, only_if_cstring<T>>
    {
    public:
         using size_type = std::uint16_t;
    };

    template<typename T>
    class stream<T, only_if_iserializable<T>>
    {
    public:
         using value_type = T;

    public:
         static auto size_of(const T& value)->std::size_t;
    };

    template<typename T>
    inline auto stream<T, only_if_iserializable<T>>::size_of(const T& value) -> std::size_t
    {
        return value.serialized_size();
    }

#if _HAS_CXX20
    template <stack_container T>
    class stream<T, void> {
#else
    template <typename T>
    class stream<T, only_if_stack_container<T>> {
#endif  // _HAS_CXX20

    public:
         static std::size_t size_of(const T& value);

         using value_type = typename T::value_type;
         using size_type = std::uint16_t;
    };

#if _HAS_CXX20
    template<stack_container T>
    inline std::size_t stream<T, void>::size_of(const T& value) {
#else
    template<typename T>
    inline std::size_t stream<T, only_if_stack_container<T>>::size_of(const T & value) {
#endif  // _HAS_CXX20
        T tmp = value;
        
        typename stream::size_type size = 0;

        while (!tmp.empty()) {
            size += size_of(tmp.top());
            tmp.pop();
        }
        return static_cast<std::size_t>(size);
    }

#if _HAS_CXX20
    template <queue_container T>
    class stream<T, void> {
#else
    template <typename T>
    class stream<T, only_if_queue_container<T>> {
#endif  // _HAS_CXX20

    public:
         static std::size_t size_of(const T& value);

         using value_type = typename T::value_type;
         using size_type = std::uint16_t;
    };

#if _HAS_CXX20
    template<queue_container T>
    inline std::size_t stream<T, void>::size_of(const T& value) {
#else
    template<typename T>
    inline std::size_t stream<T, only_if_queue_container<T>>::size_of(const T & value) {
#endif  // _HAS_CXX20

        T tmp = value;

        typename stream::size_type size = 0;

        while (!tmp.empty()) {
            size += size_of(tmp.front());
            tmp.pop();
        }
        return static_cast<std::size_t>(size);
    }
    
    template<typename T>
     std::size_t size_of(const T& value)
    {
        return  stream<T>::size_of(value);
    }
}