import struct

# 定义一个浮点数
float_num = 5.215

# 将浮点数转换为二进制字符串
binary_string = struct.pack(">f", float_num)

# 打印二进制字符串
print(bin(int.from_bytes(binary_string, "big")))

# 如果需要转换回浮点数
float_num_converted = struct.unpack(">f", binary_string)[0]
print(float_num_converted)
