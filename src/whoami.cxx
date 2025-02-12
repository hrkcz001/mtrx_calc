#include "include/matrix.hxx"

using namespace std;

string Matrix::whoami() const{
    if (m_empty)
        return "Empty Matrix";
    return "Matrix";
}

string Number::whoami() const{
    return "Number";
}

string ZeroMatrix::whoami() const{
    return "Zero Matrix";
}

string SquareMatrix::whoami() const{
    return "Square Matrix";
}

string TriangularMatrix::whoami() const{
    return "Triangular Matrix";
}

string DiagonalMatrix::whoami() const{
    return "Diagonal Matrix";
}

string IdentityMatrix::whoami() const{
    return "Identity Matrix";
}
