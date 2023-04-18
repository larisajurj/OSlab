pid_t p1, p2;

if ((p1 = fork()) < 0)
	{
        echo "not working"
	}
	if (pid == 0)
	{
		execlp ("ls","ls","-l",NULL); 
	}
    echo hello
