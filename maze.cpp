#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

const int height = 16;
const int width = 16;
const int dead_end = height * width;
const int start_r = height - 1;
const int start_c = 0;
const int start_dir = 0; // 0 - up; 1 - right; 2 - down; 3 - left

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
	for (int i = 0; i < height; i++) {
		maze[i][0][3] = true;
		maze[i][width - 1][1] = true;
	}
	for (int j = 0; j < width; j++) {
		maze[0][j][0] = true;
		maze[height - 1][j][2] = true;
	}
}

void detect(int row, int col, int dir) {
	bool wall_front, wall_left, wall_right;  // retrieve data from sensor. True for wall.

	maze[row][col][dir] = maze[row][col][dir] || wall_front;
	maze[row][col][(dir + 3) % 4] = wall_left;
	maze[row][col][(dir + 1) % 4] = wall_right;

	if (row > 0) maze[row-1][col][2] = maze[row-1][col][2] || maze[row][col][0];
	if (col < width-1) maze[row][col+1][3] = maze[row][col+1][3] || maze[row][col][1];
	if (row < height-1) maze[row+1][col][0] = maze[row+1][col][0] || maze[row][col][2];
	if (col > 0) maze[row][col-1][1] = maze[row][col-1][1] || maze[row][col][3];
}

void update_dis() {  // BFS
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			dis[i][j] = dead_end;
		}
	}
	dis[]
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
		// Go straight
	} else if (new_dir == (dir+1)%4) {
		// Turn right
	} else if (new_dir == (dir+2)%4) {
		// Turn back
	} else {
		// Turn left
	}
}

void move() {
	// Move forward for 1 unit.
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