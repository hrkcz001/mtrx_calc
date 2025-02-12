#include "../include/matrix.hxx"

using namespace std;

Number::Number(const Number& matrix): Matrix(matrix) {}
Number::Number(double m): Matrix(vector<vector<double>>(1, vector<double>(1, m))) {}

shared_ptr<Matrix> Number::transform(){
    return make_shared<Number>(this->number());
}

bool Number::isNumber() const{
    return true;
}

double Number::number() const{
    return m_data[0][0];
}

shared_ptr<Matrix> Number::add(const shared_ptr<Matrix> rhs) const{
    return make_shared<Number>(this->number() + rhs->number());
}

shared_ptr<Matrix> Number::sub(const shared_ptr<Matrix> rhs) const{
    return make_shared<Number>(this->number() - rhs->number());
}

shared_ptr<Matrix> Number::prod(const shared_ptr<Matrix> rhs) const{
    return make_shared<Number>(this->number() * rhs->number());
}

shared_ptr<Matrix> Number::div(const shared_ptr<Matrix> rhs) const{
    double n = rhs->number();
    if(n == 0){
        throw runtime_error("Division by zero");
    }
    return make_shared<Number>(this->number() / n);
}

shared_ptr<Matrix> Number::power(const shared_ptr<Matrix> rhs) const{
    return make_shared<Number>(pow(this->number(), rhs->number()));
}

shared_ptr<Matrix> Number::det() const{
    return make_shared<Number>(this->number());
}
