#pragma once

#include <RED4ext/Common.hpp>
#include <cstdint>

namespace RED4ext
{
struct Vector2
{
    static constexpr const char* NAME = "Vector2";
    static constexpr const char* ALIAS = NAME;

    Vector2()
        : X(0)
        , Y(0)
    {
    }

    Vector2(float x, float y)
        : X(x)
        , Y(y)
    {
    }

    inline Vector2& operator=(const Vector2& aOther)
    {
        X = aOther.X;
        Y = aOther.Y;

        return *this;
    }

    inline Vector2 operator+(const Vector2& aOther) const
    {
        return {X + aOther.X, Y + aOther.Y};
    }

    inline Vector2 operator-() const
    {
        return {-X, -Y};
    }

    inline Vector2 operator-(const Vector2& aOther) const
    {
        return {X - aOther.X, Y - aOther.Y};
    }

    inline Vector2 operator*(const float aScale) const
    {
        return {X * aScale, Y * aScale};
    }

    inline Vector2 operator*(const Vector2& aOther) const
    {
        return {X * aOther.X, Y * aOther.Y};
    }

    inline bool operator==(const Vector2& aOther) const
    {
        return X == aOther.X && Y == aOther.Y;
    }

    inline bool operator!=(const Vector2& aOther) const
    {
        return !operator==(aOther);
    }

    inline float Magnitude() const
    {
        return std::sqrt(X * X + Y * Y);
    }

    inline void Normalize()
    {
        const float mag = Magnitude();

        if (mag != 0) // prevent divide by zero
        {
            const float invertedMag = 1.f / mag; // invert magnitude so we only divide once

            X *= invertedMag;
            Y *= invertedMag;
        }
    }

    inline Vector2 AsNormalized() const
    {
        auto copyVec = *this;

        copyVec.Normalize();

        return copyVec;
    }

    inline float Dot(const Vector2& aOther) const
    {
        return X * aOther.X + Y * aOther.Y;
    }

    inline float Cross(const Vector2& aOther) const
    {
        return X * aOther.Y - Y * aOther.X;
    }

    float X; // 00
    float Y; // 04
};
RED4EXT_ASSERT_SIZE(Vector2, 0x8);
} // namespace RED4ext
