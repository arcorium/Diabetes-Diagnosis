import struct
import cbr_knn


def create_message(type: int, payload: bytes):
  # Pre-allocate the full message size to avoid repeated concatenation
  header_size = 5  # 1 byte type + 4 bytes length
  msg_size = header_size + len(payload)
  result = bytearray(msg_size)
  
  result[0:1] = type.to_bytes(1, 'little')
  result[1:5] = len(payload).to_bytes(4, 'little')
  result[5:] = payload
  
  return bytes(result)


def deserialize_diagnose(payload: bytes):
  pregnancies = payload[0]
  glucose = int.from_bytes(payload[1:3], "little")
  blood_pressure = payload[3]
  skin_thickness = payload[4]
  insulin = int.from_bytes(payload[5:7], "little")  # Fixed: insulin is 2 bytes (values up to 846)
  bmi = struct.unpack('f', payload[7:11])[0]  # Adjusted offset
  dpf = struct.unpack('f', payload[11:15])[0]  # Adjusted offset
  age = payload[15]  # Adjusted offset

  return (pregnancies, glucose, blood_pressure, skin_thickness, insulin, bmi, dpf, age)


def serialize_diagnose_response(predict: bool, index: int, similarity: float) -> bytes:
  pred_byte = predict.to_bytes(1, 'little')
  index_byte = index.to_bytes(4, "little")
  sim_byte = struct.pack('f', similarity)
  return b''.join([pred_byte, index_byte, sim_byte])


def serialize_data() -> [bytes]:
  # Pre-allocate the result list for better performance
  data_result = [None] * len(cbr_knn.data)
  
  for idx, data in enumerate(cbr_knn.data):
    (pregnancies, glucose, blood_pressure, skin_thickness, insulin, bmi, dpf, age, result) = data
    
    # Pre-allocate bytearray for this data item (1+2+1+1+2+4+4+1+1 = 17 bytes)
    data_bytes = bytearray(17)
    data_bytes[0:1] = int(pregnancies).to_bytes(1, 'little')
    data_bytes[1:3] = int(glucose).to_bytes(2, 'little')
    data_bytes[3:4] = int(blood_pressure).to_bytes(1, 'little')
    data_bytes[4:5] = int(skin_thickness).to_bytes(1, 'little')
    data_bytes[5:7] = int(insulin).to_bytes(2, 'little')
    data_bytes[7:11] = struct.pack('f', float(bmi))
    data_bytes[11:15] = struct.pack('f', float(dpf))
    data_bytes[15:16] = int(age).to_bytes(1, 'little')
    data_bytes[16:17] = int(result).to_bytes(1, 'little')
    
    data_result[idx] = bytes(data_bytes)
  
  return data_result
