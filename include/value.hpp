#pragma once

#include <vector>

namespace gbpl0 {

using Value = double;

class ValueArray {
public:
  const std::vector<Value> &values() const;
  std::size_t append(const Value &value);

private:
  std::vector<Value> _values;
};

} // namespace gbpl0
