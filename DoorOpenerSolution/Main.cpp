
#include "stdafx.h"
#include "../DoorOpenerSketch/Desfire.h"

// *****************************************************************************
// The idea of this code is only to check the encryption stuff on Visual Studio.
// But you can also write your own code that communicates with a PN532 board
// through additional hardware (e.g. a PCI card that has digital in/outputs)
// See WinDefines.h
// *****************************************************************************

int _tmain(int argc, _TCHAR* argv[])
{
    const int DES_SIZE =  8; // One DES block =  8 bytes
    const int AES_SIZE = 16; // One AES block = 16 bytes
    Desfire i_Desfire;

    // ##################################################
    // Testing encryption and decryption
    // ##################################################

    byte u8_Data   [DES_SIZE] = { 'H', 'e', 'l', 'l', 'o', 0, 0, 0 };
    byte u8_Crypt  [DES_SIZE] = {0};
    byte u8_Decrypt[DES_SIZE] = {0};

    if (!i_Desfire.DES3_DEFAULT_KEY.CryptDataBlock(u8_Crypt,   u8_Data,  KEY_ENCIPHER) ||
        !i_Desfire.DES3_DEFAULT_KEY.CryptDataBlock(u8_Decrypt, u8_Crypt, KEY_DECIPHER))
        return 0;

    Utils::Print("8 data bytes:", LF);    
    Utils::PrintHexBuf(u8_Data, DES_SIZE, LF);

    Utils::Print("Encrypted with default DES3 key:", LF);
    Utils::PrintHexBuf(u8_Crypt, DES_SIZE, LF);

    Utils::Print("Decrypted with default DES3 key:", LF);
    Utils::PrintHexBuf(u8_Decrypt, DES_SIZE, LF);

    Utils::Print(LF, LF);

    // ##################################################
    // Testing CMAC calculation
    // 
    // See "Desfire EV1 Communication Examples.htm":
    //
    // ........
    // * SessKey:   90 F7 A2 01 91 03 68 45 EC 63 DE CD 54 4B 99 31 (AES)
    // 
    // *** GetKeyVersion()
    // TX CMAC:  25 7F C5 38 61 8A 94 4A 3A 20 96 7B 6F 31 43 48
    // Sending:  00 00 FF 05 FB <D4 40 01 64 00> 87 00
    // Response: 00 00 FF 0D F3 <D5 41 00 00 10 8A 8F A3 6F 55 CD 21 0D> 5F 00
    // RX CMAC:  8A 8F A3 6F 55 CD 21 0D D8 05 46 58 AC 70 D9 9A
    // Version: 0x10
    // ##################################################

    byte u8_SessKey[AES_SIZE] = { 0x90, 0xF7, 0xA2, 0x01, 0x91, 0x03, 0x68, 0x45, 0xEC, 0x63, 0xDE, 0xCD, 0x54, 0x4B, 0x99, 0x31 };

    AES i_SessionKey;
    if (!i_SessionKey.SetKeyData(u8_SessKey, sizeof(u8_SessKey), 0) ||
        !i_SessionKey.GenerateCmacSubkeys())
        return 0;

    Utils::Print("Session Key:", LF);
    i_SessionKey.PrintKey(LF);

    // Calculate CMAC of send data
    TX_BUFFER(i_TxData, AES_SIZE);
    i_TxData.AppendUint8(DF_INS_GET_KEY_VERSION); // Command     = 0x64
    i_TxData.AppendUint8(0);                      // Parameter 1 = Key 0

    byte u8_Cmac[AES_SIZE];
    if (!i_SessionKey.CalculateCmac(i_TxData, u8_Cmac))
        return 0;

    Utils::Print("TX CMAC of Command GET_KEY_VERSION:", LF);
    Utils::PrintHexBuf(u8_Cmac, sizeof(u8_Cmac), LF);

    // -------------------

    // Calculate CMAC of receive data
    TX_BUFFER(i_RxData, AES_SIZE);
    i_RxData.AppendUint8(0x10);       // Key version
    i_RxData.AppendUint8(ST_Success); // Status 0

    if (!i_SessionKey.CalculateCmac(i_RxData, u8_Cmac))
        return 0;

    Utils::Print("RX CMAC of Response GET_KEY_VERSION:", LF);
    Utils::PrintHexBuf(u8_Cmac, sizeof(u8_Cmac), LF);


    // ##################################################
    // Wait for a keypress before exiting
    // ##################################################

    getch();
	return 0;
}



