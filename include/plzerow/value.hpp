#pragma once

#include <cstdint>
#include <iostream>
#include <ostream>
#include <utility>
#include <variant>
#include <vector>
#include "token_type.hpp"

namespace plzerow {

template <class... Ts>
struct Visitor : Ts... {
  using Ts::operator()...;
};
template <class... Ts>
Visitor(Ts...) -> Visitor<Ts...>;

using ValueVariant = std::variant<double, std::int32_t, bool, std::string>;

struct Value {
  Value(std::string value) : _type(TOKEN::STRING), _value(value) {}
  Value(bool value) : _type(TOKEN::BOOL), _value(value) {}
  Value(std::int32_t value) : _type(TOKEN::INTEGER), _value(value) {}
  Value(double value) : _type(TOKEN::DOUBLE), _value(value) {}

  bool as_bool() const;
  const std::string &as_string() const;
  std::int32_t as_int() const;
  double as_double() const;

  TOKEN _type;
  ValueVariant _value;
};

using ValueContainer = std::vector<Value>;
using ValuePointer = ValueContainer::const_pointer;

class ValueArray {
 public:
  const std::vector<Value> &values() const;
  bool as_bool(std::size_t index) const;
  const std::string &as_string(std::size_t index) const;
  std::int32_t as_int(std::size_t index) const;
  double as_double(std::size_t index) const;

  template <typename Visitor>
  auto visit(std::size_t index, Visitor &&visitor) const;
  std::size_t append(Value &&value);

 private:
  TOKEN _type;
  std::vector<Value> _values;
};

template <typename Visitor>
auto ValueArray::visit(std::size_t index, Visitor &&visitor) const {
  return std::visit(std::forward<Visitor>(visitor), _values[index]);
}

}  // namespace plzerow
