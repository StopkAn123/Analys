#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>  // Для EXIT_FAILURE

using namespace std;

struct Token {
    string type;
    string value;
};

// Типы токенов
const vector<pair<string, string>> TOKEN_TYPES = {
    {"Ключевое_слово", R"(\b(for|do)\b)"},
    {"Римская_цифра", R"(\b(I|V|X|L|C|D|M)+\b)"},
    {"Идентификатор", R"(\b[a-zA-Z_][a-zA-Z0-9_]*\b)"},
    {"Открывающая_скобка", R"(\()"},
    {"Закрывающая_скобка", R"(\))"},
    {"Оператор_сравнения", R"(<=|>=|<|>|=)"},
    {"Оператор_присваивания", R"(:=)"},
    {"Разделитель", R"(;)"}
};

void analyzeLine(const string& line, int lineNumber, vector<Token>& tokens) {
    size_t commentPos = line.find("//");
    string cleanLine = (commentPos != string::npos) ? line.substr(0, commentPos) : line;

    // Пропускаем пустую строку после удаления комментария
    if (cleanLine.empty()) {
        return;
    }

    // Проверяем на неправильный идентификатор
    regex invalidIdentifierRegex(R"(\b\d[a-zA-Z0-9_]*\b)");
    smatch invalidMatch;
    if (regex_search(cleanLine, invalidMatch, invalidIdentifierRegex)) {
        cout << "Ошибка: неправильный идентификатор: " << invalidMatch.str() << " в строке " << lineNumber << "\n";
        exit(EXIT_SUCCESS);  // Завершаем работу после вывода ошибки
    }

    // Проверяем на наличие текста после do;
    regex extraTextAfterDoRegex(R"(do;\s*\S+)");
    smatch extraTextMatch;
    if (regex_search(cleanLine, extraTextMatch, extraTextAfterDoRegex)) {
        cout << "Ошибка: лишний текст после do; в строке " << lineNumber << "\n";
        exit(EXIT_SUCCESS);  // Завершаем работу после вывода ошибки
    }

    // Проверяем на некорректные римские цифры
    regex invalidRomanNumeralRegex(R"(\bI{4,}|X{4,}|C{4,}|M{4,}\b)");
    smatch romanMatch;
    if (regex_search(cleanLine, romanMatch, invalidRomanNumeralRegex)) {
        cout << "Ошибка: некорректные римские цифры: " << romanMatch.str() << " в строке " << lineNumber << "\n";
        exit(EXIT_SUCCESS);  // Завершаем работу после вывода ошибки
    }

    size_t position = 0;
    while (position < cleanLine.length()) {
        bool matched = false;
        for (const auto& token : TOKEN_TYPES) {
            const string& type = token.first;
            const string& pattern = token.second;
            regex patternRegex(pattern);
            smatch match;

            string subLine = cleanLine.substr(position);
            if (regex_search(subLine, match, patternRegex) && match.position() == 0) {
                tokens.push_back({ type, match.str() });
                position += match.length();
                matched = true;
                break;
            }
        }
        if (!matched) {
            ++position;
        }
    }
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");  // Установка локали для корректного отображения кириллицы

    if (argc < 2) {
        cerr << "Укажите путь к файлу.\n";
        return EXIT_FAILURE;
    }

    ifstream file(argv[1]);
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл.\n";
        return EXIT_FAILURE;
    }

    vector<Token> tokens;
    string line;
    int lineNumber = 0;

    while (getline(file, line)) {
        ++lineNumber;
        analyzeLine(line, lineNumber, tokens);
    }

    file.close();

    // Вывод заголовка таблицы
    cout << left << setw(5) << "№" << setw(20) << "Лексема" << setw(20) << "Значение" << "\n";
    cout << string(45, '-') << "\n";

    // Вывод токенов
    int index = 1;
    for (const auto& token : tokens) {
        cout << left << setw(5) << index++
            << setw(20) << token.type
            << setw(20) << token.value << "\n";
    }

    return EXIT_SUCCESS;
}
