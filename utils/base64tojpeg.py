import base64

# 指定输入的 Base64 文件路径
input_file = "input.txt"
# 指定输出的 JPEG 文件路径
output_file = "decoded_image.jpeg"

# 从文件中读取 Base64 编码数据
with open(input_file, "r") as file:
    base64_data = file.read()

# 将 Base64 数据解码为二进制数据
image_data = base64.b64decode(base64_data)

# 将解码后的数据保存到文件
with open(output_file, "wb") as file:
    file.write(image_data)

print(f"JPEG 图像已成功保存到 {output_file}")
