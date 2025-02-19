/**************************************************************************
    
    @author   Elmü   
    This file contains secret values for encryption.
  
**************************************************************************/

#ifndef SECRETS_H
#define SECRETS_H

// The PICC master key.
// This 3K3DES or AES key is the "god key".
// It allows to format the card and erase ALL it's content (except the PICC master key itself).
// This key will be stored on your Desfire card when you execute the command "ADD {Username}" in the terminal.
// To restore the master key to the factory default DES key use the command "RESTORE" in the terminal.
// If you set the compiler switch USE_AES = true, only the first 16 bytes of this key will be used.
// IMPORTANT: Before changing this key, please execute the RESTORE command on all personalized cards!
// IMPORTANT: When you compile for DES, the least significant bit (bit 0) of all bytes in this key 
//            will be modified, because it stores the key version.
const byte SECRET_PICC_MASTER_KEY[24] = { 0xED, 0x80, 0x27, 0x12, 0x1D, 0x67, 0xEF, 0x62, 0xE7, 0xD2, 0x71, 0x74, 0xF8, 0xAF, 0x8D, 0x2B, 0xC8, 0x99, 0x08, 0x76, 0x54, 0x02, 0xFF, 0x13 };

// This 3K3DES key is used to derive a 16 byte application master key from the UID of the card and the user name.
// The purpose is that each card will have it's unique application master key that can be calculated from known values.
const byte SECRET_APPLICATION_KEY[24] = { 0x88, 0x0F, 0x5A, 0xD9, 0x48, 0xE9, 0xA2, 0xDD, 0x7C, 0xB3, 0xEE, 0x9D, 0xE9, 0xA6, 0x43, 0x2E, 0x5B, 0xF6, 0x85, 0xB8, 0x94, 0xD8, 0xD0, 0x5B };

// This 3K3DES key is used to derive the 16 byte store value from the UID of the card and the user name.
// This value is stored in a standard data file on the card.
// The purpose is that each card will have it's unique store value that can be calculated from known values.
const byte SECRET_STORE_VALUE_KEY[24] = { 0xEE, 0x9D, 0x70, 0x57, 0x6F, 0xFC, 0xE4, 0xC9, 0x49, 0xEC, 0x63, 0xDE, 0xEC, 0x7A, 0x8B, 0xC5, 0x7F, 0x0A, 0xC9, 0x6C, 0xAA, 0xD8, 0x4F, 0x27 };

// -----------------------------------------------------------------------------------------------------------

// The ID of the application to be created
// This value must be between 0x000001 and 0xFFFFFF (NOT zero!)
const uint32_t CARD_APPLICATION_ID = 0xAA401F;

// The ID of the file to be created in the above application
// This value must be between 0 and 31
const byte CARD_FILE_ID = 0;

// This 8 bit version number is uploaded to the card together with the key itself.
// This version is irrelevant for encryption. 
// It is just a version number for the key that you can obtain with Desfire::GetKeyVersion().
// The key version can always be obtained without authentication.
// You can theoretically have multiple master keys and by obtaining the version you know which one to use for authentication.
// This value must be between 1 and 255 (NOT zero!)
const byte CARD_KEY_VERSION = 0x10;

#endif
