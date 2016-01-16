#include "asmUtil.h"
#include "keyboard.h"


/* 
 * Input is from processor to devices
 * Output is from devices to processor
 */

///////////////////////////////////////////////////////////////////////////////
// Functions for keyboard controller & keyboard managerment
///////////////////////////////////////////////////////////////////////////////
// Return 1 when there is received data.
unsigned char isOutputBufferFull( void )
{
	// If the output buffer bit(bit 0) in the control/state register is 1,
	// there is received data in the output buffer
	// Output buffer is 0x60, its control/state register buffer ix 0x64 
	if ( inPortByte( 0x64 ) & 0x01 )
	{
		return 1;
	}
	
	return 0;
}

// Return 1 when there is remained data to use
unsigned char isInputBufferFull( void )
{
	// If the input buffer bit(bit 1) in the control/state register is 1,
	// there is remianed data in the output buffer	
	// Output buffer is 0x60, its control/state register buffer ix 0x64
	if ( inPortByte( 0x64 ) & 0x02 )
	{
		return 1;
	}
	
	return 0;
}

// Enable Keyboard
unsigned char activateKeyboard( void )
{
	int i;
	int j;
	
	// Send enable command(0xAE) to control/state register(port 0x64)
	outPortByte( 0x64, 0xAE );
	
	// Til emptying the input buffer(port 0x60),
	// then send enable command(0xF4) to input buffer(port 0x60)
	// 0xFFFF is enough time for emptying the input buffer.
	// Even though input buffer is not empty after 0xFFFF,
	// Ignore the remained data
	for ( i = 0 ;  i < 0xFFFF ; i++ )
	{
		if ( isInputBufferFull() == 0 )
		{
			break;
		}
	}
	// Send enable command(0xF4) to control/state register(port 0x64)
	outPortByte( 0x60, 0xF4 );
	
	// Wait for ACK
	// Check ACK in received 100 data,
	// Because ACK can be aleady arrived at output buffer
	for ( j = 0 ; j < 100 ; j++ )
	{
		// 0xFFFF is enough time for emptying the input buffer.
		// Even though input buffer is not empty after 0xFFFF,
		// Ignore the remained data
		for ( i = 0 ; i < 0xFFFF ; i++ )
		{
			// If the output buffer is full, we can read data
			if ( isOutputBufferFull() == 1 )
			{
				break;
			}
		}
		
		// If the read data is ACK, return 1
		if ( inPortByte( 0x60 ) == 0xFA )
		{
			return 1;
		}
	}
	
	return 0;
}

// Read data from output buffer
unsigned char getKeyboardScanCode( void )
{
	// Wait til there is data in output buffer(port 0x60)
	while ( isOutputBufferFull() == 0 )
	{
		;
	}
	
	return inPortByte( 0x60 );
}

// Keyboard LED On/Off
unsigned char changeKeyboardLED(unsigned char capsLockOn, unsigned char numLockOn, unsigned char scrollLockOn)
{
	int i, j;
	
	// Before sending LED change command,
	// Check output buffer is empty
	for ( i = 0 ; i < 0xFFFF ; i++ )
	{
		// If input buffer is empty, available to send command
		if ( isInputBufferFull() == 0 )
		{
			break;
		}
	}
	
	// Send LED change command(0xED) to output buffer(port 0x60) 
	outPortByte( 0x60, 0xED );
	for ( i = 0 ; i < 0xFFFF ; i++ )
	{
		// If input buffer is empty,
		// sended command is taken by keyboard
		if ( isInputBufferFull() == 0 )
		{
			break;
		}
	}
	
	// Wait for ACK
	for ( j = 0 ; j < 100 ; j++ )
	{
		// 0xFFFF is enough time for emptying the input buffer.
		// Even though input buffer is not empty after 0xFFFF,
		// Ignore the remained data
		for ( i = 0 ; i < 0xFFFF ; i++ )
		{
			// If the output buffer is full, we can read data
			if ( isOutputBufferFull() == 1 )
			{
				break;
			}
		}
		
		// If the read data is ACK, return 1
		if ( inPortByte( 0x60 ) == 0xFA )
		{
			return 1;
		}
	}
	if ( i >= 100 )
	{
		return 0;
	}
	
	// Send changing value to keyboard and wait
	outPortByte( 0x60, ( capsLockOn << 2 ) | ( numLockOn << 1 ) | scrollLockOn );
		outPortByte( 0x60, 0xED );
	for ( i = 0 ; i < 0xFFFF ; i++ )
	{
		// If input buffer is empty,
		// sended command is taken by keyboard
		if ( isInputBufferFull() == 0 )
		{
			break;
		}
	}
	
	// Wait for ACK
	for ( j = 0 ; j < 100 ; j++ )
	{
		// 0xFFFF is enough time for emptying the input buffer.
		// Even though input buffer is not empty after 0xFFFF,
		// Ignore the remained data
		for ( i = 0 ; i < 0xFFFF ; i++ )
		{
			// If the output buffer is full, we can read data
			if ( isOutputBufferFull() == 1 )
			{
				break;
			}
		}
		
		// If the read data is ACK, return 1
		if ( inPortByte( 0x60 ) == 0xFA )
		{
			return 1;
		}
	}
	if ( i >= 100 )
	{
		return 0;
	}
	
	return 1;
}

// Enable A20 gate
void enableA20Gate( void )
{
	unsigned char outputPortData;
	int i;
	
	// To read data of control/state register's output port
	// Send read command(0xD0) to control/state register
	outPortByte( 0x64, 0xD0 );
	
	// Read output port data
	for ( i = 0 ; i < 0xFFFF ; i++ )
	{
		// If output buffer is full, we can read data
		if ( isOutputBufferFull() == 1 )
		{
			break;
		}
	}
	// Read output buffer data
	outputPortData = inPortByte( 0x60 );
	
	// Set A20 gate bit
	outputPortData |= 0x01;
	
	// If input buffer(port 0x60) is empty,
	// Enable A20 gate
	for ( i = 0 ; i < 0xFFFF ; i++ )
	{
		// If input buffer is full, we can send data
		if ( isInputBufferFull() == 0 )
		{
			break;
		}
	}
	
	// Send setting command(0xD1) to control/state register
	outPortByte( 0x64, 0xD1 );
	
	// Send A20 enalbe value to input buffer
	outPortByte( 0x60, outputPortData );
}

// Reset processor
void reboot( void )
{
	int i;
	
	// If input buffer(port 0x60) is empty,
	// Enable A20 gate
	for ( i = 0 ; i < 0xFFFF ; i++ )
	{
		// If input buffer is full, we can send data
		if ( isInputBufferFull() == 0 )
		{
			break;
		}
	}
	
	// Send setting command(0xD1) to control/state register
	outPortByte( 0x64, 0xD1 );
	
	// Send 0 to input buffer to reset processor
	outPortByte( 0x60, 0x00 );
	
	while ( 1 ) 
	{
		;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Functions to transform scan code to ASCII code
///////////////////////////////////////////////////////////////////////////////
// Keyboard manager
static keyboardManager gs_stKeyboardManager = { 0, } ;

// Table for transforming scan code to ASCII code
static keyMappingEntry gs_vstKeyMappingTable[KEY_MAPPINGTABLEMAXCOUNT] = 
{
	/* 0  */ { KEY_NONE		, KEY_NONE		},
	/* 1  */ { KEY_ESC		, KEY_ESC		},
	/* 2  */ { '1'			, '!'			},
	/* 3  */ { '2'			, '@'			},
	/* 4  */ { '3'			, '#'			},
	/* 5  */ { '4'			, '$'			},
	/* 6  */ { '5'			, '%'			},
	/* 7  */ { '6'			, '^'			},
	/* 8  */ { '7'			, '&'			},
	/* 9  */ { '8'			, '*'			},
	/* 10 */ { '9'			, '('			},
	/* 11 */ { '0'			, ')'			},
	/* 12 */ { '-'			, '_'			},
	/* 13 */ { '='			, '+'			},
	/* 14 */ { KEY_BACKSPACE	, KEY_BACKSPACE		},
	/* 15 */ { KEY_TAB		, KEY_TAB		},
	/* 16 */ { 'q'			, 'Q'			},
	/* 17 */ { 'w'			, 'W'			},
	/* 18 */ { 'e'			, 'E'			},
	/* 19 */ { 'r'			, 'R'			},
	/* 20 */ { 't'			, 'T'			},
	/* 21 */ { 'y'			, 'Y'			},
	/* 22 */ { 'u'			, 'U'			},
	/* 23 */ { 'i'			, 'I'			},
	/* 24 */ { 'o'			, 'O'			},
	/* 25 */ { 'p'			, 'P'			},
	/* 26 */ { '['			, '{'			},
	/* 27 */ { ']'			, '}'			},
	/* 28 */ { '\n'			, '\n'			},
	/* 29 */ { KEY_CTRL		, KEY_CTRL		},
	/* 30 */ { 'a'			, 'A'			},
	/* 31 */ { 's'			, 'S'			},
	/* 32 */ { 'd'			, 'D'			},
	/* 33 */ { 'f'			, 'F'			},
	/* 34 */ { 'g'			, 'G'			},
	/* 35 */ { 'h'			, 'H'			},
	/* 36 */ { 'j'			, 'J'			},
	/* 37 */ { 'k'			, 'K'			},
	/* 38 */ { 'l'			, 'L'			},
	/* 39 */ { ';'			, ':'			},
	/* 40 */ { '\''			, '\"'			},
	/* 41 */ { '`'			, '~'			},
	/* 42 */ { KEY_LSHIFT		, KEY_LSHIFT		},
	/* 43 */ { '\\'			, '|'			},
	/* 44 */ { 'z'			, 'Z'			},
	/* 45 */ { 'x'			, 'X'			},
	/* 46 */ { 'c'			, 'C'			},
	/* 47 */ { 'v'			, 'V'			},
	/* 48 */ { 'b'			, 'B'			},
	/* 49 */ { 'n'			, 'N'			},
	/* 50 */ { 'm'			, 'M'			},
	/* 51 */ { ','			, '<'			},
	/* 52 */ { '.'			, '>'			},
	/* 53 */ { '/'			, '?'			},
	/* 54 */ { KEY_RSHIFT		, KEY_RSHIFT		},
	/* 55 */ { '*'			, '*'			},
	/* 56 */ { KEY_LALT		, KEY_LALT		},
	/* 57 */ { ' '			, ' '			},
	/* 58 */ { KEY_CAPSLOCK		, KEY_CAPSLOCK		},
	/* 59 */ { KEY_F1		, KEY_F1		},
	/* 60 */ { KEY_F2		, KEY_F2		},
	/* 61 */ { KEY_F3		, KEY_F3		},
	/* 62 */ { KEY_F4		, KEY_F4		},
	/* 63 */ { KEY_F5		, KEY_F5		},
	/* 64 */ { KEY_F6		, KEY_F6		},
	/* 65 */ { KEY_F7		, KEY_F7		},
	/* 66 */ { KEY_F8		, KEY_F8		},
	/* 67 */ { KEY_F9		, KEY_F9		},
	/* 68 */ { KEY_F10		, KEY_F10		},
	/* 69 */ { KEY_NUMLOCK		, KEY_NUMLOCK		},
	/* 70 */ { KEY_SCROLLLOCK	, KEY_SCROLLLOCK	},
	
	/* 71 */ { KEY_HOME		, '7'			},
	/* 72 */ { KEY_UP		, '8'			},
	/* 73 */ { KEY_PAGEUP		, '9'			},
	/* 74 */ { '-'			, '-'			},
	/* 75 */ { KEY_LEFT		, '4'			},
	/* 76 */ { KEY_CENTER		, '5'			},
	/* 77 */ { KEY_RIGHT		, '6'			},
	/* 78 */ { '+'			, '+'			},
	/* 79 */ { KEY_END		, '1'			},
	/* 80 */ { KEY_DOWN		, '2'			},
	/* 81 */ { KEY_PAGEDOWN		, '3'			},
	/* 82 */ { KEY_INS		, '0'			},
	/* 83 */ { KEY_DEL		, '.'		},
	/* 84 */ { KEY_NONE		, KEY_NONE		},
	/* 85 */ { KEY_NONE		, KEY_NONE		},
	/* 86 */ { KEY_NONE		, KEY_NONE		},
	/* 87 */ { KEY_F11		, KEY_F11		},
	/* 88 */ { KEY_F12		, KEY_F12		}
};

// Check scan code is in alphabet range
unsigned char isAlphabetScanCode( unsigned char scanCode )
{
	if ( ( 'a' <= gs_vstKeyMappingTable[scanCode].normalCode ) &&
		( gs_vstKeyMappingTable[scanCode].normalCode <= 'z' ) )
	{
		return 1;
	}
	
	return 0;
}

// Check scan code is in number or symbol range
unsigned char isNumberOrSymbolScanCode( unsigned char scanCode )
{
	// In 2 ~ 53, except for alphabet, the key is number or symbol
	if ( ( 2 <= scanCode ) && ( scanCode <= 53 ) &&
		(isAlphabetScanCode( scanCode ) == 0 ) )
	{
		return 1;
	}
	
	return 0;
}

// Check scan code is number pad
unsigned char isNumberPadScanCode( unsigned char scanCode )
{
	// number pad is in 71 ~ 83
	if ( ( 71 < scanCode ) && ( scanCode <= 83 ) )
	{
		return 1;
	}
	
	return 0;
}

// Check scan code is combined 
unsigned char isUseCombinedCode( unsigned char scanCode )
{
	unsigned char downScanCode;
	unsigned char useCombinedKey = 0;
	
	downScanCode = scanCode & 0x7F;
	
	// If it is alphabet key,
	// Shift and Capslock key can affect
	if ( isNumberOrSymbolScanCode( downScanCode ) == 1 )
	{
		// One of both work
		if ( gs_stKeyboardManager.shiftDown ^ gs_stKeyboardManager.scrollLockOn )
		{
			useCombinedKey = 1;
		}
		else
		{
			useCombinedKey = 0;
		}
	}
	// If it is number or symbol key,
	// Shift key can affect
	else if ( isNumberOrSymbolScanCode( downScanCode ) == 1 )
	{
		// Check shift key 
		if ( gs_stKeyboardManager.shiftDown == 1 )
		{
			useCombinedKey = 1;
		}
		else
		{
			useCombinedKey = 0;
		}
	}
	// If it is number pas key,
	// Num Lock key can affect
	// If extended key code is not set,
	// combinedkey are the same
	else if ( ( isNumberPadScanCode( downScanCode ) == 1 ) &&
		( gs_stKeyboardManager.extendedCodeIn == 0 ) )
	{
		// Check Num Lock
		if ( gs_stKeyboardManager.numLockOn == 1 )
		{
			useCombinedKey = 1;
		}
		else
		{
			useCombinedKey = 0;
		}
	}
	
	return useCombinedKey;
}

// Update combination key and Sync LED 
void updateCombinationKeyStatusAndLED( unsigned char scanCode )
{
	unsigned char down;
	unsigned char downScanCode;
	unsigned char ledStatusChanged = 0;
	
	// Check any key is down
	// The highest bit it 1, no key is down
	if ( scanCode & 0x80 )
	{
		down = 0;
		downScanCode = scanCode & 0x70;
	}
	else
	{
		down = 1;
		downScanCode = scanCode;
	}
	
	// Search combination key 
	// Shift
	if ( ( downScanCode == 42 ) || ( downScanCode == 54 ) )
	{
		gs_stKeyboardManager.shiftDown = down;
	}
	// Caps Lock
	else if ( ( downScanCode == 58 ) || ( down == 1 ) )
	{
		gs_stKeyboardManager.capsLockOn ^= 1;
		// Update LED 
		ledStatusChanged = 1;
	}
	// Num Lock
	else if ( ( downScanCode == 69 ) || ( down = 1 ) )
	{
		gs_stKeyboardManager.numLockOn ^= 1;
		// Update LED 
		ledStatusChanged = 1;
	}
	// Scroll Lock
	else if ( ( downScanCode == 70 ) || ( down = 1 ) )
	{
		gs_stKeyboardManager.scrollLockOn ^= 1;
		// Update LED 
		ledStatusChanged = 1;
	}
	
	// Change LED State
	if ( ledStatusChanged == 1 )
	{
		changeKeyboardLED(gs_stKeyboardManager.capsLockOn, 
			gs_stKeyboardManager.numLockOn, gs_stKeyboardManager.scrollLockOn );
	}
}

// Transform scan code to ASCII cdoe
unsigned char convertScanCodeToASCIICode( unsigned char scanCode, unsigned char * asciiCode, unsigned char * flags)
{
	unsigned char useCombinedKey;
	
	// If previous key was PAUSE, 
	// Ignore remain scan code of PAUSE
	if ( gs_stKeyboardManager.skipCountForPause > 0 )
	{
		gs_stKeyboardManager.skipCountForPause--;
		return 0;
	}
	
	// Special handler for PAUSE
	if ( scanCode = 0xE1 )
	{
		*asciiCode = KEY_PAUSE;
		*flags = KEY_FLAGS_DOWN;
		gs_stKeyboardManager.skipCountForPause = KEY_SKIPCOUNTFORPAUSE;
		return 1;
	}
	// For extended key, set flag 
	else if ( scanCode == 0xE0 )
	{
		gs_stKeyboardManager.extendedCodeIn = 1;
		return 0;
	}
	
	// Return combination key?
	useCombinedKey = isUseCombinedCode( scanCode );
	
	// Set key value
	if ( useCombinedKey == 1 ) 
	{
		*asciiCode = gs_vstKeyMappingTable[scanCode & 0x70].combinedCode;
	}
	else
	{
		*asciiCode = gs_vstKeyMappingTable[scanCode & 0x7F].normalCode;
	}

	// Set extended key 
	if ( gs_stKeyboardManager.extendedCodeIn == 1 )
	{
		*flags = KEY_FLAGS_EXTENDEDKEY;
		gs_stKeyboardManager.extendedCodeIn = 0;
	}
	else
	{
		*flags = 0;
	}
	
	// Down or Up
	if ( ( scanCode & 0x80 ) == 0 )
	{
		*flags |= KEY_FLAGS_DOWN;
	}
	
	// Update combination key down or up
	updateCombinationKeyStatusAndLED( scanCode );
	return 1;
}
