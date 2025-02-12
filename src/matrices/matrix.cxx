#include "../include/matrix.hxx"

using namespace std;
    
// INFO: Matrix class implementation

Matrix::Matrix()    :   m_data(vector<vector<double>>())
                    ,   m_empty(true) {}

Matrix::Matrix (const Matrix& matrix) : m_data(matrix.m_data)
                                      , m_empty(matrix.m_empty) {}

Matrix::Matrix(vector<vector<double>> data){
    size_t size = data[0].size();
    for (size_t i = 1; i < data.size(); i++)
        if (data[i].size() != size)
            throw runtime_error("Rows have different sizes");
    m_data = data;
    m_empty = false;
}

size_t Matrix::rows() const {
    return m_data.size();
}

size_t Matrix::cols() const {
    return m_data[0].size();
}

shared_ptr<Matrix> Matrix::transform() {
    shared_ptr<Matrix> m;
    if (this->isNumber()){
        m = make_shared<Number>(this->get(0, 0));
        return m->transform();
    }
    else if (this->isZero()){
        m = make_shared<ZeroMatrix>(this->rows(), this->cols());
        return m->transform();
    }
    else if (this->isSquare()){
        m = make_shared<SquareMatrix>(*this);
        return m->transform();
    }
    else if (this->isEmpty())
        this->m_empty = true;
    return make_shared<Matrix>(*this);
}

string Matrix::toString() const {
    stringstream ss;
    for (size_t i = 0; i < this->rows(); i++){
        for (size_t j = 0; j < this->cols(); j++)
            ss << this->get(i, j) << " ";
        ss << endl;
    }
    return ss.str();
}

bool Matrix::isEmpty() const {
    return (rows() == 0 || cols() == 0);
}

bool Matrix::isNumber() const {
    return (rows() == 1 && cols() == 1);
}

bool Matrix::isZero() const {
    for (size_t i = 0; i < rows(); i++)
        for (size_t j = 0; j < cols(); j++)
            if (this->get(i, j) != 0)
                return false;
    return true;
}

bool Matrix::isSquare() const {
    return rows() == cols();
}

double Matrix::get(size_t row, size_t col) const {
    if (row >= rows())
        throw runtime_error("Row index out of range");
    if (col >= cols())
        throw runtime_error("Column index out of range");
    return m_data[row][col];
}

double Matrix::number() const {
    throw runtime_error("Not a number");
}

shared_ptr<Matrix> Matrix::add(const shared_ptr<Matrix> rhs) const {
    shared_ptr<Matrix> m;
    if (rows() != rhs->rows())
        throw runtime_error("Different number of rows");
    if (cols() != rhs->cols())
        throw runtime_error("Different number of columns");
    vector<vector<double>> result(rows());
    for (size_t i = 0; i < rows(); i++) {
        result[i].resize(cols());
        for (size_t j = 0; j < cols(); j++)
            result[i][j] = this->get(i, j) + rhs->get(i, j);
    }
    m = make_shared<Matrix>(result);
    return m->transform();
}

shared_ptr<Matrix> Matrix::neg() const {
    shared_ptr<Matrix> m;
    vector<vector<double>> result(rows());
    for (size_t i = 0; i < rows(); i++) {
        result[i].resize(cols());
        for (size_t j = 0; j < cols(); j++)
            result[i][j] = -this->get(i, j);
    }
    m = make_shared<Matrix>(result);
    return m->transform();
}

shared_ptr<Matrix> Matrix::sub(const shared_ptr<Matrix> rhs) const {
    return add(rhs->neg());
}

shared_ptr<Matrix> Matrix::prod(const shared_ptr<Matrix> rhs) const {
    //scalar multiplication
    if (rhs->isNumber()){
        shared_ptr<Matrix> m;
        vector<vector<double>> result(this->rows());
        double n = rhs->number();
        for (size_t i = 0; i < this->rows(); i++) {
            result[i].resize(cols());
            for (size_t j = 0; j < cols(); j++)
                result[i][j] = this->get(i, j) * n;
        }
        m = make_shared<Matrix>(result);
        return m->transform();
    }
    else if (cols() != rhs->rows())
        throw runtime_error("Different number of colum");
    //matrix multiplication
    shared_ptr<Matrix> m;
    vector<vector<double>> result(this->rows());
    for (size_t i = 0; i < this->rows(); i++) {
        result[i].resize(rhs->cols());
        for (size_t j = 0; j < rhs->cols(); j++) {
            result[i][j] = 0;
            for (size_t k = 0; k < cols(); k++)
                result[i][j] += this->get(i, k) * rhs->get(k, j);
        }
    }
    m = make_shared<Matrix>(result);
    return m->transform();
}

shared_ptr<Matrix> Matrix::div(const shared_ptr<Matrix> rhs) const {
    double n = rhs->number();
    if (n == 0)
        throw runtime_error("Division by zero");
    shared_ptr<Matrix> m, tmp;
    tmp = make_shared<Number>(1 / n);
    m = prod(tmp);
    return m->transform();
}

shared_ptr<Matrix> Matrix::power(const shared_ptr<Matrix>) const {
    throw runtime_error("Non-square matrix");
}

shared_ptr<Matrix> Matrix::transpose() const {
    shared_ptr<Matrix> m;
    vector<vector<double>> result(cols());
    for (size_t i = 0; i < cols(); i++) {
        result[i].resize(rows());
        for (size_t j = 0; j < rows(); j++)
            result[i][j] = this->get(j, i);
    }
    m = make_shared<Matrix>(result);
    return m->transform();
}

shared_ptr<Matrix> Matrix::hconcat(const shared_ptr<Matrix> rhs) const {
    if (rows() != rhs->rows())
        throw runtime_error("Different number of rows");
    shared_ptr<Matrix> m;
    vector<vector<double>> result(rows());
    for (size_t i = 0; i < rows(); i++) {
        result[i].resize(cols() + rhs->cols());
        for (size_t j = 0; j < cols(); j++)
            result[i][j] = this->get(i, j);
        for (size_t j = 0; j < rhs->cols(); j++)
            result[i][j + cols()] = rhs->get(i, j);
    }
    m = make_shared<Matrix>(result);
    return m->transform();
}

shared_ptr<Matrix> Matrix::vconcat(const shared_ptr<Matrix> rhs) const {
    if (cols() != rhs->cols())
        throw runtime_error("Different number of columns");
    shared_ptr<Matrix> m;
    vector<vector<double>> result(rows() + rhs->rows());
    for (size_t i = 0; i < rows(); i++) {
        result[i].resize(cols());
        for (size_t j = 0; j < cols(); j++)
            result[i][j] = this->get(i, j);
    }
    for (size_t i = 0; i < rhs->rows(); i++) {
        result[i + rows()].resize(cols());
        for (size_t j = 0; j < cols(); j++)
            result[i + rows()][j] = rhs->get(i, j);
    }
    m = make_shared<Matrix>(result);
    return m->transform();
}

shared_ptr<Matrix> Matrix::crop(const shared_ptr<Matrix> rhs) const{
    size_t rows, cols, verticalOffset, horizontalOffset;
    double tmp;

    if (rhs->cols() == 2){
        if (rhs->rows() == 1){
            if(  rhs->get(0, 0) <= 0 
              || rhs->get(0, 1) <= 0 
              || modf(rhs->get(0, 0), &tmp) > numeric_limits<double>::epsilon() 
              || modf(rhs->get(0, 1), &tmp) > numeric_limits<double>::epsilon())
            {
                throw runtime_error("Invalid parameters");
            }
            else{
                rows = rhs->get(0, 0);
                cols = rhs->get(0, 1);
                verticalOffset = 0;
                horizontalOffset = 0;
            }
        }
        else if (rhs->rows() == 2){
            if( rhs->get(0, 0) <= 0 
              || rhs->get(0, 1) <= 0 
              || modf(rhs->get(0, 0), &tmp) > numeric_limits<double>::epsilon() 
              || modf(rhs->get(0, 1), &tmp) > numeric_limits<double>::epsilon()
              || rhs->get(1, 0) < 0 
              || rhs->get(1, 1) < 0 
              || modf(rhs->get(1, 0), &tmp) > numeric_limits<double>::epsilon() 
              || modf(rhs->get(1, 1), &tmp) > numeric_limits<double>::epsilon())
            {
                throw runtime_error("Invalid parameters");
            }
            else{
                rows = rhs->get(0, 0);
                cols = rhs->get(0, 1);
                verticalOffset = rhs->get(1, 0);
                horizontalOffset = rhs->get(1, 1);
            }
        }
        else{
            throw runtime_error("Invalid parameters");
        }

        if (verticalOffset + rows > this->rows() || horizontalOffset + cols > this->cols()){
            throw runtime_error("Invalid parameters");
        }
        else{
            shared_ptr<Matrix> m;
            vector<vector<double>> result(rows);
            for (size_t i = 0; i < rows; i++) {
                result[i].resize(cols);
                for (size_t j = 0; j < cols; j++)
                    result[i][j] = this->get(i + verticalOffset, j + horizontalOffset);
            }
            m = make_shared<Matrix>(result);
            return m->transform();
        }

    }
    else{
        throw runtime_error("Invalid parameters");
    }
}

// TODO: implement rank
shared_ptr<Matrix> Matrix::rank() const {
    shared_ptr<Matrix> m;
    m = this->gem();
    size_t rank = 0;
    for (size_t i = 0; i < m->rows(); i++) {
        bool isZero = true;
        for (size_t j = 0; j < m->cols(); j++)
            if (m->get(i, j) != 0) {
                isZero = false;
                break;
            }
        if (!isZero)
            rank++;
    }
    return make_shared<Number>(rank);
}

shared_ptr<Matrix> Matrix::gem() const {
    shared_ptr<Matrix> m;
    vector<vector<double>> result(this->rows(), vector<double>(this->cols()));
    for (size_t z = 0; z < this->rows(); z++)
        for (size_t k = 0; k < this->cols(); k++)
            result[z][k] = this->get(z, k);
    for (size_t i = 0; i < this->rows(); i++) {
        size_t j = i;
        while (j < this->rows() && result[j][i] == 0)
            j++;
        if (j == this->rows())
            continue;
        result[i].swap(result[j]);
        for (size_t j = i + 1; j < this->rows(); j++) {
            double c = result[j][i] / result[i][i];
            for (size_t k = i; k < this->cols(); k++)
                result[j][k] -= result[i][k] * c;
        }
    }
    m = make_shared<Matrix>(result);
    return m->transform();
}

shared_ptr<Matrix> Matrix::det () const{
    throw runtime_error("Non square matrix");
}
