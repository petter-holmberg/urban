#ifndef __HIGHSCORE_H__
#define __HIGHSCORE_H__

#include <array>
#include <string>

/***************************************************************************/
inline constexpr auto NUM_HIGHSCORES = 10;
/***************************************************************************/
struct Score_t {
    std::array<char, 64> Name {};
    int Score {};
    int Level {};
};
/***************************************************************************/
class HighScore {
public:
    HighScore(int score, int level);
    HighScore();
    void Reset();
    ~HighScore() = default;

protected:
    void Open();
    void Save();
    static std::string GetName();
    std::array<Score_t, NUM_HIGHSCORES> highscore;
    FILE* fd {};
};
/***************************************************************************/
#endif
