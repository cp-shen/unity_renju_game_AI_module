#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include "jsoncpp/json.h"
#include "header.h"
using namespace std;

int chessData[15][15];//棋盘信息，电脑棋子为-1，玩家为1;
int order;//步数
void ComputerDown(char *json_out);
bool computercolor;//false黑色，true白色
int parseJSON(const char* jsonstr)
{
	Json::Reader reader;
	Json::Value  resp;

	if (!reader.parse(jsonstr, resp, false))
	{
		printf("bad json format!\n");
		return 1;
	}


	Json::Value & resultValue = resp["gameTrace"];
	int size = 0;
	int max_order = 0;
	int sizefororder = resultValue.size();
	
	if (sizefororder % 2 == 0) {
		//黑子和白子一样多
		//电脑执黑子
		computercolor = false;
	}
	else {
		//电脑执白子
		computercolor = true;
	}
	for (size = 0; size< resultValue.size(); size++)
	{
		Json::Value subJson = resultValue[size];
		order = subJson["order"].asInt();
		max_order = (order > max_order) ? order : max_order;
		string player = subJson["player"].asString();
		int x1 = subJson["x"].asInt();
		int y1 = subJson["y"].asInt();

		//棋子是黑色
		if (player._Equal("black")) {
			//电脑是黑色
			if (computercolor) {
				chessData[x1][y1]=-1;
			}
			//电脑是白色
			else
			{
				chessData[x1][y1] = 1;
			}
		}
		//棋子是白色
		else {
			//电脑是黑色
			if (computercolor) {
				chessData[x1][y1] = 1;
			}
			//电脑是白色
			else
			{
				chessData[x1][y1] = -1;
			}
		}


	}

	order = max_order;

	return 0;
}
void createJSON(char *block, int x, int y)
{
	Json::Value	object1;
	object1["order"] = order + 1;
	if (computercolor) {
		object1["player"] = "white";
	}
	else
	{
		object1["player"] = "black";
	}

	object1["x"] = x;
	object1["y"] = y;


	std::string s = object1.toStyledString().c_str();
	memcpy(block, s.data(), s.size());

	return;
}
void ComputerDown(char *json_out)
{
	int mode = 0, cur = 0, curX = 0, curY = 0, sum1 = 0, sum2 = 0;
	int x = 0, y = 0, cx = 0, cy = 0, cPrior = 0, prior = 0, flgL = 0, flgR = 0;

	for (x = 0; x<15; x++)
	{
		for (y = 0; y<15; y++)
		{	// 扫描全部空白点
			if (chessData[x][y] == 0)
			{
				prior = 0;
				for (mode = 1; mode<5; mode++)	// 模式1为 — 向判断，模式2为 | 向判断
				{							// 模式3为 \ 向判断，模式4为 / 向判断
					sum1 = sum2 = 0;
					flgL = flgR = 0;
					curX = x;
					curY = y; //改变模式后重置变量
					for (cur = -4; cur <= 4; cur++)
					{
						if (mode == 1) //向→检查
						{
							curX = x + cur;
							if (curX<0) continue; //左越界继续
							if (curX>14) break;   //右越界停止
						}
						else if (mode == 2) //向↓检查
						{
							curY = y + cur;
							if (curY<0) continue; //上越界继续
							if (curY>14) break;   //下越界停止
						}
						else if (mode == 3) //向↘检查
						{
							curX = x + cur;
							curY = y + cur;
							if (curX<0 || curY<0) continue; // 左或上越界继续
							if (curX>14 || curY>14) break;  // 右或下越界停止
						}
						else //向↗检查
						{
							curX = x + cur;
							curY = y - cur;
							if (curX<0 || curY>14) continue; // 左或下越界继续
							if (curX>14 || curY<0) break;  // 右或上越界停止
						}

						// 初始化棋子标志，用于判断反色
						if (cur<0 && flgL == 0 && chessData[curX][curY] != 0)
							flgL = chessData[curX][curY];

						else if (cur>0 && flgR == 0 && chessData[curX][curY] != 0)
							flgR = chessData[curX][curY];

						if (cur<0 && chessData[curX][curY] == (-flgL))
						{
							sum1 = -1;		// 左侧搜索到反色，相当于被堵住的棋
							flgL = -flgL;	// 同时标记也取反
						}

						else if (cur<0 && chessData[curX][curY] == flgL)
						{
							sum1++; // 左侧搜索到同色
						}

						if (cur == -1 && sum1>0 && chessData[curX][curY] == 0)
						{
							sum1--; // 左一位搜索到空棋
						}
						if (cur == 1 && sum2>0 && chessData[curX][curY] == 0)
						{
							sum2--; // 右一位搜索到空棋
						}

						if (cur>0 && flgL == flgR) // 如果左边的棋子和右边的同色
						{
							sum2 += sum1; // 用sum2代替sum1继续搜索
							sum1 = 0;     // sum1置0以防重复相加和影响后面的cPrior
						}

						if (cur>0 && chessData[curX][curY] == (-flgR))
						{
							sum2--; // 右边出现反色则，相当于被堵住的棋
							break;   // 不需要继续搜索了
						}

						if (cur>0 && chessData[curX][curY] == flgR)
						{
							sum2++;// 右侧搜索到同色
						}

					}
					// 长连相对短连拥有绝对的优势
					if (sum1 == 1) prior = prior + 1;
					else if (sum1 == 2) prior = prior + 10 - 2 * flgL; // 数目相同白棋优先
					else if (sum1 == 3) prior = prior + 100 - 20 * flgL;
					else if (sum1 == 4) prior = prior + 1000 - 200 * flgL;
					if (sum2 == 1) prior = prior + 1;
					else if (sum2 == 2) prior = prior + 10 - 2 * flgR;
					else if (sum2 == 3) prior = prior + 100 - 20 * flgR;
					else if (sum2 == 4) prior = prior + 1000 - 200 * flgR;
				}

				if (prior>cPrior)
				{
					cPrior = prior;
					cx = x;
					cy = y;
				}

				if (prior == cPrior && rand() < RAND_MAX / 5)
				{	// 除第一个棋外很少有优先级相等的点
					cPrior = prior;
					cx = x;
					cy = y;
				}
			}
		}
	}
	//chessData[cx][cy] = -1; 


	// 在得到的点下棋
	
	createJSON(json_out, cx, cy);
	
	
}


void drive_ai(const char *json_in, char *json_out, int out_capacity) {
	// write no more than out_capacity characters into json_out
	// out_capacity is provided by external c# code and will be 100

	//初始化
	computercolor = false;
	order = 0;
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			
			chessData[i][j] = 0;
		}
	}   

	// 读取棋盘信息
	parseJSON(json_in);
	ComputerDown(json_out);
	
}
 int main() {
	char json_in[] = "{ \"gameTrace\": [ { \"order\" : 0, \"player\" : \"black\", \"x\" : 9, \"y\" : 4 }, { \"order\" : 1, \"player\" : \"white\", \"x\" : 10, \"y\" : 5 },{ \"order\" : 2, \"player\" : \"black\", \"x\" : 10, \"y\" : 4 },{ \"order\" : 3, \"player\" : \"black\", \"x\" : 10, \"y\" : 6 } ] }";
	char* block = new char[100];
	memset(block, 0, 100);
	drive_ai(json_in, block, 100);
	std::cout << json_in << std::endl;
	std::cout << block << std::endl;
	delete[] block;
	getchar();
	return 0;
}