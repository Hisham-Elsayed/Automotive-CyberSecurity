@echo off

:: Prompt script
set /p name=algorithm name: 
set OPENSSL_DIR_TX=C:\Hisham\Automotive CyberSecurity\Repo\Lab5_prompt_script\Tx
set OPENSSL_DIR_RX=C:\Hisham\Automotive CyberSecurity\Repo\Lab5_prompt_script\Rx

if "%name%"=="AES" (
	
:: Generate key using AES
openssl rand -out "%OPENSSL_DIR_TX%\key.bin" 16
echo "AES key 128 has been generated"	
:: Encrypt plaintext using -> Symmetric
openssl enc -aes-128-cbc -in "%OPENSSL_DIR_TX%\plaintext.txt" -out "%OPENSSL_DIR_RX%\ciphertext.enc" -pass file:"%OPENSSL_DIR_TX%\key.bin"

:: Decrypt ciphertext generated
openssl enc -aes-128-cbc -d -in "%OPENSSL_DIR_RX%\ciphertext.enc" -out "%OPENSSL_DIR_RX%\plaintext.txt" -pass file:"%OPENSSL_DIR_TX%\key.bin"
)

if "%name%"=="DES" (
	
:: Generate key using DES
openssl rand -out "%OPENSSL_DIR_TX%\key.bin" 8
echo "DES key has been generated"	
:: Encrypt plaintext using -> Symmetric
openssl enc -des-ede -in "%OPENSSL_DIR_TX%\plaintext.txt" -out "%OPENSSL_DIR_RX%\ciphertext.enc" -pass file:"%OPENSSL_DIR_TX%\key.bin"

:: Decrypt ciphertext generated
openssl enc -des-ede -d -in "%OPENSSL_DIR_RX%\ciphertext.enc" -out "%OPENSSL_DIR_RX%\plaintext.txt" -pass file:"%OPENSSL_DIR_TX%\key.bin"
)


if "%name%"=="RSA" (
:: Generate key using RSA 
openssl genrsa -out "%OPENSSL_DIR_RX%\privatekey.bin" 2048

:: Generate public key using private key
openssl rsa -in "%OPENSSL_DIR_RX%\privatekey.bin" -pubout -out "%OPENSSL_DIR_TX%\publickey.bin"

:: Encyrpt plain text -> Asymmetric
openssl pkeyutl -encrypt -pubin -inkey "%OPENSSL_DIR_TX%\publickey.bin" -in "%OPENSSL_DIR_TX%\plaintext.txt" -out "%OPENSSL_DIR_RX%\ciphertext.enc"

:: Decrypt ciphertext generated
openssl pkeyutl -decrypt -inkey "%OPENSSL_DIR_RX%\privatekey.bin" -in "%OPENSSL_DIR_RX%\ciphertext.enc" -out "%OPENSSL_DIR_RX%\plaintext.txt"
)

if "%name%"=="hybrid" (
:: Generate key using RSA 
openssl genrsa -out "%OPENSSL_DIR_RX%\privatekey.bin" 2048

:: Generate public key using private key
openssl rsa -in "%OPENSSL_DIR_RX%\privatekey.bin" -pubout -out "%OPENSSL_DIR_TX%\publickey1.bin"

:: Generate public key 2 -> Symmetric  
openssl rand -out "%OPENSSL_DIR_TX%\publickey2.bin" 16

:: Encrypt public key 2 using public key 1 -> Asymmetric
openssl pkeyutl -encrypt -pubin -inkey "%OPENSSL_DIR_TX%\publickey1.bin" -in "%OPENSSL_DIR_TX%\publickey2.bin" -out "%OPENSSL_DIR_RX%\publickey2.enc"

:: Decrypt public key 2 using private key
openssl pkeyutl -decrypt -inkey "%OPENSSL_DIR_RX%\privatekey.bin" -in "%OPENSSL_DIR_RX%\publickey2.enc" -out "%OPENSSL_DIR_RX%\decryptpublickey2.bin"

:: Encrypt plaintext using public key 2 -> Symmetric
openssl enc -aes-128-cbc -in "%OPENSSL_DIR_TX%\plaintext.txt" -out "%OPENSSL_DIR_RX%\plaintext.enc" -pass file:"%OPENSSL_DIR_TX%\publickey2.bin"

:: Decrypt plain text -> Symmetric.
openssl enc -d -aes-128-cbc -in "%OPENSSL_DIR_RX%\plaintext.enc" -out "%OPENSSL_DIR_RX%\plaintext.txt" -pass file:"%OPENSSL_DIR_RX%\decryptpublickey2.bin"
)