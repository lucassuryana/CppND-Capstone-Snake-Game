#ifndef HIGH_SCORE_MANAGER_H
#define HIGH_SCORE_MANAGER_H

#include <string>
#include <vector>

class HighScoreManager {
    public:
        // Constructor
        // Initializes the high score manager through file that contains the list of high scores
        HighScoreManager(const std::string &file_name);

        // Move constructor and move assignment operator
        HighScoreManager(HighScoreManager &&other) noexcept;
        HighScoreManager& operator=(HighScoreManager &&other) noexcept;

        // Deleted copy constructor and copy assignment operator
        HighScoreManager(const HighScoreManager &) = delete;
        HighScoreManager& operator=(const HighScoreManager &) = delete;     
        // Load the scores data from file
        void LoadHighScores();
        // Save the scores to file
        void SaveHighScores();
        // Update the list of high scores
        void UpdateHighScores(const std::string &player_name, int score);
        // Pring the list of scores on terminal
        void PrintHighScores() const;
    
    private:
        std::string file_name; // The name of the file to save the list of high scores
        std::vector<std::pair<std::string, int>> high_scores; // The vector with a pair value of name and scores

        // Sort the scores from high to low scores
        void SortHighScores();
};

#endif