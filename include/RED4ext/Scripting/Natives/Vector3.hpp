#pragma once

#include <RED4ext/Common.hpp>
#include <cstdint>

namespace RED4ext
{
struct Vector3
{
    static constexpr const char* NAME = "Vector3";
    static constexpr const char* ALIAS = NAME;

    Vector3()
        : X(0)
        , Y(0)
        , Z(0)
    {
    }

    Vector3(float x, float y, float z)
        : X(x)
        , Y(y)
        , Z(z)
    {
    }

    inline Vector3& operator=(const Vector3& aOther)
    {
        X = aOther.X;
        Y = aOther.Y;
        Z = aOther.Z;

        return *this;
    }

    inline Vector3 operator+(const Vector3& aOther) const
    {
        return {X + aOther.X, Y + aOther.Y, Z + aOther.Z};
    }

    inline Vector3 operator-() const
    {
        return {-X, -Y, -Z};
    }

    inline Vector3 operator-(const Vector3& aOther) const
    {
        return {X - aOther.X, Y - aOther.Y, Z - aOther.Z};
    }

    inline Vector3 operator*(const float aScale) const
    {
        return {X * aScale, Y * aScale, Z * aScale};
    }

    inline Vector3 operator*(const Vector3& aOther) const
    {
        return {X * aOther.X, Y * aOther.Y, Z * aOther.Z};
    }

    inline bool operator==(const Vector3& aOther) const
    {
        return X == aOther.X && Y == aOther.Y && Z == aOther.Z;
    }

    inline bool operator!=(const Vector3& aOther) const
    {
        return !operator==(aOther);
    }

    inline float Magnitude() const
    {
        return std::sqrt(X * X + Y * Y + Z * Z);
    }

    inline void Normalize()
    {
        const float mag = Magnitude();

        if (mag != 0) // prevent divide by zero
        {
            const float invertedMag = 1.f / mag; // invert magnitude so we only divide once

            X *= invertedMag;
            Y *= invertedMag;
            Z *= invertedMag;
        }
    }

    inline Vector3 AsNormalized() const
    {
        auto copyVec = *this;

        copyVec.Normalize();

        return copyVec;
    }

    inline float Dot(const Vector3& aOther) const
    {
        return X * aOther.X + Y * aOther.Y + Z * aOther.Z;
    }

    inline Vector3 Cross(const Vector3& aOther) const
    {
        return {Y * aOther.Z - Z * aOther.Y, Z * aOther.X - X * aOther.Z, X * aOther.Y - Y * aOther.X};
    }

    float X; // 00
    float Y; // 04
    float Z; // 08
};
RED4EXT_ASSERT_SIZE(Vector3, 0xC);
} // namespace RED4ext
