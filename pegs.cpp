#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>
#include <set>

struct node{
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
typedef node* nodeP;

/*
* given a source peg, a board, and a direction, that peg
* jumps in the direction encoded if that is legal.
* if legal, move is executed and 1 is returned.
* if illegal, no action is taken and 0 is returned.
*/
int jump(int x, int y, int board[][7], int dir, int out[][7]){
  //dir; up = 0, right = 1, down = 2, left = 3
  int targ_x, targ_y, victum_x, victum_y;

  if(board[x][y] == 2)
    return 0;


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
    targ_y = y + 2;
    victum_x = x;
    victum_y = y + 1;
  }
  else if(dir == 3)
  {
    targ_x = x - 2;
    targ_y = y;
    victum_x = x - 1;
    victum_y = y;
  }
  for(int a = 0; a < 7; a++){
    for(int b = 0; b < 7; b++){
      out[a][b] = board[a][b];
    }
  }
  if(targ_x < 0 || targ_x > 6 || targ_y < 0 || targ_y > 6)
    return 0;

  //action to be taken determined, now we determine if its possible
//  printf("%d,%d\n", x, y);
  if(board[x][y] == 1 && board[targ_x][targ_y] == 0 && board[victum_x][victum_y] == 1){
    //then we can jump
    out[x][y] = out[victum_x][victum_y] = 0;
    out[targ_x][targ_y] = 1;

    return 1;
  }

  return 0;
}

int isBeat(int board[][7]){
  int i = 0; // one count
  for(int a = 0; a < 7; a++){
    for(int b = 0; b < 7; b++){
      i += (board[b][a] == 1);
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
      c *= 3;
    }
  }
  return cipher;
}

void dfs(int sm[][7]){
  std::stack<nodeP> open;
	nodeP *np;
	np = new nodeP[200000];
	int npCount = 0;
	std::set<unsigned long long> close;
	node *start, *current, *succ;
	long sucnum;
	start = new node(sm);
						  //	cout << start->parent << endl;
	int temp[7][7], success = 0;

	open.push(start);
	np[npCount++] = start;
	close.insert(toNumber(start->b));
	long gencount = 1;
	//	cout << getnumber(start->m) << endl;
	while (!open.empty() && !success)
	{
    current = open.top();
		open.pop();

		//		cout << getnumber(current->m) << endl;
		if (isBeat(current->b))
		{
			std::cout << "Solution Found.  Total of " << gencount
				<< " nodes examined.\n\n";
			success = 1;
		}
		else
		{
      for(int a = 0; a < 9; a++){
        for(int b = 0; b < 9; b++){
          if(jump(b, a, current->b, 0, temp) == 1){
            sucnum = toNumber(temp);
            if(close.find(sucnum) == close.end()){
              succ = new node(temp, current);
              close.insert(sucnum);
              open.push(succ);
              np[npCount++] = succ;
              gencount++;

            }

          }
          if(jump(b, a, current->b, 1, temp) == 1){
            sucnum = toNumber(temp);
            if(close.find(sucnum) == close.end()){
              succ = new node(temp, current);
              close.insert(sucnum);
              open.push(succ);
              np[npCount++] = succ;
              gencount++;

            }
          }
          if(jump(b, a, current->b, 2, temp) == 1){
            sucnum = toNumber(temp);
            if(close.find(sucnum) == close.end()){
              succ = new node(temp, current);
              close.insert(sucnum);
              open.push(succ);
              np[npCount++] = succ;
              gencount++;

            }
          }
          if(jump(b, a, current->b, 3, temp) == 1){
            sucnum = toNumber(temp);
            if(close.find(sucnum) == close.end()){
              succ = new node(temp, current);
              close.insert(sucnum);
              open.push(succ);
              np[npCount++] = succ;
              gencount++;
            }
          }
        }
      }
		}
	} // end of while

	if (!success)
	{
		std::cout << "No solution.\n";
		std::cout << "Total of " << gencount
			<< " nodes examined.\n\n";
	}

	for (int j = 0; j<npCount; j++)
		delete np[j];

	delete[] np;
}

void bfs(int sm[][7]){
  std::queue<nodeP> open;
	nodeP *np;
	np = new nodeP[200000];
	int npCount = 0;
	std::set<unsigned long long> close;
	node *start, *current, *succ;
	long sucnum;
	start = new node(sm);
						  //	cout << start->parent << endl;
	int temp[7][7], success = 0;

	open.push(start);
	np[npCount++] = start;
	close.insert(toNumber(start->b));
	long gencount = 1;
	//	cout << getnumber(start->m) << endl;
	while (!open.empty() && !success)
	{
    current = open.front();
		open.pop();

		//		cout << getnumber(current->m) << endl;
		if (isBeat(current->b))
		{
			std::cout << "Solution Found.  Total of " << gencount
				<< " nodes examined.\n\n";
			success = 1;
		}
		else
		{
      for(int a = 0; a < 9; a++){
        for(int b = 0; b < 9; b++){
          if(jump(b, a, current->b, 0, temp) == 1){
            sucnum = toNumber(temp);
            if(close.find(sucnum) == close.end()){
              succ = new node(temp, current);
              close.insert(sucnum);
              open.push(succ);
              np[npCount++] = succ;
              gencount++;
            }
          }
          if(jump(b, a, current->b, 1, temp) == 1){
            sucnum = toNumber(temp);
            if(close.find(sucnum) == close.end()){
              succ = new node(temp, current);
              close.insert(sucnum);
              open.push(succ);
              np[npCount++] = succ;
              gencount++;

            }
          }
          if(jump(b, a, current->b, 2, temp) == 1){
            sucnum = toNumber(temp);
            if(close.find(sucnum) == close.end()){
              succ = new node(temp, current);
              close.insert(sucnum);
              open.push(succ);
              np[npCount++] = succ;
              gencount++;

            }
          }
          if(jump(b, a, current->b, 3, temp) == 1){
            sucnum = toNumber(temp);
            if(close.find(sucnum) == close.end()){
              succ = new node(temp, current);
              close.insert(sucnum);
              open.push(succ);
              np[npCount++] = succ;
              gencount++;

            }
          }


        }
      }

		}
	} // end of while

	if (!success)
	{
		std::cout << "No solution.\n";
		std::cout << "Total of " << gencount
			<< " nodes examined.\n\n";
	}

	for (int j = 0; j<npCount; j++)
		delete np[j];

	delete[] np;
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


  printf("Now DFSing:\n");

  bfs(board);
  return 0;
}
