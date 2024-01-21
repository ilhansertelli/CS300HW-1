// Ilhan Sertelli HW#1

#include <iostream>
#include <fstream>
#include <vector>
#include "stack.h"
#include "RandGen.h"
#include <string>
#include "strutils.h"
using namespace std;

struct Data { // The data to be stored in the stack
    int x_coor, y_coor, left, top, right, bottom;

    Data(int x, int y, int l, int t, int r, int b): x_coor(x), y_coor(y),
         left(l), top(t), right(r), bottom(b) {}
};

bool isUnique(const vector<vector<vector<Data>>> & mazes, const vector<vector<Data>> & maze, const int & rows, const int & cols);
bool contains(const vector<vector<Data>> & cells, const Data & cell);
vector<int> randomDirection(vector<vector<Data>> & mazeMatrix, vector<int> & direction, stack<Data> & maze, const int & rows, const int & cols);
int move(vector<vector<Data>> & cellMatrix, const vector<int> & direction, stack<Data> & maze, const int & rows, const int & cols);
bool isValid(const vector<vector<Data>> & cells, stack<Data> & maze, const int & step, const int & rows, const int & cols);
bool solveMaze(vector<vector<bool>> & visitMatrix, const int & direction, stack<Data> & maze, const int & rows, const int & cols);

// Function that checks if the created maze is unique
bool isUnique(const vector<vector<vector<Data>>> & mazes, const vector<vector<Data>> & maze, const int & rows, const int & cols) {
    if (mazes.size() == 0) // Meaning that it is the first maze
        return true;
    else {
        for (int i = 0; i < mazes.size(); i++) {
            for (int j = 0; j < cols; j++) {
                for (int k = 0; k < rows; k++) {
                    if ((mazes[i][k][j].left == maze[k][j].left) && (mazes[i][k][j].top == maze[k][j].top) &&  // Checks if the mazes are equal
                            (mazes[i][k][j].right == maze[k][j].right) && (mazes[i][k][j].right == maze[k][j].right)) {
                        return false; // If returns false, meaning that the created maze is not unique
                    }
                }
            }
        }
    }
    return true; // If returns true, meaning that the created maze is unique
}

// Function that checks whether the given cell has been visited or not
bool contains(const vector<vector<Data>> & cells, const Data & cell) {
    // Checks where there is at least one wall is broken in the cell or not (1 - not broken, 0 - broken)
    if (cells[cell.y_coor][cell.x_coor].top != 1 || cells[cell.y_coor][cell.x_coor].right != 1
    || cells[cell.y_coor][cell.x_coor].left != 1 || cells[cell.y_coor][cell.x_coor].bottom != 1)
        return true; // Visited
    return false; // Not visited
}

// Function that creates a vector of random directions starting from index 0 to 3 respectively
vector<int> randomDirection() {
  vector<int>   steps(4);
    steps = {}; //1-left, 2-top, 3-right, 4-bottom

    RandGen rnd;
    int step = rnd.RandInt(1,4); // Generating a random direction
    steps.push_back(step); // Adding the random direction to the vector
    int count = 1;

    while (count != 4) { // In order to iterate the loop 3 times since there are 3 more directions to add
        bool check = false;
        step = rnd.RandInt(1,4);
        for (int i = 0; i < steps.size(); i++) { // This loops avoids the duplicate number occurrences in the vector
            if (steps[i] == step) { // Checks if the randomly generated direction is already in the vector or not
                check = true;
                break;
            }
        }
        if (!check) { // Meaning that the generated number is not in the vector, so it is going to be added to the vector
            steps.push_back(step);
            count++;
        }
    }

    return steps; // Returning the randomly generated vector
}

// The function that adds new cells to the maze
int move(vector<vector<Data>> & cellMatrix, const vector<int> & direction, stack<Data> & maze, const int & rows, const int & cols) {

    int count = 0; // A counter that counts the invalid number of directions. If it equals to 4, it means that backtrack should be done
    for (int i = 0; i < 4; i++) {
        int currentX = maze.getTop().x_coor, currentY = maze.getTop().y_coor, curLeft = maze.getTop().left, curTop = maze.getTop().top, curRight = maze.getTop().right, curBottom = maze.getTop().bottom;
        if (isValid(cellMatrix, maze, direction[i], rows, cols)) { // Checks if the cell in the given direction is safe to add
            if (direction[i] == 1) { // Left
                Data oldCell(currentX, currentY, 0, curTop, curRight, curBottom);
                Data newCell(currentX - 1, currentY, 1, 1, 0, 1); // The next cell to be added to the stack with its right wall broken
                maze.changeObj(oldCell); // Since the left wall is broken in the current cell, the data should be changed
                cellMatrix[currentY][currentX] = oldCell; // After changing, the data is stored in the cell matrix
                maze.push(newCell); // Adding the new cell to the stack
                cellMatrix[currentY][currentX - 1] = newCell; // Updating the data in the cell matrix
            } else if (direction[i] == 2) { // Top
                Data oldCell(currentX, currentY, curLeft, 0, curRight, curBottom);
                Data newCell(currentX, currentY + 1, 1, 1, 1, 0); // The next cell to be added to the stack with its bottom wall broken
                maze.changeObj(oldCell);
                cellMatrix[currentY][currentX] = oldCell;
                maze.push(newCell);
                cellMatrix[currentY + 1][currentX] = newCell;
            } else if (direction[i] == 3) { // Right
                Data oldCell(currentX, currentY, curLeft, curTop, 0, curBottom);
                Data newCell(currentX + 1, currentY, 0, 1, 1, 1); // The next cell to be added to the stack with its left wall broken
                maze.changeObj(oldCell);
                cellMatrix[currentY][currentX] = oldCell;
                maze.push(newCell);
                cellMatrix[currentY][currentX + 1] = newCell;
            } else if (direction[i] == 4) { // Bottom
                Data oldCell(currentX, currentY, curLeft, curTop, curRight, 0);
                Data newCell(currentX, currentY - 1, 1, 0, 1, 1); // The next cell to be added to the stack with its top wall broken
                maze.changeObj(oldCell);
                cellMatrix[currentY][currentX] = oldCell;
                maze.push(newCell);
                cellMatrix[currentY - 1][currentX] = newCell;
            }
            break;
        }
        else // Meaning that the cell in the given direction is not a valid one to add
            count++;
    }
    return count;
}

// The function that checks the validity of the move in the given direction
bool isValid(const vector<vector<Data>> & cells, stack<Data> & maze, const int & step, const int & rows, const int & cols) { // SEGMENTATION YEDIM :(
    //1-left, 2-top, 3-right, 4-bottom
    int curX = maze.getTop().x_coor, curY = maze.getTop().y_coor;
    if (step == 1) {
        if (curX - 1 < 0) // Checks the movement causes an out-of-range issue
            return false;
        else {
            Data cell(curX - 1, curY, 1, 1, 1, 1);
            if (contains(cells, cell)) // Checks if the given cell is visited
                return false; // Visited
            else
                return true; // Not visited
        }
    }
    else if (step == 2) {
        if (curY + 1 >= rows) // Checks the movement causes an out-of-range issue
            return false;
        else {
            Data cell(curX, curY + 1, 1, 1, 1, 1);
            if (contains(cells, cell)) // Checks if the given cell is visited
                return false; // Visited
            else
                return true; // Not visited
        }
    }
    else if (step == 3) {
        if (curX + 1 >= cols) // Checks the movement causes an out-of-range issue
            return false;
        else {
            Data cell(curX + 1, curY, 1, 1, 1, 1);
            if (contains(cells, cell)) // Checks if the given cell is visited
                return false; // Visited
            else
                return true; // Not visited
        }
    }
    else if (step == 4) {
        if (curY - 1 < 0) // Checks the movement causes an out-of-range issue
            return false;
        else {
            Data cell(curX, curY - 1, 1, 1, 1, 1);
            if (contains(cells, cell)) // Checks if the given cell is visited
                return false; // Visited
            else
                return true; // Not visited
        }
    }
    return false;
}

// The function that makes a move validity check while creating the path
bool solveMaze(vector<vector<bool>> & visitMatrix, const int & direction, stack<Data> & maze, const int & rows, const int & cols) {
    int curX = maze.getTop().x_coor, curY = maze.getTop().y_coor;
    if (direction == 1) { // Left
        if (maze.getTop().left == 0) { // Meaning that there is no wall on the left side of the cell
            if (visitMatrix[curY][curX - 1]) // Checks if the given cell is visited via visitMatrix
                return false;
            else
                return true;
        }
        else // There is a wall in the given direction
            return false;
    }
    else if (direction == 2) { // Top
        if (maze.getTop().top == 0) { // Meaning that there is no wall on the left side of the cell
            if (visitMatrix[curY + 1][curX]) // Checks if the given cell is visited via visitMatrix
                return false;
            else
                return true;
        }
        else // There is a wall in the given direction
            return false;
    }
    else if (direction == 3) { // Right
        if (maze.getTop().right == 0) { // Meaning that there is no wall on the left side of the cell
            if (visitMatrix[curY][curX + 1]) // Checks if the given cell is visited via visitMatrix
                return false;
            else
                return true;
        }
        else // There is a wall in the given direction
            return false;
    }
    else if (direction == 4) { // Bottom
        if (maze.getTop().bottom == 0) { // Meaning that there is no wall on the left side of the cell
            if (visitMatrix[curY - 1][curX]) // Checks if the given cell is visited via visitMatrix
                return false; // Not visited
            else
                return true; // Visited
        }
        else // There is a wall in the given direction
            return false;
    }
    return false;
}

int main() {
    int mazeNum, ID, iterate = 1;
    long rows, columns, x_entry, y_entry, x_exit, y_exit;
    string fileName, line;
    Data cell(0, 0, 1, 1, 1, 1), cellCheck(0, 0, 1, 1, 1, 1); // If 1-> Unvisited, If 0-> Visited;
    ofstream outFile;

    cout << "Enter the number of mazes: ";
    cin >> mazeNum;
    cout << "Enter the number of rows and columns (M and N): ";
    cin >> rows >> columns;

    vector<vector<vector<Data>>> mazeList(mazeNum, vector<vector<Data>> (rows, vector<Data> (columns, cell))); // 2D vector to store the cell datas

    // Creating the vector that contains the maze matrices
    for (int k = 0; k < mazeNum; k++) {
        vector<vector<Data>> cellMatrix(rows, vector<Data>(columns, cell));
        for (int i = 0; i < columns; i++) {
            for (int j = 0; j < rows; j++) {
                Data newCell(i, j, 1, 1, 1, 1); // Creating the default cells (x=0, y=0, left=0, top=0, right=0, bottom=0)
                cellMatrix[j][i] = newCell; // Storing the cell in its coordinate index
            }
        }
    }

    // Creating the mazes one by one
    for (int x = 0; x < mazeNum; x++) {
        iterate = 1;
        do {
            stack<Data> maze; // The maze stack that holds the current cell and the data of the current cell
            maze.push(cell); // Adding (0, 0) cell to the maze
            int currentX = maze.getTop().x_coor, currentY = maze.getTop().y_coor;
            mazeList[x][currentY][currentX] = cell; // Updating the matrix data
            do {
                RandGen rnd;
                vector<int> direction = randomDirection(); // Creating the random direction vector
                while (move(mazeList[x], direction, maze, rows, columns) == 4) { // Backtrack if move function returns 4 (Meaning that all the directions are invalid)
                    maze.pop(); // Pop until a viable cell shows up
                    direction = randomDirection(); // Create a random route again
                }
                iterate++;
            } while (iterate < rows * columns); // Iterate the loop size of the maze times

            fileName = ("maze_" + itoa(x + 1) + ".txt"); // Constructing the file name with respect to the ID of the maze
            line = itoa(rows) + " " + itoa(columns); // First line that shows the dimensions of the maze
            outFile.open(fileName);
            outFile << line << endl; // Writes the first line
            // Writes the whole data stored in the cell vector
            for (int i = 0; i < columns; i++) {
                for (int j = 0; j < rows; j++) {
                    line = "x=" + itoa(mazeList[x][j][i].x_coor) + " " + "y=" + itoa(mazeList[x][j][i].y_coor) + " " +
                            "l=" + itoa(mazeList[x][j][i].left) + " " + "r=" + itoa(mazeList[x][j][i].right) + " " +
                            "u=" + itoa(mazeList[x][j][i].top) + " " + "d=" + itoa(mazeList[x][j][i].bottom);
                    outFile << line << endl;
                }
            }
            outFile.close();
        } while (isUnique(mazeList, mazeList[x], rows, columns)); // Avoiding duplicate mazes
    }
    // Writing the file

    cout << "All mazes are generated." << endl << endl;
    cout << "Enter a maze ID between 1 to " << mazeNum << " inclusive to find a path: ";
    cin >> ID;

    // Reading the file

    cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): " << endl;
    cin >> x_entry >> y_entry;
    cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
    cin >> x_exit >> y_exit;

    stack<Data> pathFinder; // The stack that will store the coordinates of the path one-by-one
    vector<vector<Data>> pathMatrix = mazeList[ID - 1];
    vector<vector<bool>> visited(rows, vector<bool> (columns, false)); // A matrix that stores the information if the given coordinate is visited or not
    pathFinder.push(mazeList[ID - 1][y_entry][x_entry]); // Adding the start cell to the stack
    visited[y_entry][x_entry] = true; // Since the start cell was added to the stack once, it is flagged as visited in the vector
    vector<int> direction = randomDirection(); // Creates a random route to find a path

    // Iterating the loop as long as the exit cell is not reached
    while (pathFinder.getTop().x_coor != mazeList[ID - 1][y_exit][x_exit].x_coor || pathFinder.getTop().y_coor != mazeList[ID - 1][y_exit][x_exit].y_coor) {
        int count = 0;
        int curX = pathFinder.getTop().x_coor, curY = pathFinder.getTop().y_coor, curL = pathFinder.getTop().left,
        curT = pathFinder.getTop().top, curR = pathFinder.getTop().right, curB = pathFinder.getTop().bottom;
        for (int i = 0; i < 4; i++) {
            if (solveMaze(visited, direction[i], pathFinder, rows, columns)) {  // Checks whether there is an available cell to visit
               if (direction[i] == 1) { // Left
                   Data oldCell(curX, curY, curL, curT, curR, curB);
                   Data newCell(curX - 1, curY, pathMatrix[curY][curX - 1].left, pathMatrix[curY][curX - 1].top, pathMatrix[curY][curX - 1].right, pathMatrix[curY][curX - 1].bottom);
                   pathFinder.push(newCell); // Adding the new cell to the path stack
                   visited[curY][curX - 1] = true; // Since the new cell is visited after the addition to the stack, it is flagged as true
               } else if (direction[i] == 2) { // Top
                   Data oldCell(curX, curY, curL, curT, curR, curB);
                   Data newCell(curX, curY + 1, pathMatrix[curY + 1][curX].left, pathMatrix[curY + 1][curX].top, pathMatrix[curY + 1][curX].right, pathMatrix[curY + 1][curX].bottom);
                   pathFinder.push(newCell); // Adding the new cell to the path stack
                   visited[curY + 1][curX] = true; // Since the new cell is visited after the addition to the stack, it is flagged as true
               } else if (direction[i] == 3) { // Right
                   Data oldCell(curX, curY, curL, curT, curR, curB);
                   Data newCell(curX + 1, curY, pathMatrix[curY][curX + 1].left, pathMatrix[curY][curX + 1].top, pathMatrix[curY][curX + 1].right, pathMatrix[curY][curX + 1].bottom);
                   pathFinder.push(newCell); // Adding the new cell to the path stack
                   visited[curY][curX + 1] = true; // Since the new cell is visited after the addition to the stack, it is flagged as true
               } else if (direction[i] == 4) { // Bottom
                   Data oldCell(curX, curY, curL, curT, curR, curB);
                   Data newCell(curX, curY - 1, pathMatrix[curY - 1][curX].left, pathMatrix[curY - 1][curX].top, pathMatrix[curY - 1][curX].right, pathMatrix[curY - 1][curX].bottom);
                   pathFinder.push(newCell); // Adding the new cell to the path stack
                   visited[curY - 1][curX] = true; // Since the new cell is visited after the addition to the stack, it is flagged as true
               }
               break;
            }
            else
                count++;
        }
        if (count == 4) { // Backtrack
            pathFinder.pop(); // Pop until an available cell to visit shows up
            direction = randomDirection(); // Creating a random direction again
        }
    }

    stack<Data> path;
    while (!pathFinder.isEmpty()) { // Constructing the maze path from (0, 0) to the exit
        Data pathElem = pathFinder.getTop();
        pathFinder.pop();
        path.push(pathElem);
    }

    string pathFile = "maze_" + itoa(ID) + "_path_" + itoa(x_entry) + "_" + itoa(y_entry) + "_" + itoa(x_exit) + "_" + itoa(y_exit) + ".txt"; // Constructing the path file name
    outFile.open(pathFile);
    // Printing the coordinates of the path line by line
    while (!path.isEmpty()) {
        outFile << itoa(path.getTop().x_coor) + " " + itoa(path.getTop().y_coor) << endl;
        path.pop();
    }

    outFile.close();

    return 0;
}
