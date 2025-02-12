Project Description

The Matrix Calculator is capable of performing both basic matrix operations, such as addition, subtraction, multiplication, exponentiation, scalar multiplication, and scalar division, as well as more complex operations using the Gaussian elimination method. These advanced operations include calculating the determinant of a square matrix, converting a matrix to upper triangular form, and computing the rank of a matrix. The calculator also allows for matrix content manipulation, such as transposition, vertical and horizontal concatenation, and submatrix extraction.

The calculator distinguishes between different types of matrices, which helps optimize the execution of operations. For example, multiplying a zero matrix by a scalar can be optimized accordingly. The recognized matrix types include zero matrices, scalar matrices, square matrices, upper triangular matrices, diagonal matrices, and identity matrices.

The user interface is implemented as a REPL (Read-Eval-Print Loop). Users can input matrices into variables directly from the command line and perform operations on stored matrices, either individually or through infix expressions. Additionally, users can save and later load matrices.

The calculator optimizes matrix storage in memory based on their type, which contributes to faster computations.

It is also important to emphasize that matrices are implemented as immutable objects, which increases code stability and predictability. Each operation creates a new matrix object, and the original object is discarded.
Use of Polymorphism

In this project, polymorphism is used to implement different types of matrices. Depending on the type of matrix, some operations may be either invalid or optimized. To simplify the implementation of these operations and the handling of related errors, I have designed an architecture with multiple matrix types. The type of a matrix can be determined based on its current content.
