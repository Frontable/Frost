#pragma once
#include <iostream>
#include <cstdlib>

#ifdef FROST_DEBUG
#ifdef _MSC_VER
#define FROST_DEBUGBREAK() __debugbreak()
#else
#define FROST_DEBUGBREAK() __builtin_trap()
#endif

#define FROST_ASSERT(condition, message)                              \
        do {                                                              \
            if (!(condition))                                             \
            {                                                             \
                std::cerr << "[ASSERT] " << message           << "\n"    \
                          << "  File: "  << __FILE__          << "\n"    \
                          << "  Line: "  << __LINE__          << "\n"    \
                          << "  Func: "  << __func__          << "\n";   \
                FROST_DEBUGBREAK();                                       \
            }                                                             \
        } while(0)
#else
#define FROST_ASSERT(condition, message) // stripped in release
#endif

// Always on even in release
#define FROST_ASSERT_ALWAYS(condition, message)                           \
    do {                                                                  \
        if (!(condition))                                                 \
        {                                                                 \
            std::cerr << "[ASSERT] " << message           << "\n"        \
                      << "  File: "  << __FILE__          << "\n"        \
                      << "  Line: "  << __LINE__          << "\n"        \
                      << "  Func: "  << __func__          << "\n";       \
            std::abort();                                                 \
        }                                                                 \
    } while(0)