#include <CsvNokia.h>

void CsvNokia::fillHeader(std::istream &is){
    char c = '0'; 
    std::string headValue;
    for(int i = 1; c != '\n' && c != '\0'; i++){
        c = this->parseElement(is, headValue);
        header.push_back(headValue);
    }
}

CsvNokia::CsvNokia(std::istream &is)
{
    fillHeader(is);

    std::string digitKey;
    std::string digitVal;
    char c = '0';
    for(int i = 0; c != '\0'; i++){

        if (i >= static_cast<int>(header.size())) 
            invalidCsvExit();
        
        if (i == 0){
            c = this->parseElement(is, digitKey);
            numeration.push_back(digitKey);
            if (digitKey.size() == 0) 
                invalidCsvExit();
        }
        else {
            c = this->parseElement(is, digitVal);
            if (std::isdigit(digitVal[0])) {
                table.insert({ header[i] + digitKey, std::stoi(digitVal) });
            }
            else {
                toEvaluate.push_back({ header[i] + digitKey, digitVal });
                table.insert({ header[i] + digitKey, 0 });
            }

        }

        if (c == '\n' || c == '\0') {
            if (i != header.size() - 1) {
                invalidCsvExit();
            }
            else
                i = -1;
        }

        
    }

    eval(toEvaluate);
}

CsvNokia::~CsvNokia()
{
    table.clear();
    header.clear();
    numeration.clear();
    toEvaluate.clear();
    droppedExpressions.clear();
}

void CsvNokia::setElement(std::string key, int val)
{
    table[key] = val;
}

int CsvNokia::getElement(std::string key)
{
    return table[key];
}

void CsvNokia::eval(std::vector <operation> evalTarget) {
    for(auto operation: evalTarget){
        table[operation.key] = readOperation(operation.op);    
    }
}

int CsvNokia::readOperation(std::string op){

    if (op[0] != '=') {
        droppedExpressions.push_back(op);
        return 0;
    }

    int opIndex = 0;

    for (; !isMathOp(op[opIndex]) && opIndex < op.size() - 1; opIndex++) {}
    
    if (opIndex == op.size() - 1) {
        droppedExpressions.push_back(op);
        return 0;
    }

    dropIfKeyInvalid(op.substr(1, opIndex - 1));
    int operLeft = this->getElement(op.substr(1, opIndex - 1));

    dropIfKeyInvalid(op.substr(opIndex + 1, op.size()));
    int operRight = this->getElement(op.substr(opIndex + 1, op.size()));

    try {
        int result = operate(operLeft, operRight, op[opIndex]);
    }
    catch (std::overflow_error e) {
        droppedExpressions.push_back(op.substr(1, op.size()) + " (Zero Division)");
        return 0;
    }
    return operate(operLeft, operRight, op[opIndex]);
}

bool CsvNokia::isMathOp(char c) {
    switch (c) {
    case '+':
    case '-':
    case '/':
    case '*':
        return true;
    default:
        return false;
    }
}

void CsvNokia::invalidCsvExit()
{
    std::cout << "Invalid CSV file" << std::endl;
    exit(EXIT_FAILURE);
}

void CsvNokia::dropIfKeyInvalid(std::string key)
{
    if (table.find(key) == table.end()) {
        droppedExpressions.push_back(key);
    }

}

int CsvNokia::operate(int aVal, int bVal, char operSymbol)
{
    switch(operSymbol){
    case '+':
        return aVal + bVal;
    case '-':
        return aVal - bVal;
    case '/':
        if (bVal == 0)
            throw std::overflow_error("ZeroDivision");
        return aVal / bVal;
    case '*':
        return aVal * bVal;
    default:
        return 0;
    }
}

void CsvNokia::print(void){

    for (int i = 0; i < header.size() - 1; i++) {
        std::cout << header[i] << ',';
    }
    std::cout << *(header.end() - 1);
    std::cout << '\n';

    for(auto num: numeration){
        std::cout << num << ',';
        for(int i = 1; i < header.size() - 1; i++){
            std::cout << table[header[i] + num] << ',';
        }
        std::cout << table[*(header.end() - 1) + num];
        std::cout << '\n';
    }
}

void CsvNokia::printDroppedExpressions(void)
{
    if (!droppedExpressions.empty()) {
        std::cout << "Wrong expressions dropped:" << std::endl;
        for (auto drop : droppedExpressions) {
            if (drop.size() > 0)
                std::cout << drop << std::endl;
            else
                std::cout << "Empty expression" << std::endl;
        }
    }
}

char CsvNokia::parseElement(std::istream& is, std::string& str) {
    str.clear();
    while (true) {
        char symb = is.get();
        if (symb == '\n' || symb == ',')
            return symb;
        if (is.eof())
            return '\0';

        str.push_back(symb);
    }
}