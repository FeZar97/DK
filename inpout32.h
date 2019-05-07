#pragma once

//Functions exported from DLL.
//For easy inclusion is user projects.
//Original InpOut32 function support
// void _stdcall
void Out32(short PortAddress, short data);
short Inp32(short PortAddress);

//My extra functions for making life easy
bool IsInpOutDriverOpen();  //Returns TRUE if the InpOut driver was opened successfully
bool IsXP64Bit();			//Returns TRUE if the OS is 64bit (x64) Windows.

//DLLPortIO function support
unsigned short DlPortReadPortUchar (unsigned short port);
void DlPortWritePortUchar(unsigned short port, unsigned char Value);

unsigned short DlPortReadPortUshort (unsigned short port);
void DlPortWritePortUshort(unsigned short port, unsigned short Value);

unsigned long DlPortReadPortUlong(unsigned long port);
void DlPortWritePortUlong(unsigned long port, unsigned long Value);
