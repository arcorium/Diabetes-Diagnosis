import struct
import cbr_knn


def create_message(type: int, payload: bytes):
  result = bytes()
  result += type.to_bytes(1, 'little')
  result += len(payload).to_bytes(4, 'little')
  result += payload
  return result


def deserialize_diagnose(payload: bytes):
  pregnancies = payload[0]
  glucose = int.from_bytes(payload[1:3], "little")
  blood_pressure = payload[3]
  skin_thickness = payload[4]
  insulin = payload[5]
  bmi = struct.unpack('f', payload[6:10])[0]
  dpf = struct.unpack('f', payload[10:14])[0]
  age = payload[14]

  return (pregnancies, glucose, blood_pressure, skin_thickness, insulin, bmi, dpf, age)


def serialize_diagnose_response(predict: bool, index: int, similarity: float) -> bytes:
  pred_byte = predict.to_bytes(1, 'little')
  index_byte = index.to_bytes(4, "little")
  sim_byte = struct.pack('f', similarity)
  return b''.join([pred_byte, index_byte, sim_byte])


def serialize_data() -> [bytes]:
  data_result = []
  for data in cbr_knn.data:
    (pregnancies, glucose, blood_pressure, skin_thickness, insulin, bmi, dpf, age, result) = data
    prg = int(pregnancies).to_bytes(1, 'little')
    glu = int(glucose).to_bytes(2, 'little')
    bld = int(blood_pressure).to_bytes(1, 'little')
    skn = int(skin_thickness).to_bytes(1, 'little')
    ins = int(insulin).to_bytes(2, 'little')
    bm = struct.pack('f', float(bmi))
    dp = struct.pack('f', float(dpf))
    ag = int(age).to_bytes(1, 'little')
    pred = int(result).to_bytes(1, 'little')
    data_result.append(b''.join([prg, glu, bld, skn, ins, bm, dp, ag, pred]))
  return data_result
