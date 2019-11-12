#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct mesg_buffer {
  long msg_type;
  char msg_text[100];
} message;

int main(void) {
  key_t key;
  int msgid;
  int id = (int)"bmconquest";

  key = ftok("./bmconquest", id);
  printf("key -> %d\n",key);
  msgid = msgget(key, 0666 | IPC_CREAT);

  msgrcv(msgid, &message, sizeof(message), 1, 0);
  printf("Data Recieved is : %s\n",
          message.msg_text);

  msgctl(msgid, IPC_RMID, NULL);
  return 0;
}
