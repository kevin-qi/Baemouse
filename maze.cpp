#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

const int height = 16;
const int width = 16;
const int dead_end = height * width;
const int queue_len = height * width;
int queue[queue_len][2];
const int start_r = height - 1;
const int start_c = 0;
const int start_dir = 0; // 0 - up; 1 - right; 2 - down; 3 - left
const int adj[4][2]={{-1,0}, {0,1}, {1,0}, {0,-1}};

// struct cell {
// 	bool top, bot, left, right;
// };
int dis[height][width];
bool maze[height][width][4];

void init() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			dis[i][j] = std::min(abs(height / 2 - i), abs(height / 2 - 1 - i))
						 + std::min(abs(width / 2 - j), abs(width / 2 - 1 - j));
			// printf("%4d ", dis[i][j]);
		}
		// printf("\n");
	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			for (int k = 0; k < 4; k++) {
				maze[i][j][k] = 0;
			}
		}
	}
	for (int i = 0; i < height; i++) {     // Handle left and right borders
		maze[i][0][3] = true;
		maze[i][width - 1][1] = true;
	}
	for (int j = 0; j < width; j++) {     // Handle top and bottom borders
		maze[0][j][0] = true;
		maze[height - 1][j][2] = true;
	}
}

void detect(int row, int col, int dir) {
	// --retrieve data from sensor. True for wall.
	bool wall_front, wall_left, wall_right;

	maze[row][col][dir] = maze[row][col][dir] || wall_front;
	maze[row][col][(dir + 3) % 4] = wall_left;
	maze[row][col][(dir + 1) % 4] = wall_right;

	// Handle adjacent cells' borders
	if (row > 0) maze[row-1][col][2] = maze[row-1][col][2] || maze[row][col][0];
	if (col < width-1) maze[row][col+1][3] = maze[row][col+1][3] || maze[row][col][1];
	if (row < height-1) maze[row+1][col][0] = maze[row+1][col][0] || maze[row][col][2];
	if (col > 0) maze[row][col-1][1] = maze[row][col-1][1] || maze[row][col][3];
}

bool in_range(int row, int col) {
	return (row >= 0 && row < height && col >= 0 && col < width);
}
void update_dis() {  // BFS from the center to update dis[][] with the known walls
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			dis[i][j] = dead_end;
		}
	}
	dis[height/2][width/2] = 0;
	dis[height/2-1][width/2] = 0;
	dis[height/2][width/2-1] = 0;
	dis[height/2-1][width/2-1] = 0;
	queue[0][0] = height/2;    queue[0][1] = width/2;
	queue[1][0] = height/2-1;  queue[1][1] = width/2;
	queue[2][0] = height/2;    queue[2][1] = width/2-1;
	queue[3][0] = height/2-1;  queue[3][1] = width/2-1;
	int front = 0, rear = 3, num_in_queue = 4;
	while (num_in_queue > 0) {
		int row = queue[front][0], col = queue[front][1];

		for (int cell = 0; cell < 4; cell++) {
			int new_row = row + adj[cell][0];
			int new_col = col + adj[cell][1];
			if (in_range(new_row, new_col)) {
				if (dis[row][col] + 1 < dis[new_row][new_col]) {
					dis[new_row][new_col] = dis[row][col] + 1;
					num_in_queue++;
					rear = (rear + 1) % queue_len;
					queue[rear][0] = new_row;
					queue[rear][1] = new_col;
				}
			}
		}
		num_in_queue--;
		front = (front + 1) % queue_len;
	}
}

int determine_dir(int row, int col) {
	if (row > 0 && !maze[row][col][0] && dis[row-1][col] == dis[row][col]-1)
		return 0;
	if (col < width-1 && !maze[row][col][1] && dis[row][col+1] == dis[row][col]-1)
		return 1;
	if (row < height-1 && !maze[row][col][2] && dis[row+1][col] == dis[row][col]-1)
		return 2;
	if (col > 0 && !maze[row][col][3] && dis[row][col-1] == dis[row][col]-1)
		return 3;
	return -1;    // Stop. Should never reach this statement.
}

void turn(int dir, int new_dir) {
	if (dir == new_dir) {
		// --Go straight
	} else if (new_dir == (dir+1)%4) {
		// Turn right
	} else if (new_dir == (dir+2)%4) {
		// Turn back
	} else {
		// Turn left
	}
}

void move() {
	// --Move forward for 1 unit.
}

void explore(int row, int col, int dir) {
	detect(row, col, dir);
	update_dis();
	int new_dir = determine_dir(row, col);
	turn(dir, new_dir);
	move();
}

int main() {
	init();
	explore(start_r, start_c, start_dir);
}