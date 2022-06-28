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
    serverName = "excitingirc"  # "penguin.omega.example.org"


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
    if b"PING" in result:
        sock.sendall(f"PONG :{Config.serverName.value}\r\n".encode())
    return result


@dataclass
class Client:
    name: str
    sock: socket.socket = field(init=False, default_factory=create_socket)

    def __enter__(self):
        self.connect()
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close()

    def close(self):
        self.sock.close()

    def connect(self):
        self.send(
            dedent(
                f"""\
                PASS password\r
                NICK {self.name}\r
                USER person 0 * :long name here\r
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
    for i in range(100):
        with Client(f"user{i:02}") as client:
            time.sleep(0.1)
            client.send(f"PING :{Config.serverName.value}\r\n")


if __name__ == "__main__":
    main()
