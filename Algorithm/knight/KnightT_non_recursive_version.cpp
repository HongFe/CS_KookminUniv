/***************************************************************************************
*
*           문제 : Knight Non-Recursive
*
*                국민대학교 전자정보통신대학 컴퓨터공학부 3 학년
*					                             나홍철 20093284
*
*****************************************************************************************
*
* [알고리즘 설명]
* Knight가 8가지 방향으로 여행을 할 수있다.
* 그러나 이미 지나간 장소를 지나가려 한다면 이전 장소로 돌아가 8방향중 지나지 않은 장소로
* 이동한다. 이러한 행동이 stack에 쌓이며 stack에는 현재의 row,col 위치 정보와 몇번째 방향
* 까지 검사를 했는지의 정보가 있으며 만약 다음 경로에서 길을 찾지 못했다면 이 stack의
* 이전 검사한 경로 이후부터 검사한다.
* 8방향중 진행 가능한 경로가 있다면 그 경로를 Stack에 Push하며 
* 8방향 모두 찾지 못한다면 Stack은 Pop된다. 
*
****************************************************************************************
*
* [클래스 & 함수 설명]
*
* KnightTour
* =>  클래스는 Knight가 여행을 떠나기 위해 만들어지는 체스판과 길을 구한다
*
* KnightTour()
* => constructor 로서 특별한 입력이 없다면 MAX_SIZE인 8X8 체스판이 생성된다
* KnightTour(int sizeRow, int sizeCol)
* => constructor 로서 입력된 rowXcol 체스판이 생성된다
* bool buildKnightTour(int startRow, int startCol)
* => 초기 체스판을 모두 지나가지 않은 '0'으로 셋팅한 후 초기 입력된 장소를 1로 체크 한 후
*   Knight의 여행 경로를 계산하는 NonRecurKnightTour함수를 실행한다.
* void printBoard()
* => Knight가 지나간 경로를 출력한다
* bool NonRecurKnightTour(int startRow, int startCol)
* => 경로를 검사할 칸의 row값과 col값을 입력 받는다. 특징은 stack에 현재의 row,col위치
*   정보와 8방향 배열중 몇번째까지 검사했는지를 알 수 있어 다음 진행한 방향에서 경로를
*   못찾았다면 이 stack의 정보로 다음 경로를 검사할 수 있다.
*   입력받으면 8방향을 isValidMove 함수로 검사한다. 만약 진행이 가능한 경로라면
*   진행이 가능한 칸을 '1'로 셋팅한고 진행이 가능한 칸의 row와 col 값을 현재의 
*   칸으로 설정하여 while문을 재실행 한다.
*   만약 현재 칸에서 진행이 불가능 하다면 현재칸은 지나가지 않았다는 의미로 '0'이 되고
*   Stack을 Pop하고 이전 Stack의 현재칸 정보를 셋팅하고 다시 while문을 수행한다.
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
	bool NonRecurKnightTour(int startRow, int startCol);
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
*
* constructor 로서 특별한 입력이 없다면 MAX_SIZE인 8X8 체스판이 생성된다
*
*****************************************************************************************/
KnightTour::KnightTour()
{
	sizeRow = sizeCol = 8; // default size of Chessboard
}

/*****************************************************************************************
*
* constructor 로서 입력된 rowXcol 체스판이 생성된다
*
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
*
* Knight가 지나간 경로를 출력한다
*
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
*
* 초기 체스판을 모두 지나가지 않은 '0'으로 셋팅한 후 초기 입력된 장소를 1로 체크 한 후
* Knight의 여행 경로를 계산하는 recurKnightTour함수를 실행한다.
*
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

	return NonRecurKnightTour(startRow - 1, startCol - 1);
}

/****************************************************************************************
*
*  경로를 검사할 칸의 row값과 col값을 입력 받는다. 특징은 stack에 현재의 row,col위치
* 정보와 8방향 배열중 몇번째까지 검사했는지를 알 수 있어 다음 진행한 방향에서 경로를
* 못찾았다면 이 stack의 정보로 다음 경로를 검사할 수 있다.
* 입력받으면 8방향을 isValidMove 함수로 검사한다. 만약 진행이 가능한 경로라면
* 진행이 가능한 칸을 '1'로 셋팅한고 진행이 가능한 칸의 row와 col 값을 현재의 
* 칸으로 설정하여 while문을 재실행 한다.
* 만약 현재 칸에서 진행이 불가능 하다면 현재칸은 지나가지 않았다는 의미로 '0'이 되고
* Stack을 Pop하고 이전 Stack의 현재칸 정보를 셋팅하고 다시 while문을 수행한다.
*
****************************************************************************************
*
* [변수 설명]
*
* int nextRow, nextCol
*   => 다음 진행 할 경로의 row,col 위치를 임시로 할당 받는다
* int move 
*   => 움직임 & Stack의 Top 번호를 나타낸다. 
*     " 쌓인 Stack갯수 = Knight의 움직인 횟수 " 를 나타낸다
* int** stackBoard 
*   => Knight의 움직임 정보를 담을 Stack이다. 
*     0번째는 row위치정보
*     1번째는 col위치정보
*     2번째는 8방향중 진행 될 위치 정보
* int flag
*   => 진행이 가능한 방향이 있다면 '1', Stack의 Push, Pop을 결정하는 flag다
*
****************************************************************************************/
bool KnightTour::NonRecurKnightTour(int row, int col)
{
	int nextRow, nextCol; 
	int move = 0;
	int** stackBoard = new int*[sizeRow*sizeCol]; //
	int flag;

	/*
	 Stack을 초기화한다
	*/
	for (int i = 0; i < sizeRow*sizeCol; i++)
	{
		stackBoard[i] = new int[3];

		stackBoard[i][2] = -1; // 8방향중 진행하지 않은 '-1'로 초기화
	}

	/*
	 0번째 Stack을 초기화 한다
	*/
	stackBoard[0][0] = row;
	stackBoard[0][1] = col;
	stackBoard[0][2] = 0;

	/*
	 경로를 찾을때까지 진행되며 while문을 빠져나가는 2가지는
	 완벽한 답을 찾아 true를 Return 할 때와 0번 Stack이 Pop되어 False를 return할 때이다.
	*/
	while (1){
		flag = 0;

		/*
		만약 모든 경로를 찾았을 경우 true를 return 한다
		*/
		if (move == sizeRow*sizeCol - 1)
			return true;

		/*
		8방향을 차례대로 찾는다.
		*/
		for (int dir = 0; dir < 8; dir++)
		{
			/*
			 이전 검사한 방향은 지나친다
			*/
			if (stackBoard[move][2] > dir)
				continue;

			nextRow = row + direction[dir][0];
			nextCol = col + direction[dir][1];

			/*
			만약 진행 할 방향이 진행 가능한 칸이라면 경로횟수를 입력하고
			그 방향을 Stack에 Push하고 그 위치를 현재위치로 셋팅한다
			*/
			if (isValidMove(nextRow, nextCol))
			{
				board[nextRow][nextCol] = board[row][col] + 1; // 보드판의 지나온 횟수를 적는다

				stackBoard[move][2] = dir + 1; // 8가지 방향중 검사가 진행된 횟수까지 저장한다
				/*
				 Stack에 새로운 방향의 정보를 Push한다
				*/
				move++;
				stackBoard[move][0] = nextRow;
				stackBoard[move][1] = nextCol;
				stackBoard[move][2] = 0;

				row = nextRow; // 현재 위치를 다음 검사할 방향의
				col = nextCol; // 위치로 셋팅한다
				flag = 1;   // Stack의 Push를 알리고 pop이 않되도록 한다
				break;
			}
		}

		// flag가 1이 아니라면 8곳을 찾았으나 갈곳이없음으로 Pop이 진행된다
		if (flag != 1){
			board[row][col] = 0;
			stackBoard[move][2] = 0; //스택값 초기화한 후 pop
			move--; // Stack의 Top을 Pop한다

			// 모든 경로를 탐색하였고 초기 Stack마저 Pop되어 정답이 없을을 알려준다
			if (move < 0){

				// 동적할당된 Stack을 delete해준다.
				for (int j = 0; j < sizeRow*sizeCol; j++)
				{
					delete[] stackBoard[j];
				}
				delete[] stackBoard;

				return false;
			}

			row = stackBoard[move][0]; // 이전 스택의 row, col값을 현재 위치로
			col = stackBoard[move][1]; // 설정 한 후 while문을 실행
		}
	}
	return true;
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

