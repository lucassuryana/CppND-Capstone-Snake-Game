#include "high_score_manager.h"
#include <iostream>
#include <fstream>
#include <algorithm>

HighScoreManager::HighScoreManager(const std::string &file_name) : file_name(file_name) {}

void HighScoreManager::LoadHighScores() {
    std::ifstream file(file_name);
    if (file.is_open()) {
        std::string name;
        int score;
        while (file >> name >> score) {
            high_scores.push_back({name, score});
        }
        file.close();
    }
}

void HighScoreManager::SaveHighScores() {
    std::ofstream file(file_name);
    if (file.is_open()) {
        for (const auto &entry : high_scores) {
            file << entry.first << " " << entry.second << std::endl;
        }
        file.close();
    }
}

void HighScoreManager::UpdateHighScores(const std::string &player_name, int score) {
    high_scores.push_back({player_name, score});
    SortHighScores();
    if (high_scores.size() > 10) {
        high_scores.pop_back(); // Keep only the top 10 scores
    }
}

void HighScoreManager::PrintHighScores() const {
    std::cout << "High Scores:\n";
    for (const auto&entry : high_scores) {
        std::cout << entry.first << ":" << entry.second << std::endl;
    }
}

void HighScoreManager::SortHighScores() {
    std::sort(high_scores.begin(), high_scores.end(), [](const auto &a, const auto &b) {
        return b.second < a.second; // Sort in descending order of score
    });
}