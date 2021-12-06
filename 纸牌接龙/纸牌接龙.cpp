#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")//音频

#define COLUMN_GAP  60  //列间距
IMAGE background;  //游戏背景图
int deal = 2;  //发牌次数

struct card {
	int style;  //花色  红桃 方片  梅花 黑桃  分别为 3 4 5 6
	int num;    //数值 1 - 13
};
struct card back[20][16];  //提供原始数据的数组
struct card show[20][16];  //用于显示的数组

int lenArr[16] = {0};  //记录每一列的长度  包括右边的四列

int main();

//随机生成牌
void randCard() {
	int i, j;
	struct card randArr[60];  //用来写入牌  并打乱顺序
	int k = 0;
	//52张牌录入到randArr数组中
	for (i = 1; i <= 13; i++) {
		for (j = 3; j <= 6; j++) {
			randArr[k].num = i;
			randArr[k].style = j;
			k++;
		}
	}

	//randArr打乱顺序
	int n = 80;  //随意交换80次
	srand(unsigned(time(NULL)));  //随机时间种子
	while (n--) {
		int t1 = rand() % k;
		int t2 = rand() % k;
		struct card tmp = randArr[t1];
		randArr[t1] = randArr[t2];
		randArr[t2] = tmp;
	}

	//将randArr[]赋值给back[][]
	for (i = 1; i <= 8; i++) {
		for (j = 1; j <= 7; j++) {
			back[i][j] = randArr[--k];
			if (k <= 0) {
				break;
			}
		}
		if (k <= 0) {
			break;
		}
	}
	//给show[][]赋值
	for (i = 1; i <= 6; i++) {
		for (j = 1; j <= 7; j++) {
			if (i <= 2) {
				show[i][j].num = -1;
				show[i][j].style = 63;  //为?
			}
			else {
				show[i][j] = back[i][j];
			}
		}
	}

}

//计算每一列的长度
void calcLen() {
	int i, j;
	for (i = 1; i <= 11; i++) {
		for (j = 1; j <= 19; j++) {
			if (show[j][i].num == 0) {
				lenArr[i] = j - 1;
				break;
			}
		}
	}
}

//获得要显示的数组内容
void showCard() {
	int i;
	for (i = 1; i <= 7; i++) {
		if (show[lenArr[i]][i].num == -1) {
			if (lenArr[i] == 2) {
				show[lenArr[i]][i] = back[lenArr[i]][i];
			}
			else if (lenArr[i] == 1) {
				show[lenArr[i]][i] = back[lenArr[i]][i];
			}
		}
	}
}

//满足条件的card  移动到右边的四个格子
bool meetCard() {
	int i,j;
	//右格子赋值
	for (i = 12; i <= 15; i++) {  //右格子为空时
		if (show[1][i].num == 0) {
			for (j = 1; j <= 11; j++) {
				if (show[lenArr[j]][j].num == 1 && show[lenArr[j]][j].style == i - 9) {
					show[1][i] = show[lenArr[j]][j];  //赋值给右边格  
					lenArr[i]++;
					show[lenArr[j]][j].num = 0;
					show[lenArr[j]][j].style = 0;
					lenArr[j]--;  //列长度减一
					return true;
				}
			}
		}
		else {  //右格子里面有牌时
			for (j = 1; j <= 11; j++) {
				if (show[lenArr[j]][j].num == show[lenArr[i]][i].num + 1 && show[lenArr[j]][j].style == i - 9) {
					lenArr[i]++;
					show[lenArr[i]][i] = show[lenArr[j]][j];  //赋值给右边格  
					show[lenArr[j]][j].num = 0;
					show[lenArr[j]][j].style = 0;
					lenArr[j]--;  //列长度减一
					return true;
				}
			}
		}
	}
	return false;
	
}

//判断是否还可以移动牌
bool judgeMove() {
	int i, j;

	//左上角还有空
	for (i = 8; i <= 11; i++) {
		if (lenArr[i] == 0) {
			return true;
		}
	}

	//1-11列有牌可以移动
	for (i = 1; i <= 11; i++) {
		for (j = 1; j <= 7; j++) {
			if (show[lenArr[i]][i].style == show[lenArr[j]][j].style &&
				show[lenArr[i]][i].num == show[lenArr[j]][j].num - 1 || show[lenArr[i]][i].num == 13 && lenArr[j] == 0) {   //小于1  没有牌放k
				return true;
			}
		}
	}

	return false;

}

//绘制游戏
void drawGame() {
	putimage(0, 0, &background);  //放置背景图片

	//绘制列号  
	setcolor(BLACK);          //设置颜色
	settextstyle(25, 0, "华文新魏"); //设置文字样式
	setbkmode(TRANSPARENT);       //文字背景透明
	int i,j;
	char str[6];
	//  1 - 7号
	for (i = 1; i <= 7; i++) {
		sprintf(str, "%d", i); //将int转为字符串
		outtextxy(220 + i * COLUMN_GAP, 120, str);
	}
	line(240, 150, 700, 150);   //分割线
	//  8 - 11号
	for (i = 8; i <= 11; i++) {
		sprintf(str, "%d", i); //将int转为字符串
		outtextxy(220 + (i - 7) * COLUMN_GAP, 30, str);
	}
	//绘制4个放牌的格子
	for (i = 1; i <= 4; i++) {
		rectangle(200 + i * COLUMN_GAP, 60, 200 + (i + 1) * COLUMN_GAP, 100);
	}
	//绘制右侧存放a-k的4个格子
	for (i = 1; i <= 4; i++) {
		rectangle(500 + i * COLUMN_GAP, 60, 500 + (i + 1) * COLUMN_GAP, 100);
	}

	setcolor(BLACK);          //设置颜色
	settextstyle(20, 0, "terminal"); //设置文字样式
	setbkmode(TRANSPARENT);       //文字背景透明
	char str2[6];
	for (i = 1; i <= 15; i++) {
		for (j = 1; j <= lenArr[i]; j++) {
			//判断花色
			if (show[j][i].style <= 4) {
				setcolor(RED);
				sprintf(str, "%c", show[j][i].style);
			}
			else {
				setcolor(BLACK);
				sprintf(str, "%c", show[j][i].style);
			}


			//判断数字
			switch (show[j][i].num){
			case 1:
				strcat(str, "A");
				break;
			case 11:
				strcat(str, "J");
				break;
			case 12:
				strcat(str, "Q");
				break;
			case 13:
				strcat(str, "K");
				break;
			default:
				sprintf(str2, "%d", show[j][i].num);
				strcat(str, str2);
				break;
			}

			if (i <= 7) {
				outtextxy(210 + i * COLUMN_GAP, 160 + j * 30, str);
			}
			else if(i<=11){
				outtextxy(160 + (i-6) * COLUMN_GAP, 40 + j * 30, str);
			}
		}
		if(i > 11 && lenArr[i] > 0) {  //右格子
			outtextxy(160 + (i - 5) * COLUMN_GAP, 70, str);
		}
	}

}

//玩游戏
void playGame() {
	int c1, c2;
	int i, j;
	while (1) {
		if (lenArr[12] == 13 && lenArr[13] == 13 && lenArr[14] == 13 && lenArr[15] == 13) {  //游戏胜利
			cleardevice();  //清空画布
			drawGame();
			printf("\n\n游戏胜利！！！\n");
			while (1) {
				printf("-1重新开始，-2退出游戏,请选择:");
				scanf("%d", &c1);
				if (c1 == -1) {
					main();
				}
				else if (c1 == -2) {
					exit(0);
				}
			}

		}
		while (meetCard()) { //满足条件的card  移动到右边的四个格子
			showCard();  //判断有没有1 - 2 行需不需要翻开
		}
		showCard();  //判断有没有1 - 2 行需不需要翻开
		if (!judgeMove()) {  //没有牌可以移动
			if (deal == 0) {//游戏失败
				printf("\n\n游戏失败！！！\n");
				while (1) {
					printf("-1重新开始，-2退出游戏,请选择:");
					scanf("%d", &c1);
					if (c1 == -1) {
						main();
					}
					else if (c1 == -2) {
						exit(0);
					}
				}
			}
			else if (deal == 2) {
				deal--;
				for (i = 1; i <= 7; i++) {  //1-7每一列加一  并把back赋值给他
					lenArr[i]++;
					show[lenArr[i]][i] = back[7][i];
				}
			}
			else if (deal == 1) {
				deal--;
				for (i = 1; i <= 3; i++) {  //1-3每一列加一  并把back赋值给他
					lenArr[i]++;
					show[lenArr[i]][i] = back[8][i];
				}
			}
		}
		cleardevice();  //清空画布
		drawGame();
		printf("输入0发牌 -1重新开始，-2退出游戏\n");
		printf("请选择列:");
		scanf("%d", &c1);
		if (c1 == 0) {  //发牌
			if (deal == 2) {
				deal--;
				for (i = 1; i <= 7; i++) {  //1-7每一列加一  并把back赋值给他
					lenArr[i]++;
					show[lenArr[i]][i] = back[7][i];
				}
			}
			else if (deal == 1) {
				deal--;
				for (i = 1; i <= 3; i++) {  //1-3每一列加一  并把back赋值给他
					lenArr[i]++;
					show[lenArr[i]][i] = back[8][i];
				}
			}
			else {
				printf("没有手牌了！\n");
			}
			continue;
		}
		else if (c1 == -1) {
			main();
		}
		else if (c1 == -2) {
			exit(0);
		}
		printf("请输入要放到第几列:");
		scanf("%d", &c2);
		if (c1 == c2) {  //相同不处理
			continue;
		}
		if (c2 <= 7) {   //1-7列
			if (show[lenArr[c1]][c1].style == show[lenArr[c2]][c2].style &&
				show[lenArr[c1]][c1].num == show[lenArr[c2]][c2].num - 1 || show[lenArr[c1]][c1].num == 13 && lenArr[c2] == 0) {   //小于1     没有牌放k
				lenArr[c2]++;
				show[lenArr[c2]][c2] = show[lenArr[c1]][c1];
				show[lenArr[c1]][c1].num = 0;
				show[lenArr[c1]][c1].style = 0;
				lenArr[c1]--;
			}
			else {
				printf("移动失败！\n");
			}
		}
		else if(c2 <= 11) {  //8-11列
			if (lenArr[c2] == 0) {   //上面没有牌的时候才可以移动
				lenArr[c2]++;
				show[lenArr[c2]][c2] = show[lenArr[c1]][c1];
				show[lenArr[c1]][c1].num = 0;
				show[lenArr[c1]][c1].style = 0;
				lenArr[c1]--;
			}
			else {
				printf("移动失败！\n");
			}
		}
		else {
			printf("移动失败！\n");
		}
	}
}

int main() {
	//播放音乐
	mciSendString("open music.mp3 alias game", nullptr, 0, nullptr);
	mciSendString("play game repeat", nullptr, 0, nullptr);
	loadimage(&background, "background.jpg", 1000, 600);  //读取背景图片
	initgraph(1000, 600,SHOWCONSOLE);  //生成画布

	memset(back, 0, sizeof(back));    //清零
	memset(show, 0, sizeof(show));    //清零
	memset(lenArr, 0, sizeof(lenArr));    //清零
	deal = 2;  //初始化手牌
	randCard();
	/*int j;
	for (i = 1; i <= 8; i++) {
		for (j = 1; j <= 7; j++) {
			printf("\t%c %d", back[i][j].style,back[i][j].num);
		}
		printf("\n");
	}*/
	calcLen();
	drawGame();
	playGame();
	
}