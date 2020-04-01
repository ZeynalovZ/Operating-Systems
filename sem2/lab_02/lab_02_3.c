#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <wait.h>
int main(void)
{
	pid_t child1_pid, child2_pid; 
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
		if (execl("/bin/ps", "ps", "-a", NULL) == -1)
		{
			perror("exec errror/n");
			exit(1);
		}
	 	printf("Child1: %d, ppid = %d, idgr = %d\n", getpid(), getppid(), getgid());  
	} 
	else if (child2_pid == 0)
	{
		sleep(1); // для вывода ps -a  последовательно
		if (execl("/bin/ls", "ls", "-a", NULL) == -1)
		{
			
			perror("exec errror/n");
			exit(1);
		}
		printf("Child2:	%d, ppid = %d, idgr = %d\n", getpid(), getppid(), getgid());
	}
	else
	{
	    printf("Parent: pid = %d, child = %d, ingr = %d\n", getpid(), child1_pid, getgid());
	    sleep(1);	
	    wait(&status);
	    if (WIFEXITED(status))
	    	printf("exit_normal, code = %d\n", WEXITSTATUS(status));
	    else
	    	printf("Some error");
	    wait(&status);
	    if (WIFEXITED(status))
	    	printf("exit_normal, code = %d\n", WEXITSTATUS(status));
	    else
	    	printf("Some error");
	}
	return 0;
}
