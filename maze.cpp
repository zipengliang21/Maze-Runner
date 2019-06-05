#include <time.h>
#include "maze.h"
#include <map>
#include <cmath>
#include <iostream>
#include <algorithm>

#define RIGHT 0
#define BOTTOM 1
#define LEFT 2
#define UP 3

using namespace std;

/**
 * The constructor for Maze
 */
Maze::Maze()
{
    mazeWidth = 0;
    mazeHeight = 0;
    size = 0;
    cells = DisjointSets();
}

/**
 * This function is to generate a maze with the given width and height
 * It will return if the given width or height is too small
 * My approach is to consider each point as a cell; we use 2D vector "right" and "down"
 * to store the information that whether there is wall in the right side or down size of the cell
 * @param width the width of the maze
 * @param height the height of the maze
 */
void Maze::makeMaze(int width, int height)
{
    // check whether the given width or height is too small to generate a maze
    if (width < 2 || height < 2)
    {
        cout<<"The values you are given are too small to generate a maze"<<endl;
        return;
    }

    //assigns the value for corresponding instance values
    mazeWidth = width;
    mazeHeight = height;
    size = width * height;
    cells.addelements(size);

     //setup randomization
     srand (static_cast<unsigned int>(time(NULL)));

    // setup all the walls to be present
    //we just need right and down 2D vector to do it
    for(int i = 0; i < mazeWidth; i++)
    {
        right.push_back(vector<bool>());
        down.push_back(vector<bool>());
        for(int j = 0; j < mazeHeight; j++)
        {
            right[i].push_back(true);
            down[i].push_back(true);
        }
    }


    // To generate the maze, the program randomly removes the walls
    // These walls are in the right or down side of each cell
    // Of course, removing th walls cannot result in making cycle in the maze
    // The number of walls we need to remove is equal to size - 1
    int count = 0;

    while(count!= size-1)
    {
        int randomX = rand()%mazeWidth;
        int randomY = rand()%mazeHeight;
        bool randomDirectionWall = rand()%2;
        int cellIdx = mazeWidth*randomY +randomX;
        int cellRightIdx = mazeWidth*randomY +randomX +1;
        int cellDownIdx = mazeWidth*randomY + randomX + mazeWidth;

        //we remove the right wall
        if(randomDirectionWall == false)
        {
            if(randomX!= mazeWidth - 1)
            {
                if(cells.find(cellIdx)!=cells.find(cellRightIdx))
                {
                    if(right[randomX][randomY] == true)
                    {
                        setWall(randomX,randomY,0,false);
                        cells.setunion(cellIdx,cellRightIdx);
                        count++;
                    }
                }
            }
        }

        //we remove the down wall
        if(randomDirectionWall == true)
        {
            if(randomY!=mazeHeight - 1)
            {
                if(cells.find(cellIdx)!=cells.find(cellDownIdx))
                {
                    if(down[randomX][randomY] == true)
                    {
                        setWall(randomX,randomY,1,false);
                        cells.setunion(cellIdx,cellDownIdx);
                        count++;
                    }
                }
            }
        }
    }
}

/**
 * Given the location of the cell, and one direction, this function is to
 * judge whether it can travel one unit in this direction
 * @param x the given x coordinate of the cell
 * @param y the given y coordinate of the cell
 * @param dir the given direction of the cell
 * @return
 */
bool Maze::canTravel(int x, int y, int dir) const
{
    // check out of bounds
    if ( x < 0 || y < 0 || x > mazeWidth || y > mazeHeight )
        return false;

    //edge case
    //Since, if the cell in this edge case, it is impossible to move in the given direction
    if ( ( x == 0 && dir == LEFT ) ||
         ( y == 0 && dir == UP ) ||
         ( y == mazeHeight -1 && dir == BOTTOM ) ||
         ( x == mazeWidth -1 && dir == RIGHT ) )
        return false;


    //check different cases
    switch (dir)
    {
        case RIGHT:
            return right[x][y]==false;
        case BOTTOM:
            return down[x][y]==false;
        case LEFT:
            return right[x-1][y]==false;
        case UP:
            return down[x][y-1]==false;
        default:
            return false;
    }
}

/**
 *
 * @param x the given x coordinate of the cell
 * @param y the given y coordinate of the cell
 * @param dir the given direction of the cell
 * @param exists boolean variable. If yes, it indicates there exist a wall in the given location
 */
void Maze::setWall (int x, int y, int dir, bool exists)
{
    if(dir == 0)        right[x][y] = exists;
    else if(dir ==1)    down[x][y] = exists;
}

/**
 * This function is to draw the maze in PNG form with the given width*10+1 and given height*10+1
 * The thickness of the wall is 1; The thickness of the cell is 10
 * We draw the color by changing the hue, saturation, and brightness of the pixel
 * @return the PNG containing the maze after drawing
 */
PNG* Maze::drawMaze() const
{
    PNG *maze = new PNG(mazeWidth*10+1,mazeHeight*10+1);

    //draw walls for the whole maze in the left and up size
    //since there always exists walls except the left upper corner which is the entry

    //draw the upper walls of the whole maze
    for(int i = 10; i < mazeWidth*10+1;i++)
    {
        HSLAPixel* pixelPointer = maze->getPixel(i,0);
        pixelPointer->h = 0;
        pixelPointer->s = 0;
        pixelPointer->l = 0;
    }

    //draw the left walls of the whole maze
    for(int j = 0; j < mazeHeight*10+1;j++)
    {
        HSLAPixel* pixelPointer = maze->getPixel(0, j);
        pixelPointer->h = 0;
        pixelPointer->s = 0;
        pixelPointer->l = 0;
    }

    //x, y coordinate of the right or down walls
    int xValue;
    int yValue;

    //draw the right or down wall if there's ones
    for(int x = 0; x < mazeWidth; x++)
    {
        for(int y = 0; y < mazeHeight; y++)
        {
            if(right[x][y]){
                for(int m = 0; m < 11;m++)
                {
                    xValue = ( x + 1) * 10;
                    yValue = y * 10 + m;

                    HSLAPixel* pixelPointer = maze->getPixel(xValue,yValue);
                    pixelPointer->h = 0;
                    pixelPointer->s = 0;
                    pixelPointer->l = 0;
                }
            }
            if(down[x][y]){
                for(int m = 0; m < 11;m++)
                {
                    xValue = 10 * x + m;
                    yValue = (y + 1) * 10;

                    HSLAPixel* pixelPointer = maze->getPixel(xValue,yValue);
                    pixelPointer->h = 0;
                    pixelPointer->s = 0;
                    pixelPointer->l = 0;
                }
            }
        }
    }

    return maze;
}

/**
 * This function is to solve the maze by using vector, map, and stack
 * we draw the longest path and make the end of this path as exit
 * @return the vector containing the path to get out of the maze!
 */
vector<int> Maze::solveMaze()
{
    vector<vector<bool>> visited;//store the information whether the cell has been visited
    map<int,int> pathMap;

    //initialize the 2D array
    for(int i = 0; i < mazeWidth; i++)
    {
        visited.push_back(vector<bool>());
        for(int j = 0; j < mazeHeight; j++)
        {
            visited[i].push_back(false);
        }
    }

    //stack to store the neighbors
    stack<int> stack;

    //push the entry cell
    stack.push(0);

    //and mark it as visited
    visited[0][0]=true;

    //while there's cells are able to move in the maze in the stack
    while(!stack.empty())
    {
        //get the location of the current cell
        int currCell = stack.top();
        int cX = getCellX(currCell);
        int cY = getCellY(currCell);

        //take it out
        stack.pop();

        //loop through the four direction, to see whether the current cell can move to its neighbours
        for(int direction=0; direction<4; direction++)
        {
            //its neighbours location
            int nextCell = getNextCell(cX, cY, direction);
            int nextX = getCellX(nextCell);
            int nextY = getCellY(nextCell);

            //to see whether the current cell can move to the neighbours
            if(canTravel(cX, cY, direction) && visited[nextX][nextY] == false)
            {
                visited[nextX][nextY]=true;
                stack.push(nextCell);
                pathMap[nextCell] = currCell;
            }
        }
    }

    //to find the longest path between the entry and the cells in the bottom location
    //the cell in the bottom in the longest path will later be set as the exit!
    int max = 0;
    vector<int> longest;
    vector<int> temp;

    //look through all the cells in the bottom, and to find the longest path
    for(int i = 0; i < mazeWidth; i++)
    {
        temp.clear(); // clear out the info in the previous path
        int count = 0;
        int end = mazeWidth*(mazeHeight-1) + i;

        //while the cell we are analysing is still able to move to the neightbours
        while(end!=0)
        {
            //find out the difference of their cells' location
            int difference = end - pathMap[end];

            //push back the direction to the vector
            //so that we can make a path go from the entry to the exit
            if(difference == 1)
            {
                temp.push_back(RIGHT);
            }
            else if(difference == mazeWidth)
            {
                temp.push_back(BOTTOM);
            }
            else if(difference == -1)
            {
                temp.push_back(LEFT);
            }
            else if(difference == -mazeWidth)
            {
                temp.push_back(UP);
            }
            //make end equal the next cell, so that it can do the same thing
            end = pathMap[end];
            //increase the length of the path
            count++;
        }

        //update the longest path if there is one
        if(count > max)
        {
            max = count;
            longest = temp;
        }
    }

    //This is important! we have to reserve it
    //so the set of directions can guide us find a path from the entry to the exit
    reverse(longest.begin(),longest.end());
    return longest;
}

/**
 * draw the maze with the solution path from the entry to the exit
 * @return the PNG which has drawed the maze with the solution path from the entry to the exit
 */
PNG* Maze::drawMazeWithSolution()
{
    //make the maze
    PNG *maze = drawMaze();
    //get the solution path of the above maze
    vector<int> solution = solveMaze();

    //the location of the entry
    unsigned int x = 5;
    unsigned int y = 5;

    //draw the path in red
    for(size_t i = 0; i < solution.size(); i++)
    {
        //draw in red for the right direction
        if(solution[i]==RIGHT)
        {
            for(int j = 0; j < 10; j++)
            {
                HSLAPixel* pixelPointer = maze->getPixel(x,y);
                pixelPointer->h = 0;
                pixelPointer->s = 1;
                pixelPointer->l = 0.5;
                x++;
            }
        }

        //draw in red for the bottom direction
        if(solution[i]==BOTTOM)
        {
            for(int j = 0; j < 10; j++)
            {
                HSLAPixel* pixelPointer = maze->getPixel(x,y);
                pixelPointer->h = 0;
                pixelPointer->s = 1;
                pixelPointer->l = 0.5;
                y++;
            }
        }

        //draw in red for the left direction
        if(solution[i]==LEFT)
        {
            for(int j = 0; j < 10; j++)
            {
                HSLAPixel* pixelPointer = maze->getPixel(x,y);
                pixelPointer->h = 0;
                pixelPointer->s = 1;
                pixelPointer->l = 0.5;
                x--;
            }
        }

        //draw in red for the up direction
        if(solution[i]==UP)
        {
            for(int j = 0; j < 10; j++)
            {
                HSLAPixel* pixelPointer = maze->getPixel(x,y);
                pixelPointer->h = 0;
                pixelPointer->s = 1;
                pixelPointer->l = 0.5;
                y--;
            }
        }
    }

    //draw the exit
    //we draw it in white. So, it seems like we take off the wall
    //And therefore, it will become the exit
    x=x-4;
    y=y+5;
    for(int i = 1; i <=9; i++)
    {
        HSLAPixel* pixelPointer = maze->getPixel(x,y);
        pixelPointer->h = 0;
        pixelPointer->s = 0;
        pixelPointer->l = 1;
        x++;
    }

    return maze;

}

/**
 * This is the helper function to get the index of the given cell
 * @param x the x coordinate of the given cell
 * @param y the y coordinate of the given cell
 * @return the index of the given cell
 */
int Maze::getCellIndex(int x, int y) const
{
    return y * mazeWidth + x;
}

/**
 * This is the helper function to get the x coordinate of the given cell
 * @param cellIndex the index of the given cell
 * @return the x coordinate of the given cell
 */
int Maze::getCellX (int cellIndex)
{
    return cellIndex % mazeWidth;
}

/**
 * This is the helper function to get the y coordinate of the given cell
 * @param cellIndex the index of the given cell
 * @return the y coordinate of the given cell
 */
int Maze::getCellY (int cellIndex)
{
    return cellIndex / mazeWidth;
}

/**
 * This is the helper function to get the next cell of the current cell in the given direction
 * @param x the x coordinate of the given cell
 * @param y the y coordinate of the given cell
 * @param dir the given direction
 * @return the location of the next cell
 */
int Maze::getNextCell( int x, int y, int dir )
{
    switch(dir)
    {
        case BOTTOM:
            return getCellIndex(x, y) + mazeWidth;
        case UP:
            return getCellIndex(x, y) - mazeWidth;
        case RIGHT:
            return getCellIndex(x, y) + 1;
        case LEFT:
            return getCellIndex(x, y) - 1;
        default: return 0;
    }
}