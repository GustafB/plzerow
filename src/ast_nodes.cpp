
#include <plzerow/ast_nodes.hpp>
#include <plzerow/assert.hpp>

namespace plzerow {

bool Literal::as_bool() const {
  PLZEROW_ASSERT(std::holds_alternative<bool>(_value) &&
                 "literal does not contain type 'bool'");
  return std::get<bool>(_value);
}

const std::string& Literal::as_string() const {
  PLZEROW_ASSERT(std::holds_alternative<std::string>(_value) &&
                 "literal does not contain type 'string'");
  return std::get<std::string>(_value);
}

std::int32_t Literal::as_int() const {
  PLZEROW_ASSERT(std::holds_alternative<std::int32_t>(_value) &&
                 "literal does not contain type 'int'");
  return std::get<std::int32_t>(_value);
}

double Literal::as_double() const {
  PLZEROW_ASSERT(std::holds_alternative<double>(_value) &&
                 "literal does not contain type 'double'");
  return std::get<double>(_value);
}



} // namespace plzerow
