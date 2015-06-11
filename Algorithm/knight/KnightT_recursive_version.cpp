/***************************************************************************************
*
*           문제 : Knight Recursive
*
*                국민대학교 전자정보통신대학 컴퓨터공학부 3 학년
*					                             나홍철 20093284
*
*****************************************************************************************
*
* [알고리즘 설명]
* Knight가 8가지 방향으로 여행을 할 수있다. 
* 그러나 이미 지나간 장소를 지나가려 한다면 이전 장소로 돌아가 8방향중 지나지 않은 장소로 
* 이동한다. 이러한 행동이 recursive로 진행된다.
*
****************************************************************************************
*
* [클래스 & 함수 설명]
*
* KnightTour
* => 클래스는 Knight가 여행을 떠나기 위해 만들어지는 체스판과 길을 구한다
*
* KnightTour() 
* => constructor 로서 특별한 입력이 없다면 MAX_SIZE인 8X8 체스판이 생성된다 
* KnightTour(int sizeRow, int sizeCol) 
* => constructor 로서 입력된 rowXcol 체스판이 생성된다
* bool buildKnightTour(int startRow, int startCol)  
* => 초기 체스판을 모두 지나가지 않은 '0'으로 셋팅한 후 초기 입력된 장소를 1로 체크 한 후
*   Knight의 여행 경로를 계산하는 recurKnightTour함수를 실행한다.
* void printBoard()  
* => Knight가 지나간 경로를 출력한다
* bool recurKnightTour(int startRow, int startCol, int move) 
* => 경로를 검사할 칸의 row값과 col값을 입력 받으며 얼마나 움직였는가를 알려주는 move값을
*   입력받으면 8방향을 isValidMove 함수로 검사한다. 만약 true 즉, 진행이 가능한 경로라면
*   진행이 가능한 칸을 '1'로 셋팅한고 진행이 가능한 칸의 row와 col 값과 
*   한번 움직였다는 의미의 move+1값을 인자로 주어 recursive하게 호출한다
*   만약 진행이 불가능 하다면 진행하려했던 칸은 지나가지 않았다는 의미로 '0'이 되고 
*   다음 방향을 조사한다.
* bool isValidMove(int row, int col) 
* => 현재의 칸이 진행이 가능한가를 검사한다. 최소 미만, 최대 초과하는 칸과 이미 지나간
*   칸인 '1'은 지나갈 수 없다.
*
****************************************************************************************/

#include <fstream>
#include <cstdlib>
#include <iostream>

using namespace std;

const int MAX_SIZE = 8;      // 체스판의 최대 SIZE
static int direction[8][2] = { { -2, 1 }, { -1, 2 }, { 1, 2 }, { 2, 1 }, { 2, -1 }, { 1, -2 },
{ -1, -2 }, { -2, -1 } };    // Knight의 이동 가능한 여덟 방향

class KnightTour
{
public:
	KnightTour();
	KnightTour(int sizeRow, int sizeCol);
	bool buildKnightTour(int startRow, int startCol);
	void printBoard();
private:
	typedef int boardType[MAX_SIZE][MAX_SIZE];           //Board의 최대 size지정 
	bool recurKnightTour(int startRow, int startCol, int move);
	bool isValidMove(int row, int col);
	int sizeRow, sizeCol; // 입력받은 체스판의 row,col 사이즈를 할당받는다
	boardType board;
};


void main()
{
	ifstream inStream;
	int numTestCases;
	inStream.open("input.txt"); // input.txt에서 input값을 입력받는다
	if (inStream.fail())
	{
		cerr << "Input file opening failed.\n";
		exit(1);
	}
	inStream >> numTestCases;  // 진행 될 testcase를 입력받는다

	for (int i = 0; i<numTestCases; i++)
	{
		int sizeRow, sizeCol;     // 체스판의 row,col 사이즈를 입력받는 변수
		int startRow, startCol;   // 처음 진행할 위치의 row,col
		inStream >> sizeRow >> sizeCol;
		inStream >> startRow >> startCol;
		KnightTour kTour(sizeRow, sizeCol);  // 입력받은 체스판의 사이즈의 knight여행 클래스 생성

		/*
		만약 recurKnightTour 함수가 길을 찾지 못하고 false를 return한다면 화면에 '0'을 출력한다
		반대로 true값을 return한다면 화면에 1과 함께 경로를 출력한다.
		*/
		if (kTour.buildKnightTour(startRow, startCol))
		{
			cout << 1 << endl;
			kTour.printBoard();
		}
		else
			cout << 0 << endl;
	}
	inStream.close();
}

/*****************************************************************************************
* constructor 로서 특별한 입력이 없다면 MAX_SIZE인 8X8 체스판이 생성된다
*****************************************************************************************/
KnightTour::KnightTour()
{
	sizeRow = sizeCol = 8; // default size of Chessboard
}

/*****************************************************************************************
* constructor 로서 입력된 rowXcol 체스판이 생성된다
*****************************************************************************************/
KnightTour::KnightTour(int sRow, int sCol)
{
	if (sRow >= 2 && sRow <= MAX_SIZE)
		sizeRow = sRow;
	else
		sizeRow = 8; // set to default size
	if (sCol >= 2 && sCol <= MAX_SIZE)
		sizeCol = sCol;
	else
		sizeCol = 8; // set to default size
}

/*****************************************************************************************
* Knight가 지나간 경로를 출력한다
*****************************************************************************************/
void KnightTour::printBoard()
{
	for (int r = 0; r < sizeRow; r++)
	{
		for (int c = 0; c < sizeCol; c++)
			cout << board[r][c] << " ";
		cout << endl;
	} 
}

/*****************************************************************************************
* 초기 체스판을 모두 지나가지 않은 '0'으로 셋팅한 후 초기 입력된 장소를 1로 체크 한 후
* Knight의 여행 경로를 계산하는 recurKnightTour함수를 실행한다.
*****************************************************************************************/
bool KnightTour::buildKnightTour(int startRow, int startCol)
{
	for (int r = 0; r<sizeRow; r++)
	for (int c = 0; c<sizeCol; c++)
		board[r][c] = 0;

	if (startRow <= 0 || startRow > sizeRow)
		startRow = 0;
	if (startCol <= 0 || startCol > sizeCol)
		startCol = 0;
	board[startRow - 1][startCol - 1] = 1;

	return recurKnightTour(startRow - 1, startCol - 1, 1);
}

/****************************************************************************************
*
* 경로를 검사할 칸의 row값과 col값을 입력 받으며 얼마나 움직였는가를 알려주는 move값을
* 입력받으면 8방향을 isValidMove 함수로 검사한다. 만약 true 즉, 진행이 가능한 경로라면
* 진행이 가능한 칸을 '1'로 셋팅한고 진행이 가능한 칸의 row와 col 값과 
* 한번 움직였다는 의미의 move+1값을 인자로 주어 recursive하게 호출한다
* 만약 진행이 불가능 하다면 진행하려했던 칸은 지나가지 않았다는 의미로 '0'이 되고 
* 다음 방향을 조사한다.
*
****************************************************************************************/
bool KnightTour::recurKnightTour(int row, int col, int move)
{
	int nextRow, nextCol; // 다음 진행 할 방향의 위치를 임시로 저장한다.

	/*
	 만약 모든 경로를 찾았을 경우 true를 return 한다
	*/
	if (move == sizeRow*sizeCol)
		return true;

	/*
	 8방향을 차례대로 찾는다.
	*/
	for (int dir = 0; dir<8; dir++)
	{
		nextRow = row + direction[dir][0];
		nextCol = col + direction[dir][1];

		/*
		 만약 진행 할 방향이 진행 가능한 칸이라면 경로횟수를 입력하고
		 그 방향으로 부터 또 8방향을 검사하기 위해 함수를 호출한다.
		*/
		if (isValidMove(nextRow, nextCol))
		{
			board[nextRow][nextCol] = move + 1;
			/*
			 만약 진행한 방향으로부터 true를 return 받았다면 모든 경로를
			 찾았으므로 true를 return한다
			*/
			if (recurKnightTour(nextRow, nextCol, move + 1))
				return 1;
			board[nextRow][nextCol] = 0; // 경로를 찾지 못하면 진행될 line이다
			                             // 이는 경로가 없으므로 지나가지 않은길로 셋팅
		}
	}
	return false;
}

/****************************************************************************************
*
* 현재의 칸이 진행이 가능한가를 검사한다. 최소 미만, 최대 초과하는 칸과
* 이미 지나간 칸인 '1'은 지나갈 수 없다.
*
****************************************************************************************/
bool KnightTour::isValidMove(int row, int col)
{
	if (row >= sizeRow || row<0)
		return false;
	if (col >= sizeCol || col<0)
		return false;
	if (board[row][col] != 0)
		return false;
	else
		return true;
}