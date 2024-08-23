#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gmssl/sm4.h>
#include <gmssl/rand.h>
#include <memory>
#include <iostream>
#include <chrono>
#include <thread>
#include "util/log.h"
#include "util/common.h"
#include "fmt/format.h"

RETCODE sm4_cbc_mode_encrypt(unsigned char* key, unsigned char* iv,
                            uint8_t* plaintext, size_t plain_len,
                            uint8_t* ciphertext, size_t* cipher_len) {
  SM4_CBC_CTX cbc_ctx;
  size_t outlen = 0;
  LOG(INFO) << BASE_INFO() << ENDL;
  if (sm4_cbc_encrypt_init(&cbc_ctx, key, iv) != 1) {
    LOG(ERROR) << BASE_INFO() << ": error \n";
		return RETCODE_FAIL;
	}
  if (sm4_cbc_encrypt_update(&cbc_ctx,
                            plaintext, plain_len,
                            ciphertext, &outlen) != 1) {
    LOG(ERROR) << BASE_INFO() << "error \n";
    return RETCODE_FAIL;
  }
  *cipher_len = outlen;
  uint8_t* ptr_offset = ciphertext + outlen;
  if (sm4_cbc_encrypt_finish(&cbc_ctx, ptr_offset, &outlen) != 1) {
		LOG(ERROR) << BASE_INFO() << "error \n";
		return RETCODE_FAIL;
	}
  *cipher_len += outlen;
  return RETCODE_SUCCESS;
}

RETCODE sm4_cbc_mode_decrypt(unsigned char* key, unsigned char* iv,
                            uint8_t* ciphertext, size_t cipher_len,
                            uint8_t* plaintext, size_t* plain_len) {
  SM4_CBC_CTX cbc_ctx;
  size_t outlen;
	if (sm4_cbc_decrypt_init(&cbc_ctx, key, iv) != 1) {
		LOG(ERROR) << BASE_INFO() << "error \n";
		return RETCODE_FAIL;
	}

  if (sm4_cbc_decrypt_update(&cbc_ctx,
                            ciphertext, cipher_len,
                            plaintext, &outlen) != 1) {
    LOG(ERROR) << BASE_INFO() << "error \n";
    return RETCODE_FAIL;
  }
  *plain_len = outlen;
  unsigned char outbuf[SM4_BLOCK_SIZE];
	if (sm4_cbc_decrypt_finish(&cbc_ctx, outbuf, &outlen) != 1) {
		LOG(ERROR) << BASE_INFO() << "error \n";
		return RETCODE_FAIL;
	}
  if (outlen > 0) {
    uint8_t* dest = ciphertext+(*plain_len);
    memcpy(dest, outbuf, outlen);
    *plain_len += outlen;
  }
	return RETCODE_SUCCESS;
}

constexpr size_t PaddedDataSize(size_t data_len) {
  return data_len + (data_len % SM4_BLOCK_SIZE ?
                        (SM4_BLOCK_SIZE - data_len % SM4_BLOCK_SIZE) :
                        SM4_BLOCK_SIZE);
}

int main(void)
{
	SM4_CBC_CTX cbc_ctx;
  unsigned char key[SM4_BLOCK_SIZE];
  unsigned char iv[SM4_BLOCK_SIZE];
  rand_bytes(key, SM4_BLOCK_SIZE);
  rand_bytes(iv, SM4_BLOCK_SIZE);
  auto _start = std::chrono::high_resolution_clock::now();
  constexpr size_t plaintext_len = 5 * 1024 * 1024ll;

  LOG(INFO) << "plain text len: " << plaintext_len << "\n";
  constexpr size_t ciphertext_len = PaddedDataSize(plaintext_len);
  auto inbuf_ptr = std::make_unique<unsigned char[]>(plaintext_len);
	auto inbuf = inbuf_ptr.get();
	// unsigned char outbuf[2048 + 32];
  LOG(INFO) << "ciphertext_len: " << ciphertext_len << "\n";
  auto outbuf_ptr = std::make_unique<unsigned char[]>(ciphertext_len);
  auto outbuf = outbuf_ptr.get();
	size_t inlen = plaintext_len;
	size_t outlen;
  memset(inbuf, 'A', inlen);
  auto _end = std::chrono::high_resolution_clock::now();
  auto time_cost = std::chrono::duration_cast<std::chrono::milliseconds>(_end - _start).count();
  LOG(INFO) << "prepare data time cost(ms): " << time_cost << "\n";
  {
    _start = std::chrono::high_resolution_clock::now();
    sm4_cbc_mode_encrypt(key, iv, inbuf, inlen, outbuf, &outlen);
    _end = std::chrono::high_resolution_clock::now();
    time_cost = std::chrono::duration_cast<std::chrono::milliseconds>(_end - _start).count();
    LOG(INFO) << "encrypt data time cost(ms): " << time_cost << "\n";
  }

  LOG(INFO) << "inlen: " << inlen << " outlen: " << outlen << "\n";

  auto rec_buf_ptr = std::make_unique<unsigned char[]>(plaintext_len);
  auto rec_buf = rec_buf_ptr.get();
   _start = std::chrono::high_resolution_clock::now();
  sm4_cbc_mode_decrypt(key, iv, outbuf, outlen, rec_buf, &inlen);
  _end = std::chrono::high_resolution_clock::now();
  time_cost = std::chrono::duration_cast<std::chrono::milliseconds>(_end - _start).count();
  LOG(INFO) << "decrypt data time cost(ms): " << time_cost << "\n";
  LOG(INFO) << "inlen: " << inlen << " outlen: " << outlen << "\n";
  // std::string rec_str{reinterpret_cast<char*>(rec_buf), inlen};
  outbuf_ptr.reset();
  // std::this_thread::sleep_for(std::chrono::seconds(5));
  if (memcmp(inbuf, rec_buf, plaintext_len) == 0) {
    LOG(INFO) << "equal with origin data\n";
  } else {
    LOG(INFO) << "not equal with orgin data\n";
  }
	return 0;
}
