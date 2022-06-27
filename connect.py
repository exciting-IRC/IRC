#!/usr/bin/env python3

import logging
import socket
import time
from dataclasses import dataclass, field
from enum import Enum
from textwrap import dedent
from typing import ClassVar

logging.basicConfig(level=logging.NOTSET)


class Config(Enum):
    host = socket.gethostname()
    port = 6667
    serverName = "excitingirc"


def create_socket() -> socket.socket:
    "소켓을 만들고 연결해서 돌려주기"
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((Config.host.value, Config.port.value))
    return sock


def send_and_recieve(sock: socket.socket, msg: bytes) -> bytes:
    "소켓에 메시지를 보내고 받아서 돌려주기"
    sock.sendall(msg)
    logging.info(f"sent {msg = }")
    result = sock.recv(1024)
    logging.info(f"received {result = }")
    return result


@dataclass
class Client:
    no: ClassVar[int] = 0
    name: str = field(init=False)
    sock: socket.socket = field(init=False, default_factory=create_socket)

    def __post_init__(self):
        self.name = f"user{Client.no:02}"
        Client.no += 1

    def connect(self):
        self.send(
            dedent(
                f"""\
                PASS :password\r
                NICK {self.name}\r
                USER user user localhost :longusername\r
                """
            )
        )

    def send(self, msg: str | bytes) -> str:
        if isinstance(msg, str):
            if not msg.endswith("\r\n"):
                msg += "\r\n"
            msg = msg.encode()

        return send_and_recieve(self.sock, msg).decode()


def main() -> None:
    client = Client()
    client.connect()
    client.send(f"PING :{Config.serverName.value}\r\n")


if __name__ == "__main__":
    main()
