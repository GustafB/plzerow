#pragma once

#include <cstdint>
#include <iostream>
#include <ostream>
#include <utility>
#include <variant>
#include <vector>

namespace plzerow {

template <class... Ts>
struct Visitor : Ts... {
  using Ts::operator()...;
};
template <class... Ts>
Visitor(Ts...) -> Visitor<Ts...>;

constexpr Visitor PrintVisitor{
    [](double arg) { std::cout << arg; },
    [](std::int32_t arg) { std::cout << arg; },
};

constexpr Visitor Number{
    [](bool arg) -> std::int32_t {
      std::cout << "bool\n";
      return arg;
    },
    [](double arg) -> std::int32_t { return arg; },
    [](std::int32_t arg) -> std::int32_t { return arg; },
};

using Value = std::variant<double, std::int32_t, bool>;
using ValueContainer = std::vector<Value>;
using ValuePointer = ValueContainer::const_pointer;

class ValueArray {
 public:
  const std::vector<Value> &values() const;
  template <typename T>
  T at(std::size_t index) const;

  template <typename Visitor>
  auto visit(std::size_t index, Visitor &&visitor) const;
  std::size_t append(Value &&value);

 private:
  std::vector<Value> _values;
};

template <typename T>
T ValueArray::at(std::size_t index) const {
  return std::get<T>(_values[index]);
}

template <typename Visitor>
auto ValueArray::visit(std::size_t index, Visitor &&visitor) const {
  return std::visit(std::forward<Visitor>(visitor), _values[index]);
}

}  // namespace plzerow
