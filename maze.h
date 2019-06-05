#ifndef MAZE_H
#define MAZE_H

#include "dsets.h"
#include <stack>
#include "utility/PNG.h"
#include "utility/HSLAPixel.h"
#include "utility/RGB_HSL.h"
using namespace utility;

class Maze{
public:
    Maze();
    void makeMaze(int width, int height);
    bool canTravel(int x, int y, int dir) const;
    void setWall(int x, int y, int dir, bool exists);
    vector<int> solveMaze();
    PNG* drawMaze() const;
    PNG* drawMazeWithSolution();

private:
    int getCellIndex(int x, int y) const;
    int getCellX( int cellID);
    int getCellY( int cellID);
    int getNextCell( int x, int y, int dir );

    int mazeWidth;
    int mazeHeight;
    int size;
    DisjointSets cells;
    vector<vector<bool>> down;
    vector<vector<bool>> right;
};


#endif
