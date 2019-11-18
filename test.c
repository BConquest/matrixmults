#include <stdio.h>
#include <sys/msg.h>

int main(void)
{
  key_t key = ftok("./matrixmults/bmconquest", 0);
  for (int i = 0; i < 1000; i++)
  {
    int msgid = msgget(key+i, 0666 | IPC_CREAT);
    printf("%d\n", msgid);
  }
}
