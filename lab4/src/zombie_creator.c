#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
int main(){
pid_t child_pid;
//созд дочь проц
child_pid = fork();
if (child_pid > 0){
//род проц пауза минута
sleep(60);
}
else{//выходим из дочь проц
exit(0);
}
return 0;
}
