#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(void)
{
	pid_t child1_pid, child2_pid; 
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
		sleep(1);
	 	printf("Child1: %d, ppid = %d, idgr = %d\n", getpid(), getppid(), getgid());  
	} 
	else if (child2_pid == 0)
	{
		sleep(1);
		printf("Child2:	%d, ppid = %d, idgr = %d\n", getpid(), getppid(), getgid());
	}
	else
	{
		//sleep(1);
	    printf("Parent: pid = %d, child1 = %d, child2 = %d, ingr = %d\n", getpid(), child1_pid, child2_pid, getgid());	
	}
	return 0;
}
