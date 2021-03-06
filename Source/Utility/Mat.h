/*
 Copyright (C) 2010-2012 Kristian Duske

 This file is part of TrenchBroom.

 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with TrenchBroom.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TrenchBroom_Mat_h
#define TrenchBroom_Mat_h

#include "Utility/Quat.h"
#include "Utility/Vec.h"

namespace TrenchBroom {
    namespace VecMath {
        template <typename T, size_t R, size_t C>
        class Mat {
        public:
            static const Mat<T,R,C> Null;
            static const Mat<T,R,C> Identity;
            static const Mat<T,R,C> Rot90XCW;
            static const Mat<T,R,C> Rot90YCW;
            static const Mat<T,R,C> Rot90ZCW;
            static const Mat<T,R,C> Rot90XCCW;
            static const Mat<T,R,C> Rot90YCCW;
            static const Mat<T,R,C> Rot90ZCCW;
            static const Mat<T,R,C> Rot180X;
            static const Mat<T,R,C> Rot180Y;
            static const Mat<T,R,C> Rot180Z;
            static const Mat<T,R,C> MirX;
            static const Mat<T,R,C> MirY;
            static const Mat<T,R,C> MirZ;
            static const Mat<T,R,C> YIQToRGB;
            static const Mat<T,R,C> RGBToYIQ;

            typedef std::vector<Mat<T,R,C> > List;

            // we store in column-major format
            // every vector is one column
            Vec<T,R> v[C];

            Mat<T,R,C>() {
                setIdentity();
            }

            Mat<T,R,C>(const T v11, const T v12, const T v13,
                       const T v21, const T v22, const T v23,
                       const T v31, const T v32, const T v33) {
                v[0][0] = v11; v[1][0] = v12; v[2][0] = v13;
                v[0][1] = v21; v[1][1] = v22; v[2][1] = v23;
                v[0][2] = v31; v[1][2] = v32; v[2][2] = v33;
                for (size_t c = 3; c < C; c++)
                    for (size_t r = 3; r < R; r++)
                        v[c][r] = static_cast<T>(0.0);
            }

            Mat<T,R,C>(const T v11, const T v12, const T v13, const T v14,
                       const T v21, const T v22, const T v23, const T v24,
                       const T v31, const T v32, const T v33, const T v34,
                       const T v41, const T v42, const T v43, const T v44) {
                v[0][0] = v11; v[1][0] = v12; v[2][0] = v13; v[3][0] = v14;
                v[0][1] = v21; v[1][1] = v22; v[2][1] = v23; v[3][1] = v24;
                v[0][2] = v31; v[1][2] = v32; v[2][2] = v33; v[3][2] = v34;
                v[0][3] = v41; v[1][3] = v42; v[2][3] = v43; v[3][3] = v44;
                for (size_t c = 4; c < C; c++)
                    for (size_t r = 4; r < R; r++)
                        v[c][r] = static_cast<T>(0.0);
            }
            
            Mat<T,R,C>(const Mat<T,R,C>& other) {
                for (size_t c = 0; c < C; c++)
                    v[c] = other[c];
            }

            inline Mat<T,R,C>& operator= (const Mat<T,R,C>& right) {
                for (size_t c = 0; c < C; c++)
                    for (size_t r = 0; r < R; r++)
                        v[c][r] = right[c][r];
                return *this;
            }

            inline const Mat<T,R,C> operator- () const {
                Mat<T,R,C> result;
                for (size_t c = 0; c < C; c++)
                    result[c] = -v[c];
                return result;
            }
            
            inline bool operator== (const Mat<T,R,C>& right) const {
                for (size_t c = 0; c < C; c++)
                    for (size_t r = 0; r < R; r++)
                        if (v[c][r] != right[c][r])
                            return false;
                return true;
            }

            // Matrix addition and subtraction
            inline const Mat<T,R,C> operator+ (const Mat<T,R,C>& right) const {
                Mat<T,R,C> result(*this);
                return result += right;
            }

            inline Mat<T,R,C>& operator+= (const Mat<T,R,C>& right) {
                for (size_t c = 0; c < C; c++)
                    v[c] += right[c];
                return *this;
            }

            inline const Mat<T,R,C> operator- (const Mat<T,R,C>& right) const {
                Mat<T,R,C> result(*this);
                return result -= right;
            }

            inline Mat<T,R,C>& operator-= (const Mat<T,R,C>& right) {
                for (size_t c = 0; c < C; c++)
                    v[c] -= right[c];
                return *this;
            }

            // Matrix multiplication
            inline const Mat<T,R,C> operator* (const Mat<T,C,R>& right) const {
                Mat<T,R,C> result(Mat<T,R,C>::Null);
                for (size_t c = 0; c < C; c++)
                    for (size_t r = 0; r < R; r++)
                        for (size_t i = 0; i < C; i++)
                            result[c][r] += v[i][r] * right[c][i];
                return result;
            }

            inline Mat<T,R,C>& operator*= (const Mat<T,C,R>& right) {
                return *this = *this * right;
            }

            // Scalar multiplication
            inline const Mat<T,R,C> operator* (const T right) const {
                Mat<T,R,C> result(*this);
                return result *= right;
            }

            inline Mat<T,R,C>& operator*= (const T right) {
                for (size_t c = 0; c < C; c++)
                    v[c] *= right;
                return *this;
            }

            inline const Mat<T,R,C> operator/ (const T right) const {
                Mat<T,R,C> result(*this);
                return result /= right;
            }

            inline Mat<T,R,C>& operator/= (const T right) {
                for (size_t c = 0; c < C; c++)
                    v[c] /= right;
                return *this;
            }

            // Vector right multiplication
            inline const Vec<T,C> operator* (const Vec<T,C>& right) const {
                Vec<T,C> result;
                for (size_t r = 0; r < R; r++)
                    for (size_t i = 0; i < C; i++)
                        result[r] += v[i][r] * right[i];
                return result;
            }

            inline const Vec<T,C-1> operator* (const Vec<T,C-1>& right) const {
                return (*this * Vec<T,C>(right, static_cast<T>(1.0))).overLast();
            }

            inline const typename Vec<T,C>::List operator* (const typename Vec<T,C>::List& right) const {
                typename Vec<T,C>::List result;
                result.reserve(right.size());

                typename Vec<T,C>::List::const_iteartor it, end;
                for (it = right.begin(), end = right.end(); it != end; ++it)
                    result.push_back(*this * *it);
                return result;
            }

            inline const typename Vec<T,C-1>::List operator* (const typename Vec<T,C-1>::List& right) const {
                typename Vec<T,C-1>::List result;
                result.reserve(right.size());

                typename Vec<T,C-1>::List::const_iterator it, end;
                for (it = right.begin(), end = right.end(); it != end; ++it)
                    result.push_back(*this * *it);
                    return result;
            }

            // indexed access, returns one column
            inline Vec<T,R>& operator[] (const size_t index) {
                assert(index < C);
                return v[index];
            }

            inline const Vec<T,R>& operator[] (const size_t index) const {
                assert(index < C);
                return v[index];
            }

            inline bool equals(const Mat<T,R,C>& other, const T epsilon = Math<T>::AlmostZero) const {
                for (size_t c = 0; c < C; c++)
                    if (!v[c].equals(other[c], epsilon))
                        return false;
                return true;
            }

            inline bool null() const {
                return equals(Null);
            }

            inline Mat<T,R,C>& setIdentity() {
                for (size_t c = 0; c < C; c++)
                    for (size_t r = 0; r < R; r++)
                        v[c][r] = c == r ? static_cast<T>(1.0) : static_cast<T>(0.0);
                return *this;
            }

            inline Mat<T,R,C>& setNull() {
                for (size_t c = 0; c < C; c++)
                    for (size_t r = 0; r < R; r++)
                        v[c][r] = static_cast<T>(0.0);
                return *this;
            }
            
            inline const Mat<T,C,R> transposed() const {
                Mat<T,C,R> result;
                for (size_t c = 0; c < C; c++)
                    for (size_t r = 0; r < R; r++)
                        result[c][r] = v[r][c];
                return result;
            }
            
            inline void write(float* buffer) const {
                for (size_t c = 0; c < C; c++)
                    for (size_t r = 0; r < R; r++)
                        buffer[(c*C + r)] = v[c][r];
            }
        };

        template <typename T, size_t R, size_t C>
        inline Mat<T,R,C> operator* (const T left, const Mat<T,R,C>& right) {
            return right * left;
        }
        
        // Vector left multiplication with vector of dimension R
        template <typename T, size_t R, size_t C>
        inline const Vec<T,R> operator* (const Vec<T,R> left, const Mat<T,R,C>& right) {
            Vec<T,R> result;
            for (size_t c = 0; c < C; c++)
                result[c] = left.dot(right[c]);
            return result;
        }
    
        template <typename T, size_t R, size_t C>
        inline Vec<T,R>& operator*= (Vec<T,R> left, const Mat<T,R,C>& right) {
            return left = left * right;
        }
    
        // Vector left multiplication with list of vectors of dimension R
        template <typename T, size_t R, size_t C>
        inline const typename Vec<T,R>::List operator* (const typename Vec<T,R>::List& left, const Mat<T,R,C>& right) {
            typename Vec<T,R>::List result;
            result.reserve(left.size());
            
            typename Vec<T,R>::List::const_iterator it, end;
            for (it = left.begin(), end = left.end(); it != end; ++it)
                result.push_back(*it * right);
            return result;
        }
        
        template <typename T, size_t R, size_t C>
        inline const typename Vec<T,R>::List& operator*= (typename Vec<T,R>::List& left, const Mat<T,R,C>& right) {
            typename Vec<T,R>::List::iterator it, end;
            for (it = left.begin(), end = left.end(); it != end; ++it)
                *it *= right;
            return left;
        }

        // Vector left multiplication with vector of dimension R-1
        template <typename T, size_t R, size_t C>
        inline const Vec<T,R-1> operator* (const Vec<T,R-1>& left, const Mat<T,R,C>& right) {
            return (Vec<T,R>(left, static_cast<T>(1.0)) * right).overLast();
        }
        
        template <typename T, size_t R, size_t C>
        inline Vec<T,R-1>& operator*= (Vec<T,R-1>& left, const Mat<T,R,C>& right) {
            return left = left * right;
        }
        
        // Vector left multiplication with list of vectors of dimension R-1
        template <typename T, size_t R, size_t C>
        inline const typename Vec<T,R-1>::List operator* (const typename Vec<T,R-1>::List& left, const Mat<T,R,C>& right) {
            typename Vec<T,R-1>::List result;
            result.reserve(right.size());
            
            typename Vec<T,R-1>::List::const_iterator it, end;
            for (it = left.begin(), end = left.end(); it != end; ++it)
                result.push_back(*it * right);
            return result;
        }

        template <typename T, size_t R, size_t C>
        inline typename Vec<T,R-1>::List& operator*= (typename Vec<T,R-1>::List& left, const Mat<T,R,C>& right) {
            typename Vec<T,R-1>::List::iterator it, end;
            for (it = left.begin(), end = left.end(); it != end; ++it)
                *it *= right;
            return left;
        }
    
        template <typename T, size_t S>
        inline Mat<T,S,S>& transposeMatrix(Mat<T,S,S>& mat) {
            for (size_t c = 0; c < S; c++)
                for (size_t r = c + 1; r < S; r++)
                    std::swap(mat[c][r], mat[r][c]);
            return mat;
        }
        
        template <typename T, size_t S>
        inline const Mat<T,S-1,S-1> minorMatrix(const Mat<T,S,S>& mat, const size_t row, const size_t col) {
            Mat<T,S-1,S-1> min;
            size_t minC, minR;
            minC = 0;
            for (size_t c = 0; c < S; c++) {
                if (c != col) {
                    minR = 0;
                    for (size_t r = 0; r < S; r++)
                        if (r != row)
                            min[minC][minR++] = mat[c][r];
                    minC++;
                }
            }
            return min;
        }
        
        template <typename T, size_t S>
        struct MatrixDeterminant {
            inline T operator() (const Mat<T,S,S>& mat) const {
                // Laplace after first col
                T det = static_cast<T>(0.0);
                for (size_t r = 0; r < S; r++) {
                    const T f = static_cast<T>(r % 2 == 0 ? 1.0 : -1.0);
                    det += f * mat[0][r] * MatrixDeterminant<T,S-1>()(minorMatrix(mat, r, 0));
                }
                return det;
            }
        };

        // TODO: implement faster block-matrix based method for NxN matrices where N = 2^n
        
        template <typename T>
        struct MatrixDeterminant<T,3> {
            inline T operator() (const Mat<T,3,3>& mat) const {
                return (  mat[0][0]*mat[1][1]*mat[2][2]
                        + mat[1][0]*mat[2][1]*mat[0][2]
                        + mat[2][0]*mat[0][1]*mat[1][2]
                        - mat[2][0]*mat[1][1]*mat[0][2]
                        - mat[1][0]*mat[0][1]*mat[2][2]
                        - mat[0][0]*mat[2][1]*mat[1][2]);
            }
        };
        
        template <typename T>
        struct MatrixDeterminant<T,2> {
            inline T operator() (const Mat<T,2,2>& mat) const {
                return mat[0][0]*mat[1][1] - mat[1][0]*mat[0][1];
            }
        };
        
        template <typename T>
        struct MatrixDeterminant<T,1> {
            inline T operator() (const Mat<T,1,1>& mat) const {
                return mat[0][0];
            }
        };
        
        template <typename T, size_t S>
        inline T matrixDeterminant(const Mat<T,S,S>& mat) {
            return MatrixDeterminant<T,S>()(mat);
        }
        
        template <typename T, size_t S>
        inline Mat<T,S,S>& adjoinMatrix(Mat<T,S,S>& mat) {
            mat = adjointMatrix(mat);
            return mat;
        }
        
        template <typename T, size_t S>
        const Mat<T,S,S> adjointMatrix(const Mat<T,S,S>& mat) {
            Mat<T,S,S> result;
            for (size_t c = 0; c < S; c++) {
                for (size_t r = 0; r < S; r++) {
                    const T f = static_cast<T>((c + r) % 2 == 0 ? 1.0 : -1.0);
                    result[r][c] = f * matrixDeterminant(minorMatrix(mat, r, c)); // transpose the matrix on the fly
                }
            }
            return result;
        }
        
        template <typename T, size_t S>
        inline Mat<T,S,S>& invertMatrix(Mat<T,S,S>& mat, bool& invertible) {
            mat = invertedMatrix(mat, invertible);
            return mat;
        }

        template <typename T, size_t S>
        inline const Mat<T,S,S> invertedMatrix(const Mat<T,S,S>& mat, bool& invertible) {
            const T det = matrixDeterminant(mat);
            invertible = det != 0.0;
            if (!invertible)
                return mat;

            return adjointMatrix(mat) / det;
        }

        template <typename T>
        inline const Mat<T,4,4> perspectiveMatrix(const T fov, const T nearPlane, const T farPlane, const int width, const int height) {
            const T vFrustum = std::tan(Math<T>::radians(fov) / static_cast<T>(2.0)) * static_cast<T>(0.75) * nearPlane;
            const T hFrustum = vFrustum * static_cast<T>(width) / static_cast<T>(height);
            const T depth = farPlane - nearPlane;

            static const T zero = static_cast<T>(0.0);
            static const T one  = static_cast<T>(1.0);
            static const T two  = static_cast<T>(2.0);

            return Mat<T,4,4>(nearPlane / hFrustum, zero,                    zero,                               zero,
                              zero,                 nearPlane / vFrustum,    zero,                               zero,
                              zero,                 zero,                   -(farPlane + nearPlane) / depth,    -two * farPlane * nearPlane / depth,
                              zero,                 zero,                   -one,                                zero);
        }

        template <typename T>
        inline const Mat<T,4,4> orthoMatrix(const T nearPlane, const T farPlane, const T left, const T top, const T right, const T bottom) {
            const T width = right - left;
            const T height = top - bottom;
            const T depth = farPlane - nearPlane;

            static const T zero = static_cast<T>(0.0);
            static const T one  = static_cast<T>(1.0);
            static const T two  = static_cast<T>(2.0);

            return Mat<T,4,4>(two / width,  zero,            zero,          -(left + right) / width,
                              zero,         two / height,    zero,          -(top + bottom) / height,
                              zero,         zero,           -two / depth,   -(farPlane + nearPlane) / depth,
                              zero,         zero,            zero,           one);
        }

        template <typename T>
        inline const Mat<T,4,4> viewMatrix(const Vec<T,3>& direction, const Vec<T,3>& up) {
            const Vec<T,3>& f = direction;
            const Vec<T,3> s = crossed(f, up);
            const Vec<T,3> u = crossed(s, f);

            static const T zero = static_cast<T>(0.0);
            static const T one  = static_cast<T>(1.0);

            return Mat<T,4,4>( s[0],  s[1],  s[2], zero,
                               u[0],  u[1],  u[2], zero,
                              -f[0], -f[1], -f[2], zero,
                               zero,  zero,  zero, one);
        }

        // The returned matrix will rotate any point counter-clockwise about the given axis by the given angle.
        template <typename T>
        inline const Mat<T,4,4> rotationMatrix(const T angle, const Vec<T,3>& axis) {
            const T s = sinf(angle);
            const T c = cosf(angle);
            const T i = static_cast<T>(1.0 - c);
            
            const T ix  = i  * axis[0];
            const T ix2 = ix * axis[0];
            const T ixy = ix * axis[1];
            const T ixz = ix * axis[2];
            
            const T iy  = i  * axis[1];
            const T iy2 = iy * axis[1];
            const T iyz = iy * axis[2];
            
            const T iz2 = i  * axis[2] * axis[2];
            
            const T sx = s * axis[0];
            const T sy = s * axis[1];
            const T sz = s * axis[2];
            
            Mat<T,4,4> rotation;
            rotation[0][0] = ix2 + c;
            rotation[0][1] = ixy - sz;
            rotation[0][2] = ixz + sy;
            
            rotation[1][0] = ixy + sz;
            rotation[1][1] = iy2 + c;
            rotation[1][2] = iyz - sx;
            
            rotation[2][0] = ixz - sy;
            rotation[2][1] = iyz + sx;
            rotation[2][2] = iz2 + c;
            
            return rotation;
        }

        template <typename T>
        inline const Mat<T,4,4> rotationMatrix(const Quat<T>& quat) {
            const T a = quat.s;
            const T b = quat.v[0];
            const T c = quat.v[1];
            const T d = quat.v[2];

            const T a2 = a * a;
            const T b2 = b * b;
            const T c2 = c * c;
            const T d2 = d * d;

            Mat<T,4,4> rotation;
            rotation[0][0] = a2 + b2 - c2 - d2;
            rotation[0][1] = static_cast<T>(2.0 * b * c + 2.0 * a * d);
            rotation[0][2] = static_cast<T>(2.0 * b * d - 2.0 * a * c);

            rotation[1][0] = static_cast<T>(2.0 * b * c - 2.0 * a * d);
            rotation[1][1] = a2 - b2 + c2 - d2;
            rotation[1][2] = static_cast<T>(2.0 * c * d + 2.0 * a * b);

            rotation[2][0] = static_cast<T>(2.0 * b * d + 2.0 * a * c);
            rotation[2][1] = static_cast<T>(2.0 * c * d - 2.0 * a * b);
            rotation[2][2] = a2 - b2 - c2 + d2;

            return rotation;
        }

        template <typename T>
        inline const Mat<T,4,4> translationMatrix(const Vec<T,3>& delta) {
            Mat<T,4,4> translation;
            for (size_t i = 0; i < 3; i++)
                translation[3][i] = delta[i];
            return translation;
        }

        template <typename T>
        inline const Mat<T,4,4> scalingMatrix(const Vec<T,3>& factors) {
            Mat<T,4,4> scaling;
            for (size_t i = 0; i < 3; i++)
                scaling[i][i] = factors[i];
            return scaling;
        }

        template <typename T>
        inline const Mat<T,4,4> scalingMatrix(const T f) {
            return scalingMatrix(Vec<T,3>(f, f, f));
        }

        template <typename T, size_t R, size_t C>
        const Mat<T,R,C> Mat<T,R,C>::Identity = Mat<T,R,C>().setIdentity();

        template <typename T, size_t R, size_t C>
        const Mat<T,R,C> Mat<T,R,C>::Null = Mat<T,R,C>().setNull();
        
        template <typename T, size_t R, size_t C>
        const Mat<T,R,C> Mat<T,R,C>::Rot90XCW    = Mat<T,R,C>( static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(1.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0), -static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(1.0));
        template <typename T, size_t R, size_t C>
        const Mat<T,R,C> Mat<T,R,C>::Rot90YCW    = Mat<T,R,C>( static_cast<T>(0.0),  static_cast<T>(0.0), -static_cast<T>(1.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                               static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(1.0));
        template <typename T, size_t R, size_t C>
        const Mat<T,R,C> Mat<T,R,C>::Rot90ZCW    = Mat<T,R,C>( static_cast<T>(0.0),  static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                              -static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(1.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(1.0));
        template <typename T, size_t R, size_t C>
        const Mat<T,R,C> Mat<T,R,C>::Rot90XCCW   = Mat<T,R,C>( static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(0.0), -static_cast<T>(1.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(1.0));
        template <typename T, size_t R, size_t C>
        const Mat<T,R,C> Mat<T,R,C>::Rot90YCCW   = Mat<T,R,C>( static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(1.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                              -static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(1.0));
        template <typename T, size_t R, size_t C>
        const Mat<T,R,C> Mat<T,R,C>::Rot90ZCCW   = Mat<T,R,C>( static_cast<T>(0.0), -static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                               static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(1.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(1.0));
        template <typename T, size_t R, size_t C>
        const Mat<T,R,C> Mat<T,R,C>::Rot180X     = Mat<T,R,C>( static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0), -static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(0.0), -static_cast<T>(1.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(1.0));
        template <typename T, size_t R, size_t C>
        const Mat<T,R,C> Mat<T,R,C>::Rot180Y     = Mat<T,R,C>(-static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(0.0), -static_cast<T>(1.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(1.0));
        template <typename T, size_t R, size_t C>
        const Mat<T,R,C> Mat<T,R,C>::Rot180Z     = Mat<T,R,C>(-static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0), -static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(1.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(1.0));
        template <typename T, size_t R, size_t C>
        const Mat<T,R,C> Mat<T,R,C>::MirX        = Mat<T,R,C>(-static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(1.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(1.0));
        template <typename T, size_t R, size_t C>
        const Mat<T,R,C> Mat<T,R,C>::MirY        = Mat<T,R,C>( static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0), -static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(1.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(1.0));
        template <typename T, size_t R, size_t C>
        const Mat<T,R,C> Mat<T,R,C>::MirZ        = Mat<T,R,C>( static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(1.0),  static_cast<T>(0.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(0.0), -static_cast<T>(1.0),  static_cast<T>(0.0),
                                                               static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(0.0),  static_cast<T>(1.0));
        template <typename T, size_t R, size_t C>
        const Mat<T,R,C> Mat<T,R,C>::YIQToRGB     = Mat<T,R,C>( static_cast<T>(1.0), static_cast<T>( 0.9563), static_cast<T> (0.6210),
                                                                static_cast<T>(1.0), static_cast<T>(-0.2721), static_cast<T>(-0.6474),
                                                                static_cast<T>(1.0), static_cast<T>(-1.1070), static_cast<T>( 1.7046));

        template <typename T, size_t R, size_t C>
        const Mat<T,R,C> Mat<T,R,C>::RGBToYIQ     = Mat<T,R,C>( static_cast<T>(0.299),    static_cast<T>( 0.587),    static_cast<T>( 0.114),
                                                                static_cast<T>(0.595716), static_cast<T>(-0.274453), static_cast<T>(-0.321263),
                                                                static_cast<T>(0.211456), static_cast<T>(-0.522591), static_cast<T>( 0.311135));


        typedef Mat<float,2,2> Mat2f;
        typedef Mat<float,3,3> Mat3f;
        typedef Mat<float,4,4> Mat4f;
        typedef Mat<double,2,2> Mat2d;
        typedef Mat<double,3,3> Mat3d;
        typedef Mat<double,4,4> Mat4d;
    }
}

#endif
