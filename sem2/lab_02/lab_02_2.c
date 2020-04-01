#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <wait.h>
int main(void)
{
	pid_t child1_pid, child2_pid; 
	pid_t childpid1, childpid2; 
	int status;
	child1_pid = fork();	
	if (child1_pid != -1 && child1_pid != 0)
	{
		child2_pid = fork();   
	} 
	if (child1_pid == -1 || child2_pid == -1)
	{
		perror("Cant fork");
	     exit(1);  	
	}
	else if (child1_pid == 0)
	{
	 	printf("Child1: %d, ppid = %d, idgr = %d\n", getpid(), getppid(), getgid());  
	} 
	else if (child2_pid == 0)
	{
		printf("Child2:	%d, ppid = %d, idgr = %d\n", getpid(), getppid(), getgid());
	}
	else
	{
	    printf("Parent: pid = %d, child = %d, ingr = %d\n", getpid(), child1_pid, getgid());
	    sleep(1);	
	    childpid1 = wait(&status);

	    if (WIFEXITED(status))
	    	printf("exit_normal, code = %d\n", WEXITSTATUS(status));
	    else
	    	printf("Some error");

	    childpid2 = wait(&status);

	    if (WIFEXITED(status))
	    	printf("exit_normal, code = %d\n", WEXITSTATUS(status));
	    else
	    	printf("some error\n");

	    //printf("hello world\n");
	    printf("childpid1 is %d, childpid2 is %d\n", childpid1, childpid2);
	}
	return 0;
}
