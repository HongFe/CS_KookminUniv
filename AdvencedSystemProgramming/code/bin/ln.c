/*------------------------------------------------------
��޽ý��� ���α׷��� : Command [ ln ]

							���δ��б� ��ǻ�Ͱ��а�
							20093284 ��ȫö
-------------------------------------------------------*/
/*
*	 ln ��ɾ�� ����Ȥ�� ���丮�� hard link, soft link�� �����ϴ�
*	��ɾ��.
*	 -s �ɼ� : soft link�� �����Ѵ�.
*/

#include "minishlib.h"

/* 
* LN ��ɾ�
*/
int main(int argc, char **argv)
{
	struct stat buf; // Stat structure
	char *optionTmp = argv[1]; // Temporally get option
	int sFlag = FALSE; // Option 's'(symbolick link) flag
	int i;
	char *name1,*name2; //  ��ũ �� ����(name1)�� ��ũ �� ����(name2)

	// Option Check
	if(argc > 2)
	{
		// Existing Option
		if(optionTmp[0] == '-')
		{
			name1 = argv[2];
			name2 = argv[3];
			// 's' Option	
			if(strchr(optionTmp,S))	sFlag=TRUE;			
		}
		// Not Existing Option
		else 
		{
			name1 = argv[1];
			name2 = argv[2];		
		}
	}
	// Lack of arguements
	else 
	{
		printf("ERROR : [ln] Wrong Command\n");
		return 1;	
	}

	// Symbolic Link
	if(sFlag) symlink(name1,name2);

	// Hard Link
	else link(name1,name2);	
		
	return 0;
}
