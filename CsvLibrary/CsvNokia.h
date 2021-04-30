
#include <iostream>
#include <istream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>

struct operation{
    std::string key;
    std::string op;
};

class CsvNokia{
public:
    CsvNokia(std::istream &is);
    ~CsvNokia();
    void setElement(std::string key, int val);
    int getElement(std::string key);
    void print(void);
    void printDroppedExpressions(void);

private:
    std::unordered_map <std::string, int> table;
    std::vector <std::string> header;
    std::vector <std::string> numeration;
    std::vector <operation> toEvaluate;
    std::vector < std::string > droppedExpressions;

private:
    void eval(std::vector <operation>);
    int readOperation(std::string op);
    int operate(int aVal, int bVal, char operSymbol);
    void fillHeader(std::istream &);
    char parseElement(std::istream& is, std::string& str);
    bool isMathOp(char);
    void invalidCsvExit();
    void dropIfKeyInvalid(std::string key);
};