#include "../include/matrix.hxx"

using namespace std;

// INFO: IdentityMatrix class implementation

IdentityMatrix::IdentityMatrix() : DiagonalMatrix() {}

IdentityMatrix::IdentityMatrix(const IdentityMatrix& matrix) : DiagonalMatrix() {
    m_size = matrix.rows();
    m_empty = false;
}

IdentityMatrix::IdentityMatrix(size_t size) : DiagonalMatrix() {
    m_size = size;
    m_empty = false;
}

shared_ptr<Matrix> IdentityMatrix::transform() {
    return make_shared<IdentityMatrix>(*this);
}

bool IdentityMatrix::isIdentity() const {
    return true;
}

double IdentityMatrix::get(size_t i, size_t j) const {
    if (i >= rows())
        throw runtime_error("Row index out of range");
    if (j >= cols())
        throw runtime_error("Column index out of range");
    if (i != j)
        return 0;
    return 1;
}
