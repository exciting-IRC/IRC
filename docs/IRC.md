# IRC

## 관련 RFC문서
  - RFC 1459: [Internet Relay Chat Protocol](1)
  - RFC 2810: [IRC: Architecture](2)
  - RFC 2811: [IRC: Channel Management](3)
  - RFC 2812: [IRC: Client Protocol](4)
  - RFC 2813: [IRC: Server Protocol](5)
  - RFC 7194: [Default Port for IRC via TLS/SSL](6)
   
---

## 설명

일대일 / 일대다 채팅이 가능하게 만들어주는 프로토콜이다. 단순히 클라이언트 -> 서버 -> 클라이언트로 메시지가 전달되는 것 뿐만 아니라 클라이언트 -> 중계서버 -> 서버 -> 클라이언트 같이 메시지를 서버에서
또 다른 서버로 중계해주는 기능도 있다.

---

Mandatory
  - authenticate -> `PASS`
  - set nickname -> `NICK`
  - set username -> `USER`?
  - join channel -> `JOIN` / `PART`
  - send and receive private messages -> `PRIVMSG`
  - send and receive channel 
  - operators <- (admin)  / regular users -> `ADMIN`
  - commands that are specific to operators. -> `KILL`

BONUS
 - file transfer
 - bot

---
## 서버
  클라이언트가 연결해서 서로 대화하기 위한 종단점을 제공한다. <br />
  서버와 서버끼리의 연결로 네트워크를 구성할 수 있다. (구현할 필요 X)

## 클라이언트
  서버에 연결할 수 있는 요소 중 서버가 아닌것들.
  모든 클라이언트는 최대 9글자의 유일한 닉네임을 가진다.

## 오퍼레이터
  IRC를 관리하기 위해 존재하는 특별한 클라이언트이댜. <br />
  네트워크 간의 연결을 조절하기 위한 `SQUIT`, `CONNECT` 명령과, <br />
  특정 유저를 추방하기 위한 `KILL` 명령 등을 사용할 권한을 가진다.

## 채널
  채널은 하나 이상의 클라이언트가 속할 수 있는 그룹이다. <br />
  채널에 전달된 메시지는 해당 채널에 속하는 모든 클라이언트에게 전해진다 <br />
  채널은 첫번째 클라이언트가 접속할 때 암시적으로 생성되고, 마지막 클라이언트가 접속을 종료할 때 소멸된다.

## 채널 오퍼레이터 (chop)
  특정 채널을 관리하기 위한 오퍼레이터이다. `KICK`, `MODE`, `INVITE`, `TOPIC`명령을 사용할 권한을 가진다. <br />
  닉네임 맨 끝에 `@`문자로 일반 클라이언트와 구별할 수 있다.

## 메시지

모든 IRC메시지는 `prefix`, `command`, `parameters`의 3 부분으로 분리되고, 각 필드는 하나 이상의 `' '`(공백)문자로 구별된다. 한 메시지의 최대 길이는 512글자이며, 마지막 CR LF로 메시지를 구분한다.

### `prefix`
  - `prefix`필드는 메시지를 보낸 정확한 출처를 확인하는데 쓰인다. `prefix`필드가 생략된 경우에는 메시지의 출처는 메시지를 보낸 클라이언트라고 간주한다.
  
  - `prefix`필드는 선택적으로 존재하거나, 존재하지 않을 수 있는데, `prefix`필드의 존재는 `prefix`필드 직전에 나오는 `':'`문자로 확인할 수 있다.

### `command` 
  - `command`는 유효한 IRC 명령어이거나 3글자의 응답 코드이다.

### 메시지 포맷 (BNF)

#### 기본
```BNF
<message>  ::= [':' <prefix> <SPACE> ] <command> <params> <crlf>
<prefix>   ::= <servername> | <nick> [ '!' <user> ] [ '@' <host> ]
<command>  ::= <letter> { <letter> } | <number> <number> <number>
<SPACE>    ::= ' ' { ' ' }
<params>   ::= <SPACE> [ ':' <trailing> | <middle> <params> ]
<middle>   ::= <Any *non-empty* sequence of octets not including SPACE
               or NUL or CR or LF, the first of which may not be ':'>
<trailing> ::= <Any, possibly *empty*, sequence of octets
                not including NUL or CR or LF>
<crlf>     ::= CR LF
```

#### 파라미터
```BNF
<target>     ::= <to> [ "," <target> ]
<to>         ::= <channel> | <user> '@' <servername> | <nick> | <mask>
<channel>    ::= ('#' | '&') <chstring>
<servername> ::= <host>
<host>       ::= see RFC 952 [DNS:4] for details on allowed hostnames
<nick>       ::= <letter> { <letter> | <number> | <special> }
<mask>       ::= ('#' | '$') <chstring>
<chstring>   ::= <any 8bit code except SPACE, BELL, NUL, CR, LF and
                  comma (',')>
 
<user>       ::= <nonwhite> { <nonwhite> }
<letter>     ::= 'a' ... 'z' | 'A' ... 'Z'
<number>     ::= '0' ... '9'
<special>    ::= '-' | '[' | ']' | '\\' | '`' | '^' | '{' | '}'
<nonwhite>   ::= <any 8bit code exceptSPACE (0x20), NUL (0x0),
                     CR(0xd), and LF (0xa)>
```

## 명령어

[1]: https://datatracker.ietf.org/doc/html/rfc1459
[2]: https://datatracker.ietf.org/doc/html/rfc2810
[3]: https://datatracker.ietf.org/doc/html/rfc2811
[4]: https://datatracker.ietf.org/doc/html/rfc2812
[5]: https://datatracker.ietf.org/doc/html/rfc2813
[6]: https://datatracker.ietf.org/doc/html/rfc7194
