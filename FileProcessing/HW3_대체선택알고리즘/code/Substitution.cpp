#include "Substitution.h"

Substitution::Substitution(){ max = 4; }

Substitution::Substitution(unsigned int num){ max = num; }

int Substitution::replacementSelection(){

	int m = max;	// ���ۿ� ���� ���ڵ��
	int *Buffer = new int[m];	//����-���ڵ� �迭
	int *written = new int[m]; //�ش� ���� ���ڵ尡 ��µǾ������� ��Ÿ���� �÷��� �迭
	int *frozen = new int[m];	//�ش� ���۷��ڵ尡 ����Ǿ������� ��Ÿ���� �÷��� �迭
	ifstream inStream;
	inStream.open("input.txt", std::ios::in);
	int i;
	int s = 0;		// ���� key�� ���� ���� ��ġ�� �����Ѵ�
	int lastKey;	// �������� ���� ��µ� ���ڵ� key��
	int count;		// ī���� ����

	/* ó�� ��� �÷��� ���� ��� TREU �ʱ�ȭ */
	for (i = 0; i < m; i++){
		written[i] = TRUE;
	}
	i = 0;

	/* key���� �����鼭 ��� �÷��׸� FALSE�� �Ѵ�
	 �Է������� ���̰ų� ������ �뷮�� �ʰ��ϴ� ���߾�� �� ��Ȳ�̸� ����� */
	do{
		inStream >> Buffer[i];
		written[i] = FALSE;
		i = i + 1;
	} while (!inStream.eof() && i != m);

	/*���� ����*/
	while (!inStream.eof()){
		// ���ο� �� �ϳ��� ����
		// ���� �÷��� �ʱ�ȭ
		for (i = 0; i < m; i++){
			if (!written[i])		// �� if�� ��Ǿ�� ������ �ǹ̻� Unwritten��
				frozen[i] = FALSE;	// Unfrozen ��Ų��.
		}

		while (1){
			/* ���ڵ��ϳ��� ���� ���*/

			count = 0;	// �����ִ� FALSE �����÷��� ������ ����

			/* �� �ϳ��� FALSE �����÷���(Unfrozen)�� �����ִٸ� �����ϰ�
			 FALSE ���� �÷��װ� ���� ���ٸ� ���̻� ������� �Ұ����ϹǷ� ���ο� ��������
			 ���� ����� while���� ������.*/
			for (int k = 0; k < m; k++){
				if (frozen[k] == FALSE)		// FALSE �����÷��װ� �����Ѵٸ� ī�����Ѵ�
					count++;				
			}
			if (count == 0){				// FALSE �����÷��װ� �ϳ��� ���ٸ� while���� ������
				cout << endl;
				break;
			}


			lastKey = 9999;		// ó�� ���� ����ū��. ������ ��� ������ Ŀ���Ѵ�

			/* ����� ���� ������ ���鳢�� �񱳸��Ͽ� ���� ���� ��ġ�� ã�´� */
			for (int j = 0; j < m; j++){
				if (frozen[j] == FALSE){
					if (lastKey > Buffer[j]){
						s = j;		// �������� key�� ��ġ�������Ѵ�
						lastKey = Buffer[j];
					}
				}
			}
			
			cout << Buffer[s] << " ";	// �������� key ���
			lastKey = Buffer[s];		
			written[s] = TRUE;			// �������� key�� ��ġ ����÷��� TRUE
			frozen[s] = TRUE;			// �������� key�� ��ġ�� �����÷��� TRUE

			if (!inStream.eof()){			//�Է� ������ ���� �ƴ϶�� ���ο� key�� �Է¹޴´�
				inStream >> Buffer[s];		//��ü�Ǵ� �����ڸ��� ���ο� key�� �Է¹޴´�
				written[s] = FALSE;			// ���� �Է¹��� key�� ����÷��� FALSE
				if (Buffer[s] > lastKey)	// ���ο� key���� ��ü�Ǵ� key������ ũ�ٸ� Unfrozen
					frozen[s] = FALSE;		// ���� �Է¹��� key�� �����÷��� FALSE
			}
		}
	}

	/* ���ۿ� �����ְ� ��µ������� key�� ������ ���� ��� */
	while (1){
		count = 0;	// �����ִ� Unwritten ���� 
		lastKey = 9999;	// ó�� ���� ����ū��. ������ ��� ������ Ŀ���Ѵ�

		/* ������ �����ִ� key������ ���Ͽ� ���� �������� ���Ѵ� */
		for (int j = 0; j < m; j++){
			if (written[j] == FALSE){
				if (lastKey > Buffer[j]){
					s = j;					// �������� key�� ��ġ�������Ѵ�
					lastKey = Buffer[j];	// �������� ket�� ��
				}
				count++;
			}
		}
		if (count == 0)		// ����÷��װ� ��� TRUE��� while���� ������.
			break;

		cout << Buffer[s] << " ";	// ���� ���
		written[s] = TRUE;			// ��� �÷��� TRUE
		frozen[s] = TRUE;			// ���� �÷��� TRUE
	}

	inStream.close();
	delete[] Buffer;	
	delete[] written;
	delete[] frozen;

	return 0;
}
