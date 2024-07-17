#ifndef HIGH_SCORE_MANAGER_H
#define HIGH_SCORE_MANAGER_H

#include <string>
#include <vector>

class HighScoreManager {
    public:
        HighScoreManager(const std::string &file_name);
        void LoadHighScores();
        void SaveHighScores();
        void UpdateHighScores(const std::string &player_name, int score);
        void PintHighScores();
    
    private:
        std::string file_name;
        std::vector<std::pair<std::string, int>> high_scores;
};

#endif