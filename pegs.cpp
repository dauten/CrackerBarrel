#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>


int main(){

  //begin
  std::ifstream infile("plus.in");
  int board[7][7];
  std::string line;
  int x = 0;
  int y = 0;
  int broke = 0;
  if(infile.is_open()){
      char temp = infile.get();
      while(temp > 0){
        //read line #x...
        if(temp == 32){
          broke++;
          if(broke % 2 == 1){
            board[x][y] = 2;
            x++;
          }
        }
        else broke = 0;
        if(temp == 48){
          board[x][y] = 0;
          broke = 1;
          x++;
        }
        else if(temp == 49){
          board[x][y] = 1;
          broke = 1;
          x++;
        }
        else if(temp == 13){
          while(x < 7){
            board[x][y] = 2;
            broke = 1;
            x++;
          }
          x = 0;
          broke = 1;
          y++;
        }
        else if(x > 6){
          x = 0;
          broke = 1;
          y++;
        }

        temp = infile.get();
      }
      while(x < 7){
        board[x][y] = 2;
        broke = 1;
        x++;
      }
  }

  for(int a = 0; a < 7; a++){
    for(int b = 0; b < 7; b++){
      printf("%d ", board[a][b]);
    }
    printf("\n");
  }

  return 0;
}
