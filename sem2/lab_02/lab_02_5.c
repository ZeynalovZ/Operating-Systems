#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <wait.h>
#include <signal.h>

int flag_catch_signal = 0;


void catch_signal(int sig_num)
{
	flag_catch_signal = 1;
	//printf("catch sig = %d\n", sig_num);
}

int main(void)
{
	pid_t child1_pid, child2_pid; 
	int status;
	char msg1[] = "msg1 is working\n";
	char msg2[] = "msg2 is working after SIGINT\n";
	char buf[40];
	int mypipe[2];

	signal(SIGINT, catch_signal);

	if (pipe(mypipe) == -1)
	{
		perror("pipe");
		return 1;
	}

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
		sleep(2);
	 	printf("Child1: %d, ppid = %d, idgr = %d\n", getpid(), getppid(), getgid());  
	 	if (flag_catch_signal == 0)
	 	{
	 		close(mypipe[0]);
	 		write(mypipe[1], msg1, sizeof(msg1));
	 	}
	 	if (execl("/bin/ps", "ps", "-a", NULL) == -1)
		{
			perror("exec errror/n");
			exit(1);
		}
	 	
	} 
	else if (child2_pid == 0)
	{
		sleep(2);
		printf("Child2:	%d, ppid = %d, idgr = %d\n", getpid(), getppid(), getgid());
		if (flag_catch_signal == 1)
		{
			close(mypipe[0]);
	 		write(mypipe[1], msg2, sizeof(msg2));
		}
		if (execl("/bin/ls", "ls", "-a", NULL) == -1)
		{
			
			perror("exec errror/n");
			exit(1);
		}
		
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

		// close(mypipe[1]);
	 //    read(mypipe[0], buf, sizeof(buf));
	 //    printf(" parent buf: %s\n", buf);

	    wait(&status);
	    if (WIFEXITED(status))
	    	printf("exit_normal, code = %d\n", WEXITSTATUS(status));
	    else
	    	printf("Some error");

	    close(mypipe[1]);
	    read(mypipe[0], buf, sizeof(buf));
	    printf(" parent buf: %s\n", buf);

	}
	return 0;
}
