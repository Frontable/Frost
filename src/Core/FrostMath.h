#pragma once
#include <cmath>
#include <iostream>
#include <algorithm>

namespace Frost
{
    // 
    // consts
    // 
    constexpr float PI = 3.14159265359f;
    constexpr float TWO_PI = PI * 2.0f;
    constexpr float HALF_PI = PI * 0.5f;
    constexpr float DEG2RAD = PI / 180.0f;
    constexpr float RAD2DEG = 180.0f / PI;

    // 
    // vector struct
    //
    template <typename T, size_t S>
    struct vec
    {
        T data[S]{};
        T& operator[](size_t i) { return data[i]; }
        const T& operator[](size_t i) const { return data[i]; }
    };

    //
    // vec2
    // 
    template <typename T>
    struct vec<T, 2>
    {
        union
        {
            struct { T x, y; };
            struct { T r, g; };
            struct { T s, t; };
            T data[2];
        };

        vec() : x(T{}), y(T{}) {}
        vec(T v) : x(v), y(v) {}
        vec(T x, T y) : x(x), y(y) {}

        T& operator[](size_t i) { return data[i]; }
        const T& operator[](size_t i) const { return data[i]; }

        vec& operator+=(const vec& r) { x += r.x; y += r.y; return *this; }
        vec& operator-=(const vec& r) { x -= r.x; y -= r.y; return *this; }
        vec& operator*=(T s) { x *= s;   y *= s;   return *this; }
        vec& operator/=(T s) { x /= s;   y /= s;   return *this; }
    };

    // 
    // vec3
    //
    template <typename T>
    struct vec<T, 3>
    {
        union
        {
            struct { T x, y, z; };
            struct { T r, g, b; };
            T data[3];
        };

        vec() : x(T{}), y(T{}), z(T{}) {}
        vec(T v) : x(v), y(v), z(v) {}
        vec(T x, T y, T z) : x(x), y(y), z(z) {}

        T& operator[](size_t i) { return data[i]; }
        const T& operator[](size_t i) const { return data[i]; }

        vec& operator+=(const vec& r) { x += r.x; y += r.y; z += r.z; return *this; }
        vec& operator-=(const vec& r) { x -= r.x; y -= r.y; z -= r.z; return *this; }
        vec& operator*=(T s) { x *= s;   y *= s;   z *= s;   return *this; }
        vec& operator/=(T s) { x /= s;   y /= s;   z /= s;   return *this; }
    };

    // 
    // vec4
    // 
    template <typename T>
    struct vec<T, 4>
    {
        union
        {
            struct { T x, y, z, w; };
            struct { T r, g, b, a; };
            T data[4];
        };

        vec() : x(T{}), y(T{}), z(T{}), w(T{}) {}
        vec(T v) : x(v), y(v), z(v), w(v) {}
        vec(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

        T& operator[](size_t i) { return data[i]; }
        const T& operator[](size_t i) const { return data[i]; }

        vec& operator+=(const vec& r) { x += r.x; y += r.y; z += r.z; w += r.w; return *this; }
        vec& operator-=(const vec& r) { x -= r.x; y -= r.y; z -= r.z; w -= r.w; return *this; }
        vec& operator*=(T s) { x *= s;   y *= s;   z *= s;   w *= s;   return *this; }
        vec& operator/=(T s) { x /= s;   y /= s;   z /= s;   w /= s;   return *this; }
    };

    // 
    // types
    // 
    using vec2 = vec<float, 2>;
    using vec3 = vec<float, 3>;
    using vec4 = vec<float, 4>;
    using ivec2 = vec<int, 2>;
    using ivec3 = vec<int, 3>;

    // 
    // vec2 ops
    // 
    inline vec2 operator+(const vec2& a, const vec2& b) { return { a.x + b.x, a.y + b.y }; }
    inline vec2 operator-(const vec2& a, const vec2& b) { return { a.x - b.x, a.y - b.y }; }
    inline vec2 operator*(const vec2& v, float s) { return { v.x * s,   v.y * s }; }
    inline vec2 operator*(float s, const vec2& v) { return { v.x * s,   v.y * s }; }
    inline vec2 operator/(const vec2& v, float s) { return { v.x / s,   v.y / s }; }
    inline bool operator==(const vec2& a, const vec2& b) { return a.x == b.x && a.y == b.y; }
    inline bool operator!=(const vec2& a, const vec2& b) { return !(a == b); }

    // 
    // vec3 ops
    // 
    inline vec3 operator+(const vec3& a, const vec3& b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
    inline vec3 operator-(const vec3& a, const vec3& b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }
    inline vec3 operator*(const vec3& v, float s) { return { v.x * s,   v.y * s,   v.z * s }; }
    inline vec3 operator*(float s, const vec3& v) { return { v.x * s,   v.y * s,   v.z * s }; }

    // 
    // vec functions
    // 
    template <typename T, size_t S>
    T magnitude(const vec<T, S>& v)
    {
        T sum{};
        for (size_t i = 0; i < S; i++) sum += v[i] * v[i];
        return std::sqrt(sum);
    }

    template <typename T, size_t S>
    vec<T, S> normalize(const vec<T, S>& v)
    {
        T mag = magnitude(v);
        if (mag == T{}) return vec<T, S>{};
        vec<T, S> result{};
        for (size_t i = 0; i < S; i++) result[i] = v[i] / mag;
        return result;
    }

    inline float dot(const vec2& a, const vec2& b)
    {
        return a.x * b.x + a.y * b.y;
    }

    inline float cross(const vec2& a, const vec2& b)
    {
        return a.x * b.y - a.y * b.x;
    }

    inline vec2 lerp(const vec2& a, const vec2& b, float t)
    {
        return { a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t };
    }

    inline vec2 reflect(const vec2& v, const vec2& n)
    {
        return v - n * (2.0f * dot(v, n));
    }

    inline vec2 perpendicular(const vec2& v)
    {
        return { -v.y, v.x };
    }

    inline float distanceSq(const vec2& a, const vec2& b)
    {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        return dx * dx + dy * dy;
    }

    inline float distance(const vec2& a, const vec2& b)
    {
        return std::sqrt(distanceSq(a, b));
    }

    inline float clamp(float v, float min, float max)
    {
        return std::max(min, std::min(max, v));
    }

    inline float lerp(float a, float b, float t)
    {
        return a + (b - a) * t;
    }

    // 
    // aabb
    // 
    struct AABB
    {
        vec2 min;
        vec2 max;

        bool Intersects(const AABB& other) const
        {
            return min.x < other.max.x && max.x > other.min.x &&
                min.y < other.max.y && max.y > other.min.y;
        }

        bool Contains(const vec2& point) const
        {
            return point.x >= min.x && point.x <= max.x &&
                point.y >= min.y && point.y <= max.y;
        }

        vec2 Center() const { return { (min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f }; }
        vec2 Size()   const { return { max.x - min.x, max.y - min.y }; }

        static AABB FromCenterSize(const vec2& center, const vec2& size)
        {
            vec2 half = size * 0.5f;
            return { center - half, center + half };
        }
    };

    // 
    // mat — column major for OpenGL compatible
    // 
    template <typename T, size_t C, size_t R>
    struct mat
    {
        T data[C][R]{};

        mat() = default;
        explicit mat(T diagonal)
        {
            for (size_t c = 0; c < C; c++)
                for (size_t r = 0; r < R; r++)
                    data[c][r] = (c == r) ? diagonal : T{};
        }

        T* operator[](size_t col) { return data[col]; }
        const T* operator[](size_t col) const { return data[col]; }

        T* dataPtr() { return &data[0][0]; }
        const T* dataPtr() const { return &data[0][0]; }

        void print() const
        {
            for (size_t r = 0; r < R; r++)
            {
                std::cout << "{ ";
                for (size_t c = 0; c < C; c++)
                    std::cout << data[c][r] << " ";
                std::cout << "}\n";
            }
        }
    };

    using mat4 = mat<float, 4, 4>;

    // 
    // mat operators
    // 
    inline mat4 operator*(const mat4& lhs, const mat4& rhs)
    {
        mat4 result(0.0f);
        for (int col = 0; col < 4; col++)
            for (int row = 0; row < 4; row++)
                for (int k = 0; k < 4; k++)
                    result[col][row] += lhs[k][row] * rhs[col][k];
        return result;
    }

    inline vec4 operator*(const mat4& m, const vec4& v)
    {
        vec4 result{ 0,0,0,0 };
        for (int row = 0; row < 4; row++)
            for (int col = 0; col < 4; col++)
                result[row] += m[col][row] * v[col];
        return result;
    }

    // 
    // mat transforms
    // 
    inline mat4 translate(const vec3& t)
    {
        mat4 r(1.0f);
        r[3][0] = t.x;
        r[3][1] = t.y;
        r[3][2] = t.z;
        return r;
    }

    inline mat4 scale(const vec3& s)
    {
        mat4 r(1.0f);
        r[0][0] = s.x;
        r[1][1] = s.y;
        r[2][2] = s.z;
        return r;
    }

    inline mat4 rotateZ(float radians)
    {
        mat4  r(1.0f);
        float c = std::cos(radians);
        float s = std::sin(radians);
        r[0][0] = c; r[1][0] = -s;
        r[0][1] = s; r[1][1] = c;
        return r;
    }

    inline mat4 ortho(float left, float right, float bottom, float top)
    {
        mat4 r(1.0f);
        r[0][0] = 2.0f / (right - left);
        r[1][1] = 2.0f / (top - bottom);
        r[2][2] = -1.0f;
        r[3][0] = -(right + left) / (right - left);
        r[3][1] = -(top + bottom) / (top - bottom);
        r[3][2] = 0.0f;
        return r;
    }

} // namespace Frost