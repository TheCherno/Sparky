/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file rs_matrix.rsh
 *  \brief Matrix functions.
 *
 * These functions let you manipulate square matrices of rank 2x2, 3x3, and 4x4.
 * They are particularly useful for graphical transformations and are
 * compatible with OpenGL.
 *
 * A few general notes:
 *
 * \li We use a zero-based index for rows and columns.  E.g. the last element of
 * a \ref rs_matrix4x4 is found at (3, 3).
 *
 * \li RenderScript uses column-based vectors.  Transforming a vector is done by
 * postmultiplying the vector, e.g. <em>(matrix * vector)</em>, as provided by
 * \ref rsMatrixMultiply.
 *
 * \li To create a transformation matrix that performs two transformations at
 * once, multiply the two source matrices, with the first transformation as the
 * right argument.  E.g. to create a transformation matrix that applies the
 * transformation \e s1 followed by \e s2, call
 * </c>rsMatrixLoadMultiply(&combined, &s2, &s1)</c>.
 * This derives from <em>s2 * (s1 * v)</em>, which is <em>(s2 * s1) * v</em>.
 *
 * \li We have two style of functions to create transformation matrices:
 * rsMatrixLoad<em>Transformation</em> and rsMatrix<em>Transformation</em>.  The
 * former style simply stores the transformation matrix in the first argument.
 * The latter modifies a pre-existing transformation matrix so that the new
 * transformation happens first.  E.g. if you call \ref rsMatrixTranslate
 * on a matrix that already does a scaling, the resulting matrix when applied
 * to a vector will first do the translation then the scaling.
 *
 */

#ifndef __RS_MATRIX_RSH__
#define __RS_MATRIX_RSH__

/**
 * Set an element of a matrix.
 *
 * @param m The matrix that will be modified.
 * @param col The zero-based column of the element to be set.
 * @param row The zero-based row of the element to be set.
 * @param v The value to set.
 *
 * \warning The order of the column and row parameters may be
 * unexpected.
 *
 * @return void
 */
_RS_RUNTIME void __attribute__((overloadable))
rsMatrixSet(rs_matrix4x4 *m, uint32_t col, uint32_t row, float v);
/**
 * \overload
 */
_RS_RUNTIME void __attribute__((overloadable))
rsMatrixSet(rs_matrix3x3 *m, uint32_t col, uint32_t row, float v);
/**
 * \overload
 */
_RS_RUNTIME void __attribute__((overloadable))
rsMatrixSet(rs_matrix2x2 *m, uint32_t col, uint32_t row, float v);

/**
 * Returns one element of a matrix.
 *
 * @param m The matrix to extract the element from.
 * @param col The zero-based column of the element to be extracted.
 * @param row The zero-based row of the element to extracted.
 *
 * \warning The order of the column and row parameters may be
 * unexpected.
 *
 * @return float
 */
_RS_RUNTIME float __attribute__((overloadable))
rsMatrixGet(const rs_matrix4x4 *m, uint32_t col, uint32_t row);
/**
 * \overload
 */
_RS_RUNTIME float __attribute__((overloadable))
rsMatrixGet(const rs_matrix3x3 *m, uint32_t col, uint32_t row);
/**
 * \overload
 */
_RS_RUNTIME float __attribute__((overloadable))
rsMatrixGet(const rs_matrix2x2 *m, uint32_t col, uint32_t row);

/**
 * Set the elements of a matrix to the identity matrix.
 *
 * @param m The matrix to set.
 */
extern void __attribute__((overloadable)) rsMatrixLoadIdentity(rs_matrix4x4 *m);
/**
 * \overload
 */
extern void __attribute__((overloadable)) rsMatrixLoadIdentity(rs_matrix3x3 *m);
/**
 * \overload
 */
extern void __attribute__((overloadable)) rsMatrixLoadIdentity(rs_matrix2x2 *m);

/**
 * Set the elements of a matrix from an array of floats.
 *
 * The array of floats should be in row-major order, i.e. the element a
 * <em>row 0, column 0</em> should be first, followed by the element at
 * <em>row 0, column 1</em>, etc.
 *
 * @param m The matrix to set.
 * @param v The array of values to set the matrix to. These arrays should be
 * 4, 9, or 16 floats long, depending on the matrix size.
 */
extern void __attribute__((overloadable)) rsMatrixLoad(rs_matrix4x4 *m, const float *v);
/**
 * \overload
 */
extern void __attribute__((overloadable)) rsMatrixLoad(rs_matrix3x3 *m, const float *v);
/**
 * \overload
 */
extern void __attribute__((overloadable)) rsMatrixLoad(rs_matrix2x2 *m, const float *v);
/**
 * Set the elements of a matrix from another matrix.
 *
 * If the source matrix is smaller than the destination, the rest of the
 * destination is filled with elements of the identity matrix.  E.g.
 * loading a rs_matrix2x2 into a rs_matrix4x4 will give:
 *
 * \htmlonly<table>
 * <tr><td>m00</td><td>m01</td><td>0.0</td><td>0.0</td></tr>
 * <tr><td>m10</td><td>m11</td><td>0.0</td><td>0.0</td></tr>
 * <tr><td>0.0</td><td>0.0</td><td>1.0</td><td>0.0</td></tr>
 * <tr><td>0.0</td><td>0.0</td><td>0.0</td><td>1.0</td></tr>
 * </table>\endhtmlonly
 *
 * @param m The matrix to set.
 * @param v The source matrix.
 */
extern void __attribute__((overloadable)) rsMatrixLoad(rs_matrix4x4 *m, const rs_matrix4x4 *v);
/**
 * \overload
 */
extern void __attribute__((overloadable)) rsMatrixLoad(rs_matrix4x4 *m, const rs_matrix3x3 *v);
/**
 * \overload
 */
extern void __attribute__((overloadable)) rsMatrixLoad(rs_matrix4x4 *m, const rs_matrix2x2 *v);
/**
 * \overload
 */
extern void __attribute__((overloadable)) rsMatrixLoad(rs_matrix3x3 *m, const rs_matrix3x3 *v);
/**
 * \overload
 */
extern void __attribute__((overloadable)) rsMatrixLoad(rs_matrix2x2 *m, const rs_matrix2x2 *v);

/**
 * Load a rotation matrix.
 *
 * This function creates a rotation matrix.  The axis of rotation is the
 * <em>(x, y, z)</em> vector.
 *
 * To rotate a vector, multiply the vector by the created matrix
 * using \ref rsMatrixMultiply.
 *
 * See http://en.wikipedia.org/wiki/Rotation_matrix .
 *
 * @param m The matrix to set.
 * @param rot How much rotation to do, in degrees.
 * @param x The x component of the vector that is the axis of rotation.
 * @param y The y component of the vector that is the axis of rotation.
 * @param z The z component of the vector that is the axis of rotation.
 */
extern void __attribute__((overloadable))
rsMatrixLoadRotate(rs_matrix4x4 *m, float rot, float x, float y, float z);

/**
 * Load a scale matrix.
 *
 * This function creates a scaling matrix, where each component of a
 * vector is multiplied by a number.  This number can be negative.
 *
 * To scale a vector, multiply the vector by the created matrix
 * using \ref rsMatrixMultiply.
 *
 * @param m The matrix to set.
 * @param x The multiple to scale the x components by.
 * @param y The multiple to scale the y components by.
 * @param z The multiple to scale the z components by.
 */
extern void __attribute__((overloadable))
rsMatrixLoadScale(rs_matrix4x4 *m, float x, float y, float z);

/**
 * Load a translation matrix.
 *
 * This function creates a translation matrix, where a
 * number is added to each element of a vector.
 *
 * To translate a vector, multiply the vector by the created matrix
 * using \ref rsMatrixMultiply.
 *
 * @param m The matrix to set.
 * @param x The number to add to each x component.
 * @param y The number to add to each y component.
 * @param z The number to add to each z component.
 */
extern void __attribute__((overloadable))
rsMatrixLoadTranslate(rs_matrix4x4 *m, float x, float y, float z);

/**
 * Multiply two matrices.
 *
 * Sets \e m to the matrix product of <em>lhs * rhs</em>.
 *
 * To combine two 4x4 transformaton matrices, multiply the second transformation matrix
 * by the first transformation matrix.  E.g. to create a transformation matrix that applies
 * the transformation \e s1 followed by \e s2, call
 * </c>rsMatrixLoadMultiply(&combined, &s2, &s1)</c>.
 *
 * \warning Prior to version 21, storing the result back into right matrix is not supported and
 * will result in undefined behavior.  Use rsMatrixMulitply instead.   E.g. instead of doing
 * rsMatrixLoadMultiply (&m2r, &m2r, &m2l), use rsMatrixMultiply (&m2r, &m2l).
 * rsMatrixLoadMultiply (&m2l, &m2r, &m2l) works as expected.
 *
 * @param m The matrix to set.
 * @param lhs The left matrix of the product.
 * @param rhs The right matrix of the product.
 */
extern void __attribute__((overloadable))
rsMatrixLoadMultiply(rs_matrix4x4 *m, const rs_matrix4x4 *lhs, const rs_matrix4x4 *rhs);
/**
 * \overload
 */
extern void __attribute__((overloadable))
rsMatrixLoadMultiply(rs_matrix3x3 *m, const rs_matrix3x3 *lhs, const rs_matrix3x3 *rhs);
/**
 * \overload
 */
extern void __attribute__((overloadable))
rsMatrixLoadMultiply(rs_matrix2x2 *m, const rs_matrix2x2 *lhs, const rs_matrix2x2 *rhs);

/**
 * Multiply a matrix into another one.
 *
 * Sets \e m to the matrix product <em>m * rhs</em>.
 *
 * When combining two 4x4 transformation matrices using this function, the resulting
 * matrix will correspond to performing the \e rhs transformation first followed by
 * the original \e m transformation.
 *
 * @param m The left matrix of the product and the matrix to be set.
 * @param rhs The right matrix of the product.
 */
extern void __attribute__((overloadable))
rsMatrixMultiply(rs_matrix4x4 *m, const rs_matrix4x4 *rhs);
/**
 * \overload
 */
extern void __attribute__((overloadable))
rsMatrixMultiply(rs_matrix3x3 *m, const rs_matrix3x3 *rhs);
/**
 * \overload
 */
extern void __attribute__((overloadable))
rsMatrixMultiply(rs_matrix2x2 *m, const rs_matrix2x2 *rhs);

/**
 * Multiply the matrix \e m with a rotation matrix.
 *
 * This function modifies a transformation matrix to first do a rotation.
 * The axis of rotation is the <em>(x, y, z)</em> vector.
 *
 * To apply this combined transformation to a vector, multiply
 * the vector by the created matrix using \ref rsMatrixMultiply.
 *
 * @param m The matrix to modify.
 * @param rot How much rotation to do, in degrees.
 * @param x The x component of the vector that is the axis of rotation.
 * @param y The y component of the vector that is the axis of rotation.
 * @param z The z component of the vector that is the axis of rotation.
 */
extern void __attribute__((overloadable))
rsMatrixRotate(rs_matrix4x4 *m, float rot, float x, float y, float z);

/**
 * Multiply the matrix \e m with a scaling matrix.
 *
 * This function modifies a transformation matrix to first do a scaling.
 * When scaling, each component of a vector is multiplied by a number.
 * This number can be negative.
 *
 * To apply this combined transformation to a vector, multiply
 * the vector by the created matrix using \ref rsMatrixMultiply.
 *
 * @param m The matrix to modify.
 * @param x The multiple to scale the x components by.
 * @param y The multiple to scale the y components by.
 * @param z The multiple to scale the z components by.
 */
extern void __attribute__((overloadable))
rsMatrixScale(rs_matrix4x4 *m, float x, float y, float z);

/**
 * Multiply the matrix \e m with a translation matrix.
 *
 * This function modifies a transformation matrix to first
 * do a translation.  When translating, a number is added
 * to each component of a vector.
 *
 * To apply this combined transformation to a vector, multiply
 * the vector by the created matrix using \ref rsMatrixMultiply.
 *
 * @param m The matrix to modify.
 * @param x The number to add to each x component.
 * @param y The number to add to each y component.
 * @param z The number to add to each z component.
 */
extern void __attribute__((overloadable))
rsMatrixTranslate(rs_matrix4x4 *m, float x, float y, float z);

/**
 * Load an orthographic projection matrix.
 *
 * Constructs an orthographic projection matrix, transforming the box
 * identified by the six clipping planes <em>left, right, bottom, top,
 * near, far</em> into a unit cube with a corner at
 * <em>(-1, -1, -1)</em> and the opposite at <em>(1, 1, 1)</em>.
 *
 * To apply this projection to a vector, multiply the vector by the
 * created matrix using \ref rsMatrixMultiply.
 *
 * See https://en.wikipedia.org/wiki/Orthographic_projection .
 *
 * @param m The matrix to set.
 * @param left
 * @param right
 * @param bottom
 * @param top
 * @param near
 * @param far
 */
extern void __attribute__((overloadable))
rsMatrixLoadOrtho(rs_matrix4x4 *m, float left, float right, float bottom, float top, float near, float far);

/**
 * Load a frustum projection matrix.
 *
 * Constructs a frustum projection matrix, transforming the box
 * identified by the six clipping planes <em>left, right, bottom, top,
 * near, far</em>.
 *
 * To apply this projection to a vector, multiply the vector by the
 * created matrix using \ref rsMatrixMultiply.
 *
 * @param m The matrix to set.
 * @param left
 * @param right
 * @param bottom
 * @param top
 * @param near
 * @param far
 */
extern void __attribute__((overloadable))
rsMatrixLoadFrustum(rs_matrix4x4 *m, float left, float right, float bottom, float top, float near, float far);

/**
 * Load a perspective projection matrix.
 *
 * Constructs a perspective projection matrix, assuming a symmetrical field of view.
 *
 * To apply this projection to a vector, multiply the vector by the
 * created matrix using \ref rsMatrixMultiply.
 *
 * @param m The matrix to set.
 * @param fovy Field of view, in degrees along the Y axis.
 * @param aspect Ratio of x / y.
 * @param near The near clipping plane.
 * @param far The far clipping plane.
 */
extern void __attribute__((overloadable))
rsMatrixLoadPerspective(rs_matrix4x4* m, float fovy, float aspect, float near, float far);

#if !defined(RS_VERSION) || (RS_VERSION < 14)
/**
 * Multiply a vector by a matrix.
 *
 * Returns the post-multiplication of the vector by the matrix, ie. <em>m * in</em>.
 *
 * When multiplying a \e float3 to a \e rs_matrix4x4, the vector is expanded with (1).
 *
 * When multiplying a \e float2 to a \e rs_matrix4x4, the vector is expanded with (0, 1).
 *
 * When multiplying a \e float2 to a \e rs_matrix3x3, the vector is expanded with (0).
 *
 * This function is available in API version 10-13.  Starting with API 14,
 * the function takes a const matrix as the first argument.
 */
_RS_RUNTIME float4 __attribute__((overloadable))
rsMatrixMultiply(rs_matrix4x4 *m, float4 in);

/**
 * \overload
 */
_RS_RUNTIME float4 __attribute__((overloadable))
rsMatrixMultiply(rs_matrix4x4 *m, float3 in);

/**
 * \overload
 */
_RS_RUNTIME float4 __attribute__((overloadable))
rsMatrixMultiply(rs_matrix4x4 *m, float2 in);

/**
 * \overload
 */
_RS_RUNTIME float3 __attribute__((overloadable))
rsMatrixMultiply(rs_matrix3x3 *m, float3 in);

/**
 * \overload
 */
_RS_RUNTIME float3 __attribute__((overloadable))
rsMatrixMultiply(rs_matrix3x3 *m, float2 in);

/**
 * \overload
 */
_RS_RUNTIME float2 __attribute__((overloadable))
rsMatrixMultiply(rs_matrix2x2 *m, float2 in);
#else
/**
 * Multiply a vector by a matrix.
 *
 * Returns the post-multiplication of the vector of the matrix, i.e. <em>m * in</em>.
 *
 * When multiplying a \e float3 to a \e rs_matrix4x4, the vector is expanded with (1).
 *
 * When multiplying a \e float2 to a \e rs_matrix4x4, the vector is expanded with (0, 1).
 *
 * When multiplying a \e float2 to a \e rs_matrix3x3, the vector is expanded with (0).
 *
 * This function is available starting with API version 14.
 */
_RS_RUNTIME float4 __attribute__((overloadable))
rsMatrixMultiply(const rs_matrix4x4 *m, float4 in);

/**
 * \overload
 */
_RS_RUNTIME float4 __attribute__((overloadable))
rsMatrixMultiply(const rs_matrix4x4 *m, float3 in);

/**
 * \overload
 */
_RS_RUNTIME float4 __attribute__((overloadable))
rsMatrixMultiply(const rs_matrix4x4 *m, float2 in);

/**
 * \overload
 */
_RS_RUNTIME float3 __attribute__((overloadable))
rsMatrixMultiply(const rs_matrix3x3 *m, float3 in);

/**
 * \overload
 */
_RS_RUNTIME float3 __attribute__((overloadable))
rsMatrixMultiply(const rs_matrix3x3 *m, float2 in);

/**
 * \overload
 */
_RS_RUNTIME float2 __attribute__((overloadable))
rsMatrixMultiply(const rs_matrix2x2 *m, float2 in);
#endif


/**
 * Inverts a matrix in place.
 *
 * Returns true if the matrix was successfully inverted.
 *
 * @param m The matrix to invert.
 */
extern bool __attribute__((overloadable)) rsMatrixInverse(rs_matrix4x4 *m);

/**
 * Inverts and transpose a matrix in place.
 *
 * The matrix is first inverted then transposed.
 * Returns true if the matrix was successfully inverted.
 *
 * @param m The matrix to modify.
 */
extern bool __attribute__((overloadable)) rsMatrixInverseTranspose(rs_matrix4x4 *m);

/**
 * Transpose the matrix m in place.
 *
 * @param m The matrix to transpose.
 */
extern void __attribute__((overloadable)) rsMatrixTranspose(rs_matrix4x4 *m);
/**
 * \overload
 */
extern void __attribute__((overloadable)) rsMatrixTranspose(rs_matrix3x3 *m);
/**
 * \overload
 */
extern void __attribute__((overloadable)) rsMatrixTranspose(rs_matrix2x2 *m);


#endif
