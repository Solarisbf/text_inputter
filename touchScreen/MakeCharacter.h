#ifndef HW3_MakeCharacter
#define HW3_MakeCharacter

// Define input KeyType
#define KEYPAD_KOR		0
#define KEYPAD_ENG		2
#define KEYPAD_ENG_CAPS		3
#define KEYPAD_NUM		4

// Define KEYPAD_KOR
#define KEY_G		0x00
#define KEY_N		0x01
#define KEY_A		0x02
#define KEY_R		0x04
#define KEY_M		0x05
#define KEY_U		0x06
#define KEY_S		0x08
#define KEY_O		0x09
#define KEY_I		0x0A
#define KEY__		0x0D
#define KEY_DEL		0x03
#define KEY_CHMOD	0x07
#define KEY_SPACE	0x0F
#define KEY_ADD		0x0C
#define KEY_DOUBLE	0x0E

// Define KEYPAD_ENG
#define KEY_abc		0x01
#define KEY_def		0x02
#define KEY_ghi		0x04
#define KEY_jkl		0x05
#define KEY_mno		0x06
#define KEY_pqrs	0x08
#define KEY_tuv		0x09
#define KEY_wxyz	0x0A
#define KEY_DEL		0x03
#define KEY_CHMOD	0x07
#define KEY_CAPS	0x0B
#define KEY_SPACE	0x0F

// Define KEYPAD_NUM
#define KEY_1		0x00
#define KEY_2		0x01
#define KEY_3		0x02
#define KEY_4		0x04
#define KEY_5		0x05
#define KEY_6		0x06
#define KEY_7		0x08
#define KEY_8		0x09
#define KEY_9		0x0A
#define KEY_0		0x0D
#define KEY_DEL		0x03
#define KEY_CHMOD	0x07
#define KEY_SPACE	0x0F
#define KEY_star	0x0C
#define KEY_pppp	0x0E

// Define Change (Return) Type (int)
// RT_CHMOD > RT_MIX > RT_ENDEDIT > RT_DEL > RT_NEW
#define RT_NULL 		0x00000000		// Not Happen
#define RT_MASK			0xFF000000
#define RT_CHMOD		0x01000000			// RT_CHMOD | (Define input KeyType)

#define RT_MIX			0x02000000
#define RT_MIX_C		0x02000010
#define RT_MIX_P		0x02000020
#define RT_MIX_A		0x02000030

#define RT_ENDEDIT		0x04000000
#define RT_ENDEDIT_C	0x04001000
#define RT_ENDEDIT_P	0x04002000
#define RT_ENDEDIT_A	0x04003000

#define RT_DEL			0x08000000
#define RT_NEW			0x10000000
#define RT_NEW_ENDEDTER	0x10010000


// Struct
typedef struct {
	unsigned short prevChar;
	unsigned short Char;
	unsigned short newChar;
}MakeBoard;

// Function
/*
 * makeCharacter
 * MakeBoard *mb    : change Value (return value)
 * unsigned int key : input Key
 * int *inputKeyType: Define input KeyType
 * return           : change Type  (Define Change (Return) Type (int))
 */
unsigned int makeCharacter(MakeBoard *mb, unsigned int key, int inputKeyType);

#endif
