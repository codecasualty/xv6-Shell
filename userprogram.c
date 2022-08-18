#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int args , char *argv[]){

// char* cd  = {"  ls    "};

// char**p = &cd;

// exec("ls"  , p);

//1st question
int sum = 0;
for(int i = 0;i < args;++i){
        sum += 10;
}

printf(1,"%d",sum);

helloWorld();

//2nd question
printf(1, "Total Number of Open Files: %d\n", numOpenFiles());
int fd;
fd = open("backup", O_CREATE | O_RDWR);
printf(1, "Total Number of Open Files: %d\n", numOpenFiles());
close(fd);
printf(1, "Total Number of Open Files: %d\n", numOpenFiles());


// //3rd question
// printf(2,"Memory allocated");
printf(1,"Memory allocated till now: %d bytes\n", memAlloc());
sleep(90);

char* x = sbrk(1024);
printf(1,"Memory allocated till now: %d bytes\n", memAlloc());

//free(x);
char* y = sbrk(4096);
printf(1,"Memory allocated till now: %d bytes\n", memAlloc());


printf(1,"Memory allocated till now: %d bytes\n", memAlloc());


char* z = sbrk(-10);
printf(1,"Memory allocated till now: %d bytes\n", memAlloc());

char* a = sbrk(-20000);
printf(1,"Memory allocated till now: %d bytes\n", memAlloc());

printf(1,"for process  process date time\n ", getprocesstimedetails());

if(fork() == 0){

        printf(1,"child forked\n", getprocesstimedetails());
        sleep(170);

        printf(1,"2nd new process date time\n ", getprocesstimedetails());
        sleep(170);

        printf(1,"3rd time process date time\n ", getprocesstimedetails());
        sleep(170);

}else{

       wait(0);
        printf(1,"for process  process date time\n ", getprocesstimedetails());


}


free(x);
free(y);
free(z);  
free(a);
exit(0);

}
