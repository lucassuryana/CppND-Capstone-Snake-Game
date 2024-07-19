#include "high_score_manager.h"
#include <iostream>
#include <fstream>
#include <algorithm>

// Constructor
// Initialize the high score manager with initial value from file
HighScoreManager::HighScoreManager(const std::string &file_name) : file_name(file_name) {}

// Load the list of high scores value from file
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

// Save the list of high scores value to file
void HighScoreManager::SaveHighScores() {
    std::ofstream file(file_name);
    if (file.is_open()) {
        for (const auto &entry : high_scores) {
            file << entry.first << " " << entry.second << std::endl;
        }
        file.close();
    }
}

// Update the list of high scores with the current game's score
void HighScoreManager::UpdateHighScores(const std::string &player_name, int score) {
    high_scores.push_back({player_name, score});
    SortHighScores();
    if (high_scores.size() > 10) {
        high_scores.pop_back(); // Keep only the top 10 scores
    }
}

// Update the list of high scores in the terminal
void HighScoreManager::PrintHighScores() const {
    std::cout << "High Scores:\n";
    for (const auto&entry : high_scores) {
        std::cout << entry.first << ":" << entry.second << std::endl;
    }
}

// Sort the list of high scores from high to low scores
void HighScoreManager::SortHighScores() {
    std::sort(high_scores.begin(), high_scores.end(), [](const auto &a, const auto &b) {
        return b.second < a.second; // Sort in descending order of score
    });
}