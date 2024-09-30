#include <fstream>
#include <string>
#include <unordered_set>
#include <iostream>
#include <future>
#include <vector>

std::unordered_set<std::string> readLines(const std::string& filename) {
    std::unordered_set<std::string> lines;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        throw std::runtime_error("error opening: " + filename);
    }

    while (std::getline(file, line)) {
        lines.insert(line);
    }
    return lines;
}

void writeUniqueLines(const std::unordered_set<std::string>& fromSet, 
                      const std::unordered_set<std::string>& toSet, 
                      const std::string& outputFile) {
    std::ofstream out(outputFile);
    if (!out.is_open()) {
        throw std::runtime_error("error opening: " + outputFile);
    }

    for (const auto& str : fromSet) {
        if (toSet.find(str) == toSet.end()) {
            out << str << std::endl;
        }
    }
}

int main() {
    try {
        // Считываем данные асинхронно
        auto future1 = std::async(std::launch::async, readLines, "file1.txt");
        auto future2 = std::async(std::launch::async, readLines, "file2.txt");

        // Получаем результаты асинхронных задач
        auto set1 = future1.get();
        auto set2 = future2.get();

        // Записываем уникальные строки в файлы асинхронно
        auto writeFuture1 = std::async(std::launch::async, writeUniqueLines, set1, set2, "file3.txt");
        auto writeFuture2 = std::async(std::launch::async, writeUniqueLines, set2, set1, "file4.txt");

        // Ожидаем завершения записи
        writeFuture1.get();
        writeFuture2.get();

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
