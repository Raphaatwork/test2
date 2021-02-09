/********************************************************************************
*                 TeleAlarm SA - Care solutions                                 *
* Module:               Tools.c                                                 *
* Product:              NurseCall                                               *
* Development system:   IAR Embedded Workbench 8.20                             *
* Description:          Useful stuff like converters, decoders etc              *
* Edited:               Tim Steinberg, 2020-03-03                               *
*                                                                               *
*********************************************************************************/

#include "MasterDefine.h"
#include "main.h"
#include "Tools.h"
#include "string.h"
#include "stm32l0xx_hal_crc.h"

extern CRC_HandleTypeDef hcrc;

uint8_t posValHexToDez(uint8_t byte){
  switch(byte){
    case 0x30:
      return 0;
    
    case 0x31:
      return 1;
    
    case 0x32:
      return 2;
    
    case 0x33:
      return 3;
    
    case 0x34:
      return 4;
    
    case 0x35:
      return 5;
    
    case 0x36:
      return 6;
    
    case 0x37:
      return 7;
    
    case 0x38:
      return 8;
    
    case 0x39:
      return 9;
    
    default:
      return 0xFF;
  }
}


/****************************************************************************************/
/* ====> unsigned long to ascii converter                                               */
/*       The result null terminated string has a length of n characters. Heading 0s are */
/*       added if necessary. If n parameter is smaller than actual string length, then  */
/*       the most significant digits are lost                                           */
/*       INPUT: lo:   long integer to convert to ascii                                  */
/*              s:    Address where to place the resulting string                       */
/*              n:    resulting string length                                           */
/*                                                                                      */
/****************************************************************************************/
void ultoaf(u32 lo, char *s, u8 n) /* This one returns fixed n number of digits */
{
  s16 i;

  for(i=n-1; i>=0; i--)
  {
    s[i] = (lo % 10) + '0';
    lo /= 10;
  }
  s[n] = 0;
}


/****************************************************************************************/
/* ====> unsigned long to ascii converter                                               */
/*       The result null terminated string has an unpredictable length because it has   */
/*       no heading 0s                                                                  */
/*       INPUT: lo:   long integer to convert to ascii                                  */
/*              s:    Address where to place the resulting string                       */
/*                                                                                      */
/****************************************************************************************/
void ultoa(u32 lo, char *s)
{
  s8 i;
  u8 j = 0;
  char go = 0;
  char tmp[11];

  tmp[10] = 0;
  
  for(i=9; i>=0; i--)
  {
    tmp[i] = (lo % 10) + '0';
    lo /= 10;
  }
  for(i=0; i<10; i++)
  {
    if(tmp[i] != '0' || go)
    {
      go = 1;
      s[j++] = tmp[i];
    }
  }
  if(j==0)
  	s[j++] = '0';
  s[j] = 0;
}



/****************************************************************************************/
/* ====> ASCII to unsigned long converter                                               */
/*       ASCII string of specified length is converted to an unsigned long              */
/*       Remarks:                                                                       */
/*        - The string to convert is not null terminated but its length is specified.   */
/*        - No support for exponent, no string syntax validity check                    */
/*       INPUT: s:    string to convert                                                 */
/*              n:    length of the string in digits.                                   */
/*                                                                                      */
/****************************************************************************************/
unsigned long atoul(char *s, char n)
{
  int i;
  unsigned long m = 1;
  unsigned long result = 0;
  
  
  if (n != 0)
  {
    for(i=n-1; i>=0; i--)
    {
      result = result + m * (*(s + i) - '0');
      m *= 10;
    }
  }
  return(result);
}

/****************************************************************************************/
/* ====> byte to hex ASCII converter                                                    */
/*       Convert the byte theByte to its 2 digits hexadecimal representation            */
/*       INPUT: theByte:  Byte to convert to hex ascii                                  */
/*              result:   Address where to place the 2 digits null terminated string    */
/*                                                                                      */
/****************************************************************************************/
void u82hex(u8 theByte, char *result)
{
  u8 temp;

  temp = theByte >> 4;
  if (temp <= 9)
    *result = temp + '0';
  else
    *result = temp + 0x37;
  result++;
  temp = theByte & 0x0f;
  if (temp <= 9)
    *result = temp + '0';
  else
    *result = temp + 0x37;
  result++;
  *result = 0;
}

/****************************************************************************************/
/* ====> Generate a pseudo-random number                                                */
/*       Use Park & Miller formula to generate a pseudo random number between           */
/*       1 and 2^31 -2. Number is in the unsigned long gl_rnd                           */
/*                                                                                      */
/*       x(n+1) = 16'807 x(n) mod (2^31- 1)                                             */
/*                                                                                      */
/*       Output:  Random number (0..2^16-1)                                             */
/*                                                                                      */
/*       For a larger random number, please directly use "unsigned long gl_rnd" global  */
/****************************************************************************************/
u32 gl_rnd = 1;                                                     // Random number initial seed = 1

u16 rnd()
{
  gl_rnd *= 16807;
  gl_rnd %= 2147483647;
  return((u16)gl_rnd);
}

/****************************************************************************************/
/* ====> Hash a u32 into a u32                                                          */
/*       u32 -> u32 hash inspired from Park & Miller formula.                           */
/*                                                                                      */
/*       INPUT:   u32: n: Number to hash                                                */
/*       OUTPUT:  u32: Result                                                           */
/****************************************************************************************/

u32 hash(u32 n)
{
  u8 i;
  u8 j;
  
  j = (u8)(n & 0x00000007) + 3;
  for(i=0; i<j; i++)
  {
    n *= 16807;
    n %= 2147483647;
  }
  return n;
}

/****************************************************************************************/
/* ====> Generate a pseudo-random number beween 0 and n-1                               */
/*                                                                                      */
/*                                                                                      */
/*                                                                                      */
/****************************************************************************************/

u16 rnd_bounded(u16 n)
{
  return(rnd() % n);
}

/********************************************************************************
* Function Name  : hex2u8                                                       *
* Description    : Convert 2 chars hex number 2 u8                              *
* Input          : s: Address of 2-digits ASCII buffer                          *
* Return         : u8 value                                                     *
*********************************************************************************/
u8 hex2u8(char* s)
{
  u8 tmp = 0;
  u8 res = 0;
  u8 i;

  for (i = 0; i <= 1; i++)
  {
    if (s[i] >= '0' && s[i] <= '9')
      tmp = s[i] - '0';
    else if (s[i] >= 'a' && s[i] <= 'f')
      tmp = s[i] - 'a' + 10;
    else if (s[i] >= 'A' && s[i] <= 'F')
      tmp = s[i] - 'A' + 10;

    res <<= 4;
    res += tmp;
  }
  return res;
}

/********************************************************************************
* Function Name  : rssi2a                                                       *
* Description    : Converts RSSI value read from S2LP chip to signed ascii      *
*                  (S2LP value has a +146dBm offset)                            *
* Input          : s: Address of 2-digits ASCII buffer                          *
* Return         : u8 value                                                     *
*********************************************************************************/
void rssi2a(u8 rssi, char *s)
{
  if(rssi < 146)
  {
    *s = '-';
    ultoa(146 - rssi, s+1);
  }
  else
  {
    ultoa(rssi - 146, s);
  }
}

/****************************************************************************************/
/* ====> unsigned long to ascii converter                                               */
/*       Same as ultoaf, but without left padding with 0's                              */
/*       INPUT: val   :  long integer to convert to ascii                               */
/*              string:    Address where to place the resulting string                  */
/*                                                                                      */
/****************************************************************************************/
int u32toString(u32 val, char *string)
{
  if (val < 10)
  {
    ultoaf(val, string, 1);
    return 1;
  }
  else if (val < 100)
  {
    ultoaf(val, string, 2);
    return 2;
  }
  else if (val < 1000)
  {
    ultoaf(val, string, 3);
    return 3;
  }
  else if (val < 10000)
  {
    ultoaf(val, string, 4);
    return 4;
  }
  else
  {
    ultoaf(val, string, 5);
    return 5;
  }
}

/****************************************************************************************/
/* ====> ascii to unsigned long converter                                               */
/*       Same as ultoaf, but without left padding with 0's                              */
/*       INPUT:   string:  long integer to convert to ascii                             */
/*       OUTPUT:           The converted value                                          */
/*                                                                                      */
/****************************************************************************************/
u32 Stringtou32(char* string)
{
  u32 res = 0;
  u8 digit;
  u32 mult = 1;
  u8 i;
  
  if (!string)
    return 0;
  
  for (i = strlen(string); i > 0; i--)
  {
    if (string[i - 1] >= '0' && string[i - 1] <= '9')
    {
      digit = string[i - 1] - '0';
      res += digit *mult;
    }
    mult *= 10;
  }
  return res;
}


/****************************************************************************************/
/* ====> Copy 12 bytes CPU ID to here                                                   */
/*       INPUT:  (u8*)here:    Address where to place 12 bytes                          */
/*                                                                                      */
/****************************************************************************************/
void getCpuid(u32* here)
{
  u32* cpuidptr = (u32*)0x1FF80050;                                             // Ptr to 96 bits CPU ID
  
  *here = *cpuidptr;
  *(here+1) = *(cpuidptr + 0x01);
  *(here+2) = *(cpuidptr + 0x05);
}

/****************************************************************************************/
/* ====> Get CPU ID CRC                                                                 */
/*                                                                                      */
/*       OUTPUT u32 crc                                                                 */
/*                                                                                      */
/****************************************************************************************/
u32 getCpuidCrc(void)
{
  u32 tmp[3];
  u32 result;
  
  __HAL_RCC_CRC_CLK_ENABLE();                                   // Initialise CRC clock
  HAL_StatusTypeDef HAL_CRC_Init(CRC_HandleTypeDef *hcrc);      // Initialize CRC calculator
  getCpuid(tmp);                                                // Place CPU ID in tmp 
  result = HAL_CRC_Calculate(&hcrc, tmp, 3);
  __HAL_RCC_CRC_CLK_DISABLE();                                  // Disable CRC clock
  return(result);
}


/*****************************************************************************************
* Function Name  : CompareUID                                                            *
*                                                                                        *
* Description    : Compare a 32 bits formatted UID with a u8 uid[4] formatted UID        *
*                                                                                        *
*       INPUT:   u32 uid32                                                               *
*                u8* uid8                                                                *
*                                                                                        *
*       OUTPUT:  1: Match                                                                *
*                0: No match                                                             *
*                                                                                        *
******************************************************************************************/

u8 CompareUID(u32 uid32, u8* uid8)
{
  return(uid8[0] == (u8)uid32 && uid8[1] == (u8)(uid32 >> 8) && uid8[2] == (u8)(uid32 >> 16) && uid8[3] == (u8)(uid32 >> 24));
}


