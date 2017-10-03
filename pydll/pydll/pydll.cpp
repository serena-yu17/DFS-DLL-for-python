// pydll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include <vector>
#include <unordered_set>
using namespace std;

template<>
struct hash<pair<int, int>>
{
	size_t operator()(const pair<int, int>& pt)	 const
	{
		return pt.first << 16 + pt.second;
	}
};

void vector_str(vector<pair<int, int>>& path, char* line)
{
	if (!path.size())
		return;
	line[0] = '[';
	int i = 1;
	for (auto& pt : path)
	{
		line[i++] = '(';
		line[i++] = pt.first + '0';
		line[i++] = ',';
		line[i++] = ' ';
		line[i++] = pt.second + '0';
		line[i++] = ')';
		line[i++] = ',';
		line[i++] = ' ';
	}
	line[i - 2] = ']';
	line[i - 1] = 0;
}

int sumGrid(const int grid[10][10])
{
	int sum = 0;
	for (int y = 0; y < 10; y++)
		for (int x = 0; x < 10; x++)
			sum += grid[y][x];
	return sum;
}

vector<pair<int, int>> seek(const pair<int, int>& root, const int grid[10][10], const int& target)
{
	int sum = grid[root.first][root.second];
	vector<pair<int, int>> stack;
	if (sum == target)
	{
		stack.push_back(root);
		return stack;
	}
	char directions[10][10][5];
	memset(directions, 0, 500);
	
	unordered_set<pair<int, int>> stack_set;
	stack_set.insert(root);
	stack.push_back(root);
	int direction = 0;
	pair<int, int>* recurPoint = NULL;
	while (stack.size())
	{
		pair<int, int> nxt = make_pair(-1, -1);
		pair<int, int> *pt = &stack[stack.size() - 1];
		bool isRecur = 0;
		if (sum < target)
		{
			for (int i = 0; i < 4; i++)
			{
				if (direction == 3 && pt->second != 0 && directions[pt->first][pt->second][direction] == 0)
				{
					nxt.first = pt->first;
					nxt.second = pt->second - 1;
					if (stack_set.find(nxt) == stack_set.end())
						break;
					else
					{
						nxt.first = -1;
						isRecur = 1;
					}
				}
				else if (direction == 2 && pt->first != 9 && directions[pt->first][pt->second][direction] == 0)
				{
					nxt.first = pt->first + 1;
					nxt.second = pt->second;
					if (stack_set.find(nxt) == stack_set.end())
						break;
					else
					{
						nxt.first = -1;
						isRecur = 1;
					}
				}
				else if (direction == 1 && pt->second != 9 && directions[pt->first][pt->second][direction] == 0)
				{
					nxt.first = pt->first;
					nxt.second = pt->second + 1;
					if (stack_set.find(nxt) == stack_set.end())
						break;
					else
					{
						nxt.first = -1;
						isRecur = 1;
					}
				}
				else if (direction == 0 && pt->first != 0 && directions[pt->first][pt->second][direction] == 0)
				{
					nxt.first = pt->first - 1;
					nxt.second = pt->second;
					if (stack_set.find(nxt) == stack_set.end())
						break;
					else
					{
						nxt.first = -1;
						isRecur = 1;
					}
				}
				direction++;
				if (direction == 4)
					direction = 0;
			}
		}
		if (nxt.first != -1)
		{
			if (!recurPoint && isRecur && pt->first != 0 && pt->second != 0 && pt->first != 9 && pt->second != 9)
				recurPoint = pt;
			int backDirection = direction + 2;
			if (backDirection > 3)
				backDirection -= 4;
			directions[nxt.first][nxt.second][backDirection] = 1;
			directions[pt->first][pt->second][4] = direction;
			directions[pt->first][pt->second][direction] = 1;
			stack.push_back(nxt);
			stack_set.insert(nxt);
			sum += grid[nxt.first][nxt.second];
			if (sum == target)
				return stack;
		}
		else
		{
			if (recurPoint)
			{
				while (*pt != *recurPoint)
				{
					sum -= grid[pt->first][pt->second];
					memset(&directions[pt->first][pt->second][0], 0, 5);
					stack_set.erase(*pt);
					stack.pop_back();
					pt = &stack[stack.size() - 1];
				}
			}
			else
			{
				sum -= grid[pt->first][pt->second];
				memset(&directions[pt->first][pt->second][0], 0, 5);
				stack_set.erase(*pt);
				stack.pop_back();
			}
			if (stack.size())
				direction = directions[stack[stack.size() - 1].first][stack[stack.size() - 1].second][4];
		}
	}
}


extern "C" _declspec(dllexport) void DFS(char* out, const int grid[10][10], const int& x, const int& y, const int& target)
{
	if (sumGrid(grid) < target)
		return;
	int sum = grid[x][y];
	pair<int, int> root = make_pair(x, y);
	if (sum <= target)
	{
		vector<pair<int, int>> vec = seek(root, grid, target);
		vector_str(vec, out);
	}
}

