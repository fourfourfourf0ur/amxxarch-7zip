#pragma once

#include "amxxmodule.h"

#include <string>

namespace amxxarch::pawn {

std::string get_string(AMX* amx, cell param, int buffer_id);

}  // namespace amxxarch::pawn
