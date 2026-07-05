#pragma once
#include <cstdint>
#include <limits>

namespace Frost
{
    using Entity = uint32_t;
    constexpr Entity NULL_ENTITY = std::numeric_limits<uint32_t>::max();
}