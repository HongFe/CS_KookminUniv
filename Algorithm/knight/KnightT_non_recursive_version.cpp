/***************************************************************************************
*
*           ���� : Knight Non-Recursive
*
*                ���δ��б� ����������Ŵ��� ��ǻ�Ͱ��к� 3 �г�
*					                             ��ȫö 20093284
*
*****************************************************************************************
*
* [�˰��� ����]
* Knight�� 8���� �������� ������ �� ���ִ�.
* �׷��� �̹� ������ ��Ҹ� �������� �Ѵٸ� ���� ��ҷ� ���ư� 8������ ������ ���� ��ҷ�
* �̵��Ѵ�. �̷��� �ൿ�� stack�� ���̸� stack���� ������ row,col ��ġ ������ ���° ����
* ���� �˻縦 �ߴ����� ������ ������ ���� ���� ��ο��� ���� ã�� ���ߴٸ� �� stack��
* ���� �˻��� ��� ���ĺ��� �˻��Ѵ�.
* 8������ ���� ������ ��ΰ� �ִٸ� �� ��θ� Stack�� Push�ϸ� 
* 8���� ��� ã�� ���Ѵٸ� Stack�� Pop�ȴ�. 
*
****************************************************************************************
*
* [Ŭ���� & �Լ� ����]
*
* KnightTour
* =>  Ŭ������ Knight�� ������ ������ ���� ��������� ü���ǰ� ���� ���Ѵ�
*
* KnightTour()
* => constructor �μ� Ư���� �Է��� ���ٸ� MAX_SIZE�� 8X8 ü������ �����ȴ�
* KnightTour(int sizeRow, int sizeCol)
* => constructor �μ� �Էµ� rowXcol ü������ �����ȴ�
* bool buildKnightTour(int startRow, int startCol)
* => �ʱ� ü������ ��� �������� ���� '0'���� ������ �� �ʱ� �Էµ� ��Ҹ� 1�� üũ �� ��
*   Knight�� ���� ��θ� ����ϴ� NonRecurKnightTour�Լ��� �����Ѵ�.
* void printBoard()
* => Knight�� ������ ��θ� ����Ѵ�
* bool NonRecurKnightTour(int startRow, int startCol)
* => ��θ� �˻��� ĭ�� row���� col���� �Է� �޴´�. Ư¡�� stack�� ������ row,col��ġ
*   ������ 8���� �迭�� ���°���� �˻��ߴ����� �� �� �־� ���� ������ ���⿡�� ��θ�
*   ��ã�Ҵٸ� �� stack�� ������ ���� ��θ� �˻��� �� �ִ�.
*   �Է¹����� 8������ isValidMove �Լ��� �˻��Ѵ�. ���� ������ ������ ��ζ��
*   ������ ������ ĭ�� '1'�� �����Ѱ� ������ ������ ĭ�� row�� col ���� ������ 
*   ĭ���� �����Ͽ� while���� ����� �Ѵ�.
*   ���� ���� ĭ���� ������ �Ұ��� �ϴٸ� ����ĭ�� �������� �ʾҴٴ� �ǹ̷� '0'�� �ǰ�
*   Stack�� Pop�ϰ� ���� Stack�� ����ĭ ������ �����ϰ� �ٽ� while���� �����Ѵ�.
* bool isValidMove(int row, int col)
* => ������ ĭ�� ������ �����Ѱ��� �˻��Ѵ�. �ּ� �̸�, �ִ� �ʰ��ϴ� ĭ�� �̹� ������
*   ĭ�� '1'�� ������ �� ����.
*
****************************************************************************************/

#include <fstream>
#include <cstdlib>
#include <iostream>

using namespace std;

const int MAX_SIZE = 8;      // ü������ �ִ� SIZE
static int direction[8][2] = { { -2, 1 }, { -1, 2 }, { 1, 2 }, { 2, 1 }, { 2, -1 }, { 1, -2 },
{ -1, -2 }, { -2, -1 } };    // Knight�� �̵� ������ ���� ����

class KnightTour
{
public:
	KnightTour();
	KnightTour(int sizeRow, int sizeCol);
	bool buildKnightTour(int startRow, int startCol);
	void printBoard();
private:
	typedef int boardType[MAX_SIZE][MAX_SIZE];           //Board�� �ִ� size���� 
	bool NonRecurKnightTour(int startRow, int startCol);
	bool isValidMove(int row, int col);
	int sizeRow, sizeCol; // �Է¹��� ü������ row,col ����� �Ҵ�޴´�
	boardType board;
};


void main()
{
	ifstream inStream;
	int numTestCases;
	inStream.open("input.txt"); // input.txt���� input���� �Է¹޴´�
	if (inStream.fail())
	{
		cerr << "Input file opening failed.\n";
		exit(1);
	}
	inStream >> numTestCases;  // ���� �� testcase�� �Է¹޴´�

	for (int i = 0; i<numTestCases; i++)
	{
		int sizeRow, sizeCol;     // ü������ row,col ����� �Է¹޴� ����
		int startRow, startCol;   // ó�� ������ ��ġ�� row,col
		inStream >> sizeRow >> sizeCol;
		inStream >> startRow >> startCol;
		KnightTour kTour(sizeRow, sizeCol);  // �Է¹��� ü������ �������� knight���� Ŭ���� ����

		/*
		���� recurKnightTour �Լ��� ���� ã�� ���ϰ� false�� return�Ѵٸ� ȭ�鿡 '0'�� ����Ѵ�
		�ݴ�� true���� return�Ѵٸ� ȭ�鿡 1�� �Բ� ��θ� ����Ѵ�.
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
* constructor �μ� Ư���� �Է��� ���ٸ� MAX_SIZE�� 8X8 ü������ �����ȴ�
*
*****************************************************************************************/
KnightTour::KnightTour()
{
	sizeRow = sizeCol = 8; // default size of Chessboard
}

/*****************************************************************************************
*
* constructor �μ� �Էµ� rowXcol ü������ �����ȴ�
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
* Knight�� ������ ��θ� ����Ѵ�
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
* �ʱ� ü������ ��� �������� ���� '0'���� ������ �� �ʱ� �Էµ� ��Ҹ� 1�� üũ �� ��
* Knight�� ���� ��θ� ����ϴ� recurKnightTour�Լ��� �����Ѵ�.
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
*  ��θ� �˻��� ĭ�� row���� col���� �Է� �޴´�. Ư¡�� stack�� ������ row,col��ġ
* ������ 8���� �迭�� ���°���� �˻��ߴ����� �� �� �־� ���� ������ ���⿡�� ��θ�
* ��ã�Ҵٸ� �� stack�� ������ ���� ��θ� �˻��� �� �ִ�.
* �Է¹����� 8������ isValidMove �Լ��� �˻��Ѵ�. ���� ������ ������ ��ζ��
* ������ ������ ĭ�� '1'�� �����Ѱ� ������ ������ ĭ�� row�� col ���� ������ 
* ĭ���� �����Ͽ� while���� ����� �Ѵ�.
* ���� ���� ĭ���� ������ �Ұ��� �ϴٸ� ����ĭ�� �������� �ʾҴٴ� �ǹ̷� '0'�� �ǰ�
* Stack�� Pop�ϰ� ���� Stack�� ����ĭ ������ �����ϰ� �ٽ� while���� �����Ѵ�.
*
****************************************************************************************
*
* [���� ����]
*
* int nextRow, nextCol
*   => ���� ���� �� ����� row,col ��ġ�� �ӽ÷� �Ҵ� �޴´�
* int move 
*   => ������ & Stack�� Top ��ȣ�� ��Ÿ����. 
*     " ���� Stack���� = Knight�� ������ Ƚ�� " �� ��Ÿ����
* int** stackBoard 
*   => Knight�� ������ ������ ���� Stack�̴�. 
*     0��°�� row��ġ����
*     1��°�� col��ġ����
*     2��°�� 8������ ���� �� ��ġ ����
* int flag
*   => ������ ������ ������ �ִٸ� '1', Stack�� Push, Pop�� �����ϴ� flag��
*
****************************************************************************************/
bool KnightTour::NonRecurKnightTour(int row, int col)
{
	int nextRow, nextCol; 
	int move = 0;
	int** stackBoard = new int*[sizeRow*sizeCol]; //
	int flag;

	/*
	 Stack�� �ʱ�ȭ�Ѵ�
	*/
	for (int i = 0; i < sizeRow*sizeCol; i++)
	{
		stackBoard[i] = new int[3];

		stackBoard[i][2] = -1; // 8������ �������� ���� '-1'�� �ʱ�ȭ
	}

	/*
	 0��° Stack�� �ʱ�ȭ �Ѵ�
	*/
	stackBoard[0][0] = row;
	stackBoard[0][1] = col;
	stackBoard[0][2] = 0;

	/*
	 ��θ� ã�������� ����Ǹ� while���� ���������� 2������
	 �Ϻ��� ���� ã�� true�� Return �� ���� 0�� Stack�� Pop�Ǿ� False�� return�� ���̴�.
	*/
	while (1){
		flag = 0;

		/*
		���� ��� ��θ� ã���� ��� true�� return �Ѵ�
		*/
		if (move == sizeRow*sizeCol - 1)
			return true;

		/*
		8������ ���ʴ�� ã�´�.
		*/
		for (int dir = 0; dir < 8; dir++)
		{
			/*
			 ���� �˻��� ������ ����ģ��
			*/
			if (stackBoard[move][2] > dir)
				continue;

			nextRow = row + direction[dir][0];
			nextCol = col + direction[dir][1];

			/*
			���� ���� �� ������ ���� ������ ĭ�̶�� ���Ƚ���� �Է��ϰ�
			�� ������ Stack�� Push�ϰ� �� ��ġ�� ������ġ�� �����Ѵ�
			*/
			if (isValidMove(nextRow, nextCol))
			{
				board[nextRow][nextCol] = board[row][col] + 1; // �������� ������ Ƚ���� ���´�

				stackBoard[move][2] = dir + 1; // 8���� ������ �˻簡 ����� Ƚ������ �����Ѵ�
				/*
				 Stack�� ���ο� ������ ������ Push�Ѵ�
				*/
				move++;
				stackBoard[move][0] = nextRow;
				stackBoard[move][1] = nextCol;
				stackBoard[move][2] = 0;

				row = nextRow; // ���� ��ġ�� ���� �˻��� ������
				col = nextCol; // ��ġ�� �����Ѵ�
				flag = 1;   // Stack�� Push�� �˸��� pop�� �ʵǵ��� �Ѵ�
				break;
			}
		}

		// flag�� 1�� �ƴ϶�� 8���� ã������ �����̾������� Pop�� ����ȴ�
		if (flag != 1){
			board[row][col] = 0;
			stackBoard[move][2] = 0; //���ð� �ʱ�ȭ�� �� pop
			move--; // Stack�� Top�� Pop�Ѵ�

			// ��� ��θ� Ž���Ͽ��� �ʱ� Stack���� Pop�Ǿ� ������ ������ �˷��ش�
			if (move < 0){

				// �����Ҵ�� Stack�� delete���ش�.
				for (int j = 0; j < sizeRow*sizeCol; j++)
				{
					delete[] stackBoard[j];
				}
				delete[] stackBoard;

				return false;
			}

			row = stackBoard[move][0]; // ���� ������ row, col���� ���� ��ġ��
			col = stackBoard[move][1]; // ���� �� �� while���� ����
		}
	}
	return true;
}


/****************************************************************************************
*
* ������ ĭ�� ������ �����Ѱ��� �˻��Ѵ�. �ּ� �̸�, �ִ� �ʰ��ϴ� ĭ��
* �̹� ������ ĭ�� '1'�� ������ �� ����.
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

