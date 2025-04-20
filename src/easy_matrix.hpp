//
// Created by marc on 20/04/25.
//

#ifndef EASY_MATRIX_HPP
#define EASY_MATRIX_HPP

namespace Abyss
{

/**
 * A utility class for simplifying common matrix operations.
 * The class maintains a 4x4 transformation matrix and provides chainable methods
 * for performing operations like scaling, rotation, and translation.
 * Correct order to chain transformations is:
 *   1. Scale
 *   2. Rotate
 *   3. Translate
 */
struct EasyMatrix
{
    float m_matrix[16] = {};

    /**
     * Constructs an identity matrix
     */
    EasyMatrix();

    /**
     * Applies scaling to the matrix
     * @param scale Uniform scaling factor
     * @return Reference to this object for method chaining
     */
    EasyMatrix& Scale(float scale);

    /**
     * Applies scaling to the matrix
     * @param sx X-axis scaling factor
     * @param sy Y-axis scaling factor
     * @param sz Z-axis scaling factor
     * @return Reference to this object for method chaining
     */
    EasyMatrix& Scale(float sx, float sy, float sz);
    
    /**
     * Applies XY rotation to the matrix
     * @param ax X-axis rotation in radians
     * @param ay Y-axis rotation in radians
     * @return Reference to this object for method chaining
     */
    EasyMatrix& RotateXY(float ax, float ay);
    
    /**
     * Applies translation to the matrix
     * @param x X-axis translation
     * @param y Y-axis translation
     * @param z Z-axis translation
     * @return Reference to this object for method chaining
     */
    EasyMatrix& Translate(float x, float y, float z);

    /**
     * Gets a pointer to the underlying matrix
     * @return Pointer to the 16-element float array representing the matrix
     */
    [[nodiscard]] const float* GetMatrix() const;

    /**
     * Combines the current matrix with another matrix
     * @param temp Pointer to the matrix to combine with
     * @return Reference to this object for method chaining
     */
    EasyMatrix& Combine(const float* temp);
};

} // Abyss

#endif //EASY_MATRIX_HPP
