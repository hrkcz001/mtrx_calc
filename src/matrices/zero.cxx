#include "../include/matrix.hxx"

using namespace std;

// INFO: ZeroMatrix class implementation

ZeroMatrix::ZeroMatrix(const ZeroMatrix& matrix)    :   Matrix() 
                                                    ,   m_rows(matrix.rows())
                                                    ,   m_cols(matrix.cols())
{
    m_empty = false;
}

ZeroMatrix::ZeroMatrix(size_t rows, size_t cols)    :   Matrix() 
                                                    ,   m_rows(rows)
                                                    ,   m_cols(cols)
{
    if (rows == 0 || cols == 0)
        throw runtime_error("Zero matrix must have at least one row and one column");
    m_empty = false;
}

size_t ZeroMatrix::rows() const {
    return m_rows;
}
size_t ZeroMatrix::cols() const {
    return m_cols;
}

shared_ptr<Matrix> ZeroMatrix::transform(){
    return make_shared<ZeroMatrix>(*this);
}

bool ZeroMatrix::isZero() const {
    return true;
}

double ZeroMatrix::get(size_t row, size_t col) const {
    if (row >= rows())
        throw runtime_error("Row index out of range");
    if (col >= cols())
        throw runtime_error("Column index out of range");
    return 0;
}

shared_ptr<Matrix> ZeroMatrix::add(const shared_ptr<Matrix> rhs) const {
    shared_ptr<Matrix> m;
    if (rows() != rhs->rows())
        throw runtime_error("Different number of rows");
    if (cols() != rhs->cols())
        throw runtime_error("Different number of columns");
    m = make_shared<Matrix>(*rhs);
    return m->transform();
}

shared_ptr<Matrix> ZeroMatrix::sub(const shared_ptr<Matrix> rhs) const {
    if (rows() != rhs->rows())
        throw runtime_error("Different number of rows");
    if (cols() != rhs->cols())
        throw runtime_error("Different number of columns");
    return rhs->neg();
}

shared_ptr<Matrix> ZeroMatrix::prod(const shared_ptr<Matrix> rhs) const {
    if (rhs->isNumber())
        return make_shared<ZeroMatrix>(rows(), cols());
    if (rows() != rhs->rows())
        throw runtime_error("Different number of rows");
    if (cols() != rhs->cols())
        throw runtime_error("Different number of columns");
    return make_shared<ZeroMatrix>(rows(), rhs->cols());
}

shared_ptr<Matrix> ZeroMatrix::div(const shared_ptr<Matrix> rhs) const {
    if (!rhs->isNumber())
        throw runtime_error("Division by non-number");
    if (rhs == 0)
        throw runtime_error("Division by zero");
    return make_shared<ZeroMatrix>(rows(), cols());
}

shared_ptr<Matrix> ZeroMatrix::power(const shared_ptr<Matrix> rhs) const {
    if (rows() != cols())
        throw runtime_error("Non-square matrix");
    if (rhs == 0)
        return make_shared<IdentityMatrix>(rows());
    return make_shared<ZeroMatrix>(rows(), cols());
}

shared_ptr<Matrix> ZeroMatrix::transpose() const {
    return make_shared<ZeroMatrix>(cols(), rows());
}

shared_ptr<Matrix> ZeroMatrix::det() const {
    if (rows() != cols())
        throw runtime_error("Non-square matrix");
    return make_shared<Number>(0);
}
