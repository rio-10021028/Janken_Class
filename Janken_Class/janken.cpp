#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include "janken.h"
using namespace std;

enum RPSHands
{
	ROCK,		// グー   > 0
	SCISSORS,   // チョキ > 1
	PAPER       // パー   > 2
};

enum RPSResults
{
	LOSE,  // 負け > 0
	WIN,   // 勝ち > 1
	TIE    // 分け > 2
};

const char* RPSHandMessage[]
{
	"グー",
	"チョキ",
	"パー",
};

const char* RPSResultsMessage[]
{
	"負け...",
	"勝ち！！",
};

struct Result
{
	int lose;
	int win;
};

class JankenPlayer
{
public:
	bool user;
	int wins;
	RPSHands hand;

	JankenPlayer(const char* pname);
	void jankenHandMaker();
};

// プロトタイプ宣言群
static RPSHands userHandMaker();
static RPSHands cpuHandMaker();

JankenPlayer::JankenPlayer(const char* pname)
{
	if (pname == nullptr)
	{
		cout << "エラー！";
	}
	else
	{
		cout << pname;
	}

	// user かどうか
	int num;

	num = inputInt(" は cpu > 0, user > 1 ");

	switch (num)
	{
	case 0:
		user = false;
		break;
	case 1:
		user = true;
		break;
	default:
		user = false;
	}

	// 他項目初期化
	wins = 0;
	hand = ROCK;
}

void JankenPlayer::jankenHandMaker()
{
	if (user)
	{
		hand = userHandMaker();
	}
	else
	{
		hand = cpuHandMaker();
	}
}

static RPSHands userHandMaker()
{
	while (true)
	{
		int num = -1;

		cout << "グー   > 0" << endl
			<< "チョキ > 1" << endl
			<< "パー   > 2" << endl;
		num = inputInt("じゃんけん... > ");
		if (0 <= num && num <= 2)
		{
			return (RPSHands)num;
		}
		else
		{
			cout << "0, 1, 2 で入力してください" << endl;
		}
	}
}

static void initRand()
{
	srand((unsigned int)time(nullptr));
}

int inputInt(const char* message)
{
	int num;

	while (true)
	{
		cout << message << flush;

		if (cin >> num)
		{
			return num;
		}
		else
		{
			cout << "無効な入力です" << endl;

			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
}

static RPSHands cpuHandMaker()
{
	int hand = rand() % 3;

	return (RPSHands)hand;
}

static RPSResults judgeResult(RPSHands user, RPSHands cpu)
{
	int result = ((user - cpu) + 2) % 3;

	switch (result)
	{
	case LOSE:
		return LOSE;
	case WIN:
		return WIN;
	case TIE:
		return TIE;
	default:
		return LOSE;
	}
}

static RPSResults janken(JankenPlayer* pp1, JankenPlayer* pp2)
{

	RPSResults currentResult = LOSE;

	// あいこならループ
	while (true)
	{
		bool inputFlag = true;

		// p1の手決定
		pp1->jankenHandMaker();

		// cpuの手決定
		pp2->jankenHandMaker();

		// 手を表示
		cout << "player 1 > " << RPSHandMessage[pp1->hand] << endl
			<< "player 2 > " << RPSHandMessage[pp2->hand] << endl << endl;

		// ジャッジ
		currentResult = judgeResult(pp1->hand, pp2->hand);

		// あいこじゃなければ結果を返す
		if (currentResult != TIE)
		{
			return currentResult;
		}

		cout << "あいこ！　もう一度！\n\n";
	}
}

void jankenSimurator()
{
	initRand();
	int round = 1;
	int num = 0;

	cout << "じゃんけんゲームへようこそ！" << endl;

	// n本か決まるまでループ
	num = 0;

	while (true)
	{
		num = inputInt("何本先取にしますか > ");
		if (num <= 0)
		{
			cout << "入力しなおしてください" << endl;
			continue;
		}
		else if (0 < num && num <= 10)
		{
			break;
		}
		else
		{
			cout << "大きすぎるから、10 にしておくね" << endl;
			num = 10;
			break;
		}
	}

	cout << num << " 本先取！　じゃんけんゲーム！\n\n";

	JankenPlayer p1("player 1"), p2("player 2");

	// n本終わるまでループ
	while (true)
	{
		RPSResults currentResult = LOSE;
		cout << "===== 第 " << round << " 回戦 =====\n\n";

		// Janken 呼び出し
		currentResult = janken(&p1, &p2);

		// 結果
		switch (currentResult)
		{
		case LOSE:
			p2.wins++;
			break;
		case WIN:
			p1.wins++;
		}

		// n本終わったらループ脱出
		if (p1.wins == num || p2.wins == num)
		{
			break;
		}

		// 暫定結果表示
		cout << RPSResultsMessage[currentResult] << endl
			<< "----- 暫定結果 ----- " << endl
			<< "勝ち > " << p1.wins << endl
			<< "負け > " << p2.wins << endl << endl;

		// round数加算
		round++;
	}

	// 最終結果表示
	cout << "===== 最終結果 =====\n\n"
		<< "勝ち > " << p1.wins << endl
		<< "負け > " << p2.wins << endl
		<< "player 1 の " << (p1.wins == num ? RPSResultsMessage[WIN] : RPSResultsMessage[LOSE])
		<< endl << endl;

}