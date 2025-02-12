#include "../include/matrix.hxx"

using namespace std;

// INFO: SquareMatrix class implementation

SquareMatrix::SquareMatrix()    : Matrix()
                                , m_size(0) {}

SquareMatrix::SquareMatrix(const SquareMatrix& matrix)    : Matrix(matrix)
                                                          , m_size(matrix.rows()) {}

SquareMatrix::SquareMatrix(const Matrix& matrix)    : Matrix(vector<vector<double>>(matrix.rows(), vector<double>(matrix.rows())))
                                                    , m_size(matrix.rows()){
    if (matrix.rows() != matrix.cols())
        throw runtime_error("Non-square matrix");
    for (size_t i = 0; i < matrix.cols(); i++)
        for (size_t j = 0; j < matrix.cols(); j++)
            m_data[i][j] = matrix.get(i, j);
}

size_t SquareMatrix::rows() const {
    return m_size;
}
size_t SquareMatrix::cols() const {
    return m_size;
}

shared_ptr<Matrix> SquareMatrix::transform(){
    shared_ptr<Matrix> m;
    if (this->isTriangular()){
        m = make_shared<TriangularMatrix>(*this);
        return m->transform();
    }
    else
        return make_shared<SquareMatrix>(*this);
}

bool SquareMatrix::isSquare() const {
    return true;
}

bool SquareMatrix::isTriangular() const {
    for (size_t i = 1; i < this->rows(); i++)
        for (size_t j = 0; j < i; j++)
            if (this->get(i, j) != 0)
                return false;
    return true;
}

shared_ptr<Matrix> SquareMatrix::power(const shared_ptr<Matrix> rhs) const {
    shared_ptr<Matrix> m;
    double n;
    if (modf(rhs->number(), &n) > numeric_limits<double>::epsilon() * 10)
        throw runtime_error("Non-integer power");
    if (n < 0)
        throw runtime_error("Negative matrix power");
    if (n > 100)
        throw runtime_error("Matrix power too large");
    m = make_shared<IdentityMatrix>(this->rows());
    for (size_t i = 0; i < n; i++) {
        m = this->prod(m);
        m = m->transform();
    }
    return m->transform();
}

shared_ptr<Matrix> SquareMatrix::det() const {
    shared_ptr<Matrix> m;
    m = this->gem();
    double result = 1;
    for (size_t i = 0; i < m->rows(); i++)
        result *= m->get(i, i);
    return make_shared<Number>(result);
}
