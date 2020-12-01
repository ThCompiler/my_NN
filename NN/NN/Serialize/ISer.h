#pragma once
#include <iostream>

class ISerializable {
public:
    ~ISerializable() = default;

    virtual std::size_t serialize(std::ostream& is) = 0;
    virtual std::size_t deserialize(std::istream& is) = 0;
    virtual std::size_t serialize_size() noexcept = 0;
};