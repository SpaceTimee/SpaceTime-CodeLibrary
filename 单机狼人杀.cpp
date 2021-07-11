/*
时间:			版本:			描述:
19.8.31			1.0.0			代码整体修正,完善原有功能,大量增强游戏内容;
19.9.12			1.1.0			修复大量BUG,调整结尾排版
19.9.13 		1.1.1			修复投票BUG,完成inline函数检查,缩短单个函数体长度
19.9.13			1.1.2			校订代码排版
19.9.13			1.2.0			修改序号0~6为1~7,增加1个函数的inline,修改1段垃圾代码
21.6.27			1.2.1			替换万能头文件,整理代码排版,修复time_t->int的收缩转换错误
21.7.11			1.2.2			正式创建为一个项目，升级为64位程序，少量优化代码
Future/Fornever	?.?.?			修复部分输出一闪而过BUG，修复输入污染缓冲区BUG，添加使用教程
*/

#include<iostream>
#include<windows.h>
#include<iomanip>
#include<algorithm>

using namespace std;

struct position
{
	string pos{};	//职业
	int rank{};	//编号
	bool alive{};	//死活
	bool lucky{};	//是否被守
	bool haveLucky{};	//是否被守过

	position() :alive{ 1 }, lucky{ 0 }, haveLucky{ 0 }{}
	~position() {}
};

struct ticket
{
	int rank{};	//编号 
	int num{};	//票数 

	bool operator<(const ticket& other)const	//为sort服务 
	{
		return this->num > other.num;
	}

	ticket() :num{ 0 } {}
	~ticket() {}
};


position person[9]{};	//人物
ticket castVote[9]{};	//投票

int predictor{}, witch{}, guard{};	//神职特殊处理 
int killedPerson{ 0 },	//狼人杀的人 
piosonedPerson{ 0 },	//女污毒的人 
defendedPerson{ 0 };	//守卫守的人
int goodPerson{ 5 }, badPerson{ 2 };	//好人数与坏人数 
int day{ 0 };	//存活天数

bool milk{ 0 };	//是否喂奶 
bool poisonNum{ 1 }, milkNum{ 1 };	//毒与奶剩余数 

string name[9]{ "","狼人","狼人","女污","预言家","守卫","平民","平民" };

inline void clh()	//缓冲清理 
{
	cin.clear();
	cin.sync();
}

inline void prsToCls()	//回车清屏
{
	getchar();
	clh();
	system("cls");
}

void head()	//初始界面 
{
	system("color c");
	cout << "\n\n\n";
	cout << setw(30) << "狼";	Sleep(500);
	cout << setw(30) << "人";	Sleep(500);
	cout << setw(30) << "杀" << "\n\n\n\n\n\n\n";	Sleep(1000);

	system("color d");
	cout << setw(70) << "Made by 二叉堆STUDIO" << "\n\n\n\n\n\n\n\n\n\n";	Sleep(1000);

	system("color e");
	cout << setw(71) << "加入我们:bhstudio.club";

	prsToCls();
}

void statusTip()	//分配前的准备和提示
{
	srand(unsigned(time(NULL)));	//种子 
	random_shuffle(name + 1, name + 8);	//随机分配人物

	system("color F");
	cout << "你是谁?想知道吗?";

	prsToCls();

	cout << "黄昏请闭眼!";
	Sleep(3000);
	system("cls");
	cout << '\a';

	prsToCls();
}

void statusTime()	//分配职业
{
	statusTip();	//分配前的准备和提示 

	for (int i = 1;i <= 7;i++)
	{
		person[i].pos = name[i];
		person[i].rank = i;
		castVote[i].rank = i;

		if (name[i] == "预言家") predictor = i;
		if (name[i] == "女污") witch = i;
		if (name[i] == "守卫") guard = i;

		cout << i << "号啊,真不幸,你是" << name[i];	//告诉玩家身份

		prsToCls();
		Sleep(3000);

		cout << '\a';
		prsToCls();
	}

	Beep(666, 500);	//睁眼提示 
}

void wolfCheckIn(int& k)	//判断狼人输入是否有误
{
	if (k > 7 || k < 1)
	{
		cout << "你杀歪了,什么都没有杀到";
		k = 0;
	}
	else if (!person[k].alive)
	{
		cout << "达成成就:鞭尸";
		k = 0;
	}
}

void wolfTime()	//狼人时刻 
{
	cout << "月圆之夜,狼人睁眼" << endl;
	cout << "今晚杀谁呢?" << endl;
	cin >> killedPerson;
	clh();
	wolfCheckIn(killedPerson);

	system("cls");
}

void predictorCheckIn(int& p)	//判断预言家输入是否有误
{
	if (p > 7 || p < 1)
	{
		cout << "你眨了下眼,什么都没有看到";
		p = 0;
	}
	else if (!person[p].alive)
	{
		cout << "你只看到了一具冰冷的尸体...";
		p = 0;
	}
}

void predictorTime()	//预言家时刻 
{
	int prop{};

	cout << "预言家,睁开你的预言之眼吧" << endl;
	cout << "偷偷告诉我,你预言了谁的身份?" << endl;
	cin >> prop;
	clh();
	predictorCheckIn(prop);

	if (prop)
	{
		if (person[prop].pos == "狼人")
			cout << "它是...狼";
		else
			cout << "他是...人";
	}

	prsToCls();
}

void witchCheckIn(int& w)	//判断女污输入是否有误
{
	if (w > 7 || w < 1)
	{
		cout << "很不幸,你的药打翻了,没有伤到任何人";
		w = 0;
	}
	else if (!person[w].alive)
	{
		cout << "达成成就:鞭尸";
		w = 0;
	}
}

void witchTime()	//女污时刻 
{
	bool s{};

	cout << "美丽善良的女污请睁眼" << endl;

	if (milkNum)
	{
		cout << "在这充斥着杀戮的血夜中," << killedPerson << "号死了" << endl << "你要用你的复活药水救他吗[1:救他 0:不管他]" << endl;
		cin >> s;
		clh();

		if (s)
		{
			milkNum = 0;	//奶用完 
			milk = 1;	//本局喂了奶
		}
	}

	if (!s && poisonNum)
	{
		cout << "你给谁喂了敌敌畏?请在死亡名单上记下他[不喂请输入0]" << endl;
		cin >> piosonedPerson;
		if (piosonedPerson)
		{
			clh();
			witchCheckIn(piosonedPerson);
			poisonNum = 0;	//毒用完
		}
	}

	system("cls");
}

void guardCheckIn(int& d, bool& hefa)	//判断守卫输入是否有误
{
	if (d > 7 || d < 1)
	{
		cout << "达成成就:空气守护者";
		d = 0;
	}
	else if (!person[d].alive)
	{
		cout << "达成成就:守墓人";
		d = 0;
	}
	else if (person[d].haveLucky)
	{
		cout << "在这个世界,每个人只能拥有一次护盾,再给你一次选择机会,你选择守护谁?" << endl;
		hefa = 0;	//重输 
	}
}

void guardTime()	//守卫时刻
{
	bool hefa{ 1 };

	cout << "守卫请睁眼,来看看这美丽的夜色吧" << endl;
	cout << "你想守护谁?" << endl;

	do
	{
		hefa = 1;	//判断是否需要重输 

		cin >> defendedPerson;
		clh();

		guardCheckIn(defendedPerson, hefa);
	} while (!hefa);

	if (defendedPerson)
	{
		person[defendedPerson].haveLucky = 1;
		person[defendedPerson].lucky = 1;
	}

	system("cls");
}

inline void killOut(const int death)	//杀死 
{
	person[death].alive = 0;

	if (person[death].pos == "狼人")
		badPerson--;
	else
		goodPerson--;
}

void wolfKill()	//狼杀判定 
{
	if (killedPerson)
	{
		if (person[killedPerson].lucky)
		{
			if (milk)
			{
				killOut(killedPerson);
			}
			else
			{
				person[killedPerson].lucky = 0;
				killedPerson = 0;	//没杀成功 
			}
		}
		else if (!milk)
		{
			killOut(killedPerson);
		}
		else	//奶了 
		{
			killedPerson = 0;	//没杀成功 
		}
	}
}

void witchKill()	//女污判定杀
{
	if (piosonedPerson)
	{
		if (person[piosonedPerson].lucky)
		{
			person[piosonedPerson].lucky = 0;
			piosonedPerson = 0;	//没杀成功 
		}
		else
		{
			killOut(piosonedPerson);
		}
	}
}

void killCheckIn()	//杀死死者 
{
	wolfKill();
	witchKill();

	if (killedPerson)
		cout << killedPerson << "号,你死了" << endl;
	if (piosonedPerson && piosonedPerson != killedPerson)
		cout << piosonedPerson << "号,你死了";
	else if (!killedPerson && !piosonedPerson)
		cout << "昨晚平安夜";

	prsToCls();
}

void survivorDisplayTime()	//输出幸存者
{
	cout << "第" << day << "夜的幸存者名单:";

	for (int i = 1;i <= 7;i++)
	{
		if (person[i].alive)
		{
			cout << i << " ";
		}
	}

	prsToCls();
}

void speakTime()	//讨论发言 
{
	cout << "快要投票了,不如讨论一下?请幸存者发言";

	prsToCls();
}

void voteCheckIn(int& v)	//判断投票者输入是否有误
{
	if (v > 7 || v < 1)
	{
		cout << "你弃票了";
		v = 0;
	}
	else if (!person[v].alive)
	{
		cout << "达成成就:鞭尸";
		v = 0;
	}
}

void voteKill()	//票死 
{
	cout << castVote[1].rank << "号,你被众人的怒火吞没了";
	killOut(castVote[1].rank);
}

void voteTime()	//投票 
{
	cout << "投票开始,你的票决定着生死";

	prsToCls();

	for (int i = 1;i <= goodPerson + badPerson;i++)
	{
		int temp;
		cin >> temp;
		voteCheckIn(temp);
		if (temp)
		{
			castVote[temp].num++;
		}

		prsToCls();
	}

	sort(castVote + 1, castVote + 8);

	if (castVote[1].num == castVote[2].num)
	{
		cout << "平票";
	}
	else
	{
		voteKill();
	}

	prsToCls();
}

void numDeal()	//变量重赋值处理
{
	killedPerson = piosonedPerson = defendedPerson = milk = 0;
	for (auto i = 1;i <= 7;i++)	//看这段代码不爽很久了,凭什么不支持for(:),auto是我故意加的,你们要改就改吧 
	{
		castVote[i].num = 0;
	}
}

void bodyTip()	//每晚开始前的准备 
{
	numDeal();

	day++;
	cout << "第" << day << "天" << endl;

	cout << "天黑请闭眼";
	Sleep(5000);
	system("cls");
	cout << '\a';
}

void dayTip()	//天亮后的发言和投票
{
	Beep(666, 500);
	cout << "天亮请睁眼" << endl;

	killCheckIn();	//杀死死者

	if (!goodPerson || !badPerson)
	{
		return;
	}
	else
	{
		survivorDisplayTime();	//输出幸存者名单 

		speakTime();	//发言

		voteTime();	//投票
		survivorDisplayTime();	//再次输出幸存者名单 	
	}
}

void body()	//游戏主体 
{
	while (goodPerson && badPerson)
	{
		bodyTip();	//每晚开始前的准备

		wolfTime();	//狼人回合
		Sleep(3000);
		cout << '\a';

		if (person[predictor].alive)	//预言家回合
		{
			predictorTime();
			Sleep(3000);
			cout << '\a';
		}

		if (person[witch].alive)	//女巫回合 
		{
			witchTime();
			Sleep(3000);
			cout << '\a';
		}

		if (person[guard].alive)	//守卫回合 
		{
			guardTime();
			Sleep(3000);
		}

		dayTip();	//天亮后的发言和投票
	}
}

void godEnding()	//平局 
{
	cout << "狼人与好人互相残杀,两败俱伤,我看着最后一只狼倒在血泊中,我笑了,胜利者是谁呢?" << endl << "达成成就:平局";

	prsToCls();
}

void badEnding()	//狼人获胜 
{
	cout << "狼人杀光了所有好人,狼人胜利,但,你们不觉得还漏了谁吗?" << endl << "达成成就:狼人获胜";

	prsToCls();
}

void goodEnding()
{
	cout << "人与神联手杀光了狼,好人胜利,但看着一只只狼死去,我陷入沉思,你们...真的是好人吗?" << endl << "达成成就:狼人获胜";

	prsToCls();
}

void endingHouse()	//游戏结束 
{
	if (!goodPerson && !badPerson)
		godEnding();	//平局
	if (!goodPerson)
		badEnding();	//狼赢
	if (!badPerson)
		goodEnding();	//人赢
}

void tail()
{
	system("color d");
	cout << "\n\n\n\n\n\n\n\n";
	cout << setw(70) << "欢迎加入二叉堆STUDIO" << "\n\n" << setw(72) << "我们的官网:bhstudio.club";

	prsToCls();
}

int main()
{
	head();	//展示标题 
	statusTime();	//分配职业 
	body();	//正式从第一夜开始
	endingHouse();	//结束屋
	tail();	//展示结尾

	return 0;
}
