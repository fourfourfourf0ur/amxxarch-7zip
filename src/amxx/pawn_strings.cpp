#include "amxx/pawn_strings.hpp"

namespace amxxarch::pawn {

std::string get_string(AMX* amx, cell param, int buffer_id) {
  int length = 0;
  const char* raw = MF_GetAmxString(amx, param, buffer_id, &length);

  if (!raw || length <= 0) {
    return std::string();
  }

  return {raw, static_cast<std::string::size_type>(length)};
}

}  // namespace amxxarch::pawn
