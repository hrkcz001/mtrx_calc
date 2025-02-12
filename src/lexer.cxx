#include "include/parser.hxx"

using namespace std;

const string single_char_tokens = "+-*/^()[]!|&=";

// Implementation of Lexer
// Constructor
Lexer::Lexer(istream &is) : m_is(is), m_pos(0) {}

// Public methods
void Lexer::getInput() {
    getline(m_is, m_input);
    m_pos = 0;
}
string Lexer::getCurrentToken() const { return m_token; }
string Lexer::peekToken() {
    size_t pos = m_pos;
    string token = nextToken();
    m_pos = pos;
    return token;
}
string Lexer::getNextToken() {
    m_token = nextToken();
    return m_token;
}

// Private methods
string Lexer::nextToken() {
    if (m_pos >= m_input.size()) {
        return "";
    }
    while (m_input[m_pos] == ' ') {
        m_pos++;
        if (m_pos >= m_input.size()) {
            return "";
        }
    }
    if (single_char_tokens.find(m_input[m_pos]) != string::npos) {
        return string(1, m_input[m_pos++]);
    }
    string token = "";
    while (m_pos < m_input.size() && !isspace(m_input[m_pos]) && single_char_tokens.find(m_input[m_pos]) == string::npos) {
        token += m_input[m_pos++];
    }
    return token;
}
