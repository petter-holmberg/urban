#ifndef __HIGHSCORE_H__
#define __HIGHSCORE_H__

/***************************************************************************/
#define NUM_HIGHSCORES 10
/***************************************************************************/
struct Score_t {
    char Name[64];
    int Score;
    int Level;
};
/***************************************************************************/
class HighScore {
public:
    HighScore(int score, int level);
    HighScore();
    void Reset();
    ~HighScore();

protected:
    void Open();
    void Save();
    char* GetName();
    Score_t highscore[NUM_HIGHSCORES];
    FILE* fd;
};
/***************************************************************************/
#endif
