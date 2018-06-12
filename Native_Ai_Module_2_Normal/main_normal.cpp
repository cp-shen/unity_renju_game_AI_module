#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include "jsoncpp/json.h"
#include "header.h"
using namespace std;

int chessData[15][15];//������Ϣ����������Ϊ-1�����Ϊ1;
int order;//����
void ComputerDown(char *json_out);
bool computercolor;//false��ɫ��true��ɫ
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
		//���ӺͰ���һ����
		//����ִ����
		computercolor = false;
	}
	else {
		//����ִ����
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

		//�����Ǻ�ɫ
		if (player._Equal("black")) {
			//�����Ǻ�ɫ
			if (computercolor) {
				chessData[x1][y1]=-1;
			}
			//�����ǰ�ɫ
			else
			{
				chessData[x1][y1] = 1;
			}
		}
		//�����ǰ�ɫ
		else {
			//�����Ǻ�ɫ
			if (computercolor) {
				chessData[x1][y1] = 1;
			}
			//�����ǰ�ɫ
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
		{	// ɨ��ȫ���հ׵�
			if (chessData[x][y] == 0)
			{
				prior = 0;
				for (mode = 1; mode<5; mode++)	// ģʽ1Ϊ �� ���жϣ�ģʽ2Ϊ | ���ж�
				{							// ģʽ3Ϊ \ ���жϣ�ģʽ4Ϊ / ���ж�
					sum1 = sum2 = 0;
					flgL = flgR = 0;
					curX = x;
					curY = y; //�ı�ģʽ�����ñ���
					for (cur = -4; cur <= 4; cur++)
					{
						if (mode == 1) //������
						{
							curX = x + cur;
							if (curX<0) continue; //��Խ�����
							if (curX>14) break;   //��Խ��ֹͣ
						}
						else if (mode == 2) //������
						{
							curY = y + cur;
							if (curY<0) continue; //��Խ�����
							if (curY>14) break;   //��Խ��ֹͣ
						}
						else if (mode == 3) //��K���
						{
							curX = x + cur;
							curY = y + cur;
							if (curX<0 || curY<0) continue; // �����Խ�����
							if (curX>14 || curY>14) break;  // �һ���Խ��ֹͣ
						}
						else //��J���
						{
							curX = x + cur;
							curY = y - cur;
							if (curX<0 || curY>14) continue; // �����Խ�����
							if (curX>14 || curY<0) break;  // �һ���Խ��ֹͣ
						}

						// ��ʼ�����ӱ�־�������жϷ�ɫ
						if (cur<0 && flgL == 0 && chessData[curX][curY] != 0)
							flgL = chessData[curX][curY];

						else if (cur>0 && flgR == 0 && chessData[curX][curY] != 0)
							flgR = chessData[curX][curY];

						if (cur<0 && chessData[curX][curY] == (-flgL))
						{
							sum1 = -1;		// �����������ɫ���൱�ڱ���ס����
							flgL = -flgL;	// ͬʱ���Ҳȡ��
						}

						else if (cur<0 && chessData[curX][curY] == flgL)
						{
							sum1++; // ���������ͬɫ
						}

						if (cur == -1 && sum1>0 && chessData[curX][curY] == 0)
						{
							sum1--; // ��һλ����������
						}
						if (cur == 1 && sum2>0 && chessData[curX][curY] == 0)
						{
							sum2--; // ��һλ����������
						}

						if (cur>0 && flgL == flgR) // �����ߵ����Ӻ��ұߵ�ͬɫ
						{
							sum2 += sum1; // ��sum2����sum1��������
							sum1 = 0;     // sum1��0�Է��ظ���Ӻ�Ӱ������cPrior
						}

						if (cur>0 && chessData[curX][curY] == (-flgR))
						{
							sum2--; // �ұ߳��ַ�ɫ���൱�ڱ���ס����
							break;   // ����Ҫ����������
						}

						if (cur>0 && chessData[curX][curY] == flgR)
						{
							sum2++;// �Ҳ�������ͬɫ
						}

					}
					// ������Զ���ӵ�о��Ե�����
					if (sum1 == 1) prior = prior + 1;
					else if (sum1 == 2) prior = prior + 10 - 2 * flgL; // ��Ŀ��ͬ��������
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
				{	// ����һ��������������ȼ���ȵĵ�
					cPrior = prior;
					cx = x;
					cy = y;
				}
			}
		}
	}
	//chessData[cx][cy] = -1; 


	// �ڵõ��ĵ�����
	
	createJSON(json_out, cx, cy);
	
	
}


void drive_ai(const char *json_in, char *json_out, int out_capacity) {
	// write no more than out_capacity characters into json_out
	// out_capacity is provided by external c# code and will be 100

	//��ʼ��
	computercolor = false;
	order = 0;
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			
			chessData[i][j] = 0;
		}
	}   

	// ��ȡ������Ϣ
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