/*------------------------------------------------------
��޽ý��� ���α׷��� : Command [ chmod ]

							���δ��б� ��ǻ�Ͱ��а�
							20093284 ��ȫö
-------------------------------------------------------*/
/*
*	 chmod ��ɾ�� �ش� ���ϰ� ���丮��
*	�۹̼� ��带 ������ �ټ� �ִ�.
*	�̶� �۹̼Ǹ��� ���ڿ� ���ڷ� ������ �����ϴ�.
*	���ڷ� �Է¹޴� �ٸ� �� ���� ��Ʈ���� �������ִ� 
*	�Լ��� ���� �ѹ� �۹̼����� �����Ѵ�.
*/
#include "minishlib.h"

/* ��Ʈ���� ������ �۹̼� ���� �������ش�. */
int StrChange(char *modeStr, int modeNum)
{
	int operFlag;	// '-' => 0 / '+' => 1 / '=' => 2
	int shiftNum;	/* �ѹ� �۹̼ǿ��� ���� user�� 6��Ʈ, other�� 3��Ʈ �������� �̵��Ѵ�.*/
	int i,tmp;
	
	for( i = 0 ; i<strlen(modeStr);i++)
	{
		tmp=0;
		/* �ѹ� �۹̼ǿ��� �󸶳� ����Ʈ�� �ؾ��ϴ��� ���Ѵ�. 
		�� 9��Ʈ�� user, group, other ��Ʈ�� ���ϴ°���. */
		if(modeStr[i] == 'u')
			shiftNum=6;
		else if(modeStr[i] == 'g')
			shiftNum=3;
		else if(modeStr[i] == 'o') 
			shiftNum=0;
		else 
		{ 
			printf("chmod: Wrong Command [u,g,o]\n"); 
			return 1; 
		}

		i++; 
		/* '-'�� �⺻ �۹̼ǿ��� ����
		'+'�� �⺻ �۹̼ǿ� �߰�
		'='�� �⺻ �۹̼��� �����ϰ� ����Ⱑ �ȴ�. */
		if(modeStr[i]=='-') 
			operFlag = 0;
		else if(modeStr[i]=='+') 
			operFlag = 1;
		else if(modeStr[i]=='=') 
			operFlag = 2;
		else 
		{ 
			printf("chmod: Wrong Operator\n"); 
			return 1; 
		}
				
		i++; 
		/* ��Ʈ������ read, write, excute�� ã�´�. */
		while(i<strlen(modeStr) )
		{
			if(modeStr[i]=='r')
			{
				tmp=tmp|4; 
				i++;
			}
			else if(modeStr[i]=='w') 
			{		
				tmp=tmp|2; 
				i++;
			}
			else if(modeStr[i]=='x')
			{		
				tmp=tmp|1; 
				i++;
			}
			else if(modeStr[i]==',') 
				break; /* ���� ��� ���� ��Ʈ������ ã�´�. */
			else 
			{ 
				printf("chmod: Wrong Command\n"); 
				return 1; 
			}
		}
			/*	�м��� ��Ʈ���� ��� ���氪�� ���� ������ ���� �԰� �˸°� ó���Ѵ�. */
			/* '-' */
			if(operFlag == 0) 
				modeNum = (modeNum & ~(tmp << shiftNum)); 
			/* '+' */
			else if(operFlag == 1)
				modeNum = (modeNum | (tmp << shiftNum));
			/* '=' */
			else if(operFlag == 2) 
			{
				/* ���� ��� �κ��� �����ϰ� �����. */
				if (shiftNum == 6)
					modeNum=modeNum & 65087; // 177077 (octal)
				else if (shiftNum == 3)
					modeNum=modeNum & 65479; // 177707 (octal)
				else if (shiftNum == 0)
					modeNum=modeNum & 65528; // 177770 (octal)
					
				modeNum = (modeNum | (tmp << shiftNum) ); 
			}			
	}
	return modeNum;	/* ���� �۹̼� ��带 �����Ѵ�. */
}

/*
*	chmod ��ɾ�
*/
int main(int argc, char **argv)
{
	char *modeTmp = argv[1]; /* �ӽ÷� ����� �����Ѵ�. */
	char *name;		/* ������ ����,������ �̸� */
	char *ptr;		/* ��ū �����ʹ� */
	int modeNum;	/* ������ �۹̼� �ѹ�*/
	struct stat buf;

	/* ���ڸ� üũ�Ѵ�. */
	if(argc > 2)
	{		
		name = argv[2];
		/* ���� �ӽ÷� ����� ������ ���� ���ڶ�� �۹̼� ���� ���� */
		if(isdigit(modeTmp[0]))
		{
			modeNum = strtol(modeTmp, &ptr, 8); /* �۹̼� ���ڸ� 8������ �����Ѵ�. */
			
			/* �Է¹��� �ѹ��� �۹̼Ǹ�带 �ٲپ� �ش� */
			if( chmod(name, modeNum)<-1)
				perror("chmod: Call to chmod failed.");					
		}
		/* �۹̼� ��尡 ���� */
		else
		{
			/* ������ ���� ���� �ʴ´ٸ� ����*/
			if (stat(name,&buf) == -1) 
			{
				printf("chmod: \n");
				exit(1);
			}
			modeNum = buf.st_mode;	/* �ش� ������ �۹̼� ��带 �Ҵ�޴´�. */
			
			/* �۹̼� ��带 �����Ѵ�. �̶� �����۹̼� �Է½� 
			�۹̼� �ѹ��� ��ȯ���ִ� �Լ��� ����Ͽ� ��ȯ�޾� ����Ѵ�. */
			if( chmod(name, StrChange(modeTmp,modeNum))<-1)
				perror("chmod: Call to chmod failed.");
		}
	}
	else 
	{
		printf("chmod:Wrong Command\n");
		return 1;
	}

	return 0;	
}
