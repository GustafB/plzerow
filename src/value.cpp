#include <plzerow/value.hpp>

namespace plzerow {

const std::vector<Value> &ValueArray::values() const { return _values; }

std::size_t ValueArray::append(Value &&value) {
  _values.push_back(std::forward<Value>(value));
  return _values.size() - 1;
}

bool Value::as_bool() const { return std::get<bool>(_value); }

const std::string &Value::as_string() const {
  return std::get<std::string>(_value);
}

std::int32_t Value::as_int() const { return std::get<std::int32_t>(_value); }

double Value::as_double() const { return std::get<double>(_value); }

bool ValueArray::as_bool(std::size_t index) const {
  return _values[index].as_bool();
}

const std::string &ValueArray::as_string(std::size_t index) const {
  return _values[index].as_string();
}

std::int32_t ValueArray::as_int(std::size_t index) const {
  return _values[index].as_int();
}

double ValueArray::as_double(std::size_t index) const {
  return _values[index].as_double();
}

}  // namespace plzerow
