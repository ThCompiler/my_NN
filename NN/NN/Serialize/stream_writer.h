#pragma once
#include "stream.h"
#include <string.h>

namespace binary {

    template<typename T>
    std::size_t write(std::ostream& out, const T& value);

#if _HAS_CXX20
    template <binary_local::serializable T, typename U = void>
    class writer
#else
    template <typename T, typename U = void>
    class writer
#endif // _HAS_CXX20

        : public binary_local::stream<T> {
    public:
        static std::size_t write(std::ostream & out, const T & value);
    };

#if _HAS_CXX20
    template<binary_local::serializable T, typename U>
    inline std::size_t writer<T, U>::write(std::ostream& out, const T& value) {
#else
    template<typename T, typename U>
    inline std::size_t writer<T, U>::write(std::ostream & out, const T & value) {
#endif // _HAS_CXX20

        const auto start = out.tellp();
        out.write(reinterpret_cast<const char*>(&value), sizeof(value));
        return static_cast<std::size_t>(out.tellp() - start);
    }

    template <typename T>
    class writer<T, binary_local::only_if_string<T>>
        : public binary_local::stream<T> {
    public:
        static std::size_t write(std::ostream& out, const T& value);
    };

    template<typename T>
    inline std::size_t writer<T, binary_local::only_if_string<T>>
        ::write(std::ostream & out, const T & value) {
        const auto start = out.tellp();
        const auto size = static_cast<typename writer::size_type> (value.size());

        out.write(reinterpret_cast<const char*>(&size), sizeof(size));
        if (size > 0)
            out.write(value.data(), size);

        return static_cast<std::size_t>(out.tellp() - start);
    }

    template <typename T>
    class writer<T, binary_local::only_if_boolean<T>>
        : public binary_local::stream<T> {
    public:
        static std::size_t write(std::ostream& out, const T& value);
    };

    template<typename T>
    inline std::size_t writer<T, binary_local::only_if_boolean<T>>::write(std::ostream & out, const T & value) {
        const auto start = out.tellp();
        const auto val = value ? writer::t_value : writer::f_value;

        out.write(reinterpret_cast<const char*>(&val), sizeof(val));

        return static_cast<std::size_t>(out.tellp() - start);
    }

    template <typename T>
    class writer<T, binary_local::only_if_iserializable<T>>
        : public binary_local::stream<T> {
    public:
        static std::size_t write(std::ostream& out, const T& value);
    };

    template<typename T>
    inline std::size_t writer<T, binary_local::only_if_iserializable<T>>::
        write(std::ostream & out, const T & value) {
        return value.serialize(out);
    }

    template <typename T>
    class writer<T, binary_local::only_if_cstring<T>>
        : public binary_local::stream<T> {
    public:
        static std::size_t write(std::ostream& out, const T& value);
    };

    template<typename T>
    inline std::size_t writer<T, binary_local::only_if_cstring<T>>
        ::write(std::ostream & out, const T & value) {
        const auto start = out.tellp();
        const auto size = static_cast<typename writer::size_type>(std::strlen(value));

        out.write(reinterpret_cast<const char*>(&size), sizeof(size));
        if (size > 0)
            out.write(value, size);

        return static_cast<std::size_t>(out.tellp() - start);
    }

#if _HAS_CXX20
    template <binary_local::sequence_container T>
    class writer<T, void>
#else
    template <typename T>
    class writer<T, binary_local::only_if_sequence_container<T>>
#endif // _HAS_CXX20

        : public binary_local::stream<T> {
    public:
        static std::size_t write(std::ostream & out, const T & value);
    };

#if _HAS_CXX20
    template<binary_local::sequence_container T>
    inline std::size_t writer<T, void>::write(std::ostream& out, const T& value) {
#else
    template<typename T>
    inline std::size_t writer<T, binary_local::only_if_sequence_container<T>>
        ::write(std::ostream & out, const T & value) {
#endif // _HAS_CXX20

        const auto size = static_cast<typename writer::size_type> (value.size());

        out.write(reinterpret_cast<const char*>(&size), sizeof(size));
        auto writed = sizeof(size);

        for (auto& el : value) {
            writed += writer<typename writer::value_type>::write(out, el);
        }

        return writed;
    }

    template <typename T>
    class writer<T, binary_local::only_if_pair<T>>
        : public binary_local::stream<T> {
    public:
        static std::size_t write(std::ostream& out, const T& value);
    };

    template<typename T>
    inline std::size_t writer<T, binary_local::only_if_pair<T>>
        ::write(std::ostream & out, const T & value) {
        auto size = 0U;

        size += writer<typename writer::f_type>::write(out, value.first);
        size += writer<typename writer::s_type>::write(out, value.second);

        return size;
    }

#if _HAS_CXX20
    template <binary_local::associative_container T>
    class writer<T, void>
#else
    template <typename T>
    class writer<T, binary_local::only_if_associative_container<T>>
#endif // _HAS_CXX20

        : public binary_local::stream<T> {
    public:
        static std::size_t write(std::ostream & out, const T & value);
    };

#if _HAS_CXX20
    template<binary_local::associative_container T>
    inline std::size_t writer<T, void>::write(std::ostream& out, const T& value) {
#else
    template<typename T>
    inline std::size_t writer<T, binary_local::only_if_associative_container<T>>
        ::write(std::ostream & out, const T & value) {
#endif // _HAS_CXX20

        const auto size = static_cast<typename writer::size_type> (value.size());

        out.write(reinterpret_cast<const char*>(&size), sizeof(size));
        auto writed = sizeof(size);

        for (const auto& el : value) {
            writed += writer<const typename writer::value_type>::write(out, el);
        }

        return writed;
    }

#if _HAS_CXX20
    template <binary_local::stack_container T>
    class writer<T, void>
#else
    template <typename T>
    class writer<T, binary_local::only_if_stack_container<T>>
#endif // _HAS_CXX20

        : public binary_local::stream<T> {
    public:
        static std::size_t write(std::ostream & out, const T & value);
    };

#if _HAS_CXX20
    template<binary_local::stack_container T>
    inline std::size_t writer<T, void>::write(std::ostream& out, const T& value) {
#else
    template<typename T>
    inline std::size_t writer<T, binary_local::only_if_stack_container<T>>
        ::write(std::ostream & out, const T & value) {
#endif // _HAS_CXX20

        const auto size = static_cast<typename writer::size_type> (value.size());

        out.write(reinterpret_cast<const char*>(&size), sizeof(size));
        auto writed = sizeof(size);

        T tmp = value;

        T buffer;

        while (!tmp.empty()) {
            buffer.push(tmp.top());
            tmp.pop();
        }

        while (!buffer.empty()) {
            writed += writer<typename writer::value_type>::write(out, buffer.top());
            buffer.pop();
        }

        return writed;
    }

#if _HAS_CXX20
    template <binary_local::queue_container T>
    class writer<T, void>
#else
    template <typename T>
    class writer<T, binary_local::only_if_queue_container<T>>
#endif // _HAS_CXX20

        : public binary_local::stream<T> {
    public:
        static std::size_t write(std::ostream & out, const T & value);
    };

#if _HAS_CXX20
    template<binary_local::queue_container T>
    inline std::size_t writer<T, void>::write(std::ostream& out, const T& value) {
#else
    template<typename T>
    inline std::size_t writer<T, binary_local::only_if_queue_container<T>>
        ::write(std::ostream & out, const T & value) {
#endif // _HAS_CXX20

        const auto size = static_cast<typename writer::size_type> (value.size());

        out.write(reinterpret_cast<const char*>(&size), sizeof(size));
        auto writed = sizeof(size);

        T tmp = value;

        while (!tmp.empty()) {
            writed += writer<typename writer::value_type>::write(out, tmp.front());
            tmp.pop();
        }

        return writed;
    }
    
    template<typename T>
    std::size_t write(std::ostream& out, const T& value) {
        return writer<T>::write(out, value);
    }
}