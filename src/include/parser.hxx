/**
 * @file parser.hxx
 * @author morozan1
 * @brief Parser and Lexer for Matrix Calculator
 */
#include "matrix.hxx"
#include <map>
#include <fstream>
#include <iostream>
/**
 * @brief Lexer that reads input from std::cin and returns tokens
 */
class Lexer {
public:
    /**
     * @brief Constructor
     * @param is: input stream
     */
    Lexer(std::istream& is);
    /**
     * @brief Get next line from input stream
     */
    void getInput();
    /**
     * @brief Get current token
     */
    std::string getCurrentToken() const;
    /**
     * @brief Get next token
     */
    std::string peekToken();
    /**
     * @brief Get next token and advance
     */
    std::string getNextToken();

private:
    std::istream& m_is; ///< input stream
    std::string m_input, m_token; ///< current input line and current token
    size_t m_pos; ///< current position in input line
    // NOTE: token is either single char or string of digits or letters
    /**
     * @brief calculate next token
     */
    std::string nextToken();
};
/**
 * @brief Parser that reads tokens from Lexer and returns matrices by its grammar rules
 */
class Parser { 
public :
    /**
     * @brief Constructor
     * @param workingDirectory: directory where to read/write files
     * @param os: output stream
     * @param is: input stream
     */
    Parser(std::string workingDirectory = ".", std::ostream& os = std::cout, std::istream& is = std::cin);
    /**
     * @brief REPL loop
     */
    void run();
    /**
     * @brief Parse tokens from Lexer
     */
    std::shared_ptr<Matrix> parse();

private:
    std::string m_workingDirectory; ///< directory where to read/write files
    std::ostream& m_os; ///< output stream
    std::istream& m_is; ///< input stream
    std::map<std::string, std::shared_ptr<Matrix> > m_matrices; ///< map of matrices
    Lexer m_lexer; ///< lexer
    bool m_running; ///< is REPL running

    /**
     * @brief read matrix from file
     * @param filename: name of file
     */
    std::shared_ptr<Matrix> readFromFile(std::string filename);
    
    /**
     * @brief write matrix to file
     * @param filename: name of file
     * @param matrix: matrix to write
     */
    void writeToFile(std::string filename, std::shared_ptr<Matrix> matrix);
    
    std::shared_ptr<Matrix>  parseRow(); ///< parse a row
    /**
     * Parse a matrix [row & row & row & ...]
     * @throws std::invalid_argument if matrix is invalid
     */
    std::shared_ptr<Matrix>  parseMatrix();

    std::shared_ptr<Matrix>  parseUnary(); ///< negation, transpose, GEM, determinant, rank, parenthesis, variable

    std::shared_ptr<Matrix>  parseBackslash(); ///< parse submatrix operation(crop)

    std::shared_ptr<Matrix>  parsePower(); ///< parse power operation

    std::shared_ptr<Matrix>  parseAnd(); ///< parse vertical concatenation operation(&)

    std::shared_ptr<Matrix>  parseOr(); ///< parse horizontal concatenation operation(|)

    std::shared_ptr<Matrix>  parseMulDiv(); ///< parse multiplication and division operations

    std::shared_ptr<Matrix>  parseAddSub(); ///< parse addition and subtraction operations

    std::shared_ptr<Matrix>  parseAssign(); ///< parse assignment operation and commands

};
