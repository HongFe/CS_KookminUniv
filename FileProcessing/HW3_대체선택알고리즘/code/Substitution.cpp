#include "Substitution.h"

Substitution::Substitution(){ max = 4; }

Substitution::Substitution(unsigned int num){ max = num; }

int Substitution::replacementSelection(){

	int m = max;	// 버퍼에 들어가는 레코드수
	int *Buffer = new int[m];	//버퍼-레코드 배열
	int *written = new int[m]; //해당 버퍼 레코드가 출력되었는지를 나타내는 플래그 배열
	int *frozen = new int[m];	//해당 버퍼레코드가 동결되었는지를 나타내는 플래그 배열
	ifstream inStream;
	inStream.open("input.txt", std::ios::in);
	int i;
	int s = 0;		// 버퍼 key중 작은 값의 위치를 저장한다
	int lastKey;	// 마지막에 런에 출력된 레코드 key값
	int count;		// 카운팅 변수

	/* 처음 출력 플래그 값을 모두 TREU 초기화 */
	for (i = 0; i < m; i++){
		written[i] = TRUE;
	}
	i = 0;

	/* key값을 받으면서 출력 플래그를 FALSE로 한다
	 입력파일의 끝이거나 버퍼의 용량을 초과하는 둘중어느 한 상황이면 멈춘다 */
	do{
		inStream >> Buffer[i];
		written[i] = FALSE;
		i = i + 1;
	} while (!inStream.eof() && i != m);

	/*런의 생성*/
	while (!inStream.eof()){
		// 새로운 런 하나를 생성
		// 동결 플래그 초기화
		for (i = 0; i < m; i++){
			if (!written[i])		// 이 if는 사실없어도 되지만 의미상 Unwritten을
				frozen[i] = FALSE;	// Unfrozen 시킨다.
		}

		while (1){
			/* 레코드하나를 런에 출력*/

			count = 0;	// 남아있는 FALSE 동결플래그 갯수를 센다

			/* 단 하나라도 FALSE 동결플래그(Unfrozen)가 남아있다면 진행하고
			 FALSE 동결 플래그가 전혀 없다면 더이상 런출력이 불가능하므로 새로운 런생성을
			 위해 런출력 while문을 나간다.*/
			for (int k = 0; k < m; k++){
				if (frozen[k] == FALSE)		// FALSE 동결플래그가 존재한다면 카운팅한다
					count++;				
			}
			if (count == 0){				// FALSE 동결플래그가 하나도 없다면 while문을 나간다
				cout << endl;
				break;
			}


			lastKey = 9999;		// 처음 비교할 가장큰값. 버퍼의 어느 값보다 커야한다

			/* 동결된 값을 제외한 값들끼리 비교를하여 작은 값과 위치를 찾는다 */
			for (int j = 0; j < m; j++){
				if (frozen[j] == FALSE){
					if (lastKey > Buffer[j]){
						s = j;		// 가장작은 key의 위치를저장한다
						lastKey = Buffer[j];
					}
				}
			}
			
			cout << Buffer[s] << " ";	// 가장작은 key 출력
			lastKey = Buffer[s];		
			written[s] = TRUE;			// 가장작은 key값 위치 출력플래그 TRUE
			frozen[s] = TRUE;			// 가장작은 key값 위치는 동결플래그 TRUE

			if (!inStream.eof()){			//입력 파일의 끝이 아니라면 새로운 key를 입력받는다
				inStream >> Buffer[s];		//대체되는 버퍼자리에 새로운 key를 입력받는다
				written[s] = FALSE;			// 새로 입력받은 key의 출력플래그 FALSE
				if (Buffer[s] > lastKey)	// 새로운 key값이 교체되는 key값보다 크다면 Unfrozen
					frozen[s] = FALSE;		// 새로 입력받은 key의 동결플래그 FALSE
			}
		}
	}

	/* 버퍼에 남아있고 출력되지않은 key를 마지막 런에 출력 */
	while (1){
		count = 0;	// 남아있는 Unwritten 갯수 
		lastKey = 9999;	// 처음 비교할 가장큰값. 버퍼의 어느 값보다 커야한다

		/* 버퍼의 남아있는 key값들을 비교하여 가장 작은값을 구한다 */
		for (int j = 0; j < m; j++){
			if (written[j] == FALSE){
				if (lastKey > Buffer[j]){
					s = j;					// 가장작은 key의 위치를저장한다
					lastKey = Buffer[j];	// 가장작은 ket의 값
				}
				count++;
			}
		}
		if (count == 0)		// 출력플래그가 모두 TRUE라면 while문을 나간다.
			break;

		cout << Buffer[s] << " ";	// 런에 출력
		written[s] = TRUE;			// 출력 플래그 TRUE
		frozen[s] = TRUE;			// 동결 플래그 TRUE
	}

	inStream.close();
	delete[] Buffer;	
	delete[] written;
	delete[] frozen;

	return 0;
}
