import time

# New key generation function [Dr. N, 10/24/2020]
def genkey(keylen):
	seed = int(time.time()) & 0xffffff
	key = ''
	modulus = pow(2,24)
	a = 1140671485 
	b = 12820163
	for i in range(keylen):
		seed = (a*seed + b) % modulus
		key = key + chr(seed >> 16)
	return(key)
