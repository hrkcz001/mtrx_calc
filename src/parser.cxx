#include "include/parser.hxx"

using namespace std;

//Implementation of Parser

//Constructor
Parser::Parser(string workingDirectory, ostream& os, istream& is) : m_workingDirectory(workingDirectory), m_os(os), m_is(is), m_matrices(), m_lexer(is), m_running(true) {}

//Public methods
void Parser::run() {
    shared_ptr<Matrix>  m;
    while (m_running) {
        m_os << m_workingDirectory << "> ";
        m_lexer.getInput();
        try {
            m = parse();
            if (m_lexer.getCurrentToken() != "") 
                throw invalid_argument("Ignored from '" + m_lexer.getCurrentToken() + "'");
            if (m != nullptr) {
                m_os << m->whoami() << endl;
                m_os << m->toString();
            }
        }
        catch (exception& e) {
            m_os << e.what() << endl;
        }
    } 
}

shared_ptr<Matrix>  Parser::parse() {
    m_lexer.getNextToken();
    return parseAssign();
}

//Private methods
shared_ptr<Matrix>  Parser::parseRow() {
    // vector of doubles
    vector<double> row;
    while(true){
    // try to convert current token to double exit loop if fail
        try {
            row.push_back(stod(m_lexer.getCurrentToken()));
        } catch (invalid_argument& e) {
            break;
        }
        m_lexer.getNextToken();
    }
    // create matrix from vector
    return make_shared<Matrix>(vector<vector<double>>({row}));
}

shared_ptr<Matrix>  Parser::parseMatrix() {
    shared_ptr<Matrix> m, t1, t2;
    // check that current token is '['
    if (m_lexer.getCurrentToken() != "[") {
        throw invalid_argument("Expected '['");
    }
    m_lexer.getNextToken();
    // check if next token is ']'
    if (m_lexer.getCurrentToken() == "]") {
        m_lexer.getNextToken();
        return make_shared<Matrix>();
    }
    // parse first row
    m = parseRow();
    // check that row is not empty
    if (m->isEmpty())
        throw invalid_argument("Empty row");
    // parse next rows
    while (m_lexer.getCurrentToken() == "&") {
        m_lexer.getNextToken();
        t1 = m;
        t2 = parseRow();
        m = t1->vconcat(t2);
    }
    // check that current token is ']'
    if (m_lexer.getCurrentToken() != "]") {
        throw invalid_argument("Expected ']'");
    }
    m_lexer.getNextToken();

    return m->transform();
}

shared_ptr<Matrix>  Parser::parseUnary() {
    shared_ptr<Matrix> m, t;
    // check if current token is ! , - , h or k
    if (m_lexer.getCurrentToken() == "!") {
        m_lexer.getNextToken();
        t = parseUnary();
        m = t->transpose();
        return m;
    } else if (m_lexer.getCurrentToken() == "-") {
        m_lexer.getNextToken();
        t = parseUnary();
        m = t->neg();
        return m;
    } else if (m_lexer.getCurrentToken() == "rank") {
        m_lexer.getNextToken();
        t = parseUnary();
        m = t->rank();
        return m;
    } else if (m_lexer.getCurrentToken() == "gem") {
        m_lexer.getNextToken();
        t = parseUnary();
        m = t->gem();
        return m;
    } else if (m_lexer.getCurrentToken() == "det") {
        m_lexer.getNextToken();
        t = parseUnary();
        m = t->det();
        return m;
    }
    // if current token is parenthesis parse expression inside(from the beginning)
    else if (m_lexer.getCurrentToken() == "(") {
        m_lexer.getNextToken();
        m = parseAddSub();
        if (m_lexer.getCurrentToken() != ")") {
            throw invalid_argument("Expected ')'" + m_lexer.getCurrentToken());
        }
        m_lexer.getNextToken();
        return m;
    }
    // if token is [ parse Matrix
    if (m_lexer.getCurrentToken() == "[") {
        return parseMatrix();
    }
    // if can convert token to double return make_shared<Number> 
    try {
        m = make_shared<Number>(stod(m_lexer.getCurrentToken()));
        m_lexer.getNextToken();
        return m;
    } catch (invalid_argument&) {
        // if token is not a number check if it is a variable
        if (m_matrices.find(m_lexer.getCurrentToken()) != m_matrices.end()) {
            m = m_matrices.at(m_lexer.getCurrentToken());
            m_lexer.getNextToken();
            return m;
        }
        else {
            throw invalid_argument("Semantic error at '" + m_lexer.getCurrentToken() + "'");
        }
    }
}

shared_ptr<Matrix>  Parser::parseBackslash() {
    shared_ptr<Matrix> m, t1, t2;
    m = parseUnary();
    if (m_lexer.getCurrentToken() == "\\") {
        m_lexer.getNextToken();
        t1 = m;
        t2 = parseBackslash();
        m = t1->crop(t2);
    }
    return m;
}
// parse ^ operations
shared_ptr<Matrix>  Parser::parsePower() {
    shared_ptr<Matrix> m, t1, t2;
    m = parseBackslash();
    if (m_lexer.getCurrentToken() == "^") {
        m_lexer.getNextToken();
        t1 = m;
        t2 = parsePower();
        m = t1->power(t2);
    }
    return m;
}

shared_ptr<Matrix>  Parser::parseOr() {
    shared_ptr<Matrix> m, t1, t2;
    m = parsePower();
    if (m_lexer.getCurrentToken() == "|") {
        m_lexer.getNextToken();
        t1 = m;
        t2 = parseOr();
        m = t1->hconcat(t2);
    }
    return m;
}

shared_ptr<Matrix>  Parser::parseAnd() {
    shared_ptr<Matrix> m, t1, t2;
    m = parseOr();
    if (m_lexer.getCurrentToken() == "&") {
        m_lexer.getNextToken();
        t1 = m;
        t2 = parseAnd();
        m = t1->vconcat(t2);
    }
    return m;
}

shared_ptr<Matrix>  Parser::parseMulDiv() {
    shared_ptr<Matrix> m, t1, t2;
    string op;
    m = parseAnd();
    while (m_lexer.getCurrentToken() == "*" || m_lexer.getCurrentToken() == "/") {
        op = m_lexer.getCurrentToken();
        m_lexer.getNextToken();
        t1 = m;
            t2 = parseOr();
        if (op == "*") {
                m = t1->prod(t2);
        } else {
                m = t1->div(t2);
        }
    }
    return m;
}

shared_ptr<Matrix>  Parser::parseAddSub() {
    shared_ptr<Matrix> m, t1, t2;
    string op;
    m = parseMulDiv();
    while (m_lexer.getCurrentToken() == "+" || m_lexer.getCurrentToken() == "-") {
        op = m_lexer.getCurrentToken();
        m_lexer.getNextToken();
        t1 = m;
        t2 = parseMulDiv();
        if (op == "+") {
            m = t1->add(t2);
        } else {
            m = t1->sub(t2);
        }
    }
    return m;
}

//with peekToken method
shared_ptr<Matrix>  Parser::parseAssign() {
    if (m_lexer.getCurrentToken() == "exit") {
        m_running = false;
        m_lexer.getNextToken();
        return nullptr;
    } else
    if (m_lexer.getCurrentToken() == "load") {
        string name = m_lexer.getNextToken();
        shared_ptr<Matrix> m = readFromFile(name);
        m = m->transform();
        m_matrices[name] = m;
        m_lexer.getNextToken();
        return nullptr;
    } else
    if (m_lexer.getCurrentToken() == "save") {
        string name = m_lexer.getNextToken();
        if (m_matrices.find(name) == m_matrices.end()) {
            throw invalid_argument("Matrix '" + name + "' not found");
        }
        writeToFile(name, m_matrices.at(name));
        m_lexer.getNextToken();
        return nullptr;
    } else
    if (m_lexer.peekToken() == "=") {
        string name = m_lexer.getCurrentToken();
        m_lexer.getNextToken();
        m_lexer.getNextToken();
        shared_ptr<Matrix>  m = parseAddSub();
        m_matrices[name] = m;
        return nullptr;
    } else {
        return parseAddSub();
    }
}

shared_ptr<Matrix>  Parser::readFromFile(string filename) {
    ifstream file(m_workingDirectory + "/" + filename + ".matix");
    if (!file.is_open()) {
        throw invalid_argument("File '" + filename + ".matix' not found");
    }
    string line;
    vector<vector<double>> data;
    while (getline(file, line)) {
        vector<double> row;
        stringstream ss(line);
        double d;
        while (ss >> d) {
            row.push_back(d);
        }
        data.push_back(row);
    }
    file.close();
    return make_shared<Matrix>(data)->transform();
}

void Parser::writeToFile(string filename, shared_ptr<Matrix> matrix) {
    ofstream file(m_workingDirectory + "/" + filename + ".matix");
    if (!file.is_open()) {
        throw invalid_argument("Cannot write to file '" + filename + ".matix'");
    }
    file << matrix->toString();
    file.close();
}
