#include "include/parser.hxx"

using namespace std;

int main ( void ) {
    string saveDir = "examples";
    try{
        fstream file("examples/config.txt");
        if(!getline(file, saveDir))
            throw invalid_argument("Error reading config file");
        file.close();
    } catch (exception& e) {
        cout << e.what() << endl;
        cout << "Using default directory: " << saveDir << endl;
    }
    Parser repl = Parser(saveDir);
    repl.run();
    return 0;
}
