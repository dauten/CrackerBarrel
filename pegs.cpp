//#include "stdafx.h" //for Visual Studio
/*
* Author: Dale Auten
* DateMod:2/10/2019
* Desc:   Peg hopping game naive solution.
*         DFS solves all given inputs
*         BFS solves all but diamond and solitaire (runs out of memory)
*         Some pieces of code modifed from 8-tile game given.
* DueDate:2/14/2019
*
* Notes:  This is assuming you run the exe put in the Debug folder.
*		  Relative filenames may not work when run from debugger unless they're added to the project
*/

#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>
#include <set>


int abs(int a, int b){
  if(a > b)
	return a-b;
  else
	return b-a;
}

/*
* Returns score of a board state by calculating the manhatten distance
* From each peg to ever other peg
*/
int manhattenScore(int board[][9]){
  int pegs = 0;
  int man = 0;
  for(int x = 0; x < 9; x++){
    for(int y = 0; y < 9; y++){
      if(board[x][y] == 1){
        pegs++;
        for(int a = 0; a < 9; a++){
          for(int b = 0; b < 9; b++){
            man += abs(x,a) + abs(y,b);
          }
        }
      }
    }
  }
  return (man / (pegs * 2))*170;
}


/*
* data structure for backtracking, from 8-tile
*/
struct node {
	int b[9][9];
	node *parent;
	node *next;
  int fv, hv, gv;

	//ctor
	node(int sm[][9], node* p = NULL, node* n = NULL)
	{
		for (int r = 0; r<9; r++)
			for (int c = 0; c<9; c++)
				b[r][c] = sm[r][c];

		parent = p;
		next = n;
	}
};
typedef node* nodeP;

struct LE{
	bool operator()( node*  lhs, node* rhs) const{
		return lhs->fv > rhs->fv; // use > for less than since priority queue is a max heap
	}
};

/*
* given a source peg, a board, and a direction, that peg
* jumps in the direction encoded if that is legal.
* if legal, move is executed and 1 is returned.
* if illegal, no action is taken and 0 is returned.
*/
int jump(int x, int y, int board[][9], int dir, int out[][9]) {
	//dir; up = 0, right = 1, down = 2, left = 3
	int targ_x, targ_y, victum_x, victum_y;

	//walls can't jump
	if (board[x][y] == 2)
		return 0;

	//use direction and jumping peg to find where peg goes
	//and which peg is being jumped over
	if (dir == 0)
	{
		targ_x = x;
		targ_y = y - 2;
		victum_x = x;
		victum_y = y - 1;
	}
	else if (dir == 1)
	{
		targ_x = x + 2;
		targ_y = y;
		victum_x = x + 1;
		victum_y = y;
	}
	else if (dir == 2)
	{
		targ_x = x;
		targ_y = y + 2;
		victum_x = x;
		victum_y = y + 1;
	}
	else if (dir == 3)
	{
		targ_x = x - 2;
		targ_y = y;
		victum_x = x - 1;
		victum_y = y;
	}

	//we don't want to mutate original board but rather something passed by user
	for (int a = 0; a < 9; a++) {
		for (int b = 0; b < 9; b++) {
			out[a][b] = board[a][b];
		}
	}

	//make sure we aren't going OOB
	if (targ_x < 0 || targ_x > 6 || targ_y < 0 || targ_y > 6)
		return 0;

	//action to be taken determined, now we determine if its possible
	if (board[x][y] == 1 && board[targ_x][targ_y] == 0 && board[victum_x][victum_y] == 1) {
		//then we can jump
		out[x][y] = out[victum_x][victum_y] = 0;
		out[targ_x][targ_y] = 1;

		return 1;
	}

	return 0;
}

/*
* Checks if there is only one peg remaining
*/
int isBeat(int board[][9]) {
	int i = 0; // one count
	for (int a = 0; a < 9; a++) {
		for (int b = 0; b < 9; b++) {
			i += (board[b][a] == 1);
		}
	}
	return i == 1;
}

/*
* Backtracks through nodes to get path
*/
void printsolution(node* n) {
	// set up next for printing
	//each node has a parent but no child,
	//so set each node's parent's child equal to original node
	//this way we can start at original state and go down descendents
	while (n->parent)
	{
		n->parent->next = n;
		n = n->parent;
	}

	//n is set to original state
	int count = -1;
	while (n)
	{
		//go through all children, printing them out
		count++;
		printf("\n");
		for (int r = 0; r<9; r++)
		{
			for (int c = 0; c<9; c++)
				if (n->b[c][r] == 1)
					printf("%d ", n->b[c][r]);
				else if (n->b[c][r])
					printf("  ");
				else
					printf("0 ");
			printf("\n");
		}
		n = n->next;
	}
	printf("Optimal solution has %d steps.\n", count);
}


/*
* Encodes state as a number, similar logic to how chmod on Unix
* works but exapanded to include walls and 49 locations
*/
unsigned long long toNumber(int plain[9][9]) {
	unsigned long long cipher = 0;
	//while it would be very dumb to keep the array
	//in a numeric format, we can encode it for
	//comparative purposes.  There are 3^49 states
	int c = 1;
	for (int x = 0; x < 9; x++) {
		for (int y = 0; y < 9; y++) {
			cipher += c * (1 == plain[x][y]);
			c *= 3;
		}
	}
	return cipher;
}

/*
* Modified from 8-tile game
*/
void dfs(int sm[][9]) {
	//declarations
	std::stack<nodeP> open;
	nodeP *np;
	np = new nodeP[200000];
	int npCount = 0;
	std::set<unsigned long long> close;
	node *start, *current, *succ;
	long sucnum;
	start = new node(sm);
	int temp[9][9], success = 0;

	//add start state to set and stack
	open.push(start);
	np[npCount++] = start;
	close.insert(toNumber(start->b));
	long gencount = 1;
	while (!open.empty() && !success)
	{
		current = open.top();
		open.pop();

		if (isBeat(current->b))
		{
			printsolution(current);
			printf("Solution Found.  Total of %d nodes examined.\n\n", gencount);

			success = 1;
		}
		else  //if we can keep going
		{
			//for every possible peg...
			for (int a = 0; a < 9; a++) {
				for (int b = 0; b < 9; b++) {
					//...see if we can jump in every direction...
					if (jump(b, a, current->b, 0, temp) == 1) {
						//...and for each direction we can jump in
						sucnum = toNumber(temp);
						if (close.find(sucnum) == close.end()) {
							//...add that jump to stack if it hasn't already been seen
							succ = new node(temp, current);
							close.insert(sucnum);
							open.push(succ);
							np[npCount++] = succ;
							gencount++;
						}
					}
					if (jump(b, a, current->b, 1, temp) == 1) {
						sucnum = toNumber(temp);
						if (close.find(sucnum) == close.end()) {
							succ = new node(temp, current);
							close.insert(sucnum);
							open.push(succ);
							np[npCount++] = succ;
							gencount++;

						}
					}
					if (jump(b, a, current->b, 2, temp) == 1) {
						sucnum = toNumber(temp);
						if (close.find(sucnum) == close.end()) {
							succ = new node(temp, current);
							close.insert(sucnum);
							open.push(succ);
							np[npCount++] = succ;
							gencount++;

						}
					}
					if (jump(b, a, current->b, 3, temp) == 1) {
						sucnum = toNumber(temp);
						if (close.find(sucnum) == close.end()) {
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

	  //we either ended because we saw every state or found Solution
	  //if we found solution, print it.  then clear memory and exit
	if (!success)
	{
		printf("No solution.\n");
		printf("Total of %d nodes examined.\n\n", gencount);
	}

	for (int j = 0; j<npCount; j++)
		delete np[j];

	delete[] np;
}

/*
* Modified from 8-tile game
*/
void bfs(int sm[][9]) {
	//declarations
	std::queue<nodeP> open;
	nodeP *np;
	np = new nodeP[200000];
	int npCount = 0;
	std::set<unsigned long long> close;
	node *start, *current, *succ;
	long sucnum;
	start = new node(sm);

	int temp[9][9], success = 0;

	//add to queue
	open.push(start);
	np[npCount++] = start;
	close.insert(toNumber(start->b));
	long gencount = 1;

	//while unbeaten states remain...
	while (!open.empty() && !success)
	{
		current = open.front();
		open.pop();

		if (isBeat(current->b))
		{
			printsolution(current);
			printf("Solution Found.  Total of %d nodes examined.\n\n", gencount);
			success = 1;
		}
		else
		{
			//...for each peg see if we can jump in each direction
			for (int a = 0; a < 9; a++) {
				for (int b = 0; b < 9; b++) {
					if (jump(b, a, current->b, 0, temp) == 1) {
						sucnum = toNumber(temp);
						//for each unvisited jump we can make, enqueue it
						if (close.find(sucnum) == close.end()) {
							succ = new node(temp, current);
							close.insert(sucnum);
							open.push(succ);
							np[npCount++] = succ;
							gencount++;
						}
					}
					if (jump(b, a, current->b, 1, temp) == 1) {
						sucnum = toNumber(temp);
						if (close.find(sucnum) == close.end()) {
							succ = new node(temp, current);
							close.insert(sucnum);
							open.push(succ);
							np[npCount++] = succ;
							gencount++;

						}
					}
					if (jump(b, a, current->b, 2, temp) == 1) {
						sucnum = toNumber(temp);
						if (close.find(sucnum) == close.end()) {
							succ = new node(temp, current);
							close.insert(sucnum);
							open.push(succ);
							np[npCount++] = succ;
							gencount++;

						}
					}
					if (jump(b, a, current->b, 3, temp) == 1) {
						sucnum = toNumber(temp);
						if (close.find(sucnum) == close.end()) {
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
		printf("No solution.\n");
		printf("Total of %d nodes examined.\n\n", gencount);
	}

	for (int j = 0; j<npCount; j++)
		delete np[j];

	delete[] np;
}

/*
* Modified from 8-tile game
*/
void astar(int sm[][9]) {
	//declarations
	std::priority_queue<node*, std::vector<node*>, LE> open;
	nodeP *np;
	np = new nodeP[200000];
	int npCount = 0;
	std::set<unsigned long long> close;
	node *start, *current, *succ;
	long sucnum;
	start = new node(sm);
  start->gv = 0;
	int temp[9][9], success = 0;

	//add to queue
	open.push(start);
	np[npCount++] = start;
	close.insert(toNumber(start->b));
	long gencount = 1;

	//while unbeaten states remain...
	while (!open.empty() && !success)
	{
		current = open.top();
		open.pop();

		if (isBeat(current->b))
		{
			printsolution(current);
			printf("Solution Found.  Total of %d nodes examined.\n\n", gencount);
			success = 1;
		}
		else
		{
      int fv, gv, hv;
			//...for each peg see if we can jump in each direction
			for (int a = 0; a < 9; a++) {
				for (int b = 0; b < 9; b++) {
					if (jump(b, a, current->b, 0, temp) == 1) {
						sucnum = toNumber(temp);
						//for each unvisited jump we can make, enqueue it
						if (close.find(sucnum) == close.end()) {
							succ = new node(temp, current);
							close.insert(sucnum);

              succ->hv = manhattenScore(temp);
              succ->gv = current->gv + 1;
              succ->fv = succ->hv+succ->gv;

							open.push(succ);
							np[npCount++] = succ;
							gencount++;
						}
					}
					if (jump(b, a, current->b, 1, temp) == 1) {
						sucnum = toNumber(temp);
						if (close.find(sucnum) == close.end()) {
							succ = new node(temp, current);
							close.insert(sucnum);

              succ->hv = manhattenScore(temp);
              succ->gv = current->gv + 1;
              succ->fv = succ->hv+succ->gv;

							open.push(succ);
							np[npCount++] = succ;
							gencount++;

						}
					}
					if (jump(b, a, current->b, 2, temp) == 1) {
						sucnum = toNumber(temp);
						if (close.find(sucnum) == close.end()) {
							succ = new node(temp, current);
							close.insert(sucnum);

              succ->hv = manhattenScore(temp);
              succ->gv = current->gv + 1;
              succ->fv = succ->hv+succ->gv;

							open.push(succ);
							np[npCount++] = succ;
							gencount++;

						}
					}
					if (jump(b, a, current->b, 3, temp) == 1) {
						sucnum = toNumber(temp);
						if (close.find(sucnum) == close.end()) {
							succ = new node(temp, current);
							close.insert(sucnum);

              succ->hv = manhattenScore(temp);
              succ->gv = current->gv + 1;
              succ->fv = succ->hv+succ->gv;

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
		printf("No solution.\n");
		printf("Total of %d nodes examined.\n\n", gencount);
	}

	for (int j = 0; j<npCount; j++)
		delete np[j];

	delete[] np;
}



/*
* File IO and main execution loop goes here
*/
int main() {


	//begin
	std::string input;
	int board[9][9];
	std::string line;


	while (true) {
		//get filename and exit if neccessary
		printf("Enter shape (without .in), or Q/Quit to exit\n");
		std::cin >> input;
		if (input == "Q" || input == "Quit")
			return 0;
		std::string  file = input + ".in";
		int x = 0;
		int y = 0;
		int broke = 1;

		//otherwise open and read file
		std::ifstream infile(file.c_str());
		if (infile.is_open()) {

			//read on char at a time, until EOF
			char temp = infile.get();

			while (temp > 0) {
				//read line #x...
				//we track spaces.  one space=ignore, two spaces=that's a wall
				if (temp == 32) {
					broke++;
					if (broke % 2 == 1) {
						board[x][y] = 2;
						x++;
            printf("2.");
					}
				}
				//when we get a zero or one add that to array
				if (temp == 48) {
					board[x][y] = 0;
					broke = 1;
					x++;
          printf("0 ");
				}
				else if (temp == 49) {
					board[x][y] = 1;
					broke = 1;
					x++;
          printf("1 ");
				}
				//when we get a newline, fill rest of line with wall then
				//go to next line
        //this must be 13 on linux, 10 on windows
				else if (temp == 13) {
					while (x < 9 ) {
						board[x][y] = 2;
						broke = 1;
						x++;
            printf("2 ");

					}

					x = 0;
					broke = 1;
					y++;
          printf("\n");
				}
				else if (x > 8) {
					x = 0;
					broke = 1;
					y++;
				}

				temp = infile.get();
			}
			while (x < 9 && y < 9) {
				board[x][y] = 2;
				broke = 1;
				x++;
        printf("2!");
			}
		}
		else {
			printf("Failed to open file\n");
		}
    printf("\nx is %d and y is %d\n", x, y);
    while(y < 9){
      //then fill rest of board with 2.
      while (x < 9) {
        board[x][y] = 2;
        broke = 1;
        x++;
      }
      y++;
      x=0;
    }

		infile.close();

    for(int i = 0; i < 9; i++){
      for(int y = 0; y < 9; y++){
        printf("%d ", board[y][i]);
      }
      printf("\n");
    }
		//execute the algorithms and begin loop anew
		//printf("\nNow DFSing:\n");
		//dfs(board);
		//printf("\nNow BFSing:\n");
		//bfs(board);
    printf("\nSolving with A* heuristic:\n");
    astar(board);

	}
	return 0;
}
