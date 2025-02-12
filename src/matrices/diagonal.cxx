#include "../include/matrix.hxx"

using namespace std;

// INFO: DiagonalMatrix class implementation

DiagonalMatrix::DiagonalMatrix() : TriangularMatrix() {}

DiagonalMatrix::DiagonalMatrix(const DiagonalMatrix& matrix) : TriangularMatrix() {
    m_data.resize(1, vector<double>(matrix.rows()));
    for (size_t i = 0; i < matrix.rows(); i++)
        m_data[0][i] = matrix.get(i, i);
    m_size = matrix.rows();
    m_empty = false;
}

DiagonalMatrix::DiagonalMatrix(const TriangularMatrix& matrix) : TriangularMatrix() {
    m_data.resize(1, vector<double>(matrix.rows()));
    for (size_t i = 0; i < matrix.rows(); i++)
        m_data[0][i] = matrix.get(i, i);
    m_size = matrix.rows();
    m_empty = false;
}

shared_ptr<Matrix> DiagonalMatrix::transform() {
    if (this->isIdentity()) {
        shared_ptr<Matrix> m = make_shared<IdentityMatrix>(this->rows());
        return m->transform();
    }
    return make_shared<DiagonalMatrix>(*this);
}

bool DiagonalMatrix::isDiagonal() const {
    return true;
}

bool DiagonalMatrix::isIdentity() const {
    for (size_t i = 0; i < rows(); i++)
        if (this->get(i, i) != 1)
            return false;
    return true;
}

double DiagonalMatrix::get(size_t i, size_t j) const {
    if (i >= rows())
        throw runtime_error("Row index out of range");
    if (j >= cols())
        throw runtime_error("Column index out of range");
    if (i != j)
        return 0;
    return m_data[0][i];
}
