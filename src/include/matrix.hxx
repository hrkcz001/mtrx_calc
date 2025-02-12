/**
 * @file matrix.hxx
 * @author morozan1
 * @brief Matrix class and its derived classes
 */
#include <vector>
#include <memory>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>
#include <sstream>
/**
 * @brief Generic Matrix class
 */
class Matrix {
public:
    /**
     * Constructs a new empty generic Matrix
     * @brief default constructor
     */
    Matrix();

    /**
     * Construct a new generic Matrix from another generic Matrix
     * @brief copy constructor
     * @param matrix: reference to another generic Matrix
     */
    Matrix(const Matrix& matrix);

    /**
     * Construct a new generic Matrix from a vector of vectors of doubles
     * @brief construct a new generic Matrix object
     * @param data: vector of vectors of doubles
     */
    Matrix(std::vector<std::vector<double>> data);

    virtual ~Matrix() = default; ///< default destructor
 
    /**
     * @brief Get the number of rows
     * @return size_t: number of rows
     */
    virtual size_t rows() const;

    /**
     * @brief Get the number of columns
     * @return size_t: number of columns
     */
    virtual size_t cols() const;
    
    /**
      * Transform the matrix to a more specific type
      * Can transform to: NumberMatrix, ZeroMatrix, SquareMatrix or return itself
      * @brief Matrix casting
      * @return std::shared_ptr<Matrix>: pointer to the new matrix
      */
    virtual std::shared_ptr<Matrix> transform();

    /**
     * @brief String representation of the matrix
     * @return std::string: string representation of the matrix
     */
    std::string toString() const;
   
    virtual bool isEmpty() const;   ///< returns true if matrix is empty
    virtual bool isNumber() const;  ///< returns true if matrix is single number
    virtual bool isZero() const;    ///< returns true if matrix is zero matrix
    virtual bool isSquare() const;  ///< returns true if matrix is square matrix

  // NOTE: operators
    
    /**
     * @brief get element
     * @param row: row index
     * @param col: column index
     * @throw std::out_of_range: if row or col is out of range
     * @return double: element at (row, col)
     */
    virtual double  get(size_t row, size_t col) const;
    /**
     * @brief get number
     * @throw std::runtime_error: if matrix is not single number
     * @return double: number
     */
    virtual double  number() const;
    /**
     * @brief add matrix to matrix
     * @param rhs: right hand side matrix
     * @throw std::runtime_error: if matrix has different dimensions
     * @return std::shared_ptr<Matrix>: pointer to the new matrix
     */
    virtual std::shared_ptr<Matrix> add(const std::shared_ptr<Matrix> rhs) const;
    /**
     * @brief negate matrix
     * @return std::shared_ptr<Matrix>: pointer to the new matrix
     */
    virtual std::shared_ptr<Matrix> neg() const;
    /**
     * @brief subtract matrix from matrix
     * @param rhs: right hand side matrix
     * @throw std::runtime_error: if matrix has different dimensions
     * @return std::shared_ptr<Matrix>: pointer to the new matrix
     */
    virtual std::shared_ptr<Matrix> sub(const std::shared_ptr<Matrix> rhs) const;
    /**
     * @brief multiply matrix by another matrix or scalar
     * @param rhs: right hand side matrix
     * @throw std::runtime_error: if matrix is not scalar and has different dimensions
     * @return std::shared_ptr<Matrix>: pointer to the new matrix
     */
    virtual std::shared_ptr<Matrix> prod(const std::shared_ptr<Matrix> rhs) const;
    /**
     * @brief divide matrix by scalar
     * @param rhs: right hand side matrix (scalar)
     * @throw std::runtime_error: if rhs is zero
     * @return std::shared_ptr<Matrix>: pointer to the new matrix
     */
    virtual std::shared_ptr<Matrix> div(const std::shared_ptr<Matrix> rhs) const;
    /**
     * @brief power matrix by scalar
     * @param rhs: right hand side matrix (scalar) less or equal to 100
     * @throw std::runtime_error: if matrix is not square
     * @return std::shared_ptr<Matrix>: pointer to the new matrix
     */
    virtual std::shared_ptr<Matrix> power(const std::shared_ptr<Matrix> rhs) const;
    /**
     * @brief transpose matrix
     * @return std::shared_ptr<Matrix>: pointer to the new matrix
     */
    virtual std::shared_ptr<Matrix> transpose() const;
    /**
     * @brief horizontaly concatenate matrix with matrix
     * @param rhs: right hand side matrix
     * @throw std::runtime_error: if matrix has different number of rows
     * @return std::shared_ptr<Matrix>: pointer to the new matrix
     */
    virtual std::shared_ptr<Matrix> hconcat(const std::shared_ptr<Matrix> rhs) const;
    /**
     * @brief verticaly concatenate matrix with matrix
     * @param rhs: right hand side matrix
     * @throw std::runtime_error: if matrix has different number of columns
     * @return std::shared_ptr<Matrix>: pointer to the new matrix
     */
    virtual std::shared_ptr<Matrix> vconcat(const std::shared_ptr<Matrix> rhs) const;
    /**
     * @brief crop matrix
     * @param rhs: right hand side matrix, matrix with new size and offset, first two elements are new size, second two elements are offset
     * @throw std::runtime_error: if options are invalid: has different amount of elements, new size is bigger than original size, offset is bigger than original size, options are nagative or aren't natural
     * @return std::shared_ptr<Matrix>: pointer to the new matrix
     */
    virtual std::shared_ptr<Matrix> crop(const std::shared_ptr<Matrix> rhs) const;
    /**
     * @brief get determinant of matrix
     * @throw std::runtime_error: if matrix is not square
     * @return std::shared_ptr<Matrix>: pointer to the new matrix
     */
    virtual std::shared_ptr<Matrix> det() const;
    /**
     * @brief get rank of matrix
     * @return std::shared_ptr<Matrix>: pointer to the new matrix
     */
    virtual std::shared_ptr<Matrix> rank() const;
    /**
     * @brief do Gaussian elimination on matrix
     * @return std::shared_ptr<Matrix>: pointer to the new matrix
     */
    virtual std::shared_ptr<Matrix> gem() const;

    /**
     * returns type name - "Matrix"
     * @brief get matrix type as string
     * @return std::string: matrix as string
     */
    virtual std::string whoami() const;

protected:
    std::vector<std::vector<double>> m_data; ///< vector of vectors to store matrix data
    bool m_empty; ///< true if matrix is empty
};
/**
 * @brief Number class for scalar operations
 */
class Number : public Matrix {
public:
    /**
     * Number constructor from double
     * @brief constructor
     * @param m: number
     */
    Number(double m);
    /**
     * Number constructor from another Number
     * @brief copy constructor
     * @param m: number
     */
    Number(const Number& m);

    /**
     * Can be nedded for virtual inheritance from it, always returns itself
     */
    virtual std::shared_ptr<Matrix> transform() override;

    virtual bool isNumber() const override; ///< Always returns true

    /**
     * @brief get number
     * @return double: number
     */
    virtual double  number() const override;
    /**
     * @brief sum of numbers
     * @param rhs: right hand side matrix (scalar)
     * @throw std::runtime_error: if rhs is not scalar
     * @return std::shared_ptr<Matrix>: pointer to the new matrix
     */
    virtual std::shared_ptr<Matrix> add(const std::shared_ptr<Matrix> rhs) const override;      
    /**
     * @brief subtract numbers
     * @param rhs: right hand side matrix (scalar)
     * @throw std::runtime_error: if rhs is not scalar
     * @return std::shared_ptr<Matrix>: pointer to the new matrix
     */
    virtual std::shared_ptr<Matrix> sub(const std::shared_ptr<Matrix> rhs) const override;      
    /**
     * @brief multiply numbers
     * @param rhs: right hand side matrix (scalar)
     * @throw std::runtime_error: if rhs is not scalar
     * @return std::shared_ptr<Matrix>: pointer to the new matrix
     */
    virtual std::shared_ptr<Matrix> prod(const std::shared_ptr<Matrix> rhs) const override;      
    /**
     * @brief divide numbers
     * @param rhs: right hand side matrix (scalar)
     * @throw std::runtime_error: if rhs is zero or rhs is not scalar
     * @return std::shared_ptr<Matrix>: pointer to the new matrix
     */
    virtual std::shared_ptr<Matrix> div(const std::shared_ptr<Matrix> rhs) const override;             
    /**
     * scalars can be doubles
     * @brief power number by scalar
     * @throw std::runtime_error: if rhs is not scalar
     * @return std::shared_ptr<Matrix>: pointer to the new matrix
     */
    virtual std::shared_ptr<Matrix> power(const std::shared_ptr<Matrix> rhs) const override;

    /**
     * as determinant of number is itself, returns stored number
     * @return std::shared_ptr<Matrix>: pointer to the new matrix
     */
    virtual std::shared_ptr<Matrix> det() const override;

    virtual std::string whoami() const override; ///< returns type name - "Number"
};
/**
 * @brief ZeroMatrix class for matices filled with zeros
 */
class ZeroMatrix : public Matrix {
public:
    /**
     * ZeroMatrix constructor from size
     * @brief constructor
     * @param rows: number of rows
     * @param cols: number of columns
     */
    ZeroMatrix(size_t rows, size_t cols);    
    /**
     * ZeroMatrix constructor from another ZeroMatrix
     * @brief copy constructor
     * @param m: matrix
     */
    ZeroMatrix(const ZeroMatrix& m);
    virtual size_t rows() const override;
    virtual size_t cols() const override; 

    /**
     * Can be nedded for virtual inheritance from it, always returns itself
     */
    virtual std::shared_ptr<Matrix> transform() override;

    virtual bool isZero() const override; ///< Always returns true

    virtual double  get(size_t row, size_t col) const override;
    virtual std::shared_ptr<Matrix> add(const std::shared_ptr<Matrix> rhs) const override;      
    virtual std::shared_ptr<Matrix> sub(const std::shared_ptr<Matrix> rhs) const override;      
    virtual std::shared_ptr<Matrix> prod(const std::shared_ptr<Matrix> rhs) const override;      
    virtual std::shared_ptr<Matrix> div(const std::shared_ptr<Matrix> rhs) const override;             
    virtual std::shared_ptr<Matrix> power(const std::shared_ptr<Matrix> rhs) const override; 
    virtual std::shared_ptr<Matrix> transpose() const override;  
    virtual std::shared_ptr<Matrix> det() const override;

    virtual std::string whoami() const override; ///< returns type name - "ZeroMatrix"

protected:
    size_t m_rows;
    size_t m_cols;
};
/**
 * @brief SquareMatrix class for square matrices
 */
class SquareMatrix : public Matrix {
public:
    /**
     * Constructs an empty SquareMatrix
     * @brief default constructor
     * @param size: number of rows and columns
     */
    SquareMatrix();
    /**
     * Constructs a SquareMatrix from another SquareMatrix
     * @brief copy constructor
     * @param m: square matrix
     */
    SquareMatrix(const SquareMatrix& m);
    /**
     * Constructs a SquareMatrix from another Matrix
     * @param m: matrix
     */
    SquareMatrix(const Matrix& m);
    virtual size_t rows() const override;
    virtual size_t cols() const override;

    /**
      * Transform the SquareMatrix to a more specific type
      * Can transform to: DiagonalMatrix or return itself
      * @brief Matrix casting
      * @return std::shared_ptr<Matrix>: pointer to the new matrix
      */
    virtual std::shared_ptr<Matrix> transform() override;
   
    virtual bool isSquare() const override; ///< Always returns true
    virtual bool isTriangular() const; ///< Returns true if matrix is triangular
    
    virtual std::shared_ptr<Matrix> power(const std::shared_ptr<Matrix> rhs) const override;
    /**
     * Determinant of a SquareMatrix
     * @brief Determinant
     * @return std::shared_ptr<Matrix>: pointer to the new matrix
     */
    virtual std::shared_ptr<Matrix> det() const override;

    virtual std::string whoami() const override; ///< returns type name - "SquareMatrix"

protected:
    long m_size; ///< number of rows and columns since it is a square matrix
};
/**
 * @brief TriangularMatrix class for triangular matrices
 */
class TriangularMatrix : public SquareMatrix {
public:
    /**
     * Constructs an empty TriangularMatrix
     * @brief default constructor
     * @param size: number of rows and columns
     */
    TriangularMatrix();
    /**
     * Constructs a TriangularMatrix from another TriangularMatrix
     * @brief copy constructor
     * @param m: triangular matrix
     */
    TriangularMatrix(const TriangularMatrix& m);
    /**
     * Constructs a TriangularMatrix from another SquareMatrix
     * @brief copy constructor
     * @param m: square matrix
     */
    TriangularMatrix(const SquareMatrix& m);

    /**
     * Transform the TriangularMatrix to a more specific type
     * Can transform to: DiagonalMatrix or return itself
     * @brief Matrix casting
     * @return std::shared_ptr<Matrix>: pointer to the new matrix
     */
    virtual std::shared_ptr<Matrix> transform() override;

    virtual bool isTriangular() const override; ///< Always returns true
    virtual bool isDiagonal() const; ///< Returns true if matrix is diagonal

    virtual double get(size_t row, size_t col) const override; 

    virtual std::string whoami() const override; ///< returns type name - "TriangularMatrix"
};
/**
 * @brief DiagonalMatrix class for diagonal matrices
 */
class DiagonalMatrix : public TriangularMatrix { 
public:
    /**
     * Constructs an empty DiagonalMatrix
     * @brief default constructor
     * @param size: number of rows and columns
     */
    DiagonalMatrix();
    /**
     * Constructs a DiagonalMatrix from another DiagonalMatrix
     * @brief copy constructor
     * @param m: diagonal matrix
     */
    DiagonalMatrix(const DiagonalMatrix& m);
    /**
     * Constructs a DiagonalMatrix from another TriangularMatrix
     * @brief copy constructor
     * @param m: triangular matrix
     */
    DiagonalMatrix(const TriangularMatrix& m);

    /**
     * Transform the DiagonalMatrix to a more specific type
     * Can transform to: IdentityMatrix or return itself
     * @brief Matrix casting
     * @return std::shared_ptr<Matrix>: pointer to the new matrix
     */
    virtual std::shared_ptr<Matrix> transform() override;

    virtual bool isDiagonal() const override; ///< Always returns true
    virtual bool isIdentity() const; ///< Returns true if matrix is identity

    virtual double get(size_t row, size_t col) const override;

    virtual std::string whoami() const override; ///< returns type name - "DiagonalMatrix"
};
/**
 * @brief IdentityMatrix class for identity matrices
 */
class IdentityMatrix : public DiagonalMatrix { 
public:
    /**
     * Constructs an empty IdentityMatrix
     * @brief default constructor
     * @param size: number of rows and columns
     */
    IdentityMatrix();
    /**
     * Constructs an IdentityMatrix from another IdentityMatrix
     * @brief copy constructor
     * @param m: identity matrix
     */
    IdentityMatrix(const IdentityMatrix& m);
    /**
     * Constructs an IdentityMatrix from another DiagonalMatrix
     * @brief copy constructor
     * @param m: diagonal matrix
     */
    IdentityMatrix(size_t size);

    /**
     * Can be nedded for virtual inheritance from it, always returns itself
     */
    virtual std::shared_ptr<Matrix> transform() override;

    virtual bool isIdentity() const override; ///< Always returns true

    virtual double get(size_t row, size_t col) const override;

    virtual std::string whoami() const override; ///< returns type name - "IdentityMatrix"
};
