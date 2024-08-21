#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gmssl/sm2.h>
#include <gmssl/error.h>
#include <string>
#include <iostream>
void generate_data(std::string& plain_text_data, size_t data_size) {
  plain_text_data.reserve(data_size);
  for (size_t i = 0; i < data_size; i++) {
    std::string str = "D";
    switch (i % 3) {
    case 0:
      str = "A";
      break;
    case 1:
      str = "B";
      break;
    case 2:
      str = "C";
      break;
    default:
      break;
    }
    plain_text_data.append(str);
  }
}
int main(void) {
	SM2_KEY sm2_key;
	SM2_KEY pub_key;
  const int plain_data_size = 1024;
	// unsigned char plaintext[SM2_MAX_PLAINTEXT_SIZE];
  unsigned char plaintext[plain_data_size*2];
	unsigned char ciphertext[SM2_MAX_CIPHERTEXT_SIZE];
  // unsigned char ciphertext[plain_data_size*2];
	size_t len;

	sm2_key_generate(&sm2_key);
	memcpy(&pub_key, &sm2_key, sizeof(SM2_POINT));
  std::string plain_text_data;
  generate_data(plain_text_data, plain_data_size);
  std::string ciphertext_data;

  std::cout << "SM2_MAX_PLAINTEXT_SIZE: " << SM2_MAX_PLAINTEXT_SIZE << "\n";
  std::cout << "SM2_MAX_CIPHERTEXT_SIZE: " << SM2_MAX_CIPHERTEXT_SIZE << "\n";
  uint8_t* plain_ptr = reinterpret_cast<uint8_t*>(&plain_text_data[0]);
  const int block_size = SM2_MAX_PLAINTEXT_SIZE;
  int block_num = plain_text_data.size() / block_size;
  int rem = plain_text_data.size() % block_size;
  size_t buffer_len = block_num * SM2_MAX_CIPHERTEXT_SIZE + rem > 0 ? SM2_MAX_CIPHERTEXT_SIZE : 0;
  ciphertext_data.reserve(buffer_len);
  for (int i = 0; i < block_num; i++) {
    memset(ciphertext, 0, SM2_MAX_CIPHERTEXT_SIZE);
    int offset = i * block_size;
    int ret = sm2_encrypt(&pub_key, plain_ptr + offset, block_size, ciphertext, &len);
    if (ret != 1) {
      std::cout << "sm2_encrypt failed\n";
      return -1;
    }
    std::cout << "SM2_MAX_PLAINTEXT_SIZE: cipher text size: " << len << "\n";
    ciphertext_data.append(reinterpret_cast<char*>(&len), sizeof(len));
    ciphertext_data.append(reinterpret_cast<char*>(ciphertext), len);
  }
  if (rem != 0) {
    memset(ciphertext, 0, SM2_MAX_CIPHERTEXT_SIZE);
    int offset = block_num * block_size;
    int ret = sm2_encrypt(&pub_key, plain_ptr + offset, rem, ciphertext, &len);
    if (ret != 1) {
      std::cout << "sm2_encrypt failed\n";
      return -1;
    }
    std::cout << "rem: cipher text size: " << len << "\n";
    ciphertext_data.append(reinterpret_cast<char*>(&len), sizeof(len));
    ciphertext_data.append(reinterpret_cast<char*>(ciphertext), len);
  }

	// format_bytes(stdout, 0, 0, "ciphertext", ciphertext, len);
  size_t index = 0;
  auto ptr_base = (uint8_t*)(&ciphertext_data[0]);
  uint8_t* ptr_offset = ptr_base;
  while (index < ciphertext_data.size()) {
    auto cipher_len = reinterpret_cast<size_t*>(ptr_offset);
    index += sizeof(size_t);
    std::cout << "block size: " << *cipher_len << "\n";
    ptr_offset += sizeof(size_t);
    int ret = sm2_decrypt(&sm2_key, ptr_offset, *cipher_len, plaintext, &len);
    if (ret != 1) {
      fprintf(stderr, "error\n");
		  return 1;
    } else {
      plaintext[len] = '\0';
	    printf("plaintext: %s\n", plaintext);
    }
    ptr_offset += *cipher_len;
    index += *cipher_len;
  }
	return 0;
}
