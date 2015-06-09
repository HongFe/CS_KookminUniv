/*------------------------------------------------------
��޽ý��� ���α׷��� : Command [ cat ]

							���δ��б� ��ǻ�Ͱ��а�
									20093284 ��ȫö
-------------------------------------------------------*/
/*
*	 cat ��ɾ�� �Է¹��� ���ϵ��� ����ϴ� ��ɾ��.
*	-b�ɼ��� ������ ������ ��Ʈ���� ���γѹ��� �Բ� ��µǸ�
*	-n�ɼ��� ������ ������ ��Ʈ���� ���γѹ��� �Բ� ����Ѵ�.
*/

#include "minishlib.h"

#define NL '\n'

// cat Command (complete)
int main(int argc, char **argv)
{
	char *optionTmp = argv[1];	/* �ӽ÷� �ɼ��� �����Ѵ�. */
	int fileNum=0;				/* �������� ������ �Է¹��� ��� ���޹��� ���ڿ��� ������ ���� �ε�����. */
	char charBuf[1];			/* ��µ� ���� �� ����Ʈ�� �о�´�. */
	int nread, bNread;			/* �ɼǿ� ���ȴ�. '\n' �ϳ����� ����ó���� �����̴�. */
	int bFlag = FALSE;	/* �÷��� �ɼ��̴�. bFlag -> ������ ���� �� ������ �տ� ��ȣ�� ǥ���Ѵ�. */
	int nFlag = FALSE;	/* �÷��� �ɼ��̴�. nFlag -> ������ ���� �� ������ �տ� ��ȣ�� ǥ���Ѵ�. */
	int redir = FALSE;	/* ���ڿ� �Է¹��� ������ ��� ���ٸ� redirection �� ���̴�. */
	int fpRead;			/* ���� ������ fd ��ȣ��. */
	int count;			/* ���γѹ��� �����Ѵ�. */
	char str[BUFSIZ];	/* �� ����Ʈ�� �Է¹��� ĳ���͵��� ����Ǿ� ��Ʈ���� �ǰ� ��µȴ�.*/
	
	/* �ɼ��� üũ�Ѵ�. */
	if(argc > 1)
	{	
		if(optionTmp[0] == '-')
		{
			/* 'b' Option */	
			if(strchr(optionTmp,B)) bFlag=TRUE;
			
			/* 'n' Option */	
			else if(strchr(optionTmp,N)) nFlag=TRUE;
			
			/* �Է������̾����Ƿ� redirection �Ȱ��̴�. */
			if(argc == 2)
			{
				redir=TRUE;
				fileNum = 1;	
				fpRead=0;	/* standard in */
			}
			/* �Է� ������ �����Ѵ�. */
			else				
				fileNum = 2;			
		}
			
		/* �ɼ��� ���� ���� �ʴ´�. */
		else 
		{
			fileNum = 1;	
		}			
	}
	/* �ɼǰ� �Է������� �������� �ʴ´�. */
	else 
	{
		redir=TRUE;
		fileNum = 0;	
		fpRead=0;	/* standard in */
	}

	/* �Է¹��� �Է����� ��� �����Ѵ�. */
	while(fileNum<argc)
	{
		/* redirection�� �ƴ� �Է������� ���� ��� �б� ���� ������ ����. */
		if (!redir)
			if( (fpRead = open(argv[fileNum],O_RDONLY)) < 0)
			{
				perror("cat : f open");
				return 1;
			}

		/* �ɼ��� ���� ��� ����� ���� ���γѹ��� ��µ� �� �տ� �ٿ��ش�. */
		if(bFlag || nFlag)
		{			
			count=1;
			sprintf(str,"%d  ",count);
		}

		bNread = 0;  /* �Է¹��� ����Ʈ ���� �����ϸ� ���ٿ� '\n'�� �ִ°��� �Ǻ��Ѵ�. */
		
		/* ����ǰų� Ȥ�� redirection�� ���Ͽ��� 1����Ʈ�� �о�´�. */
		while( (nread=read(fpRead,charBuf,1))>0 ){
			bNread+=nread;
			
			/* b�ɼ��� �����Ǿ� �ְ� '\n'�� ������ ���ζ�� '\n'�� ����Ѵ�. 
			b�ɼ��� ��� ������ �����ϰ� ����ϹǷ� ���� �� ó�� ���ۿ� ī��Ʈ�� �ٿ��ش�.*/
			if (bFlag && (charBuf[0]==NL) && bNread==1)
			{				
				printf("\t\n");
				sprintf(str,"%d  ",count);
				bNread=0;
				continue;
			}

			/* ������ ���� �´ٸ� ����� �Ѵ�. */
			else if (charBuf[0]==NL)
			{	
				/* �ɼ��� �����Ǿ��ִٸ� ī��Ʈ�� �ٿ��� �Է¹��� ��Ʈ���� ����ϰ�
				���� �Է¹��� ��Ʈ���� ī��Ʈ�� ���δ�. */
				if(bFlag || nFlag)
				{							
					printf("\t%s\n",str);
					count++;	
					sprintf(str,"%d  ",count);
					bNread=0;
				}
				/* �ƹ� �ɼ��� ���� ��� ī��Ʈ�� �ٿ����� ������ �ܽ� ��Ʈ���� ����Ѵ�.*/
				else	
				{									
					printf("%s\n",str);		
					sprintf(str,"%c  ",0);
				}
			}
			/* ���� �ƴ϶�� ĳ���͸� ��Ʈ���� ���δ�. */
			else
				sprintf(str,"%s%c",str,charBuf[0]);			
		}
		close(fpRead);
		
		/* ���� �Է������� �����´�. */
		fileNum++;
	}		
	return 0;	
}
