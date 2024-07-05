#include <iostream>
#include <vector>
#include <string>

#define ROW 3
#define COL 3

using namespace std;

void displayBoard(char board[ROW][COL]){
    for(int i=0; i < ROW; i++){
        for(int j=0; j < COL; j++){
            cout << board[i][j] << "| ";
        }
        cout << endl;
    }
}

bool checkWin(char board[ROW][COL], char player1) {
    for(int i = 0; i < 3; i++)
    {
        if(board[i][0] == player1 && board[i][1] == player1 && board[i][2] == player1)
        {
            return true;
        }
        else if(board[0][i] == player1 && board[1][i] == player1 && board[2][i] == player1)
        {
            return true;
        }
        else if(board[0][0] == player1 && board[1][1] == player1 && board[2][2] == player1)
        {
            return true;
        }
        else if(board[0][2] == player1 && board[1][1] == player1 && board[2][0] == player1)
        {
            return true;
        }
    }
    return false;
}

int suggestion(char board[ROW][COL], char player1) {
    for(int i = 0; i < 3; i++)
    {
        if(board[i][0] == player1 && board[i][1] == player1 && board[i][2] == ' ')
        {
            cout << "You can win by placing at row " << i << " column 2 ";
            return 0;
        }
        else if(board[i][0] == player1 && board[i][2] == player1 && board[i][1] == ' ')
        {
            cout << "You can win by placing at row " << i << " column 1 ";
            return 0;
        }
        else if(board[i][1] == player1 && board[i][2] == player1 && board[i][0] == ' ')
        {
            cout << "You can win by placing at row " << i << " column 0 ";
            return 0;
        }
        else if(board[0][i] == player1 && board[1][i] == player1 && board[2][i] == ' ')
        {
            cout << "You can win by placing at row 2 column " << i << " ";
            return 0;
        }
        else if(board[0][i] == player1 && board[2][i] == player1 && board[1][i] == ' ')
        {
            cout << "You can win by placing at row 1 column " << i << " ";
            return 0;
        }
        else if(board[1][i] == player1 && board[2][i] == player1 && board[0][i] == ' ')
        {
            cout << "You can win by placing at row 0 column " << i << " ";
            return 0;
        }
        else if(board[0][0] == player1 && board[1][1] == player1 && board[2][2] == ' ')
        {
            cout << "You can win by placing at row 2 column 2 ";
            return 0;
        }
        else if(board[0][0] == player1 && board[2][2] == player1 && board[1][1] == ' ')
        {
            cout << "You can win by placing at row 1 column 1 ";
            return 0;
        }
        else if(board[1][1] == player1 && board[2][2] == player1 && board[0][0] == ' ')
        {
            cout << "You can win by placing at row 0 column 0 ";
            return 0;
        }
        else if(board[0][2] == player1 && board[1][1] == player1 && board[2][0] == ' ')
        {
            cout << "You can win by placing at row 2 column 0 ";
            return 0;
        }
        else if(board[0][2] == player1 && board[2][0] == player1 && board[1][1] == ' ')
        {
            cout << "You can win by placing at row 1 column 1 ";
            return 0;
        }
        else if(board[1][1] == player1 && board[2][0] == player1 && board[0][2] == ' ')
        {
            cout << "You can win by placing at row 0 column 2 ";
            return 0;
        }
        else
        {
            cout << "You can't win in this move. You cann place at row 1 column 1 to win next time. ";
            return 0;
        }
    }
}

int main()
{
    char board[ROW][COL] = {{' ',' ',' '},{' ',' ',' '}, {' ',' ',' '}};
    char player1 = 'X';
    int row, col;
    int turn;
    bool win = false;
    int hint1 = 1;
    int hint2 = 1;

    for(turn = 0; turn < 9; turn++)
    {
        while (true)
        {
            cout << "Player " << player1 << ", enter your move (row[0-2] column[0-2])(For Hints enter '-1 -1'): ";
            cin >> row >> col;

            if(row == -1 && col == -1) {
                if(player1 == 'X') {
                    hint1 = hint1 - 1;
                    if(hint1 <= -1) {
                        cout << "You can't use hint anymore. ";
                    }
                    else {
                        suggestion(board, player1);
                    }
                }
                else {
                    hint2 = hint2 - 1;
                    if(hint2 <= -1) {
                        cout << "You can't use hint anymore. ";
                    }
                    else {
                        suggestion(board, player1);
                    }
                }
                continue;
            }
            else if(board[row][col] != ' ' || row < 0 || row > 2 || col < 0 || col > 2)
            {
                cout << "Invalid move, try again.";
                continue;
            }
            else
            {
                board[row][col] = player1;
                break;
            }
        }

        if(checkWin(board, player1))
        {
            cout << "Player " << player1 << " wins!";
            win = true;
            break;
        }

        displayBoard(board);

        player1 = (player1 == 'X') ? 'O' : 'X';
    }

    if (win == false)
    {
        cout << "It's a tie!";
    }
    
    return 0;

}