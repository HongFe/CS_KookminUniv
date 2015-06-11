/***************************************************************************************
*
*           ���� : Knight Recursive
*
*                ���δ��б� ����������Ŵ��� ��ǻ�Ͱ��к� 3 �г�
*					                             ��ȫö 20093284
*
*****************************************************************************************
*
* [�˰��� ����]
* Knight�� 8���� �������� ������ �� ���ִ�. 
* �׷��� �̹� ������ ��Ҹ� �������� �Ѵٸ� ���� ��ҷ� ���ư� 8������ ������ ���� ��ҷ� 
* �̵��Ѵ�. �̷��� �ൿ�� recursive�� ����ȴ�.
*
****************************************************************************************
*
* [Ŭ���� & �Լ� ����]
*
* KnightTour
* => Ŭ������ Knight�� ������ ������ ���� ��������� ü���ǰ� ���� ���Ѵ�
*
* KnightTour() 
* => constructor �μ� Ư���� �Է��� ���ٸ� MAX_SIZE�� 8X8 ü������ �����ȴ� 
* KnightTour(int sizeRow, int sizeCol) 
* => constructor �μ� �Էµ� rowXcol ü������ �����ȴ�
* bool buildKnightTour(int startRow, int startCol)  
* => �ʱ� ü������ ��� �������� ���� '0'���� ������ �� �ʱ� �Էµ� ��Ҹ� 1�� üũ �� ��
*   Knight�� ���� ��θ� ����ϴ� recurKnightTour�Լ��� �����Ѵ�.
* void printBoard()  
* => Knight�� ������ ��θ� ����Ѵ�
* bool recurKnightTour(int startRow, int startCol, int move) 
* => ��θ� �˻��� ĭ�� row���� col���� �Է� ������ �󸶳� �������°��� �˷��ִ� move����
*   �Է¹����� 8������ isValidMove �Լ��� �˻��Ѵ�. ���� true ��, ������ ������ ��ζ��
*   ������ ������ ĭ�� '1'�� �����Ѱ� ������ ������ ĭ�� row�� col ���� 
*   �ѹ� �������ٴ� �ǹ��� move+1���� ���ڷ� �־� recursive�ϰ� ȣ���Ѵ�
*   ���� ������ �Ұ��� �ϴٸ� �����Ϸ��ߴ� ĭ�� �������� �ʾҴٴ� �ǹ̷� '0'�� �ǰ� 
*   ���� ������ �����Ѵ�.
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
	bool recurKnightTour(int startRow, int startCol, int move);
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
* constructor �μ� Ư���� �Է��� ���ٸ� MAX_SIZE�� 8X8 ü������ �����ȴ�
*****************************************************************************************/
KnightTour::KnightTour()
{
	sizeRow = sizeCol = 8; // default size of Chessboard
}

/*****************************************************************************************
* constructor �μ� �Էµ� rowXcol ü������ �����ȴ�
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
* Knight�� ������ ��θ� ����Ѵ�
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
* �ʱ� ü������ ��� �������� ���� '0'���� ������ �� �ʱ� �Էµ� ��Ҹ� 1�� üũ �� ��
* Knight�� ���� ��θ� ����ϴ� recurKnightTour�Լ��� �����Ѵ�.
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
* ��θ� �˻��� ĭ�� row���� col���� �Է� ������ �󸶳� �������°��� �˷��ִ� move����
* �Է¹����� 8������ isValidMove �Լ��� �˻��Ѵ�. ���� true ��, ������ ������ ��ζ��
* ������ ������ ĭ�� '1'�� �����Ѱ� ������ ������ ĭ�� row�� col ���� 
* �ѹ� �������ٴ� �ǹ��� move+1���� ���ڷ� �־� recursive�ϰ� ȣ���Ѵ�
* ���� ������ �Ұ��� �ϴٸ� �����Ϸ��ߴ� ĭ�� �������� �ʾҴٴ� �ǹ̷� '0'�� �ǰ� 
* ���� ������ �����Ѵ�.
*
****************************************************************************************/
bool KnightTour::recurKnightTour(int row, int col, int move)
{
	int nextRow, nextCol; // ���� ���� �� ������ ��ġ�� �ӽ÷� �����Ѵ�.

	/*
	 ���� ��� ��θ� ã���� ��� true�� return �Ѵ�
	*/
	if (move == sizeRow*sizeCol)
		return true;

	/*
	 8������ ���ʴ�� ã�´�.
	*/
	for (int dir = 0; dir<8; dir++)
	{
		nextRow = row + direction[dir][0];
		nextCol = col + direction[dir][1];

		/*
		 ���� ���� �� ������ ���� ������ ĭ�̶�� ���Ƚ���� �Է��ϰ�
		 �� �������� ���� �� 8������ �˻��ϱ� ���� �Լ��� ȣ���Ѵ�.
		*/
		if (isValidMove(nextRow, nextCol))
		{
			board[nextRow][nextCol] = move + 1;
			/*
			 ���� ������ �������κ��� true�� return �޾Ҵٸ� ��� ��θ�
			 ã�����Ƿ� true�� return�Ѵ�
			*/
			if (recurKnightTour(nextRow, nextCol, move + 1))
				return 1;
			board[nextRow][nextCol] = 0; // ��θ� ã�� ���ϸ� ����� line�̴�
			                             // �̴� ��ΰ� �����Ƿ� �������� ������� ����
		}
	}
	return false;
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