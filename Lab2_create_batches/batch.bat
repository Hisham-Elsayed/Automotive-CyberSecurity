@echo off

REM Generate a key command
openssl rand -out "C:\Hisham\Automotive CyberSecurity\Repo\Lab2_create_batches\Tx\key.bin" 128

echo AES Key 128 has been generated...

REM Generate encrypted file 
openssl aes-128-cbc -in "C:\Hisham\Automotive CyberSecurity\Repo\Lab2_create_batches\Tx\plaintext.txt" -out "C:\Hisham\Automotive CyberSecurity\Repo\Lab2_create_batches\Rx\ciphertext.enc" -pass file:"C:\Hisham\Automotive CyberSecurity\Repo\Lab2_create_batches\Tx\key.bin"
echo plaintext has been encrypted

REM Decrypt file 
openssl aes-128-cbc -d -in "C:\Hisham\Automotive CyberSecurity\Repo\Lab2_create_batches\Rx\ciphertext.enc" -out "C:\Hisham\Automotive CyberSecurity\Repo\Lab2_create_batches\Rx\plaintext.txt" -pass file:"C:\Hisham\Automotive CyberSecurity\Repo\Lab2_create_batches\Tx\key.bin"
echo cipher file has been decrypted...