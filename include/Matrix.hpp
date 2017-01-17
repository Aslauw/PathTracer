#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <cuda_runtime.h>

#include "Vector.hpp"

template <class T>
class Matrix4
{
public:
    // Ctors
    __host__ __device__ Matrix4()
    {
        memset(_m, 0, 16 * sizeof(T));
        _m[0] = 1;
        _m[5] = 1;
        _m[10] = 1;
        _m[15] = 1;
    }

    __host__ __device__ Matrix4(Vec3<T> tx, Vec3<T> ty, Vec3<T> tz, Vec3<T> t)
    {
        _m[0] = tx.x();
        _m[4] = tx.y();
        _m[8] = tx.z();
        _m[12] = 0;
        _m[1] = ty.x();
        _m[5] = ty.y();
        _m[9] = ty.z();
        _m[13] = 0;
        _m[2] = tz.x();
        _m[6] = tz.y();
        _m[10] = tz.z();
        _m[14] = 0;
        _m[3] = t.x();
        _m[7] = t.y();
        _m[11] = t.z();
        _m[15] = 1;
    }

    __host__ __device__ Matrix4(const Matrix4<T>& rhs)
    {
        *this = rhs;
    }

    // Functions
    __host__ __device__ void    set(int id, T v)
    {
        _m[id] = v;
    }
    __host__ __device__ void    set(Vec3<T> tx, Vec3<T> ty, Vec3<T> tz, Vec3<T> t)
    {
        _m[0] = tx.x();
        _m[4] = tx.y();
        _m[8] = tx.z();
        _m[12] = 0;
        _m[1] = ty.x();
        _m[5] = ty.y();
        _m[9] = ty.z();
        _m[13] = 0;
        _m[2] = tz.x();
        _m[6] = tz.y();
        _m[10] = tz.z();
        _m[14] = 0;
        _m[3] = t.x();
        _m[7] = t.y();
        _m[11] = t.z();
        _m[15] = 1;
    }

    __host__ __device__ Matrix4<T>  inverse()
    {
        Matrix4<T> inv, invOut;
        float det;
        int i;

        inv[0] = _m[5]  * _m[10] * _m[15] - 
                _m[5]  * _m[11] * _m[14] - 
                _m[9]  * _m[6]  * _m[15] + 
                _m[9]  * _m[7]  * _m[14] +
                _m[13] * _m[6]  * _m[11] - 
                _m[13] * _m[7]  * _m[10];

        inv[4] = -_m[4]  * _m[10] * _m[15] + 
                _m[4]  * _m[11] * _m[14] + 
                _m[8]  * _m[6]  * _m[15] - 
                _m[8]  * _m[7]  * _m[14] - 
                _m[12] * _m[6]  * _m[11] + 
                _m[12] * _m[7]  * _m[10];

        inv[8] = _m[4]  * _m[9] * _m[15] - 
                _m[4]  * _m[11] * _m[13] - 
                _m[8]  * _m[5] * _m[15] + 
                _m[8]  * _m[7] * _m[13] + 
                _m[12] * _m[5] * _m[11] - 
                _m[12] * _m[7] * _m[9];

        inv[12] = -_m[4]  * _m[9] * _m[14] + 
                _m[4]  * _m[10] * _m[13] +
                _m[8]  * _m[5] * _m[14] - 
                _m[8]  * _m[6] * _m[13] - 
                _m[12] * _m[5] * _m[10] + 
                _m[12] * _m[6] * _m[9];

        inv[1] = -_m[1]  * _m[10] * _m[15] + 
                _m[1]  * _m[11] * _m[14] + 
                _m[9]  * _m[2] * _m[15] - 
                _m[9]  * _m[3] * _m[14] - 
                _m[13] * _m[2] * _m[11] + 
                _m[13] * _m[3] * _m[10];

        inv[5] = _m[0]  * _m[10] * _m[15] - 
                _m[0]  * _m[11] * _m[14] - 
                _m[8]  * _m[2] * _m[15] + 
                _m[8]  * _m[3] * _m[14] + 
                _m[12] * _m[2] * _m[11] - 
                _m[12] * _m[3] * _m[10];

        inv[9] = -_m[0]  * _m[9] * _m[15] + 
                _m[0]  * _m[11] * _m[13] + 
                _m[8]  * _m[1] * _m[15] - 
                _m[8]  * _m[3] * _m[13] - 
                _m[12] * _m[1] * _m[11] + 
                _m[12] * _m[3] * _m[9];

        inv[13] = _m[0]  * _m[9] * _m[14] - 
                _m[0]  * _m[10] * _m[13] - 
                _m[8]  * _m[1] * _m[14] + 
                _m[8]  * _m[2] * _m[13] + 
                _m[12] * _m[1] * _m[10] - 
                _m[12] * _m[2] * _m[9];

        inv[2] = _m[1]  * _m[6] * _m[15] - 
                _m[1]  * _m[7] * _m[14] - 
                _m[5]  * _m[2] * _m[15] + 
                _m[5]  * _m[3] * _m[14] + 
                _m[13] * _m[2] * _m[7] - 
                _m[13] * _m[3] * _m[6];

        inv[6] = -_m[0]  * _m[6] * _m[15] + 
                _m[0]  * _m[7] * _m[14] + 
                _m[4]  * _m[2] * _m[15] - 
                _m[4]  * _m[3] * _m[14] - 
                _m[12] * _m[2] * _m[7] + 
                _m[12] * _m[3] * _m[6];

        inv[10] = _m[0]  * _m[5] * _m[15] - 
                _m[0]  * _m[7] * _m[13] - 
                _m[4]  * _m[1] * _m[15] + 
                _m[4]  * _m[3] * _m[13] + 
                _m[12] * _m[1] * _m[7] - 
                _m[12] * _m[3] * _m[5];

        inv[14] = -_m[0]  * _m[5] * _m[14] + 
                _m[0]  * _m[6] * _m[13] + 
                _m[4]  * _m[1] * _m[14] - 
                _m[4]  * _m[2] * _m[13] - 
                _m[12] * _m[1] * _m[6] + 
                _m[12] * _m[2] * _m[5];

        inv[3] = -_m[1] * _m[6] * _m[11] + 
                _m[1] * _m[7] * _m[10] + 
                _m[5] * _m[2] * _m[11] - 
                _m[5] * _m[3] * _m[10] - 
                _m[9] * _m[2] * _m[7] + 
                _m[9] * _m[3] * _m[6];

        inv[7] = _m[0] * _m[6] * _m[11] - 
                _m[0] * _m[7] * _m[10] - 
                _m[4] * _m[2] * _m[11] + 
                _m[4] * _m[3] * _m[10] + 
                _m[8] * _m[2] * _m[7] - 
                _m[8] * _m[3] * _m[6];

        inv[11] = -_m[0] * _m[5] * _m[11] + 
                _m[0] * _m[7] * _m[9] + 
                _m[4] * _m[1] * _m[11] - 
                _m[4] * _m[3] * _m[9] - 
                _m[8] * _m[1] * _m[7] + 
                _m[8] * _m[3] * _m[5];

        inv[15] = _m[0] * _m[5] * _m[10] - 
                _m[0] * _m[6] * _m[9] - 
                _m[4] * _m[1] * _m[10] + 
                _m[4] * _m[2] * _m[9] + 
                _m[8] * _m[1] * _m[6] - 
                _m[8] * _m[2] * _m[5];

        det = _m[0] * inv[0] + _m[1] * inv[4] + _m[2] * inv[8] + _m[3] * inv[12];
        if (det == 0)
            return invOut;

        det = 1.0 / det;
        for (i = 0; i < 16; i++)
            invOut[i] = inv[i] * det;

        return invOut;
    }

    // Otors
    __host__ __device__ T           operator[](int i) const    { return _m[i]; }
    __host__ __device__ T&          operator[](int i)          { return _m[i]; }

    __host__ __device__ Matrix4<T>& operator=(const Matrix4<T>& rhs)
    {
        std::memcpy(_m, rhs._m, 16 * sizeof(T));
        return *this;
    }

    __host__ __device__ Matrix4<T>& operator*(const Matrix4<T>& m2)
    {
        Matrix4<T> m1(*this);

        _m[0] = m1[0] * m2[0] + m1[1] * m2[4] + m1[2] * m2[8] + m1[3] * m2[12];
        _m[1] = m1[0] * m2[1] + m1[1] * m2[5] + m1[2] * m2[9] + m1[3] * m2[13];
        _m[2] = m1[0] * m2[2] + m1[1] * m2[6] + m1[2] * m2[10] + m1[3] * m2[14];
        _m[3] = m1[0] * m2[3] + m1[1] * m2[7] + m1[2] * m2[11] + m1[3] * m2[15];
        //
        _m[4] = m1[4] * m2[0] + m1[5] * m2[4] + m1[6] * m2[8] + m1[7] * m2[12];
        _m[5] = m1[4] * m2[1] + m1[5] * m2[5] + m1[6] * m2[9] + m1[7] * m2[13];
        _m[6] = m1[4] * m2[2] + m1[5] * m2[6] + m1[6] * m2[10] + m1[7] * m2[14];
        _m[7] = m1[4] * m2[3] + m1[5] * m2[7] + m1[6] * m2[11] + m1[7] * m2[15];
        //
        _m[8] = m1[8] * m2[0] + m1[9] * m2[4] + m1[10] * m2[8] + m1[11] * m2[12];
        _m[9] = m1[8] * m2[1] + m1[9] * m2[5] + m1[10] * m2[9] + m1[11] * m2[13];
        _m[10] = m1[8] * m2[2] + m1[9] * m2[6] + m1[10] * m2[10] + m1[11] * m2[14];
        _m[11] = m1[8] * m2[3] + m1[9] * m2[7] + m1[10] * m2[11] + m1[11] * m2[15];
        //
        _m[12] = m1[12] * m2[0] + m1[13] * m2[4] + m1[14] * m2[8] + m1[15] * m2[12];
        _m[13] = m1[12] * m2[1] + m1[13] * m2[5] + m1[14] * m2[9] + m1[15] * m2[13];
        _m[14] = m1[12] * m2[2] + m1[13] * m2[6] + m1[14] * m2[10] + m1[15] * m2[14];
        _m[15] = m1[12] * m2[3] + m1[13] * m2[7] + m1[14] * m2[11] + m1[15] * m2[15];

        return *this;
    }

    __host__ __device__ Vec3<T>& operator*(Vec3<T>& v)
    {
        Vec3<T> v1(v);

        v.setX(_m[0] * v1.x() + _m[1] * v1.y() + _m[2] * v1.z() + _m[3]);
        v.setY(_m[4] * v1.x() + _m[5] * v1.y() + _m[6] * v1.z() + _m[7]);
        v.setZ(_m[8] * v1.x() + _m[9] * v1.y() + _m[10] * v1.z() + _m[11]);
        return v;
    }

    // Dtor
    __host__ __device__ ~Matrix4() {}

private:
    T   _m[16];
};

#endif