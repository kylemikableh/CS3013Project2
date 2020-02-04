// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
//
// /*arguments of syscall can be found on http://blog.rchapman.org/post/36801038863/linux-system-call-table-for-x86-64*/
// int main(int argc, char* argv[])
// {
//   open("Zoinks.txt", 0);
// }
// open system call
#include<stdio.h>
#include<fcntl.h>
#include<errno.h>
extern int errno;
int main()
{
    // if file does not have in directory
    // then file foo.txt is created.
    int fd = open("Zoinks.txt", O_RDONLY | O_CREAT);

    printf("fd = %d/n", fd);

    if (fd ==-1)
    {
        // print which type of error have in a code
        printf("Error Number % d\n", errno);

        // print program detail "Success or failure"
        perror("Program");
    }
    return 0;
}
