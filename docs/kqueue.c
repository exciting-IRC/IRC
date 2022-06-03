#include <sys/event.h> // kqueue
#include <signal.h>
#include <stdio.h>
#include <err.h>
#include <time.h>

const char *SIGINT_MESSAGE = "SIGINT received";

int main(void) {
  // 새로운 kqueue fd를 할당한다.
  int kq = kqueue();
  if (kq == -1)
    err(1, "kqueue");
  
  struct kevent ev;
  
  // SIGINT가 발생하면 작동되는 이벤트 설정.
  EV_SET(&ev, SIGINT, EVFILT_SIGNAL, EV_ADD, 0, 0, (void *)SIGINT_MESSAGE);
  if (kevent(kq, &ev, 1, NULL, 0, NULL) == -1)
    err(1, "kevent_ctl");
  
  // SIGINT무시 설정 (kqueue는 영항받지 않음)
  signal(SIGINT, SIG_IGN);

  // 1초
  struct timespec one_sec = {1, 0};

  // 발생한 이벤트를 담을 배열
  struct kevent ev_list[16];
  while (42) {
    printf("===PRESS ANY KEY TO CONTINUE(^\\ to quit)===\n");
    getchar();
    // 최대 16개의 이벤트를 1초의 타임아웃을 가지고 대기함
    int occurred_events = kevent(kq, NULL, 0, ev_list, 16, &one_sec);
    if (occurred_events == -1)
      err(1, "kevent_wait");
    else if (occurred_events == 0)
      printf("TIMEOUT!!\n");
    else {
      // 발생한 이벤트 검사
      for (int i = 0; i < occurred_events; i++) {
        // 발생한 이벤트가 시그널일 경우
        if (ev_list[i].filter == EVFILT_SIGNAL) {
          printf("%s %ld times.\n", (char *)ev_list[i].udata, ev_list[i].data);
        }
      }
    }
  }
}