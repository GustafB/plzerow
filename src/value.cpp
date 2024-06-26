#include "value.hpp"

namespace plzerow {

const std::vector<Value> &ValueArray::values() const { return _values; }

std::size_t ValueArray::append(const Value &value) {
  _values.push_back(value);
  return _values.size() - 1;
}
} // namespace plzerow
