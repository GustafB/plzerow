#include <plzerow/value.hpp>

namespace plzerow {

const std::vector<Value> &ValueArray::values() const { return _values; }

std::size_t ValueArray::append(Value &&value) {
  _values.push_back(std::forward<Value>(value));
  return _values.size() - 1;
}
}  // namespace plzerow
