#include "../include/matrix.hxx"

using namespace std;

// INFO: TriangularMatrix class implementation

TriangularMatrix::TriangularMatrix() : SquareMatrix() {}

TriangularMatrix::TriangularMatrix(const TriangularMatrix& matrix) : SquareMatrix() {
    m_data.resize(matrix.rows(), vector<double>());
    for (size_t i = 0; i < matrix.rows(); i++) {
        m_data[i].resize(matrix.cols() - i);
        for (size_t j = i; j < matrix.cols(); j++)
            m_data[i][j - i] = matrix.get(i, j);
    }
    m_size = matrix.rows();
    m_empty = false;
}

TriangularMatrix::TriangularMatrix(const SquareMatrix& matrix) : SquareMatrix() {
    //each row(vector) is shorter than the previous one by 1
    m_data.resize(matrix.rows(), vector<double>());
    for (size_t i = 0; i < matrix.rows(); i++) {
        m_data[i].resize(matrix.cols() - i);
        for (size_t j = i; j < matrix.cols(); j++)
            m_data[i][j - i] = matrix.get(i, j);
    }
    m_size = matrix.rows();
    m_empty = false;
}

shared_ptr<Matrix> TriangularMatrix::transform() {
    if (this->isDiagonal()) {
        shared_ptr<Matrix> m = make_shared<DiagonalMatrix>(*this);
        return m->transform();
    }
    return make_shared<TriangularMatrix>(*this);
}

bool TriangularMatrix::isTriangular() const {
    return true;
}
bool TriangularMatrix::isDiagonal() const {
    for (size_t i = 0; i < rows(); i++)
        for (size_t j = 0; j < cols(); j++)
            if (i != j && this->get(i, j) != 0)
                return false;
    return true;
}

double TriangularMatrix::get(size_t i, size_t j) const {
    if (i >= rows())
        throw runtime_error("Row index out of range");
    if (j >= cols())
        throw runtime_error("Column index out of range");
    if (i > j)
        return 0;
    return m_data[i][j - i];
}
