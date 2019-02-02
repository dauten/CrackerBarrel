#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>
#include <set>

struct node
{
  int b[7][7];
  node *parent;
  node *next;

  node (int sm[][7], node* p=NULL, node* n=NULL)
	{
    for (int r=0;r<7;r++)
		  for (int c=0;c<7;c++)
			   b[r][c]=sm[r][c];

    parent=p;
		next=n;
	}
};

int isBeat(int board[][7]){
  int i = 0; // one count
  for(int a = 0; a < 7; a++){
    for(int b = 0; b < 7; b++){
      i += (board[b][a] == 1);
      printf("%d\n", i);
    }
  }
  return i == 1;
}
unsigned long long toNumber(int plain[7][7]){
  unsigned long long cipher = 0;
  //while it would be very dumb to keep the array
  //in a numeric format, we can encode it for
  //comparative purposes.  There are 2^49 states
  int c = 1;
  for(int x = 0; x < 7; x++){
    for(int y = 0; y < 7; y++){
      cipher += c*(1==plain[x][y]);
      c *= 2;
    }
  }
  return cipher;
}

void dfs(int board[][7]){
  std::stack<node*> open;
  int npC = 0;
  std::set<unsigned long long> visited;
  node *current = (new node(board));
  open.push(current);

  visited.insert(toNumber(board));
  int success = 0;
  while(!open.empty() && !success){
    current = open.top();
    open.pop();
    if(isBeat(current->b)){
        success = 1;
    }
    else{
      //    get all possible child states given possible moves
      //    each one that is not is set, enqueue and add to set
      //
    }
  }

  //insert start board into stack
  //while solution is not found and stack is not empty...
  //    look at top of stack
  //    get all possible child states given possible moves
  //    each one that is not is set, enqueue and add to set
  //
}

/*
* given a source peg, a board, and a direction, that peg
* jumps in the direction encoded if that is legal.
* if legal, move is executed and 1 is returned.
* if illegal, no action is taken and 0 is returned.
*/
int jump(int x, int y, int board[][7], int dir){
  //dir; up = 0, right = 1, down = 2, left = 3
  int targ_x, targ_y, victum_x, victum_y;
  if(dir == 0)
  {
    targ_x = x;
    targ_y = y - 2;
    victum_x = x;
    victum_y = y - 1;
  }
  else if(dir == 1)
  {
    targ_x = x + 2;
    targ_y = y;
    victum_x = x + 1;
    victum_y = y;
  }
  else if(dir == 2)
  {
    targ_x = x;
    targ_y = y - 2;
    victum_x = x;
    victum_y = y - 1;
  }
  else if(dir == 0)
  {
    targ_x = x - 2;
    targ_y = y;
    victum_x = x - 1;
    victum_y = y;
  }

  if(targ_x < 0 || targ_x > 6 || targ_y < 0 || targ_y > 6)
    return 0;

  //action to be taken determined, now we determine if its possible
  if(board[x][y] == 1 && board[targ_x][targ_y] == 0 && board[victum_x][victum_y] == 1){
    //then we can jump
    board[x][y] = board[victum_x][victum_y] = 0;
    board[targ_x][targ_y] = 1;
    return 1;
  }
  return 0;
}

int main(){

  //begin
  std::cout << "Enter Source Filename:" << '\n';
  std::string file;
  std::cin >> file;
  std::ifstream infile(file);
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
      printf("%d ", board[b][a]);
    }
    printf("\n");
  }
  int i = jump(3, 1, board, 0);
  printf("%d\n", i);
  for(int a = 0; a < 7; a++){
    for(int b = 0; b < 7; b++){
      printf("%d ", board[b][a]);
    }
    printf("\n");
  }
  return 0;
}
