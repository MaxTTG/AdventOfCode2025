#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace aoc {

// Чтение всего файла как строки
inline std::string read_file(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + path);
    return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

// Чтение файла построчно
inline std::vector<std::string> read_lines(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file: " + path);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
        lines.push_back(line);
    return lines;
}

// Разделение строки по разделителю
inline std::vector<std::string> split(const std::string &s, char delimiter = ' ') {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, delimiter))
        tokens.push_back(token);

    return tokens;
}

// Преобразование строки в число (с проверкой)
template <typename T>
inline T to_num(const std::string &s) {
    std::stringstream ss(s);
    T val;
    ss >> val;
    if (!ss.eof() || ss.fail())
        throw std::runtime_error("Invalid number: " + s);

    return val;
}

// Получение пути к input.txt для текущего дня (опционально)
// Например: get_input_path("01") -> "${AOC_PROJECT_ROOT}/days/01/input.txt"
// AOC_PROJECT_ROOT from CMakeLists.txt
inline std::string get_input_path(const std::string &day_str) {
    return std::string(AOC_PROJECT_ROOT) + "/days/" + day_str + "/input.txt";
}

// Запись ответов в answer.txt для текущего дня
inline void write_answers(const std::string &day_str, int part1, int part2) {
    std::filesystem::path answer_path(std::string(AOC_PROJECT_ROOT) + "/days/" + day_str + "/answer.txt");
    std::filesystem::create_directories(answer_path.parent_path());
    std::ofstream file(answer_path);
    if (!file.is_open())
        throw std::runtime_error("Cannot create answer file: " + answer_path.string());

    file << "Part 1: " << part1 << "\n";
    file << "Part 2: " << part2 << std::endl;
}

}  // namespace aoc
