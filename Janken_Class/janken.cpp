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

// class
class JankenPlayer
{
private:
	const char* name;
	bool user;
	Result result;
	RPSHands hand;

public:
	JankenPlayer(const char* pname);
	void userOrNot();
	RPSHands getHand();
	void setHand();
	Result getResult();
	void setResult(RPSResults index, int changeValue = 1);
	const char* showName();
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

	name = pname;

	// 他項目初期化
	user = false;
	result.win = 0;
	result.lose = 0;
	hand = ROCK;
}

void JankenPlayer::userOrNot()
{
	// user かどうか
	int num;

	while (true)
	{
		cout << name << "は";
		num = inputInt("cpu > 0, user > 1 ");

		switch (num)
		{
		case 0:
			user = false;
			return;
		case 1:
			user = true;
			return;
		default:
			cout << "0, 1 で入力してください" << endl;
		}
	}
}

void JankenPlayer::setHand()
{
	// それぞれの関数を呼び出し
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

static RPSHands cpuHandMaker()
{
	int hand = rand() % 3;

	return (RPSHands)hand;
}

RPSHands JankenPlayer::getHand()
{
	return hand;
}

Result JankenPlayer::getResult()
{
	return result;
}

void JankenPlayer::setResult(RPSResults index, int changeValue)
{
	switch (index)
	{
	case LOSE:
		result.lose += changeValue;
		break;
	case WIN:
		result.win += changeValue;
		break;
	}
}

const char* JankenPlayer::showName()
{
	return name;
}

// いつもの
static void initRand()
{
	srand((unsigned int)time(nullptr));
}

// 整数を入力させて返す関数
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

//	手を受け取って結果を返す関数
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

// player を受け取ってじゃんけんを行う関数
static RPSResults janken(JankenPlayer* pp1, JankenPlayer* pp2)
{
	// nullチェック
	if (pp1 == nullptr || pp2 == nullptr)
	{
		return LOSE;
	}

	RPSResults currentResult = LOSE;

	// あいこならループ
	while (true)
	{
		bool inputFlag = true;

		// p1の手決定
		pp1->setHand();

		// cpuの手決定
		pp2->setHand();

		// 手を表示
		cout << "player 1 > " << RPSHandMessage[pp1->getHand()] << endl
			<< "player 2 > " << RPSHandMessage[pp2->getHand()] << endl << endl;

		// ジャッジ
		currentResult = judgeResult(pp1->getHand(), pp2->getHand());

		// あいこじゃなければ結果を返す
		if (currentResult != TIE)
		{
			return currentResult;
		}

		cout << "あいこ！　もう一度！\n\n";
	}
}

// じゃんけんを n 本先取で行い、結果を管理する関数
void jankenSimurator()
{
	initRand();
	int round = 1;
	int num = 0;

	cout << "じゃんけんゲームへようこそ！" << endl;

	// num 本か決まるまでループ
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

	// player 用意
	JankenPlayer p1("player 1"), p2("player 2");
	p1.userOrNot();
	p2.userOrNot();

	// num 本終わるまでループ
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
			p2.setResult(WIN);
			p1.setResult(LOSE);
			break;
		case WIN:
			p1.setResult(WIN);
			p2.setResult(LOSE);
		}

		// num 本終わったらループ脱出
		if (p1.getResult().win == num || p2.getResult().win == num)
		{
			break;
		}

		// 暫定結果表示
		cout << RPSResultsMessage[currentResult] << endl
			<< "----- 暫定結果 ----- " << endl
			<< "勝ち > " << p1.getResult().win << endl
			<< "負け > " << p1.getResult().lose << endl << endl;

		// round 数加算
		round++;
	}

	// 最終結果表示
	cout << "===== 最終結果 =====\n\n"
		<< "勝ち > " << p1.getResult().win << endl
		<< "負け > " << p1.getResult().lose << endl
		<< p1.showName() << " の " << (p1.getResult().win == num ? RPSResultsMessage[WIN] : RPSResultsMessage[LOSE])
		<< endl << endl;

}