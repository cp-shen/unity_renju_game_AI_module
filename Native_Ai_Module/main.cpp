#include <string>
#include "jsoncpp/json.h"
#include "header.h"
//#include <iostream>

using namespace std;
void calculatechess();
bool limit(int i, int j);
typedef struct {
	int x;
	int y;
	int color;//1�Ǻڣ�-1�ǰ�
	int ifput;
}information;//����ÿ�����ӵ�λ�á���ɫ���Ƿ�����

typedef struct {
	int x;
	int y;
	int value;
}Chess;//�������ӵ�����ͼ�ֵ

bool people_black;//�ж��������ӵ���ɫ
int order;//����
information local[15][15];//������Ϣ
Chess chess[15][15];//���̼�ֵ��Ϣ

/*char *getfileAll(char *fname)
{
	FILE *fp;
	char *str;
	char txt[1000];
	int filesize;
	if ((fp=fopen(fname,"r"))==NULL){
		printf("open file %s fail \n",fname);
		return NULL;
	}

	fseek(fp,0,SEEK_END); 

	filesize = ftell(fp);
	str=(char *)malloc(filesize);
	str[0]=0;

	rewind(fp);
	while((fgets(txt,1000,fp))!=NULL){
		strcat(str,txt);
	}
	fclose(fp);
	return str;
}*/

/*int writefileAll(char* fname,const char* data)
{
	FILE *fp;
	if ((fp=fopen(fname, "w")) == NULL)
	{
		printf("open file %s fail \n", fname);
		return 1;
	}
	
	fprintf(fp, "%s", data);
	fclose(fp);
	
	return 0;
}*/


int parseJSON(const char* jsonstr)
{
	Json::Reader reader;
	Json::Value  resp;

	if (!reader.parse(jsonstr, resp, false))
	{
		//printf("bad json format!\n");
		return 1;
	}
	

	Json::Value & resultValue = resp["gameTrace"];
	int size = 0;
	int max_order = 0;
	for ( size = 0; size< resultValue.size(); size++)
	{
		Json::Value subJson = resultValue[size];
		 order = subJson["order"].asInt();
		 max_order = (order > max_order) ? order : max_order;
		string player = subJson["player"].asString();
		int x1 = subJson["x"].asInt();
		int y1 = subJson["y"].asInt();
	
		local[x1][y1].x = x1;
		local[x1][y1].y = y1;
		
		if (player._Equal("black")) {
			local[x1][y1].ifput = 1;
			local[x1][y1].color = 1;
		}
		
		else {
			local[x1][y1].ifput = -1;
			local[x1][y1].color = -1;
		}
			
		

		
	}
	if (size % 2 == 0) {
		//���ӺͰ���һ����
		//����ִ����
		people_black=false;
	}
	else {
		people_black = true;
	}
	order = max_order;
	return 0;
}

 void createJSON(char *block, int x,int y)
{
	Json::Value	object1;
	object1["order"] = order + 1;
	if (people_black) {
		object1["player"] = "white";
	}
	else
	{
		object1["player"] = "black";
	}

	object1["x"] = x;
	object1["y"] = y;
	
	


	/*Json::Value jarray;
	jarray.append(object1);*/
	
	
	//writefileAll("renjuout.json", object1.toStyledString().c_str());
	std::string s = object1.toStyledString().c_str();
	memcpy(block, s.data(), s.size());
	
	//std::cout << block << std::endl;
	return;
}

//����
void playchess(char *block)
{
	calculatechess();
	int a=0 ,l=0,n=0;
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			if (local[i][j].ifput == 0) {
				if ((limit(i, j) == false) && (people_black == false)) {
					chess[i][j].value = -1;
				}
				else if (chess[i][j].value >= a) {
					a = chess[i][j].value;
					l = i;
					n = j;
				}
			}
		}
	}     
	//�Ƚ�ÿ��λ�ü�ֵ�Ĵ�С
	
	createJSON(block, l,n);
	

}
bool limit(int i, int j) {//��Ҫ���ƺ��壬�����ʱ������������������γɣ������ĵ��γɣ��Լ��������гɡ����˳��������γɵ�ͬʱʤ��������ʤ���� ����1������-1 true Ϊ������ ��false Ϊ�������¡�
	int s, t, count, down1, up1, left1, right1, leftdown1, rightdown1, leftup1, rightup1, a, b;
	int down2, up2, left2, right2, leftdown2, rightdown2, leftup2, rightup2, barrier, amount;
	down1 = 0;  up1 = 0;  left1 = 0;  right1 = 0;  leftdown1 = 0;  rightdown1 = 0;  leftup1 = 0;  rightup1 = 0;//����
	down2 = 0;  up2 = 0;  left2 = 0;  right2 = 0;  leftdown2 = 0;  rightdown2 = 0;  leftup2 = 0;  rightup2 = 0;//����
																											   //����1
	local[i][j].color = 1;
	for (s = j, t = i, count = 5; local[t][s].color == 1 && s < 15 && count > 0; s++, count--)
		down1++;   //�����ж��м���ͬ������
	for (s = j, t = i, count = 5; local[t][s].color == 1 && s >= 0 && count > 0; s--, count--)
		up1++;     //�����ж��м���ͬ������
	for (s = j, t = i, count = 5; local[t][s].color == 1 && t < 15 && count > 0; t++, count--)
		right1++;   //�����ж��м���ͬ������
	for (s = j, t = i, count = 5; local[t][s].color == 1 && t >= 0 && count > 0; t--, count--)
		left1++;    //�����ж��м���ͬ������
	for (s = j, t = i, count = 5; local[t][s].color == 1 && t >= 0 && s >= 0 && count > 0; t--, s--, count--)
		leftup1++;    //�����ж��м���ͬ������
	for (s = j, t = i, count = 5; local[t][s].color == 1 && t < 15 && s < 15 && count > 0; t++, s++, count--)
		rightdown1++;    //�����ж��м���ͬ������
	for (s = j, t = i, count = 5; local[t][s].color == 1 && t >= 0 && s < 15 && count > 0; t--, s++, count--)
		leftdown1++;    //�����ж��м���ͬ������rightup1
	for (s = j, t = i, count = 5; local[t][s].color == 1 && t < 15 && s >= 0 && count > 0; t++, s--, count--)
		rightup1++;    //�����ж��м���ͬ������leftdown1
					   //����-1
	local[i][j].color = -1;
	for (s = j, t = i, count = 5; local[t][s].color == -1 && s < 15 && count > 0; s++, count--)
		down2++;    //�����ж��м���ͬ������
	for (s = j, t = i, count = 5; local[t][s].color == -1 && s >= 0 && count > 0; s--, count--)
		up2++;    //�����ж��м���ͬ������
	for (s = j, t = i, count = 5; local[t][s].color == -1 && t < 15 && count > 0; t++, count--)
		right2++;    //�����ж��м���ͬ������
	for (s = j, t = i, count = 5; local[t][s].color == -1 && t >= 0 && count > 0; t--, count--)
		left2++;    //�����ж��м���ͬ������
	for (s = j, t = i, count = 5; local[t][s].color == -1 && t >= 0 && s >= 0 && count > 0; t--, s--, count--)
		leftup2++;    //�����ж��м���ͬ������
	for (s = j, t = i, count = 5; local[t][s].color == -1 && t < 15 && s < 15 && count > 0; t++, s++, count--)
		rightdown2++;    //�����ж��м���ͬ������
	for (s = j, t = i, count = 5; local[t][s].color == -1 && t >= 0 && s < 15 && count > 0; t--, s++, count--)
		leftdown2++;    //�����ж��м���ͬ������
	for (s = j, t = i, count = 5; local[t][s].color == -1 && t < 15 && s >= 0 && count > 0; t++, s--, count--)
		rightup2++;    //�����ж��м���ͬ������
	local[i][j].color = 0;
	if ((down1 + up1 > 6) || (left1 + right1 > 6) || (leftup1 + rightdown1 > 6) || (rightup1 + leftdown1 > 6))
		return false;   //�����Ƿ��������������ϣ���˴���ֹ����//����
	if (down1 + up1 == 5 || right1 + left1 == 5 || leftdown1 + rightup1 == 5 || leftup1 + rightdown1 == 5) {
		count = 0;        //���� ˫��
		if (down1 + up1 == 5) {
			if (j + 1 <= 15 && local[i][j + 1].ifput == 0) {
				if (j + 1 + down1 <= 15 && local[i][j + 1 + down1].ifput == 0) {
					count++;
				}
			}
			else if (j + 1 <= 15 && local[i][j + 1].ifput == 1) {
				if (j + down1 <= 15 && local[i][j + down1].ifput == 0) {
					count++;
				}
			}
			if (j - 1 >= 0 && local[i][j - 1].ifput == 0) {
				if (j - 1 - up1 >= 0 && local[i][j - 1 - up1].ifput == 0) {
					count++;
				}
			}
			else if (j - 1 >= 0 && local[i][j - 1].ifput == 1) {
				if (j - up1 >= 0 && local[i][j - up1].ifput == 0) {
					count++;
				}
			}
		}
		if (right1 + left1 == 5) {
			if (i + 1 <= 15 && local[i + 1][j].ifput == 0) {
				if (i + 1 + right1 <= 15 && local[i + 1 + right1][j].ifput == 0) {
					count++;
				}
			}
			else if (i + 1 <= 15 && local[i + 1][j].ifput == 1) {
				if (i + right1 <= 15 && local[i + right1][j].ifput == 0) {
					count++;
				}
			}
			if (i - 1 >= 0 && local[i - 1][j].ifput == 0) {
				if (i - 1 - left1 >= 0 && local[i - 1 - left1][j].ifput == 0) {
					count++;
				}
			}
			else if (i - 1 >= 0 && local[i - 1][j].ifput == 1) {
				if (i - left1 >= 0 && local[i - left1][j].ifput == 0) {
					count++;
				}
			}
		}
		if (leftdown1 + rightup1 == 5) {
			if (i + 1 <= 15 && j - 1 >= 0 && local[i + 1][j - 1].ifput == 0) {
				if (i + 1 + rightup1 <= 15 && j - 1 - rightup1 >= 0 && local[i + 1 + right1][j - 1 - up1].ifput == 0) {
					count++;
				}
			}
			else if (i + 1 <= 15 && j - 1 >= 0 && local[i + 1][j - 1].ifput == 1) {
				if (i + rightup1 <= 15 && j - rightup1 >= 0 && local[i + right1][j - up1].ifput == 0) {
					count++;
				}
			}
			if (i - 1 >= 0 && j + 1 <= 15 && local[i - 1][j + 1].ifput == 0) {
				if (i - 1 - leftdown1 >= 0 && j + 1 + leftdown1 <= 15 && local[i - 1 - left1][j + 1 + down1].ifput == 0) {
					count++;
				}
			}
			else if (i - 1 >= 0 && j + 1 <= 15 && local[i - 1][j + 1].ifput == 1) {
				if (i - leftdown1 >= 0 && j + leftdown1 <= 15 && local[i - left1][j + down1].ifput == 0) {
					count++;
				}
			}
		}
		if (leftup1 + rightdown1 == 5) {
			if (i - 1 >= 0 && j - 1 >= 0 && local[i - 1][j - 1].ifput == 0) {
				if (i - 1 - leftup1 >= 0 && j - 1 - leftup1 >= 0 && local[i - 1 - left1][j - 1 - up1].ifput == 0) {
					count++;
				}
			}
			else if (i - 1 >= 0 && j - 1 >= 0 && local[i - 1][j - 1].ifput == 1) {
				if (i - leftup1 >= 0 && j - leftup1 >= 0 && local[i - left1][j - up1].ifput == 0) {
					count++;
				}
			}
			if (i + 1 <= 15 && j + 1 <= 15 && local[i + 1][j + 1].ifput == 0) {
				if (i + 1 + rightdown1 <= 15 && j + 1 + rightdown1 <= 15 && local[i + 1 + right1][j + 1 + down1].ifput == 0) {
					count++;
				}
			}
			else if (i + 1 >= 0 && j + 1 <= 15 && local[i + 1][j + 1].ifput == 1) {
				if (i + rightdown1 >= 0 && j + rightdown1 <= 15 && local[i + right1][j + down1].ifput == 0) {
					count++;
				}
			}
		}
		if (count >= 2) {//��ʱ��λ�ý���
			return false;
		}
		return true;
	}
	if (down1 + up1 == 4 || right1 + left1 == 4 || leftdown1 + rightup1 == 4 || leftup1 + rightdown1 == 4) {
		count = 0;	amount = 0;     //����������Ŀ��Ӻ������ӣ�//�ϼ��¼� ����Ҽ�
		if (down1 + up1 == 4) {//
			if (j + 1 <= 15 && local[i][j + 1].ifput == 1) {//���½���������
				if (j - 1 >= 0 && local[i][j - 1].ifput == 1) {
					if ((j + 2 <= 15 && local[i][j + 2].ifput == 0) && (j - 2 >= 0 && local[i][j - 2].ifput == 0)) {
						count++;
					}
				}

			}
			else if (j + 1 <= 15 && local[i][j + 1].ifput == 0) {//�·�������
				if ((j + 2 <= 15 && local[i][j + 2].ifput == 1) && (j + 3 <= 15 && local[i][j + 3].ifput == 1)) {
					if ((j + 4 <= 15 && local[i][j + 4].ifput == 0) && (j - 1 >= 0 && local[i][j - 1].ifput == 0)) {
						count++;
					}
				}
			}
			else if (j - 1 >= 0 && local[i][j - 1].ifput == 0) {//�Ϸ�������
				if ((j - 2 >= 0 && local[i][j - 2].ifput == 1) && (j - 3 >= 0 && local[i][j - 3].ifput == 1)) {
					if ((j - 4 >= 0 && local[i][j - 4].ifput == 0) && (j + 1 <= 15 && local[i][j + 1].ifput == 0)) {
						count++;
					}
				}
			}
			else if (j - 1 >= 0 && local[i][j - 1].ifput == 0) {//�Ϸ�һ�����п� �·�һ�����޿�
				if ((j - 2 >= 0 && local[i][j - 2].ifput == 1) && (j + 1 <= 15 && local[i][j + 1].ifput == 1)) {
					if ((j - 3 >= 0 && local[i][j - 3].ifput == 0) && (j + 2 <= 15 && local[i][j + 2].ifput == 0)) {
						count++;
					}
				}
			}
			else if (j + 1 <= 15 && local[i][j + 1].ifput == 0) {//�·�һ�����п� �Ϸ�һ�����޿�
				if ((j + 2 <= 15 && local[i][j + 2].ifput == 1) && (j - 1 >= 0 && local[i][j - 1].ifput == 1)) {
					if ((j + 3 <= 15 && local[i][j + 3].ifput == 0) && (j - 2 >= 0 && local[i][j - 2].ifput == 0)) {
						count++;
					}
				}
			}
		}
		if (right1 + left1 == 4) {
			if (i + 1 <= 15 && local[i + 1][j].ifput == 1) {//���ҽ���������
				if (i - 1 >= 0 && local[i - 1][j].ifput == 1) {
					if ((i + 2 <= 15 && local[i + 2][j].ifput == 0) && (i - 2 >= 0 && local[i - 2][j].ifput == 0)) {
						count++;
					}
				}

			}
			else if (i + 1 <= 15 && local[i + 1][j].ifput == 0) {//�ҷ�������
				if ((i + 2 <= 15 && local[i + 2][j].ifput == 1) && (i + 3 <= 15 && local[i + 3][j].ifput == 1)) {
					if ((i + 4 <= 15 && local[i + 4][j].ifput == 0) && (i - 1 >= 0 && local[i - 1][j].ifput == 0)) {
						count++;
					}
				}
			}
			else if (i - 1 >= 0 && local[i - 1][j].ifput == 0) {//��������
				if ((i - 2 >= 0 && local[i - 2][j].ifput == 1) && (i - 3 >= 0 && local[i - 3][j].ifput == 1)) {
					if ((i - 4 >= 0 && local[i - 4][j].ifput == 0) && (i + 1 <= 15 && local[i + 1][j].ifput == 0)) {
						count++;
					}
				}
			}
			else if (i - 1 >= 0 && local[i - 1][j].ifput == 0) {//��һ�����п� �·�һ�����޿�
				if ((i - 2 >= 0 && local[i - 2][j].ifput == 1) && (i + 1 <= 15 && local[i + 1][j].ifput == 1)) {
					if ((i - 3 >= 0 && local[i - 3][j].ifput == 0) && (i + 2 <= 15 && local[i + 2][j].ifput == 0)) {
						count++;
					}
				}
			}
			else if (i + 1 <= 15 && local[i + 1][j].ifput == 0) {//�ҷ�һ�����п� �Ϸ�һ�����޿�
				if ((i + 2 <= 15 && local[i + 2][j].ifput == 1) && (i - 1 >= 0 && local[i - 1][j].ifput == 1)) {
					if ((i + 3 <= 15 && local[i + 3][j].ifput == 0) && (i - 2 >= 0 && local[i - 2][j].ifput == 0)) {
						count++;
					}
				}
			}
		}
		if (leftdown1 + rightup1 == 4) {
			if (i - 1 >= 0 && j + 1 <= 15 && local[i - 1][j + 1].ifput == 1) {//�������Ͻ���������
				if (i + 1 <= 15 && j - 1 >= 0 && local[i + 1][j - 1].ifput == 1) {
					if ((i - 2 >= 0 && j + 2 <= 15 && local[i - 2][j + 2].ifput == 0) && (i + 2 <= 15 && j - 2 >= 0 && local[i + 2][j - 2].ifput == 0)) {
						count++;
					}
				}

			}
			else if (i + 1 <= 15 && j - 1 >= 0 && local[i + 1][j - 1].ifput == 0) {//���Ϸ�������
				if ((i + 2 <= 15 && j - 2 >= 0 && local[i + 2][j - 2].ifput == 1) && (i + 3 <= 15 && j - 3 >= 0 && local[i + 3][j - 3].ifput == 1)) {
					if ((i + 4 <= 15 && j - 4 >= 0 && local[i + 4][j - 4].ifput == 0) && (i - 1 >= 0 && j + 1 <= 15 && local[i - 1][j + 1].ifput == 0)) {
						count++;
					}
				}
			}
			else if (i - 1 >= 0 && j + 1 <= 15 && local[i - 1][j + 1].ifput == 0) {//���·�������
				if ((i - 2 >= 0 && j + 2 <= 15 && local[i - 2][j + 2].ifput == 1) && (i - 3 >= 0 && j + 3 <= 15 && local[i - 3][j + 3].ifput == 1)) {
					if ((i - 4 >= 0 && j + 4 <= 15 && local[i - 4][j + 4].ifput == 0) && (i + 1 <= 15 && j - 1 >= 0 && local[i + 1][j - 1].ifput == 0)) {
						count++;
					}
				}
			}
			else if (i - 1 >= 0 && j + 1 <= 15 && local[i - 1][j + 1].ifput == 0) {//���·�һ�����п� ���Ϸ�һ�����޿�
				if ((i - 2 >= 0 && j + 2 <= 15 && local[i - 2][j + 2].ifput == 1) && (i + 1 <= 15 && j - 1 >= 0 && local[i + 1][j - 1].ifput == 1)) {
					if ((i - 3 >= 0 && j + 3 <= 15 && local[i - 3][j + 3].ifput == 0) && (i + 2 <= 15 && j - 2 >= 0 && local[i + 2][j - 2].ifput == 0)) {
						count++;
					}
				}
			}
			else if (i + 1 <= 15 && j - 1 >= 0 && local[i + 1][j - 1].ifput == 0) {//���Ϸ�һ�����п� ���·�һ�����޿�
				if ((i + 2 <= 15 && j - 2 >= 0 && local[i + 2][j - 2].ifput == 1) && (i - 1 >= 0 && j + 1 <= 15 && local[i - 1][j + 1].ifput == 1)) {
					if ((i + 3 <= 15 && j - 3 >= 0 && local[i + 3][j - 3].ifput == 0) && (i - 2 >= 0 && j + 2 <= 15 && local[i - 2][j + 2].ifput == 0)) {
						count++;
					}
				}
			}
		}
		if (leftup1 + rightdown1 == 4) {
			if (i - 1 >= 0 && j - 1 >= 0 && local[i - 1][j - 1].ifput == 1) {//�������½���������
				if (i + 1 <= 15 && j + 1 <= 15 && local[i + 1][j + 1].ifput == 1) {
					if ((i - 2 >= 0 && j - 2 >= 0 && local[i - 2][j - 2].ifput == 0) && (i + 2 <= 15 && j + 2 <= 15 && local[i + 2][j + 2].ifput == 0)) {
						count++;
					}
				}

			}
			else if (i - 1 >= 0 && j - 1 >= 0 && local[i - 1][j - 1].ifput == 0) {//���Ϸ�������
				if ((i - 2 >= 0 && j - 2 >= 0 && local[i - 2][j - 2].ifput == 1) && (i - 3 >= 0 && j - 3 >= 0 && local[i - 3][j - 3].ifput == 1)) {
					if ((i - 4 >= 0 && j - 4 >= 0 && local[i - 4][j - 4].ifput == 0) && (i + 1 <= 15 && j + 1 <= 15 && local[i + 1][j + 1].ifput == 0)) {
						count++;
					}
				}
			}
			else if (i + 1 <= 15 && j + 1 <= 15 && local[i + 1][j + 1].ifput == 0) {//���·�������
				if ((i + 2 <= 15 && j + 2 <= 15 && local[i + 2][j + 2].ifput == 1) && (i + 3 <= 15 && j + 3 <= 15 && local[i + 3][j + 3].ifput == 1)) {
					if ((i + 4 >= 0 && j + 4 <= 15 && local[i + 4][j + 4].ifput == 0) && (i - 1 >= 0 && j - 1 >= 0 && local[i - 1][j - 1].ifput == 0)) {
						count++;
					}
				}
			}
			else if (i - 1 >= 0 && j - 1 >= 0 && local[i - 1][j - 1].ifput == 0) {//���Ϸ�һ�����п� ���·�һ�����޿�
				if ((i - 2 >= 0 && j - 2 >= 0 && local[i - 2][j - 2].ifput == 1) && (i + 1 <= 15 && j + 1 <= 15 && local[i + 1][j + 1].ifput == 1)) {
					if ((i - 3 >= 0 && j - 3 >= 0 && local[i - 3][j - 3].ifput == 0) && (i + 2 <= 15 && j + 2 <= 15 && local[i + 2][j + 2].ifput == 0)) {
						count++;
					}
				}
			}
			else if (i + 1 <= 15 && j + 1 <= 15 && local[i + 1][j + 1].ifput == 0) {//���·�һ�����п� ���Ϸ�һ�����޿�
				if ((i + 2 <= 15 && j + 2 <= 15 && local[i + 2][j + 2].ifput == 1) && (i - 1 >= 0 && j - 1 >= 0 && local[i - 1][j - 1].ifput == 1)) {
					if ((i + 3 <= 15 && j + 3 <= 15 && local[i + 3][j + 3].ifput == 0) && (i - 2 >= 0 && j - 2 >= 0 && local[i - 2][j - 2].ifput == 0)) {
						count++;
					}
				}
			}
		}
		if (count >= 2) {//��ʱ��λ�ý���
			return false;
		}
		return true;
	}
	return true;
}
//�����ֵ
void calculatechess()
{
	int i, j, s, t, count, down1, up1, left1, right1, leftdown1, rightdown1, leftup1, rightup1, a, b;
	int down2, up2, left2, right2, leftdown2, rightdown2, leftup2, rightup2, barrier, amount;
	for (i = 0; i < 15; i++) {
		for (j = 0; j < 15; j++) {
			if (local[i][j].ifput == 0) {
				local[i][j].color = 1;
				down1 = 0;  up1 = 0;  left1 = 0;  right1 = 0;  leftdown1 = 0;  rightdown1 = 0;  leftup1 = 0;  rightup1 = 0;
				down2 = 0;  up2 = 0;  left2 = 0;  right2 = 0;  leftdown2 = 0;  rightdown2 = 0;  leftup2 = 0;  rightup2 = 0;
				for (s = j, t = i, count = 5; local[t][s].color == 1 && s < 15 && count > 0; s++, count--)
					down1++;   //����
				for (s = j, t = i, count = 5; local[t][s].color == 1 && s >= 0 && count > 0; s--, count--)
					up1++;     //����
				for (s = j, t = i, count = 5; local[t][s].color == 1 && t < 15 && count > 0; t++, count--)
					right1++;   //����
				for (s = j, t = i, count = 5; local[t][s].color == 1 && t >= 0 && count > 0; t--, count--)
					left1++;    //����
				for (s = j, t = i, count = 5; local[t][s].color == 1 && t >= 0 && s >= 0 && count > 0; t--, s--, count--)
					leftup1++;    //����
				for (s = j, t = i, count = 5; local[t][s].color == 1 && t < 15 && s < 15 && count > 0; t++, s++, count--)
					rightdown1++;    //����
				for (s = j, t = i, count = 5; local[t][s].color == 1 && t >= 0 && s < 15 && count > 0; t--, s++, count--)
					leftdown1++;    //����
				for (s = j, t = i, count = 5; local[t][s].color == 1 && t < 15 && s >= 0 && count > 0; t++, s--, count--)
					rightup1++;    //����
				local[i][j].color = -1;
				for (s = j, t = i, count = 5; local[t][s].color == -1 && s < 15 && count > 0; s++, count--)
					down2++;    //����
				for (s = j, t = i, count = 5; local[t][s].color == -1 && s >= 0 && count > 0; s--, count--)
					up2++;    //����
				for (s = j, t = i, count = 5; local[t][s].color == -1 && t < 15 && count > 0; t++, count--)
					right2++;    //����
				for (s = j, t = i, count = 5; local[t][s].color == -1 && t >= 0 && count > 0; t--, count--)
					left2++;    //����
				for (s = j, t = i, count = 5; local[t][s].color == -1 && t >= 0 && s >= 0 && count > 0; t--, s--, count--)
					leftup2++;    //����
				for (s = j, t = i, count = 5; local[t][s].color == -1 && t < 15 && s < 15 && count > 0; t++, s++, count--)
					rightdown2++;    //����
				for (s = j, t = i, count = 5; local[t][s].color == -1 && t >= 0 && s < 15 && count > 0; t--, s++, count--)
					leftdown2++;    //����
				for (s = j, t = i, count = 5; local[t][s].color == -1 && t < 15 && s >= 0 && count > 0; t++, s--, count--)
					rightup2++;    //����
				local[i][j].color = 0;
				if ((down1 + up1 >= 6) || (left1 + right1 >= 6) || (leftup1 + rightdown1 >= 6) || (rightup1 + leftdown1 >= 6))
					chess[i][j].value += 10000;    //�����Ƿ����������
				if (down2 + up2 >= 6 || left2 + right2 >= 6 || leftup2 + rightdown2 >= 6 || rightup2 + leftdown2 >= 6)
					chess[i][j].value += 9999;     //�����Ƿ����������
				if (down1 + up1 == 5 || right1 + left1 == 5 || leftdown1 + rightup1 == 5 || leftup1 + rightdown1 == 5) {
					count = 0;        //�������
					if (down1 + up1 == 5) {
						if (j + down1 <= 15 && local[i][j - down1].ifput == 0)
							count++;
						if (j - up1 >= 0 && local[i][j - up1].ifput == 0)
							count++;
					}
					if (right1 + left1 == 5) {
						if (i - left1 >= 0 && local[i - left1][j].ifput == 0)
							count++;
						if (i + right1 <= 15 && local[i + right1][j].ifput == 0)
							count++;
					}
					if (leftdown1 + rightup1 == 5) {
						if (i - leftdown1 >= 0 && j + leftdown1 <= 15 && local[i - leftdown1][j + leftdown1].ifput == 0)
							count++;
						if (i + rightup1 <= 15 && j - rightup1 >= 0 && local[i + right1][j - rightup1].ifput == 0)
							count++;
					}
					if (leftup1 + rightdown1 == 5) {
						if (i - leftup1 >= 0 && j - leftup1 >= 0 && local[i - leftdown1][j - leftup1].ifput == 0)
							count++;
						if (i + rightdown1 <= 15 && j + rightdown1 <= 15 && local[i + rightdown1][j + rightdown1].ifput == 0)
							count++;
					}
					if (count >= 2)
						chess[i][j].value += 3000;
				}
				if (down2 + up2 == 5 || right2 + left2 == 5 || leftdown2 + rightup2 == 5 || leftup2 + rightdown2 == 5) {
					count = 0;   //�������
					if (down2 + up2 == 5) {
						if (j + down2 < 15 && local[i][j + down2].ifput == 0)
							count++;
						if (j - up2 >= 0 && local[i][j - up2].ifput == 0)
							count++;
					}
					if (right2 + left2 == 5) {
						if (i - left2 >= 0 && local[i - left2][j].ifput == 0)
							count++;
						if (i + right2 < 15 && local[i + right2][j].ifput == 0)
							count++;
					}
					if (leftdown2 + rightup2 == 5) {
						if (i - leftdown2 >= 0 && j + leftdown2 <= 15 && local[i - leftdown2][j + leftdown2].ifput == 0)
							count++;
						if (i + rightup2 < 15 && j - rightup2 >= 0 && local[i + right2][j - rightup2].ifput == 0)
							count++;
					}
					if (leftup2 + rightdown2 == 5) {
						if (i - leftup2 >= 0 && j - leftup2 >= 0 && local[i - leftdown2][j - leftup2].ifput == 0)
							count++;
						if (i + rightdown2 < 15 && j + rightdown2 <= 15 && local[i + rightdown2][j + rightdown2].ifput == 0)
							count++;
					}
					if (count >= 2)
						chess[i][j].value += 2999;
				}
				if (down1 + up1 == 4 || right1 + left1 == 4 || leftdown1 + rightup1 == 4 || leftup1 + rightdown1 == 4) {
					count = 0;	amount = 0;     //����������Ŀ��Ӻ������ӣ�
					if (down1 + up1 == 4) {//
						if (j + down1 <= 15 && local[i][j + down1].ifput == 0) {
							if (j - up1 >= 0 && local[i][j - up1].ifput == 0)
								count++;
						}
					}
					if (down1 + up1 == 5) {
						barrier = 0;
						if (j + down1 < 15 && local[i][j + down1].ifput == -1)
							barrier++;
						if (j - up1 >= 0 && local[i][j - up1].ifput == -1)
							barrier++;
						if (barrier == 1)
							amount++;
					}
					if (right1 + left1 == 4) {
						if (i - left1 >= 0 && local[i - left1][j].ifput == 0) {
							if (i + right1 <= 15 && local[i + right1][j].ifput == 0)
								count++;
						}
					}
					if (right1 + left1 == 5) {
						barrier = 0;
						if (i - left1 >= 0 && local[i - left1][j].ifput == -1)
							barrier++;
						if (i + right1 <= 15 && local[i + right1][j].ifput == -1)
							barrier++;
						if (barrier == 1)
							amount++;
					}
					if (leftdown1 + rightup1 == 4) {
						if (i - leftdown1 >= 0 && j + leftdown1 <= 15 && local[i - leftdown1][j + leftdown1].ifput == 0) {
							if (i + rightup1 <= 15 && j - rightup1 >= 0 && local[i + rightup1][j - rightup1].ifput == 0)
								count++;
						}
					}
					if (leftdown1 + rightup1 == 5) {
						barrier = 0;
						if (i - leftdown1 >= 0 && j + leftdown1 <= 15 && local[i - leftdown1][j + leftdown1].ifput == -1)
							barrier++;
						if (i + rightup1 <= 15 && j - rightup1 >= 0 && local[i + rightup1][j - rightup1].ifput == 0)
							barrier++;
						if (barrier == 1)
							amount++;
					}
					if (leftup1 + rightdown1 == 4) {
						if (i - leftup1 >= 0 && j - leftup1 >= 0 && local[i - leftdown1][j - leftup1].ifput == 0) {
							if (i + rightdown1 <= 15 && j + rightdown1 <= 15 && local[i + rightdown1][j + rightdown1].ifput == 0)
								count++;
						}
					}
					if (leftup1 + rightdown1 == 5) {
						barrier = 0;
						if (i - leftup1 >= 0 && j - leftup1 >= 0 && local[i - leftdown1][j - leftup1].ifput == -1)
							barrier++;
						if (i + rightdown1 <= 15 && j + rightdown1 <= 15 && local[i + rightdown1][j + rightdown1].ifput == -1)
							barrier++;
						if (barrier == 1)
							amount++;
					}
					if (count >= 2)
						chess[i][j].value += 110;
					if (count == 1) {
						if (amount >= 1)
							chess[i][j].value += 120;
					}
				}
				if (down2 + up2 == 4 || right2 + left2 == 4 || leftdown2 + rightup2 == 4 || leftup2 + rightdown2 == 4) {
					count = 0; amount = 0;     //����������Ŀ��Ӻ������ӣ�
					if (down2 + up2 == 4) {
						if (j + down2 <= 15 && local[i][j + down2].ifput == 0) {
							if (j - up2 >= 0 && local[i][j - up2].ifput == 0)
								count++;
						}
					}
					if (down2 + up2 == 5) {
						barrier = 0;
						if (j + down2 <= 15 && local[i][j + down2].ifput == -1)
							barrier++;
						if (j - up2 >= 0 && local[i][j - up2].ifput == -1)
							barrier++;
						if (barrier == 1)
							amount++;
					}
					if (right2 + left2 == 4) {
						if (i - left2 >= 0 && local[i - left2][j].ifput == 0) {
							if (i + right2 <= 15 && local[i + right2][j].ifput == 0)
								count++;
						}
					}
					if (right2 + left2 == 5) {
						barrier = 0;
						if (i - left2 >= 0 && local[i - left2][j].ifput == -1)
							barrier++;
						if (i + right2 <= 15 && local[i + right2][j].ifput == -1)
							barrier++;
						if (barrier == 1)
							amount++;
					}
					if (leftdown2 + rightup2 == 4) {
						if (i - leftdown2 >= 0 && j + leftdown2 <= 15 && local[i - leftdown2][j + leftdown2].ifput == 0) {
							if (i + rightup2 <= 15 && j - rightup2 >= 0 && local[i + rightup2][j - rightup2].ifput == 0)
								count++;
						}
					}
					if (leftdown2 + rightup2 == 5) {
						barrier = 0;
						if (i - leftdown2 >= 0 && j + leftdown2 <= 15 && local[i - leftdown2][j + leftdown2].ifput == -1)
							barrier++;
						if (i + rightup2 <= 15 && j - rightup2 >= 0 && local[i + rightup2][j - rightup2].ifput == 0)
							barrier++;
						if (barrier == 1)
							amount++;
					}
					if (leftup2 + rightdown2 == 4) {
						if (i - leftup2 >= 0 && j - leftup2 >= 0 && local[i - leftdown2][j - leftup2].ifput == 0) {
							if (i + rightdown2 <= 15 && j + rightdown2 <= 15 && local[i + rightdown2][j + rightdown2].ifput == 0)
								count++;
						}
					}
					if (leftup2 + rightdown2 == 5) {
						barrier = 0;
						if (i - leftup2 >= 0 && j - leftup2 >= 0 && local[i - leftdown2][j - leftup2].ifput == -1)
							barrier++;
						if (i + rightdown2 <= 15 && j + rightdown2 <= 15 && local[i + rightdown2][j + rightdown2].ifput == -1)
							barrier++;
						if (barrier == 1)
							amount++;
					}
					if (count >= 2)
						chess[i][j].value += 100;
					if (count == 1) {
						if (amount >= 1)
							chess[i][j].value += 115;
					}
				}
				if (down1 + up1 == 3 || right1 + left1 == 3 || leftdown1 + rightup1 == 3 || leftup1 + rightdown1 == 3) {
					count = 0;      //������
					if (down1 + up1 == 3) {
						if (j + down1 <= 15 && local[i][j + down1].ifput == 0) {
							if (j - up1 >= 0 && local[i][j - up1].ifput == 0)
								count++;
						}
					}
					if (right1 + left1 == 3) {
						if (i - left1 >= 0 && local[i - left1][j].ifput == 0) {
							if (i + right1 <= 15 && local[i + right1][j].ifput == 0)
								count++;
						}
					}
					if (leftdown1 + rightup1 == 3) {
						if (i - leftdown1 >= 0 && j + leftdown1 <= 15 && local[i - leftdown1][j + leftdown1].ifput == 0) {
							if (i + rightup1 <= 15 && j - rightup1 >= 0 && local[i + rightup1][j - rightup1].ifput == 0)
								count++;
						}
					}
					if (leftup1 + rightdown1 == 3) {
						if (i - leftup1 >= 0 && j - leftup1 >= 0 && local[i - leftdown1][j - leftup1].ifput == 0) {
							if (i + rightdown1 <= 15 && j + rightdown1 <= 15 && local[i + rightdown1][j + rightdown1].ifput == 0)
								count++;
						}
					}
					if (count >= 2)
						chess[i][j].value += 90;
				}
				if (down2 + up2 == 3 || right2 + left2 == 3 || leftdown2 + rightup2 == 3 || leftup2 + rightdown2 == 3) {
					count = 0;        //������
					if (down2 + up2 == 3) {
						if (j + down2 <= 15 && local[i][j + down2].ifput == 0) {
							if (j - up2 >= 0 && local[i][j - up2].ifput == 0)
								count++;
						}
					}
					if (right2 + left2 == 3) {
						if (i - left2 >= 0 && local[i - left2][j].ifput == 0) {
							if (i + right2 <= 15 && local[i + right2][j].ifput == 0)
								count++;
						}
					}
					if (leftdown2 + rightup2 == 3) {
						if (i - leftdown2 >= 0 && j + leftdown2 <= 15 && local[i - leftdown2][j + leftdown2].ifput == 0) {
							if (i + rightup2 <= 15 && j - rightup2 >= 0 && local[i + rightup2][j - rightup2].ifput == 0)
								count++;
						}
					}
					if (leftup2 + rightdown2 == 3) {
						if (i - leftup2 >= 0 && j - leftup2 >= 0 && local[i - leftdown2][j - leftup2].ifput == 0) {
							if (i + rightdown2 <= 15 && j + rightdown2 <= 15 && local[i + rightdown2][j + rightdown2].ifput == 0)
								count++;
						}
					}
					if (count >= 2)
						chess[i][j].value += 70;
				}
				if (down1 + up1 == 5 || right1 + left1 == 5 || leftdown1 + rightup1 == 5 || leftup1 + rightdown1 == 5) {
					if (down1 + up1 == 5) {       //�������
						if ((j + down1 <= 15 && local[i][j + down1].ifput == 0) || (j - up1 >= 0 && local[i][j - up1].ifput == 0)) {
							if (j + down1 <= 15 && local[i][j + down1].ifput == -1 || j - up1 >= 0 && local[i][j - up1].ifput == -1)
								chess[i][j].value += 80;
						}
					}
					if (right1 + left1 == 5) {
						if ((i - left1 >= 0 && local[i - left1][j].ifput == 0) || (i + right1 <= 15 && local[i + right1][j].ifput == 0)) {
							if ((i + right1 <= 15 && local[i + right1][j].ifput == -1) || (i - left1 >= 0 && local[i - left1][j].ifput == -1))
								chess[i][j].value += 80;
						}
					}
					if (leftdown1 + rightup1 == 5) {
						if ((i - leftdown1 >= 0 && j + leftdown1 <= 15 && local[i - leftdown1][j + leftdown1].ifput == 0) || (i + rightup1 <= 15 && j - rightup1 >= 0 && local[i + right1][j - rightup1].ifput == 0)) {
							if ((i + rightup1 <= 15 && j - rightup1 >= 0 && local[i + rightup1][j - rightup1].ifput == -1) || (i - leftdown1 >= 0 && j + leftdown1 <= 15 && local[i - leftdown1][j + leftdown1].ifput == -1))
								chess[i][j].value += 80;
						}
					}
					if (leftup1 + rightdown1 == 5) {
						if ((i - leftup1 >= 0 && j - leftup1 >= 0 && local[i - leftdown1][j - leftup1].ifput == 0) || (i + rightdown1 <= 15 && j + rightdown1 <= 15 && local[i + rightdown1][j + rightdown1].ifput == 0)) {
							if ((i + rightdown1 <= 15 && j + rightdown1 <= 15 && local[i + rightdown1][j + rightdown1].ifput == -1) || (i - leftup1 >= 0 && j - leftup1 >= 0 && local[i - leftdown1][j - leftup1].ifput == -1))
								chess[i][j].value = 80;
						}
					}
				}
				if (down2 + up2 == 5 || right2 + left2 == 5 || leftdown2 + rightup2 == 5 || leftup2 + rightdown2 == 5) {
					if (down2 + up2 == 5) {    //�������
						if ((j + down2 <= 15 && local[i][j + down2].ifput == 0) || (j - up2 >= 0 && local[i][j - up2].ifput == 0)) {
							if ((j + down2 <= 15 && local[i][j - down2].ifput == -1) || (j - up2 >= 0 && local[i][j - up2].ifput == -1))
								chess[i][j].value += 70;
						}
					}
					if (right2 + left2 == 5) {
						if ((i - left2 >= 0 && local[i - left2][j].ifput == 0) || (i + right2 <= 15 && local[i + right2][j].ifput == 0)) {
							if ((i + right2 <= 15 && local[i + right2][j].ifput == -1) || (i - left2 >= 0 && local[i - left2][j].ifput == -1))
								chess[i][j].value += 70;
						}
					}
					if (leftdown2 + rightup2 == 5) {
						if ((i - leftdown2 >= 0 && j + leftdown2 <= 15 && local[i - leftdown2][j + leftdown2].ifput == 0) || (i + rightup2 <= 15 && j - rightup2 >= 0 && local[i + right2][j - rightup2].ifput == 0)) {
							if ((i + rightup2 <= 15 && j - rightup2 >= 0 && local[i + rightup2][j - rightup2].ifput == -1) || (i - leftdown2 >= 0 && j + leftdown2 <= 15 && local[i - leftdown2][j + leftdown2].ifput == -1))
								chess[i][j].value += 70;
						}
					}
					if (leftup2 + rightdown2 == 5) {
						if ((i - leftup2 >= 0 && j - leftup2 >= 0 && local[i - leftup2][j - leftup2].ifput == 0) || (i + rightdown2 <= 15 && j + rightdown1 <= 15 && local[i + rightdown2][j + rightdown2].ifput == 0)) {
							if ((i + rightdown2 <= 15 && j + rightdown2 <= 15 && local[i + rightdown2][j + rightdown2].ifput == -1) || (i - leftup2 >= 0 && j - leftup2 >= 0 && local[i - leftdown2][j - leftup2].ifput == -1))
								chess[i][j].value += 70;
						}
					}
				}
				if (down1 + up1 == 4 || right1 + left1 == 4 || leftdown1 + rightup1 == 4 || leftup1 + rightdown1 == 4) {
					if (down1 + up1 == 4) {        //�������
						if ((j + down1 <= 15 && local[i][j + down1].ifput == 0) || (j - up1 >= 0 && local[i][j - up1].ifput == 0)) {
							if (j + down1 <= 15 && local[i][j + down1].ifput == -1 || j - up1 >= 0 && local[i][j - up1].ifput == -1)
								chess[i][j].value += 65;
						}
					}
					if (right1 + left1 == 4) {
						if ((i - left1 >= 0 && local[i - left1][j].ifput == 0) || (i + right1 <= 15 && local[i + right1][j].ifput == 0)) {
							if ((i + right1 <= 15 && local[i + right1][j].ifput == -1) || (i - left1 >= 0 && local[i - left1][j].ifput == -1))
								chess[i][j].value += 65;
						}
					}
					if (leftdown1 + rightup1 == 4) {
						if ((i - leftdown1 >= 0 && j + leftdown1 <= 15 && local[i - leftdown1][j + leftdown1].ifput == 0) || (i + rightup1 <= 15 && j - rightup1 >= 0 && local[i + right1][j - rightup1].ifput == 0)) {
							if ((i + rightup1 <= 15 && j - rightup1 >= 0 && local[i + rightup1][j - rightup1].ifput == -1) || (i - leftdown1 >= 0 && j + leftdown1 <= 15 && local[i - leftdown1][j + leftdown1].ifput == -1))
								chess[i][j].value += 65;
						}
					}
					if (leftup1 + rightdown1 == 4) {
						if ((i - leftup1 >= 0 && j - leftup1 >= 0 && local[i - leftup1][j - leftup1].ifput == 0) || (i + rightdown1 <= 15 && j + rightdown1 <= 15 && local[i + rightdown1][j + rightdown1].ifput == 0)) {
							if ((i + rightdown1 <= 15 && j + rightdown1 <= 15 && local[i + rightdown1][j + rightdown1].ifput == -1) || (i - leftup1 >= 0 && j - leftup1 >= 0 && local[i - leftdown1][j - leftup1].ifput == -1))
								chess[i][j].value = 65;
						}
					}
				}
				if (down2 + up2 == 4 || right2 + left2 == 4 || leftdown2 + rightup2 == 4 || leftup2 + rightdown2 == 4) {
					if (down2 + up2 == 4) {           ///�������
						if ((j + down2 <= 15 && local[i][j + down2].ifput == 0) || (j - up2 >= 0 && local[i][j - up2].ifput == 0)) {
							if ((j + down2 <= 15 && local[i][j - down2].ifput == -1) || (j - up2 >= 0 && local[i][j - up2].ifput == -1))
								chess[i][j].value += 60;
						}
					}
					if (right2 + left2 == 4) {
						if ((i - left2 >= 0 && local[i - left2][j].ifput == 0) || (i + right2 <= 15 && local[i + right2][j].ifput == 0)) {
							if ((i + right2 <= 15 && local[i + right2][j].ifput == -1) || (i - left2 >= 0 && local[i - left2][j].ifput == -1))
								chess[i][j].value += 60;
						}
					}
					if (leftdown2 + rightup2 == 4) {
						if ((i - leftdown2 >= 0 && j + leftdown2 <= 15 && local[i - leftdown2][j + leftdown2].ifput == 0) || (i + rightup2 <= 15 && j - rightup2 >= 0 && local[i + right2][j - rightup2].ifput == 0)) {
							if ((i + rightup2 <= 15 && j - rightup2 >= 0 && local[i + rightup2][j - rightup2].ifput == -1) || (i - leftdown2 >= 0 && j + leftdown2 <= 15 && local[i - leftdown2][j + leftdown2].ifput == -1))
								chess[i][j].value += 60;
						}
					}
					if (leftup2 + rightdown2 == 4) {
						if ((i - leftup2 >= 0 && j - leftup2 >= 0 && local[i - leftup2][j - leftup2].ifput == 0) || (i + rightdown2 <= 15 && j + rightdown2 <= 15 && local[i + rightdown2][j + rightdown2].ifput == 0)) {
							if ((i + rightdown2 <= 15 && j + rightdown2 <= 15 && local[i + rightdown2][j + rightdown2].ifput == -1) || (i - leftup2 >= 0 && j - leftup2 >= 0 && local[i - leftdown2][j - leftup2].ifput == -1))
								chess[i][j].value += 60;
						}
					}
				}
				if (down1 + up1 == 3 || right1 + left1 == 3 || leftdown1 + rightup1 == 3 || leftup1 + rightdown1 == 3) {
					if (down1 + up1 == 3) {          //��������
						if ((j + down1 <= 15 && local[i][j + down1].ifput == 0) || (j - up1 >= 0 && local[i][j - up1].ifput == 0)) {
							if (j + down1 <= 15 && local[i][j + down1].ifput == -1 || j - up1 >= 0 && local[i][j - up1].ifput == -1)
								chess[i][j].value += 50;
						}
					}
					if (right1 + left1 == 3) {
						if ((i - left1 >= 0 && local[i - left1][j].ifput == 0) || (i + right1 <= 15 && local[i + right1][j].ifput == 0)) {
							if ((i + right1 <= 15 && local[i + right1][j].ifput == -1) || (i - left1 >= 0 && local[i - left1][j].ifput == -1))
								chess[i][j].value += 50;
						}
					}
					if (leftdown1 + rightup1 == 3) {
						if ((i - leftdown1 >= 0 && j + leftdown1 <= 15 && local[i - leftdown1][j + leftdown1].ifput == 0) || (i + rightup1 <= 15 && j - rightup1 >= 0 && local[i + right1][j - rightup1].ifput == 0)) {
							if ((i + rightup1 <= 15 && j - rightup1 >= 0 && local[i + rightup1][j - rightup1].ifput == -1) || (i - leftdown1 >= 0 && j + leftdown1 <= 15 && local[i - leftdown1][j + leftdown1].ifput == -1))
								chess[i][j].value += 50;
						}
					}
					if (leftup1 + rightdown1 == 3) {
						if ((i - leftup1 >= 0 && j - leftup1 >= 0 && local[i - leftup1][j - leftup1].ifput == 0) || (i + rightdown1 <= 15 && j + rightdown1 <= 15 && local[i + rightdown1][j + rightdown1].ifput == 0)) {
							if ((i + rightdown1 <= 15 && j + rightdown1 <= 15 && local[i + rightdown1][j + rightdown1].ifput == -1) || (i - leftup1 >= 0 && j - leftup1 >= 0 && local[i - leftdown1][j - leftup1].ifput == -1))
								chess[i][j].value += 50;
						}
					}
				}
				if (down2 + up2 == 3 || right2 + left2 == 3 || leftdown2 + rightup2 == 3 || leftup2 + rightdown2 == 3) {
					if (down2 + up2 == 3) {     //��������
						if ((j + down2 <= 15 && local[i][j + down2].ifput == 0) || (j - up2 >= 0 && local[i][j - up2].ifput == 0)) {
							if ((j + down2 <= 15 && local[i][j + down2].ifput == -1) || (j - up2 >= 0 && local[i][j - up2].ifput == -1))
								chess[i][j].value += 45;
						}
					}
					if (right2 + left2 == 3) {
						if ((i - left2 >= 0 && local[i - left2][j].ifput == 0) || (i + right2 <= 15 && local[i + right2][j].ifput == 0)) {
							if ((i + right2 <= 15 && local[i + right2][j].ifput == -1) || (i - left2 >= 0 && local[i - left2][j].ifput == -1))
								chess[i][j].value += 45;
						}
					}
					if (leftdown2 + rightup2 == 3) {
						if ((i - leftdown2 >= 0 && j + leftdown2 <= 15 && local[i - leftdown2][j + leftdown2].ifput == 0) || (i + rightup2 <= 15 && j - rightup2 >= 0 && local[i + right2][j - rightup2].ifput == 0)) {
							if ((i + rightup2 <= 15 && j - rightup2 >= 0 && local[i + right2][j - rightup2].ifput == -1) || (i - leftdown2 >= 0 && j + leftdown2 <= 15 && local[i - leftdown2][j + leftdown2].ifput == -1))
								chess[i][j].value += 45;
						}
					}
					if (leftup2 + rightdown2 == 3) {
						if ((i - leftup2 >= 0 && j - leftup2 >= 0 && local[i - leftdown2][j - leftup2].ifput == 0) || (i + rightdown2 <= 15 && j + rightdown1 <= 15 && local[i + rightdown2][j + rightdown2].ifput == 0)) {
							if ((i + rightdown2 <= 15 && j + rightdown2 <= 15 && local[i + rightdown2][j + rightdown2].ifput == -1) || (i - leftup2 >= 0 && j - leftup2 >= 0 && local[i - leftdown2][j - leftup2].ifput == -1))
								chess[i][j].value += 45;
						}
					}
				}
				if (down1 == 1 && up1 == 1 && left1 == 1 && right1 == 1 && leftup1 == 1 && leftdown1 == 1 && rightup1 == 1 && rightdown1 == 1) {
					if (down2 == 2 || up2 == 2 || left2 == 2 || right2 == 2 || leftup2 == 2 || leftdown2 == 2 || rightup2 == 2 || rightdown2 == 2)
						if (down2 == 2) {     //�߶�
							if (j + down2 < 15 && local[i][j + down2].color == 0)
								chess[i][j].value += 5;
						}
					if (up2 == 2) {
						if (j - up2 >= 0 && local[i][j - up2].color == 0)
							chess[i][j].value += 5;
					}
					if (left2 == 2) {
						if (i - left2 >= 0 && local[i - left2][j].color == 0)
							chess[i][j].value += 5;
					}
					if (right2 == 2) {
						if (i + right2 < 15 && local[i + right2][j].color == 0)
							chess[i][j].value += 5;
					}
					if (leftdown2 == 2) {
						if (i - leftdown2 >= 0 && j + leftdown2 < 15 && local[i - leftdown2][j + leftdown2].color == 0)
							chess[i][j].value += 5;
					}
					if (rightdown2 == 2) {
						if (i + leftdown2 < 15 && j + leftdown2 < 15 && local[i + rightdown2][j + leftdown2].color == 0)
							chess[i][j].value += 5;
					}
					if (leftup2 == 2) {
						if (i - leftdown2 >= 0 && j - leftup2 >= 0 && local[i - leftdown2][j - leftup2].color == 0)
							chess[i][j].value += 5;
					}
					if (rightup2 == 2) {
						if (i + rightup2 < 15 && j - rightup2 >= 0 && local[i + rightup2][j - rightup2].color == 0)
							chess[i][j].value += 5;
					}
				}

				if (down1 == 2 || up1 == 2 || left1 == 2 || right1 == 2 || leftup1 == 2 || leftdown1 == 2 || rightup1 == 2 || rightdown1 == 2)
					chess[i][j].value += 1;   //һ��ʼ���ں����Ա�
			}
		}
	}
}
void drive_ai(const char *json_in, char *json_out, int out_capacity){ 
	// write no more than out_capacity characters into json_out
	// out_capacity is provided by external c# code and will be 100
		
	order = 0;
	for(int i = 0; i < 15; i++){
		for(int j = 0; j < 15; j++){
			chess[i][j].value = 0;
			local[i][j].ifput = 0;
		}
	}

	// global viriable initialization
	parseJSON(json_in); 
	playchess(json_out);
}
//int main() {
//	char json_in[] = "{ \"gameTrace\": [ { \"order\" : 0, \"player\" : \"black\", \"x\" : 0, \"y\" : 0 }, { \"order\" : 1, \"player\" : \"white\", \"x\" : 1, \"y\" : 14 } ] }";
//	char* block = new char[100];
//	memset(block, 0, 100);
//	drive_ai(json_in, block, 100);
//	std::cout << json_in << std::endl;
//	std::cout << block << std::endl;
//	delete[] block;
//	getchar();
//	return 0;
//}