#include <iostream>
#include "dsets.h"
#include "maze.h"
#include "utility/PNG.h"

using std::cout;
using std::endl;

int main()
{
    Maze m;
    m.makeMaze(50, 50);

    PNG* unsolved = m.drawMaze();
    unsolved->writeToFile("unsolved.png");
    delete unsolved;

    std::vector<int> sol = m.solveMaze();

    PNG* solved = m.drawMazeWithSolution();
    solved->writeToFile("solved.png");
    delete solved;

    return 0;
}
