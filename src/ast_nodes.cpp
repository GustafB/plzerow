
#include <cassert>
#include <plzerow/ast_nodes.hpp>
#include <variant>

namespace plzerow {

bool Literal::as_bool() const { return std::get<bool>(_value); }

const std::string& Literal::as_string() const {
  return std::get<std::string>(_value);
}

std::int32_t Literal::as_int() const { return std::get<std::int32_t>(_value); }

double Literal::as_double() const { return std::get<double>(_value); }

}  // namespace plzerow
