# KQUEUE <sys/event.h>

## struct kevent
```c
struct kevent {
  uintptr_t ident;   /* identifier for this event */
  int16_t   filter;  /* filter for event */
  uint16_t  flags;   /* general flags */
  uint32_t  fflags;  /* filter-specific flags */
  intptr_t  data;    /* filter-specific data */
  void      *udata;  /* opaque user data identifier */
};

EV_SET(ev, ident, filter, flags, fflags, data, udata);
```
### `ident` : 이벤트의 식별자
  - `EVFILT_SIGNAL`의 경우 시그널 번호
  - `EVFILT_READ`의 경우 감시할 fd
  - ...

### `filter` : 이벤트 종류
  - `EVFILT_READ`   : 읽기 작업이 준비된 경우
  - `EVFILT_WRITE`  : 쓰기 작업이 준비된 경우
  - `EVFILT_TIMER`  : 특정 시간 후에 TIMER이벤트를 발생
  - `EVFILT_SIGNAL` : 특정 시그널이 발생한 경우
  - ...

### `flags` : 이벤트 동작을 결정하는 범용적인 플래그
  - `EV_ADD`     : 이벤트 추가
  - `EV_DELETE`  : 이벤트 제거
  - `EV_ONESHOT` : 한번만 이벤트를 발생시키고 이벤트가 삭제됨
  - `EV_ENABLE`  : 이벤트 활성화
  - `EV_DISABLE` : 이벤트 비활성화
  - ...

### `fflags` (filter flags): 이벤트 동작을 결정하는 필터 종속적인 플래그
  - `EVFILT_TIMER`의 `NOTE_SECONDS` : data를 초 단위로 해석함
  - `EVFILT_TIMER`의 `NOTE_NSECONDS `: data를 나노초 단위로 해석함
  - `EVFILT_PROC`의 `NOTE_EXIT` : 프로세스가 exit되었을 경우
  - `EVFILT_VNODE`의 `NOTE_DELETE` : fd에 `unlink(2)`를 실행시켰을 경우
  - ...

### `data`: 이벤트 동작을 결정하거나 결과값이 담기는 필터 종속적인 데이터
  - `EVFILT_TIMER`의 경우: 타임아웃 주기
  - `EVFILT_READ`의 경우: 즉시 읽을 수 있는 바이트 수
  - `EVFILT_SIGNAL`의 경우: 마지막으로 확인한 이후 발생한 시그널 수
  - ...

### `udata`: 사용자가 직접 넘길 수 있는 void *형 인자

---

## kqueue(2)
```c
int kqueue(void);
```
새로운 kqueue fd를 할당한다.

반환값:
  - 성공: 할당한 fd
  - 실패: -1 

---

## kevent(2)

```c
int kevent(
  int kq,
  const struct kevent *changelist, int nchanges,
  struct kevent *eventlist, int nevents,
  const struct timespec *timeout
);
```
이벤트를 대기하거나 새로운 이벤트를 등록한다.

인자:
  - `kq` : 앞서 할당한 kqueue fd
  - `changelist` : 새로 등록할 이벤트의 배열
  - `nchanges` : `changelist`의 길이 (새로 등록할 이벤트 수)
  - `eventlist` : 발생한 이벤트를 담을 배열
  - `nevents` : `eventlist`의 길이 (한번에 처리할 최대 이벤트 수)
  - `timeout` : 이벤트 타임아웃

반환값:
  - 성공: 발생한 이벤트의 수
  - 실패: -1

### NOTE:

- 반환값이 0이 아닌 경우 발생된 이벤트의 정보는 `eventlist`의 [0, ret)에 담겨있다.
이때 발생된 이벤트의 종류는 `filter`에 담겨 있고, 플래그의 `EV_EOF`, `EV_ERROR`이 켜진 경우
적절한 대응을 해야한다.

- timeout의 경우 반환값은 0이다.

- 각각의 이벤트는 (`ident`, `filter`, `udata`) 튜플로 구분되며 서로 같은 이벤트를 등록할 경우
이전 이벤트는 무시된다. 이때 `udata`는 `EV_UDATA_SPECIFIC`플래그로 무시할 수 있다.

- fd를 감시할 때 `close(2)`같은 사유로 fd가 유효하지 않게 되었을 때는 자동적으로 해당 이벤트가 큐에서 제거된다.



