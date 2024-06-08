import socket
import os
import struct

import numpy as np

import cbr_knn
import protocol

unix_socket_loc = "/tmp/cbr_socket.s"


def main():
  if os.path.exists(unix_socket_loc):
    os.remove(unix_socket_loc)

  server = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
  server.bind(unix_socket_loc)
  while True:
    server.listen(1)
    conn, addr = server.accept()
    while conn is not None:
      type = conn.recv(1)
      type = int.from_bytes(type, byteorder='little')
      body_size = conn.recv(4)
      body_size = int.from_bytes(body_size, byteorder='little')
      data = conn.recv(body_size)

      match type:
        case 0:  # diagnose
          diagnose(data, conn)
        case 1:  # Data
          handle_get_data(data, conn)
        case 3:  # exit
          conn.close()
          break

      # conn.send(b"Hello")


def handle_get_data(payload: bytes, conn: socket.socket):
  data_bytes = protocol.serialize_data()  # array of bytes
  data_bytes = b''.join(data_bytes)
  msg = protocol.create_message(1, data_bytes)
  conn.sendall(msg)


def diagnose(payload: bytes, conn: socket.socket):
  if len(payload) == 0:
    return
  test = np.array(protocol.deserialize_diagnose(payload))
  pred, index, similarities, _ = cbr_knn.model.predict(test)
  best_index = int(index[0])
  best_similarity = float(similarities[0]) * 100
  resp = protocol.serialize_diagnose_response(bool(pred), best_index, best_similarity)
  msg = protocol.create_message(0, resp)
  conn.sendall(msg)


if __name__ == "__main__":
  cbr_knn.init()
  main()
