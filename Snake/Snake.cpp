#include<stdio.h>
//时间函数
//#include<time.h>
//调用windowsAPI
#include<windows.h>
#include<stdlib.h>
#include <time.h>
//#include <cstdlib>  //包含stdlib库

//用枚举
//蛇的状态(status)
 //蛇的状态，U：上 ；D：下；L:左 R：右
enum { U = 1, D, L, R };
/*
★：按住键盘Alt，在小键盘输入41455后放开Alt
●：按住键盘Alt，在小键盘输入41457后放开Alt
■：按住键盘Alt，在小键盘输入41462后放开Alt
*/

typedef struct SNAKE //蛇身的一个节点
{
	int x;
	int y;
	struct SNAKE* next;  //结构体指针
}snake;

//全局变量//
int score = 0, add = 10;//总得分与每次吃食物得分。

int status;//status是状态

snake* head, * food;//蛇头指针，食物指针

snake* q;//遍历蛇的时候用到的指针

int endgamestatus = 0; //游戏结束的情况，1：撞到墙；2：咬到自己；3：主动退出游戏。

int S_Speed = 150;

//声明全部函数//

//-在WindowsConcsole 相应的坐标位执行相应的程序
void Pos();

//-程序初始化函数(包含下方)(其中包含system("mode con cols=100 lines=30")定义WindowsConsole(控制台窗口)的大小)
void gamestart();
//-刚开始打开程序的界面
void welcometogame();
//-构造贪吃蛇游戏基本框架
void creatMap();
//-初始化蛇
void initsnake();
//-初始化食物
void createfood();

//-蛇移动利用WindowsAPI对键盘的读取来控制
void gamecircle();
//对蛇的状态进行调整(移动蛇)
void snakemove();
//-是否撞到墙的函数判断
void cantcrosswall();
//-咬到自己的函数判断
void biteself();


//-对程序的暂停
void pause();

//-利用endgamestatus来判断游戏是否结束,并且显示是如何结束的
void endgame();

void Pos(int x, int y)//设置光标位置
{
	//COORD是Windows API中定义的一种结构，表示一个字符在控制台屏幕上的坐标。
	/*
	定义:
	typedef struct _COORD {
	SHORT X;
	SHORT Y;
	} COORD, *PCOORD;
	*/
	COORD pos;

	HANDLE hOutput;

	//(x,y)
	pos.X = x;
	pos.Y = y;

	//GetStdHandle是一个Windows API函数。它用于从一个特定的标准设备（标准输入、标准输出或标准错误）中取得一个句柄（用来标识不同设备的数值）。可以嵌套使用。
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	//SetConsoleCursorPosition是一个window api;作用是设置控制台(cmd)光标位置
	SetConsoleCursorPosition(hOutput, pos);  //转移到(x,y)坐标上的光标位置
}

void creatMap()//创建地图
{
	int i;
	for (i = 0; i < 57; i += 2)//打印上下边框
	{
		Pos(i, 0);  //将光标指向(i,0)坐标处
		printf("★");  //每一个★占横坐标两个
		Pos(i, 29);
		printf("★");
	}
	for (i = 1; i < 29; i++)//打印左右边框(28个)
	{
		Pos(0, i);
		printf("★");
		Pos(56, i);
		printf("★");
	}
	if (score < 200)
	{
		Pos(63, 4);
		printf("200分整一个好吗= =");
	}
	else if (score >= 200 || score <= 500)
	{
		Pos(63, 4);
		printf("都200多分了???");
	}
	else if (score > 500)
	{
		Pos(63, 4);
		printf("真有你的!");
	}

	Pos(63, 7);
	printf("得分：%d ", score);
	Pos(63, 8);
	printf("每个食物得分：%d分", add);
	Pos(63, 9);
	printf("初始5档(共有11个档位)");
	Pos(63, 10);
	printf("0(1) 1(2) 2(4) 3(6) 4(8) 5(10)");
	Pos(63, 11);
	printf("6(12) 7(14) 8(16) 9(18) 10(20)");
	Pos(63, 13);
	printf("不能穿墙，不能咬到自己\n");
	Pos(64, 14);
	printf("用↑.↓.←.→分别控制蛇的移动");
	Pos(63, 15);
	printf("F1 为加速，F2 为减速\n");
	Pos(63, 16);
	printf("ESC<-退出游戏 SPACE<-暂停游戏");
	Pos(63, 17);
	printf("无法在游戏暂停时按ESC弹出游戏哦!");
	Pos(63, 18);
	printf("(如果想退出请在游戏执行阶段Press ESC)");
	Pos(63, 21);
	printf("了解规则了吧,直接起飞!!!!!");
}

void initsnake()//初始化蛇身思想(链表)(头部指向尾部)(尾部<-身子<-头)
{
	//尾部
	snake* tail;

	int i;
	tail = (snake*)malloc(sizeof(snake));//从蛇尾开始，头插法，以x,y设定开始的位置//
	//初始化尾部坐标(24.5)
	tail->x = 24;
	tail->y = 5;
	tail->next = NULL;
	for (i = 1; i <= 4; i++)  //初始蛇长度为5 (24.5)<-(26.5)<-(28.5)<-(30.5)<-(32.5)
	{
		head = (snake*)malloc(sizeof(snake));
		head->next = tail;
		head->x = 24 + 2 * i;
		head->y = 5;
		tail = head;  //head是蛇头指针
	}
	//循环结束之后tail将别为蛇头结构体指针

	while (tail != NULL)//从头到为，输出蛇身
	{
		Pos(tail->x, tail->y);
		printf("●");
		tail = tail->next;
	}
	//直到循环到蛇尾的tail->next是NULL(空指针时)结束画蛇
}

void biteself()//判断是否咬到了自己
{
	//结构体指针self是指除蛇头以外的链表
	snake* self;
	self = head->next;
	while (self != NULL)
	{
		if (self->x == head->x && self->y == head->y)  //且(找出元素重合)
		{
			//第二种游戏结束情况
			endgamestatus = 2;
			endgame();
		}
		self = self->next;  //转移到下一个链表元素
	}
}

void createfood()//随机出现食物(蛇吃了食物之后相当于在蛇头加了一节食物(蛇身))
{
	snake* food_1;

	//设置时间为随机数种子
	srand((unsigned)time(NULL));

	food_1 = (snake*)malloc(sizeof(snake));

	food_1->x = (rand() % 27) * 2 + 2;  //随机x坐标(2 4 6  ...54)

	while ((food_1->x % 2) != 0) //保证其为偶数，使得食物能与蛇头对其(因为■占两个x单位长度)
	{
		food_1->x = (rand() % 27) * 2 + 2;  //随机x坐标(2 4 6  ...54)
	}
	food_1->y = rand() % 28 + 1;  //随机y坐标(1-29)

	q = head;  //q为全局变量,用于

	while (q->next != NULL)
	{
		if (q->x == food_1->x && q->y == food_1->y) //判断蛇身是否与食物重合
		{
			//如果生成食物与蛇身重合则释放食物结构体指针
			free(food_1);
			//然后递归重新套用创建食物函数
			createfood();
		}
		//遍历到下一个元素
		q = q->next;
	}
	Pos(food_1->x, food_1->y);

	printf("■");
	food = food_1;  //然后将(临时创建的食物结构体指针)food_1赋值给(全局变量结构体指针)food
}

void cantcrosswall()//不能穿墙(穿墙判断函数)
{
	if (head->x == 0 || head->x == 56 || head->y == 0 || head->y == 30)  //或者
	{
		endgamestatus = 1;

		//游戏结束函数
		endgame();
	}
}

void snakemove()//蛇前进,上U,下D,左L,右R
{
	//蛇的下一个蛇头
	snake* nexthead;

	nexthead = (snake*)malloc(sizeof(snake));
	//如果向上移动
	if (status == U)
	{
		//横坐标x不变
		nexthead->x = head->x;
		//总左边y-1
		nexthead->y = head->y - 1;

		//将原先的头指针赋值给 新的头指针的下一个元素
		nexthead->next = head;
		//将新的头指针赋值给(全局变量头指针)
		//(更新头指针)
		head = nexthead;

		if (nexthead->x == food->x && nexthead->y == food->y)//如果下一个有食物//
		{
			//得分
			score = score + add;

			//生成新的食物
			createfood();
		}
		else //如果没有食物//
		{
			//找到蛇尾
			q = head;
			while (q->next->next != NULL)
			{
				q = q->next;
			}
			////将原先尾部残留的蛇尾用空白替换掉
			//Pos(q->next->x, q->next->y);
			//printf(" ");

			//删除蛇尾
			free(q->next);
			q->next = NULL;
		}
	}
	if (status == D)
	{
		nexthead->x = head->x;
		nexthead->y = head->y + 1;

		//将原先的头指针赋值给 新的头指针的下一个元素
		nexthead->next = head;
		//将新的头指针赋值给(全局变量头指针)
		//(更新头指针)
		head = nexthead;

		if (nexthead->x == food->x && nexthead->y == food->y) //有食物
		{
			//得分
			score = score + add;

			//生成新的食物
			createfood();
		}
		else //没有食物
		{
			//找到蛇尾
			q = head;
			while (q->next->next != NULL)
			{
				q = q->next;
			}

			//在结尾使用过一次system("cls"); 所以不用着急清除蛇尾，在结尾部分直接画蛇
			////将原先尾部残留的蛇尾用空白替换掉
			//Pos(q->next->x, q->next->y);
			//printf(" ");

			//删除蛇尾
			free(q->next);
			q->next = NULL;
		}
	}
	if (status == L)
	{
		nexthead->x = head->x - 2;
		nexthead->y = head->y;

		//将原先的头指针赋值给 新的头指针的下一个元素
		nexthead->next = head;
		//将新的头指针赋值给(全局变量头指针)
		//(更新头指针)
		head = nexthead;

		if (nexthead->x == food->x && nexthead->y == food->y)//有食物
		{
			//得分
			score = score + add;

			//生成新的食物
			createfood();
		}
		else //没有食物
		{
			//找到蛇尾
			q = head;
			while (q->next->next != NULL)
			{
				q = q->next;
			}

			//在结尾使用过一次system("cls"); 所以不用着急清除蛇尾，在结尾部分直接画蛇
			////将原先尾部残留的蛇尾用空白替换掉
			//Pos(q->next->x, q->next->y);
			//printf(" ");

			//删除蛇尾
			free(q->next);
			q->next = NULL;
		}
	}
	if (status == R)
	{
		nexthead->x = head->x + 2;
		nexthead->y = head->y;

		//将原先的头指针赋值给 新的头指针的下一个元素
		nexthead->next = head;
		//将新的头指针赋值给(全局变量头指针)
		//(更新头指针)
		head = nexthead;

		if (nexthead->x == food->x && nexthead->y == food->y)//有食物
		{
			//得分
			score = score + add;

			//生成新的食物
			createfood();
		}
		else //没有食物
		{
			//找到蛇尾
			q = head;
			while (q->next->next != NULL)
			{
				q = q->next;
			}

			//在结尾使用过一次system("cls"); 所以不用着急清除蛇尾，在结尾部分直接画蛇
			////将原先尾部残留的蛇尾用空白替换掉
			//Pos(q->next->x, q->next->y);
			//printf(" ");

			//删除蛇尾(释放掉内存空间)
			free(q->next);
			q->next = NULL;
		}
	}

	//判断撞墙函数(第一种游戏结束情况)
	cantcrosswall();

	//判断蛇是否咬到自己的函数判断
	biteself(); //判断是否会咬到自己

	//清屏
	system("cls");
	//HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	//CONSOLE_CURSOR_INFO cci;
	//GetConsoleCursorInfo(hOut, &cci);
	//cci.bVisible = FALSE;
	//SetConsoleCursorInfo(hOut, &cci);

	//1.重置边框(重置图形边框)
	creatMap();

	//2.画蛇
	q = head;
	while (q->next != NULL)
	{
		Pos(q->x, q->y);
		printf("●");
		q = q->next;
	}
	Pos(q->x, q->y);
	printf("●");


	//4.画食物
	Pos(food->x, food->y);
	printf("■");

}

void pause()//暂停
{
	while (1)
	{
		//延缓程序
		Sleep(300);

		//利用Windows API获取键盘输入SPACE
		if (GetAsyncKeyState(VK_SPACE))
		{
			//弹出while循环
			break;
		}
	}
}

void gamecircle()//循环控制游戏
{
	status = R;  //蛇的起始方向朝右

	DWORD t1, t2;
	//取当前时刻
	t1 = t2 = GetTickCount();

	while (1)
	{
		//利用API获取键盘输入
		if (GetAsyncKeyState(VK_UP) && status != D)  //如果向上则无法向下直接掉头
		{
			status = U;
		}
		else if (GetAsyncKeyState(VK_DOWN) && status != U)  //如果向下则无法向上直接掉头
		{
			status = D;
		}
		else if (GetAsyncKeyState(VK_LEFT) && status != R)  //如果向左则无法向右直接掉头
		{
			status = L;
		}
		else if (GetAsyncKeyState(VK_RIGHT) && status != L)  //如果向右则无法向左直接掉头
		{
			status = R;
		}
		else if (GetAsyncKeyState(VK_SPACE))  //如果Windows API读取到SPACE 则执行暂停程序函数
		{
			pause();
		}
		else if (GetAsyncKeyState(VK_ESCAPE))  //如果Windows API读取到ESC 则执行暂停程序函数
		{
			//第三种情况(直接用ESC弹出游戏)
			endgamestatus = 3;
			break;  //直接弹出while循环
		}

		//控制蛇的移动
		if (t2 - t1 > S_Speed)
		{
			if (GetAsyncKeyState(VK_F1))  //提速
			{
				//控制S_Speed最小为100
				if (S_Speed >= 110)  //(初始值200)
				{
					//初始速度0档(100)
					//总共有5档快速1(90).2(80).3(70).4(60).5(50)
					S_Speed = S_Speed - 10;
					//每个食物的得分
					add = add + 2;

					if (S_Speed == 190)
					{
						add = 2;  //防止减到1之后再加回来有错
					}
				}
			}
			if (GetAsyncKeyState(VK_F2))  //降速
			{
				//控制S_Speed最大为150
				if (S_Speed <= 190)
				{
					//初始速度0(100)
					//总共有五档慢速-1(110).-2(120).-3(130).-4(140).-5(150)
					S_Speed = S_Speed + 10;
					//每个食物加分
					add = add - 2;
					if (S_Speed == 200)  //
					{
						add = 1; //保证最低分为1
					}
				}
			}

			//调用蛇移动函数
			snakemove();
			t1 = t2;
		}
		//取当前时刻
		t2 = GetTickCount();

	}
}

void welcometogame()//开始界面
{
	Pos(40, 12);

	printf("欢迎来到贪食蛇游戏！");
	Pos(40, 16);
	printf("飞！！芜湖！！起飞！！");
	Pos(40, 25);

	system("pause");
	//清屏操作
	system("cls");

	Pos(13, 12);
	printf("用↑(UP).↓(DOWN).←(LEFT).→(RIGHT)分别控制蛇的移动， F1 为加速，2 为减速\n");
	Pos(13, 13);
	printf("加速将能得到更高的分数。\n");

	system("pause");
	//清屏操作
	system("cls");
}

void endgame()//结束游戏
{
	system("cls");
	Pos(24, 12);
	//撞墙
	if (endgamestatus == 1)
	{
		printf("对不起，您撞到墙了。游戏结束.");
	}
	//咬自己
	else if (endgamestatus == 2)
	{
		printf("对不起，您咬到自己了。游戏结束.");
	}
	//按ESC
	else if (endgamestatus == 3)
	{
		printf("您的已经结束了游戏。");
	}
	Pos(24, 13);
	printf("您的得分是%d\n", score);
	system("pause");
	//直接弹出程序
	exit(0);
}

void gamestart()//游戏初始化
{
	//调整控制台的长(lines)和宽(cols)
	system("mode con cols=100 lines=30");

	//初始化游戏窗口
	welcometogame();

	//画地图边框
	creatMap();

	//初始化蛇
	initsnake();

	//初始化食物
	createfood();
}

int main()
{
	//游戏开始
	gamestart();

	//游戏循环
	gamecircle();

	//游戏结束
	endgame();

	return 0;
}