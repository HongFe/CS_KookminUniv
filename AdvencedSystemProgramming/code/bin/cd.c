#include "minishlib.h"

// cd Command (complete) -> in shell
int CD(int argc, char *argv)
{
	char *chpath=getenv("HOME");

	if(argc > 1)
		realpath(chpath,argv);
	chdir(chpath);
	
	return 0;
}
