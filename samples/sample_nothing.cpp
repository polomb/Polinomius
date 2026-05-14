#include <iostream>
#include <string>

#include "HashMap.h"

int main() {
    // ── Basic insert & lookup ─────────────────────────────────────────────────
    HashMap<std::string, int> scores;

    for (auto& [name, score] : {std::pair<const char*, int>
            {"Alice", 95}, {"Bob", 82}, {"Carol", 78}, {"Dave", 91}}) {
        scores.insert(name, score);
    }

    std::cout << "=== scores ===\n";
    for (auto& [key, val] : scores) {
        std::cout << "  " << key << " -> " << val << '\n';
    }

    // ── operator[] ────────────────────────────────────────────────────────────
    scores["Eve"] = 88;
    std::cout << "\nAfter scores[\"Eve\"] = 88:\n";
    std::cout << "  Eve -> " << scores.at("Eve") << '\n';

    // ── update existing ───────────────────────────────────────────────────────
    scores.insert("Bob", 99);   // updates, not duplicates
    std::cout << "\nAfter update Bob -> 99:\n";
    std::cout << "  Bob -> " << scores.at("Bob") << '\n';

    // ── erase ─────────────────────────────────────────────────────────────────
    scores.erase("Carol");
    std::cout << "\nAfter erase(\"Carol\"), contains: "
              << scores.contains("Carol") << '\n';   // 0

    // ── size & load factor ────────────────────────────────────────────────────
    std::cout << "\nsize: " << scores.size()
              << "  buckets: " << scores.bucket_count()
              << "  load_factor: " << scores.load_factor() << '\n';

    // ── integer keys ──────────────────────────────────────────────────────────
    HashMap<int, int> squares;
    for (int i = 1; i <= 10; ++i) squares.insert(i, i * i);

    std::cout << "\n=== squares ===\n";
    for (auto& [k, v] : squares) {
        std::cout << "  " << k << "^2 = " << v << '\n';
    }

    return 0;
}