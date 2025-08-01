//Wasif Mehmood Mughal
//i240699
//Sara Kamran i243018
#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <string>
#include <fstream>
#include <cstring>
#include <cstdio>
using namespace sf;
using namespace std;
// Global variables and arrays
const int M = 25;
const int N = 40;
const int MAX_SCORE_ENTRIES = 5;
int grid[M][N] = {0};
int ts = 18;
int score1 = 0, score2 = 0; 
int rewardCount1 = 0, rewardCount2 = 0;
int bigCaptureThreshold1 = 10, bigCaptureThreshold2 = 10;
float nextPowerUpScore1 = 50.0f, nextPowerUpScore2 = 50.0f;
bool powerUpActive = false;
float enemyFreezeTimer = 0.0f, playerFreezeTimer = 0.0f;
int currentLevel = 1, enemyCount = 2;
const float speedIncrement = 0.2f;
float baseSpeed = 1.0f;
float lastLevelTime = 0.0f;
const int maxEnemies = 10;
float enemySpeeds[maxEnemies]; 
// Player 1 power-ups
int powerUpCount1 = 0, powerUpCapacity1 = 0;
bool* powerUps1 = nullptr;

// Player 2 power-ups
int powerUpCount2 = 0, powerUpCapacity2 = 0;
bool* powerUps2 = nullptr;
// Here the variables End 

void level2(int &enemyCount, int maxEnemies, float baseSpeed, float enemySpeeds[])
{
    if (enemyCount < maxEnemies) {
        int oldCount = enemyCount;
        enemyCount = 4;                      // spawn up to 4
        for (int i = oldCount; i < enemyCount; ++i)
            enemySpeeds[i] = (baseSpeed*100);      // only new ones are fast
    }
}

// Level 3: grow to 6 enemies
void level3(int &enemyCount, int maxEnemies, float baseSpeed, float enemySpeeds[])
{
    if (enemyCount < maxEnemies) {
        int oldCount = enemyCount;
        enemyCount = 6;
        for (int i = oldCount; i < enemyCount; ++i)
            enemySpeeds[i] = (baseSpeed*1000);
    }
}

// Level 4: grow to 8 enemies
void level4(int &enemyCount, int maxEnemies, float baseSpeed, float enemySpeeds[])
{
    if (enemyCount < maxEnemies) {
        int oldCount = enemyCount;
        enemyCount = 8;
        for (int i = oldCount; i < enemyCount; ++i)
            enemySpeeds[i] = (baseSpeed*2000);
    }
}



struct ScoreEntry {
    char playerName[20];
    int score;
    int timeTaken;
};

const int MAX_DISPLAY_ENTRIES = 5; 


int readScoreboard(ScoreEntry entries[], int maxEntries) {
    ifstream file("scores.txt");
    if (!file.is_open()) return 0;

    
    vector<ScoreEntry> allEntries;
    ScoreEntry temp;
    
    while (file >> temp.playerName >> temp.score >> temp.timeTaken) {
        allEntries.push_back(temp);
    }
    file.close();

    
    sort(allEntries.begin(), allEntries.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
        if (a.score != b.score) return a.score > b.score;
        return a.timeTaken < b.timeTaken;
    });


    int count = min((int)allEntries.size(), maxEntries);
    for (int i = 0; i < count; i++) {
        entries[i] = allEntries[i];
    }

    return count;
}


void writeScoreboard(const char* playerName, int score, int timeTaken) {
    ofstream file("scores.txt", ios::app);
    if (!file.is_open()) return;

    file << playerName << " " << score << " " << timeTaken << "\n";
    file.close();
}


void displayScoreboard() {
    ScoreEntry entries[MAX_DISPLAY_ENTRIES];
    int count = readScoreboard(entries, MAX_DISPLAY_ENTRIES);
    
    cout << "\nSCOREBOARD (TOP " << MAX_DISPLAY_ENTRIES << ")\n";
    for (int i = 0; i < count; i++) {
        cout << i + 1 << ". " << entries[i].playerName
             << " | Score: " << entries[i].score
             << " | Time: " << entries[i].timeTaken << "s\n";
    }
}


void addPlayerScore() {
    char name[20];
    int score, time;
    
    cout << "Enter player name: ";
    cin >> name;
    cout << "Enter score: ";
    cin >> score;
    cout << "Enter time taken: ";
    cin >> time;
    
    // Append to file
    writeScoreboard(name, score, time);
    
    // Display top 5
    displayScoreboard();
}
struct Enemy {
    int x, y, dx, dy;
    Enemy() {
        x = y = 300;
        dx = 4 - rand() % 8;
        dy = 4 - rand() % 8;
    }

    void move() {
        if (enemyFreezeTimer > 0.0f) return;
        x += dx;
        if (grid[y / ts][x / ts] == 1) {
            dx = -dx;
            x += dx;
        }
        y += dy;
        if (grid[y / ts][x / ts] == 1) {
            dy = -dy;
            y += dy;
        }
    }
};

void updateFreezeTimer(float deltaTime) {
    if (enemyFreezeTimer > 0.0f)
        enemyFreezeTimer = max(0.0f, enemyFreezeTimer - deltaTime);
    if (playerFreezeTimer > 0.0f)
        playerFreezeTimer = max(0.0f, playerFreezeTimer - deltaTime);
}

void earnPowerUp(int player) {
    if (player == 1) {
        if (powerUpCount1 == powerUpCapacity1) {
            int newCap = (powerUpCapacity1 == 0 ? 1 : powerUpCapacity1 * 2);
            bool* newArr = new bool[newCap];
            for (int i = 0; i < powerUpCount1; ++i)
                newArr[i] = powerUps1[i];
            delete[] powerUps1;
            powerUps1 = newArr;
            powerUpCapacity1 = newCap;
        }
        powerUps1[powerUpCount1++] = true;
    } else {
        if (powerUpCount2 == powerUpCapacity2) {
            int newCap = (powerUpCapacity2 == 0 ? 1 : powerUpCapacity2 * 2);
            bool* newArr = new bool[newCap];
            for (int i = 0; i < powerUpCount2; ++i)
                newArr[i] = powerUps2[i];
            delete[] powerUps2;
            powerUps2 = newArr;
            powerUpCapacity2 = newCap;
        }
        powerUps2[powerUpCount2++] = true;
    }
}

bool tryUsePowerUp(int player) {
    if (player == 1 && powerUpCount1 > 0) {
        powerUpCount1--;
        enemyFreezeTimer = 3.0f;
        playerFreezeTimer = 3.0f;
        return true;
    }
    if (player == 2 && powerUpCount2 > 0) {
        powerUpCount2--;
        enemyFreezeTimer = 3.0f;
        playerFreezeTimer = 3.0f;
        return true;
    }
    return false;
}

void awardPoints(int tilesThisMove, int player) {
    int* rewardCount = (player == 1) ? &rewardCount1 : &rewardCount2;
    int* bigCaptureThreshold = (player == 1) ? &bigCaptureThreshold1 : &bigCaptureThreshold2;
    int* score = (player == 1) ? &score1 : &score2;
    
    int multiplier = 1;
    if (tilesThisMove > *bigCaptureThreshold) {
        multiplier = (*rewardCount < 5 ? 2 : 4);
        (*rewardCount)++;
        if (*rewardCount == 3) *bigCaptureThreshold = 5;
    }
    *score += tilesThisMove * multiplier;
}

void drop(int y, int x) {
    if (grid[y][x] == 0) grid[y][x] = -1;
    if (grid[y - 1][x] == 0) drop(y - 1, x);
    if (grid[y + 1][x] == 0) drop(y + 1, x);
    if (grid[y][x - 1] == 0) drop(y, x - 1);
    if (grid[y][x + 1] == 0) drop(y, x + 1);
}

int main() {
    int moveCount= 0;
    srand(time(0));
    RenderWindow window(VideoMode(N * ts, M * ts), "Xonix Game!");
    window.setFramerateLimit(60);

    Texture t1, t2, t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/gameover.png");
    t3.loadFromFile("images/enemy.png");

    Font font;
    if (!font.loadFromFile("pokemon.ttf")) return EXIT_FAILURE;

    // Player 1 UI
    Text scoreText1, moveText1, powerUpText1;
    scoreText1.setFont(font);
    scoreText1.setCharacterSize(20);
    scoreText1.setFillColor(Color::White);
    scoreText1.setPosition(10, 40);
    Text moveText;
    moveText.setFont(font);
    moveText.setCharacterSize(20);
    moveText.setFillColor(Color::White);
    moveText.setPosition(10, 10); // Position it at the top-left corner
    // Player 2 UI
    Text scoreText2, powerUpText2;
    scoreText2.setFont(font);
    scoreText2.setCharacterSize(20);
    scoreText2.setFillColor(Color::White);
    scoreText2.setPosition(N*ts - 150, 40);

    Sprite sTile(t1), sGameover(t2), sEnemy(t3);
    sGameover.setPosition(100, 100);
    sEnemy.setOrigin(20, 20);

    
    Enemy a[10];
    bool Game = true, player1Alive = true, player2Alive = true;
    int x1 = 10, y1 = 0, dx1 = 0, dy1 = 0;
    int x2 = N-10, y2 = M-1, dx2 = 0, dy2 = 0;
    float timer = 0, delay = 0.07;
    Clock clock, levelClock,sec;

    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
                grid[i][j] = 1;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        float levelTime = levelClock.getElapsedTime().asSeconds();
         int counting = sec.getElapsedTime().asSeconds();
        timer += deltaTime;
        updateFreezeTimer(deltaTime);
        if (levelTime - lastLevelTime >= 20.0f) {
            lastLevelTime += 20.0f;
            baseSpeed += speedIncrement;
            currentLevel++;
        
            
            if (currentLevel == 2) level2(enemyCount, maxEnemies, baseSpeed, enemySpeeds);
            else if (currentLevel == 3) level3(enemyCount, maxEnemies, baseSpeed, enemySpeeds);
            else if (currentLevel == 4) level4(enemyCount, maxEnemies, baseSpeed, enemySpeeds);
        }
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed) window.close();
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Escape) {
                    for (int i = 1; i < M - 1; i++)
                        for (int j = 1; j < N - 1; j++)
                            grid[i][j] = 0;
                    x1 = 10; y1 = 0; x2 = N-10; y2 = M-1;
                    moveCount = 0;
                    Game = true; player1Alive = true; player2Alive = true;
                    counting = 0; score1 = score2 = 0;
                    nextPowerUpScore1 = nextPowerUpScore2 = 50.0f;
                    rewardCount1 = rewardCount2 = 0;
                    bigCaptureThreshold1 = bigCaptureThreshold2 = 10;
                    delete[] powerUps1; delete[] powerUps2;
                    powerUps1 = powerUps2 = nullptr;
                    powerUpCount1 = powerUpCapacity1 = 0;
                    powerUpCount2 = powerUpCapacity2 = 0;
                }
                else if (e.key.code == Keyboard::Num1) tryUsePowerUp(1);
                else if (e.key.code == Keyboard::Num2) tryUsePowerUp(2);
            }
        }
        if (e.type == Event::KeyPressed)
    {
        switch (e.key.code)
        {
            

            // Player 1 movement (Arrow keys)
            case Keyboard::Left:
                moveCount++;
                break;
            case Keyboard::Right:
                
                moveCount++;
                break;
            case Keyboard::Up:
               
                moveCount++;
                break;
            case Keyboard::Down:
                
                moveCount++;
                break;

            // Player 2 movement (WASD)
            case Keyboard::A:
                
                moveCount++;
                break;
            case Keyboard::D:
               
                moveCount++;
                break;
            case Keyboard::W:
              
                moveCount++;
                break;
            case Keyboard::S:
               
                moveCount++;
                break;
                    default :break;}}
        // Player 1 controls
        if (player1Alive && playerFreezeTimer <= 0) {
            if (Keyboard::isKeyPressed(Keyboard::Left)) { dx1 = -1; dy1 = 0;}// moveCount++; }
            if (Keyboard::isKeyPressed(Keyboard::Right)) { dx1 = 1; dy1 = 0;}// moveCount++; }
            if (Keyboard::isKeyPressed(Keyboard::Up)) { dx1 = 0; dy1 = -1; }//moveCount++; }
            if (Keyboard::isKeyPressed(Keyboard::Down)) { dx1 = 0; dy1 = 1; }//moveCount++; }
        }

        // Player 2 controls
        if (player2Alive && playerFreezeTimer <= 0) {
            if (Keyboard::isKeyPressed(Keyboard::A)) { dx2 = -1; dy2 = 0; }// moveCount++;}
            if (Keyboard::isKeyPressed(Keyboard::D)) { dx2 = 1; dy2 = 0; }// moveCount++;}
            if (Keyboard::isKeyPressed(Keyboard::W)) { dx2 = 0; dy2 = -1;  }//moveCount++;}
            if (Keyboard::isKeyPressed(Keyboard::S)) { dx2 = 0; dy2 = 1;  }
        }

        if (!Game) {
            window.clear();
            window.draw(sGameover);
             int time1;
             char player1[20], player2[20];
             int score1, score2; 
              
             
             cout << "Enter Player 1 Name: ";
             cin >> player1;
             
             cout << "Enter Player 2 Name: ";
             cin >> player2;
             
             
             writeScoreboard(player1, score1, counting);
             writeScoreboard(player2, score2, counting);
             
             
             displayScoreboard();
        
            continue;
        }
        

        if (timer > delay) {
            // Player 1 movement
            if (player1Alive) {
                x1 += dx1;
                y1 += dy1;
               
                
                // boundary
                if (x1 < 0) x1 = 0;
                if (x1 >= N) x1 = N-1;
                if (y1 < 0) y1 = 0;
                if (y1 >= M) y1 = M-1;
                
                if (grid[y1][x1] == 3) player1Alive = false;
                if (grid[y1][x1] == 0) grid[y1][x1] = 2; // Player 1 trail
                if (e.type == Event::KeyPressed) {
                    if (e.key.code == Keyboard::Escape) {
                        counting=0;}}
            }
        
            // Player 2 movement
            if (player2Alive) {
                x2 += dx2;
                y2 += dy2;
               
                
                // boundary dekhna
                if (x2 < 0) x2 = 0;
                if (x2 >= N) x2 = N-1;
                if (y2 < 0) y2 = 0;
                if (y2 >= M) y2 = M-1;
                
                if (grid[y2][x2] == 2) player2Alive = false;
                if (grid[y2][x2] == 0) grid[y2][x2] = 3; // Player 2 trail
            }
        
            // Rest of the collision checking...
            timer = 0;
        }

        for (int i = 0; i < enemyCount; i++) a[i].move();

        // Player 1 area capture
        if (player1Alive && grid[y1][x1] == 1) {
            dx1 = dy1 = 0;
            int zerosBefore = 0;
            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    if (grid[i][j] == 0) zerosBefore++;

            for (int i = 0; i < enemyCount; i++)
                drop(a[i].y / ts, a[i].x / ts);

            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    grid[i][j] = (grid[i][j] == -1) ? 0 : 1;

            int zerosAfter = 0;
            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    if (grid[i][j] == 0) zerosAfter++;

            int tilesCaptured = zerosBefore - zerosAfter;
            if (tilesCaptured > 0) {
                awardPoints(tilesCaptured, 1);
                if (score1 >= nextPowerUpScore1) {
                    earnPowerUp(1);
                    nextPowerUpScore1 += 50;
                }
            }
        }

        // Player 2 area capture
        if (player2Alive && grid[y2][x2] == 1) {
            dx2 = dy2 = 0;
            int zerosBefore = 0;
            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    if (grid[i][j] == 0) zerosBefore++;

            for (int i = 0; i < enemyCount; i++)
                drop(a[i].y / ts, a[i].x / ts);

            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    grid[i][j] = (grid[i][j] == -1) ? 0 : 1;

            int zerosAfter = 0;
            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    if (grid[i][j] == 0) zerosAfter++;

            int tilesCaptured = zerosBefore - zerosAfter;
            if (tilesCaptured > 0) {
                awardPoints(tilesCaptured, 2);
                if (score2 >= nextPowerUpScore2) {
                    earnPowerUp(2);
                    nextPowerUpScore2 += 50;
                }
            }
        }

        // Enemy collision check
        for (int i = 0; i < enemyCount; i++) {
            int ex = a[i].x / ts, ey = a[i].y / ts;
            if (grid[ey][ex] == 2) player1Alive = false;
            if (grid[ey][ex] == 3) player2Alive = false;
        }

        window.clear();

        // Draw grid
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (grid[i][j] == 0) continue;
                sTile.setTextureRect(IntRect(
                    (grid[i][j] == 1) ? 0 : 
                    (grid[i][j] == 2) ? 54 : 72, 0, ts, ts));
                sTile.setPosition(j * ts, i * ts);
                window.draw(sTile);
            }
        }

        // Draw players
        if (player1Alive) {
            sTile.setTextureRect(IntRect(36, 0, ts, ts));
            sTile.setPosition(x1 * ts, y1 * ts);
            window.draw(sTile);
        }
        if (player2Alive) {
            sTile.setTextureRect(IntRect(36, 0, ts, ts));
            sTile.setPosition(x2 * ts, y2 * ts);
            window.draw(sTile);
        }
        
        // Draw enemies
        for (int i = 0; i < enemyCount; i++) {
            sEnemy.setPosition(a[i].x, a[i].y);
            window.draw(sEnemy);
        }
        Text moveText;
Font font;
font.loadFromFile("pokemon.ttf"); // Load a font file
moveText.setFont(font);
moveText.setString("Moves: " + std::to_string(moveCount));
moveText.setCharacterSize(20);
moveText.setFillColor(Color::White);
moveText.setPosition(10, 10);

window.draw(moveText);

        // Update UI
        scoreText1.setString("Player 1: " + std::to_string(score1) + "\nPower-ups: " + std::to_string(powerUpCount1));
        scoreText2.setString("Player 2: " + std::to_string(score2) + "\nPower-ups: " + std::to_string(powerUpCount2));
        moveText.setString("Timer: " + std::to_string(counting));
        window.draw(scoreText1);
        window.draw(moveText);
        window.draw(scoreText2);

        // Game over check
        if (!player1Alive && !player2Alive) {
            window.draw(sGameover);
            Text winnerText;
            winnerText.setFont(font);
            winnerText.setCharacterSize(30);
            winnerText.setFillColor(Color::White);
            winnerText.setPosition(100, 150);
            
            if (score1 > score2) winnerText.setString("Player 1 Wins!");
            else if (score2 > score1) winnerText.setString("Player 2 Wins!");
            else winnerText.setString("It's a Tie!");
            
            window.draw(winnerText);
            Game = false;
        }

        window.display();
    }
    return 0;
}