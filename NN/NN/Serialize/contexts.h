#pragma once

#include <string>
#include <type_traits>

#include "ISer.h"

#if _HAS_CXX20

#define __cpp_lib_concepts
#include <concepts>

#else

#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#endif  // _HAS_CXX20

namespace binary_local {

#if _HAS_CXX20
    template <typename T>
    concept non_serializable = (std::is_void_v<T> || std::is_null_pointer_v<T>);

    template <typename T>
    concept serializable = !std::is_void_v<T> && !std::is_null_pointer_v<T>;

    template <typename T>
    concept string = std::is_same_v<std::string, T>;

    template <typename T>
    concept container = !string<T> && requires(T a) {
        typename T::value_type;
        typename T::reference;
        typename T::const_reference;
        typename T::iterator;
        typename T::const_iterator;
        typename T::size_type;
        { a.begin() } -> std::same_as<typename T::iterator>;
        { a.end() } -> std::same_as<typename T::iterator>;
        { a.cbegin() } -> std::same_as<typename T::const_iterator>;
        { a.cend() } -> std::same_as<typename T::const_iterator>;
        { a.clear() } -> std::same_as<void>;
    };

    template <typename T>
    concept sequence_container = container<T> && requires(T a
        , typename T::size_type count) {
            { a.resize(count) } -> std::same_as<void>;
    };

    template <typename T>
    concept associative_container = container<T> && requires(T a) {
        typename T::key_type;
    };

    template <typename T>
    concept other_container = !string<T> && requires(T a) {
        typename T::value_type;
        typename T::reference;
        typename T::const_reference;
        typename T::size_type;
        { a.empty() } -> std::same_as<bool>;
        { a.pop() } -> std::same_as<void>;
        { a.push() } -> std::same_as<void>;
    };

    template <typename T>
    concept stack_container = other_container<T> && requires(T a) {
        { a.top() } -> std::same_as<typename T::reference>;
    };

    template <typename T>
    concept queue_container = other_container<T> && requires(T a) {
        { a.front() } -> std::same_as<typename T::reference>;
    };

    template <typename T>
    using only_if_pair = std::enable_if_t<std::is_same_v<std::remove_const_t<T>,
        std::pair<typename T::first_type, typename T::second_type>>>;

    template <typename T>
    using only_if_boolean = std::enable_if_t<std::is_same_v<std::remove_const_t<T>, bool>>;

    template <typename T>
    using only_if_string = std::enable_if_t<std::is_same_v<std::remove_const_t<T>, std::string>>;

    template <typename T>
    using only_if_cstring =
        std::enable_if_t<std::is_same_v<std::remove_const_t<T>, char*>>;

    template <typename T>
    using only_if_iserializable =
        std::enable_if_t<std::is_base_of_v<ISerializable, T>>;

#else

    namespace container_type {

        template <typename T>
        struct is_sequence_container : std::false_type {};
        template <typename T, std::size_t N>
        struct is_sequence_container<std::array             <T, N>> :std::true_type {};
        template <typename... Args>
        struct is_sequence_container<std::vector            <Args...>> :std::true_type {};
        template <typename... Args>
        struct is_sequence_container<std::deque             <Args...>> :std::true_type {};
        template <typename... Args>
        struct is_sequence_container<std::list              <Args...>> :std::true_type {};
        template <typename... Args>
        struct is_sequence_container<std::forward_list      <Args...>> :std::true_type {};

        template <typename T>
        struct is_associative_container : std::false_type {};
        template <typename... Args>
        struct is_associative_container<std::set               <Args...>> :std::true_type {};
        template <typename... Args>
        struct is_associative_container<std::multiset          <Args...>> :std::true_type {};
        template <typename... Args>
        struct is_associative_container<std::map               <Args...>> :std::true_type {};
        template <typename... Args>
        struct is_associative_container<std::multimap          <Args...>> :std::true_type {};
        template <typename... Args>
        struct is_associative_container<std::unordered_set     <Args...>> :std::true_type {};
        template <typename... Args>
        struct is_associative_container<std::unordered_multiset<Args...>> :std::true_type {};
        template <typename... Args>
        struct is_associative_container<std::unordered_map     <Args...>> :std::true_type {};
        template <typename... Args>
        struct is_associative_container<std::unordered_multimap<Args...>> :std::true_type {};

        template <typename T>
        struct is_stack_container : std::false_type {};
        template <typename... Args>
        struct is_stack_container<std::stack             <Args...>> :std::true_type {};

        template <typename T>
        struct is_queue_container : std::false_type {};
        template <typename... Args>
        struct is_queue_container<std::queue             <Args...>> :std::true_type {};
        template <typename... Args>
        struct is_queue_container<std::priority_queue    <Args...>> :std::true_type {};
    }

    template <typename T, typename U = void>
    struct serializable : public std::false_type {};

    template<typename T>
    struct  serializable <T, std::enable_if_t<!std::is_void<T>::value && !std::is_null_pointer<T>::value>>
        : public std::true_type {};

    template <typename T>
    using only_if_sequence_container = std::enable_if_t<container_type::is_sequence_container
        <std::decay_t<std::remove_const_t<T>>>::value&& serializable<T>::value>;

    template <typename T>
    using only_if_associative_container = std::enable_if_t<container_type::is_associative_container
        <std::decay_t<std::remove_const_t<T>>>::value&& serializable<T>::value>;

    template <typename T>
    using only_if_stack_container = std::enable_if_t<container_type::is_stack_container
        <std::decay_t<std::remove_const_t<T>>>::value&& serializable<T>::value>;

    template <typename T>
    using only_if_queue_container = std::enable_if_t<container_type::is_queue_container
        <std::decay_t<std::remove_const_t<T>>>::value&& serializable<T>::value>;

    template <typename T>
    using only_if_pair = std::enable_if_t<std::is_same_v<std::remove_const_t<T>,
        std::pair<typename T::first_type, typename T::second_type>>&& serializable<T>::value>;

    template <typename T>
    using only_if_boolean = std::enable_if_t<std::is_same_v<std::remove_const_t<T>, bool>
        && serializable<T>::value>;

    template <typename T>
    using only_if_string = std::enable_if_t<std::is_same_v<std::remove_const_t<T>, std::string>
        && serializable<T>::value>;

    template <typename T>
    using only_if_cstring =
        std::enable_if_t<std::is_same_v<std::remove_const_t<T>, char*>&& serializable<T>::value>;

    template <typename T>
    using only_if_iserializable =
        std::enable_if_t<std::is_base_of_v<ISerializable, T>&& serializable<T>::value>;

#endif  // _HAS_CXX20

}  // namespace binary