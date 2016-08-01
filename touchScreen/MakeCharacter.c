#include "MakeCharacter.h"

// Define Character Code
#define CODE_SPACE	0x0020
#define CODE_KOR_GA 0xAC00
#define CODE_KOR_G	0x3131
#define CODE_KOR_A	0x314F
#define CODE_NUM_0	0x0030
#define CODE_ENG_A	0x0041
#define CODE_ENG_a	0x0061

#define _nil 0xFFFF

//-------------------------------------------------------------------------------- KOR
#define ConsonantCnt 30
#define getKor3Sung(ch,s,m,e) {s = (ch - CODE_KOR_GA) / 588;m = ((ch - CODE_KOR_GA) % 588) / 28;e = ((ch - CODE_KOR_GA) % 588) % 28;}

int chConsonant_StoF(int s){ // s -> f
	int consonant[19] = { 0, 1, 3, 6, 7, 8, 16, 17, 18, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 };
	return consonant[s];
}
int chConsonant_EtoF(int e){ // e -> f
	int consonant[27] = { 0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29 };
	return consonant[e-1];
}
int chConsonant_FtoS(int f){ // f -> s
	int consonant[] = { 0, 1, _nil, 2, _nil, _nil, 3, 4, 5, _nil, _nil, _nil, _nil, _nil, _nil, _nil, 6, 7, 8, _nil, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 };
	return consonant[f];
}
int chConsonant_FtoE(int f){ // f -> e
	int consonant[] = { 1, 2, 3, 4, 5, 6, 7, _nil, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, _nil, 18, 19, 20, 21, 22, _nil, 23, 24, 25, 26, 27 };
	return consonant[f];
}
int chConsonant_EtoEF(int e){
	int consonant[27] = { 0, 1, 
		/**/ (1 * ConsonantCnt) + 20, 3, 
		/**/ (4 * ConsonantCnt) + 23,
		/**/ (4 * ConsonantCnt) + 29, 6, 8, 
		/**/ (8 * ConsonantCnt) + 0, 
		/**/ (8 * ConsonantCnt) + 16, 
		/**/ (8 * ConsonantCnt) + 17, 
		/**/ (8 * ConsonantCnt) + 20, 
		/**/ (8 * ConsonantCnt) + 27, 
		/**/ (8 * ConsonantCnt) + 28, 
		/**/ (8 * ConsonantCnt) + 29, 16, 17, 
		/**/ (17 * ConsonantCnt) + 20, 20, 21, 22, 23, 25, 26, 27, 28, 29 };
	return consonant[e - 1];
}
int chConsonant_F_KEYtoF(int f, unsigned int key){
	int i, n;
	int datamap[82][2] =
	{
		  // F,cnt  / ( key , f)
	/*  */{ -1, 6 }, { KEY_G, 0 }, { KEY_N, 3 }, { KEY_R, 8 }, { KEY_M, 16 }, { KEY_S, 20 }, { KEY_O, 22 },
	/**/{  0, 3 }, { KEY_S, 2 }, { KEY_ADD, 26 }, { KEY_DOUBLE, 1 },
	/**/{  1, 1 }, { KEY_DOUBLE, 0 },
	/**/{  2, 2 }, { KEY_ADD, (23 * ConsonantCnt) + 0 }, { KEY_DOUBLE, (21 * ConsonantCnt) + 0 },
	/**/{  3, 1 }, { KEY_ADD, 6 },
	/**/{  4, 2 }, { KEY_ADD, (25 * ConsonantCnt) + 3 }, { KEY_DOUBLE, (24 * ConsonantCnt) + 3 },
	/**/{  5, 1 }, { KEY_ADD, (22 * ConsonantCnt) + 3 },
	/**/{  6, 2 }, { KEY_ADD, 27 }, { KEY_DOUBLE, 7 },
	/**/{  7, 1 }, { KEY_DOUBLE, 6 },
	/**/{  8, 3 }, { KEY_G, 9 }, { KEY_M, 10 }, { KEY_S, 12 },
	/**/{  9, 2 }, { KEY_ADD, (26 * ConsonantCnt) + 8 }, { KEY_DOUBLE, (1 * ConsonantCnt) + 8 },
	/**/{ 10, 1 }, { KEY_ADD, 11 },
	/**/{ 11, 2 }, { KEY_ADD, 14 }, { KEY_DOUBLE, (18 * ConsonantCnt) + 8 },
	/**/{ 12, 2 }, { KEY_ADD, (23 * ConsonantCnt) + 8 }, { KEY_DOUBLE, (21 * ConsonantCnt) + 8 },	
	/**/{ 13, 1 }, { KEY_ADD, (3 * ConsonantCnt) + 8 },
	/**/{ 14, 1 }, { KEY_ADD, 10 },
	/**/{ 15, 1 }, { KEY_ADD, (22 * ConsonantCnt) + 8 },
	/**/{ 16, 1 }, { KEY_ADD, 17 },
	/**/{ 17, 3 }, { KEY_S, 19 }, { KEY_ADD, 28 }, { KEY_DOUBLE, 18 },
	/**/{ 18, 1 }, { KEY_DOUBLE, 17 },
	/**/{ 19, 2 }, { KEY_ADD, (23 * ConsonantCnt) + 17 }, { KEY_DOUBLE, (21 * ConsonantCnt) + 17 },
	/**/{ 20, 2 }, { KEY_ADD, 23 }, { KEY_DOUBLE, 21 },
	/**/{ 21, 1 }, { KEY_DOUBLE, 20 },
	/**/{ 22, 1 }, { KEY_ADD, 29 },
	/**/{ 23, 2 }, { KEY_ADD, 25 }, { KEY_DOUBLE, 24 },
	/**/{ 24, 1 }, { KEY_DOUBLE, 23 },
	/**/{ 25, 1 }, { KEY_ADD, 20 },
	/**/{ 26, 1 }, { KEY_ADD, 0 },
	/**/{ 27, 1 }, { KEY_ADD, 3 },
	/**/{ 28, 1 }, { KEY_ADD, 16 },
	/**/{ 29, 1 }, { KEY_ADD, 22 },
	};

	i = 0;
	while (i < 82){
		if (f == datamap[i][0]){
			n = datamap[i][1] + i + 1;
			i++;
			while (i < n){
				if (key == datamap[i][0]){
					return datamap[i][1];
				}
				i++;
			}
			return _nil;
		}
		else{
			i += datamap[i][1] + 1;
		}
	}
	return _nil;
}
int chVowel(int v, unsigned int key){
	int i, n;
	int datamap[53][2] =
	{
		// V,cnt  / ( key , v)
	/**/{ -1, 4 }, { KEY_A, 0 }, { KEY_U, 8 }, { KEY_I, 20 }, { KEY__, 18 },
	/**/{ 0, 3 }, { KEY_A, 4 }, { KEY_I, 1 }, { KEY_ADD, 2 },
	/**/{ 1, 1 }, { KEY_ADD, 3 },
	/**/{ 2, 2 }, { KEY_I, 3 }, { KEY_ADD, 0 },
	/**/{ 3, 1 }, { KEY_ADD, 1},
	/**/{ 4, 3 }, { KEY_A, 0 }, { KEY_I, 5 }, { KEY_ADD, 6 },
	/**/{ 5, 1 }, { KEY_ADD, 7 },
	/**/{ 6, 2 }, { KEY_I, 7 }, { KEY_ADD, 4 },
	/**/{ 7, 1 }, { KEY_ADD, 5 },
	/**/{ 8, 4 }, { KEY_A, 9 }, { KEY_U, 13 }, { KEY_I, 11 }, { KEY_ADD, 12 },
	/**/{ 9, 1 }, { KEY_I, 10 },
	/**/{ 12, 1 }, { KEY_ADD, 8 },
	/**/{ 13, 4 }, { KEY_A, 14 }, { KEY_U, 8 }, { KEY_I, 14}, { KEY_ADD, 17 },
	/**/{ 14, 1 }, { KEY_I, 15 },
	/**/{ 17, 1 }, { KEY_ADD, 13 },
	/**/{ 18, 1 }, { KEY_I, 19 },
	};

	i = 0;
	while (i < 53){
		if (v == datamap[i][0]){
			n = datamap[i][1] + i + 1;
			i++;
			while (i < n){
				if (key == datamap[i][0]){
					return datamap[i][1];
				}
				i++;
			}
			return _nil;
		}
		else{
			i += datamap[i][1] + 1;
		}
	}
	return _nil;
}
void getKor4Sung(unsigned short ch, int *s, int *m, int *e, int *f){
	*s = *m = *e = *f = -1;
	if (ch >= CODE_KOR_GA){
		getKor3Sung(ch, (*s), (*m), (*e));
	}
	else if (ch >= CODE_KOR_G){
		if (ch >= CODE_KOR_A)
			*m = ch - CODE_KOR_A;
		else
			*f = ch - CODE_KOR_G;
	}
}

unsigned short makeKorCode(int s,int m,int e){
	if ((s >= 0) && (m < 0)){
		return CODE_KOR_G + chConsonant_StoF(s);
	}
	else if ((s < 0) && (m >= 0)){
		return CODE_KOR_A + m;
	}
	if ((s >= 0) && (m >= 0) && (e < 0)){
		e = 0;
	}
	return  CODE_KOR_GA + (588 * s) + (m * 28) + e;
}

unsigned int onConsonant(MakeBoard *mb, unsigned int key){
	int s, m, e, f;
	int temp;
	getKor4Sung(mb->Char, &s, &m, &e, &f);

	if ((s < 0) && (m < 0) && (e < 0) && (f < 0)){
		temp = chConsonant_F_KEYtoF(-1,key);
		if (temp == _nil) return RT_NULL;
		mb->newChar = makeKorCode(chConsonant_FtoS(temp),-1,-1);
		return RT_NEW;
	}

	if ((s < 0) && (m < 0) && (e < 0) && (f >= 0)){	//
		if (key == KEY_ADD || key == KEY_DOUBLE){
			temp = chConsonant_F_KEYtoF(f, key);
			if (temp == _nil) return RT_NULL;
			

			int ps, pm, pe, pf;
			getKor4Sung(mb->prevChar, &ps, &pm, &pe, &pf);
			if (pe == 4){ //
				if (temp == 23){ //
					mb->prevChar = makeKorCode(ps, pm, 5);
					mb->Char = 0x00;
					return RT_MIX_P | RT_DEL;
				}
				else if (temp == 29){ // 
					mb->prevChar = makeKorCode(ps, pm, 6);
					mb->Char = 0x00;
					return RT_MIX_P | RT_DEL;
				}
			}
			else if (pe == 8){ // 
				if (temp == 27){ //
					mb->prevChar = makeKorCode(ps, pm, 13);
					mb->Char = 0x00;
					return RT_MIX_P | RT_DEL;
				}
				else if (temp == 29){ //
					mb->prevChar = makeKorCode(ps, pm, 15);
					mb->Char = 0x00;
					return RT_MIX_P | RT_DEL;
				}
			}

			mb->Char = makeKorCode(chConsonant_FtoS(temp), -1, -1);
			return RT_MIX_C;
		}
	}

	if ((s >= 0) && (m >= 0) && (e == 0)){
		temp = chConsonant_F_KEYtoF(-1, key);
		if (temp == _nil) return RT_NULL;
		e = chConsonant_FtoE(temp);
		mb->Char = makeKorCode(s, m, e);
		return RT_MIX_C;
	}

	if ((s >= 0) && (m >= 0) && (e > 0)){
		if (key == KEY_ADD || key == KEY_DOUBLE){
			temp = chConsonant_F_KEYtoF(chConsonant_EtoF(e), key);
			if (temp == _nil) return RT_NULL;

			int ce = chConsonant_FtoE(temp % ConsonantCnt);
			int ne = chConsonant_FtoS(temp / ConsonantCnt);

			if (ce == _nil){
				ce = 0;
				ne = chConsonant_FtoS(temp % ConsonantCnt);
			}

			mb->Char = makeKorCode(s, m, ce);

			if (ne > 0){
				mb->newChar = makeKorCode(ne, -1, -1);
				return RT_MIX_C | RT_NEW;
			}
			return RT_MIX_C;
		}
		else{
			temp = chConsonant_F_KEYtoF(chConsonant_EtoF(e), key);
			if (temp == _nil){
				temp = chConsonant_F_KEYtoF(-1, key);
				if (temp == _nil) return RT_NULL;
				mb->newChar = makeKorCode(chConsonant_FtoS(temp), -1, -1);
				return RT_NEW;
			}
			mb->Char = makeKorCode(s, m, chConsonant_FtoE(temp));
			return RT_MIX_C;
		}
	}

	temp = chConsonant_F_KEYtoF(-1, key);
	if (temp == _nil) return RT_NULL;
	mb->newChar = makeKorCode(chConsonant_FtoS(temp), -1, -1);
	return RT_ENDEDIT_C | RT_NEW;
}
unsigned int onVowel(MakeBoard *mb, unsigned int key){
	int s, m, e, f;
	int temp;
	getKor4Sung(mb->Char, &s, &m, &e, &f);

	if ((s < 0) && (m < 0) && (e < 0) && (f < 0)){	// 
		temp = chVowel(-1, key);
		if (temp == _nil) return RT_NULL;
		mb->newChar = makeKorCode(-1, temp, -1);
		return RT_NEW;
	}

	if ((s < 0) && (m < 0) && (e < 0) && (f >= 0)){	//
		temp = chVowel(-1, key);
		if (temp == _nil) return RT_NULL;
		mb->Char = makeKorCode(chConsonant_FtoS(f), temp, -1);
		return RT_MIX_C | RT_ENDEDIT_P;
	}

	if ( (m >= 0) && (e <= 0) && (f < 0)){	//
		temp = chVowel(m, key);
		if (temp == _nil){
			temp = chVowel(-1, key);
			if (temp == _nil) return RT_NULL;
			mb->newChar = makeKorCode(-1, temp, -1);
			return RT_NEW | RT_ENDEDIT_A;
		}
		else{
			mb->Char = makeKorCode(s, temp, e);
			return RT_MIX_C | RT_ENDEDIT_P;
		}
	}

	if ((s >= 0) && (m >= 0) && (e > 0)){			//
		temp = chConsonant_EtoEF(e);
		int nf = temp % ConsonantCnt;
		int ce = temp / ConsonantCnt;

		mb->Char = makeKorCode(s, m, ce);
		mb->newChar = makeKorCode(chConsonant_FtoS(nf), chVowel(-1, key), 0);
		return RT_MIX_C | RT_NEW | RT_ENDEDIT_A;
	}

	return RT_NULL;
}

unsigned int makeKorChar(MakeBoard *mb, unsigned int key){
	// Consonant & Vowel & Add,double
	switch (key){
	case KEY_G: case KEY_N:	case KEY_R:	case KEY_M:	case KEY_S:	case KEY_O:
		return onConsonant(mb, key);
	case KEY_A:	case KEY_U:	case KEY_I:	case KEY__:
		return onVowel(mb, key);
	case KEY_ADD: case KEY_DOUBLE:
		if (mb->Char >= CODE_KOR_GA){
			int s, m, e;
			getKor3Sung(mb->Char, s, m, e);
			if (s >= 0 && m >= 0 && e > 0)
				return onConsonant(mb, key);
			else
				return onVowel(mb, key);
		}
		else if (mb->Char >= CODE_KOR_G){
			if (mb->Char >= CODE_KOR_A)
				return onVowel(mb, key);
			else
				return onConsonant(mb, key);
		}
		else{
			return RT_NULL;
		}
		return onConsonant(mb, key);
	}

	// Delete & Space Bar
	if (key == KEY_SPACE){
		mb->newChar = CODE_SPACE;
		return RT_NEW_ENDEDTER | RT_ENDEDIT_A;
	}
	else if (key == KEY_DEL){
		if (mb->Char >= CODE_KOR_GA){
			int s, m, e;
			getKor3Sung(mb->Char, s, m, e);

			if (e > 0){
				int temp = chConsonant_EtoEF(e);
				int ce = temp / ConsonantCnt;
				mb->Char = makeKorCode(s, m, ce);
				return RT_MIX_C | RT_ENDEDIT_P;
			}
			else if (m >= 0){
				mb->Char = makeKorCode(s, -1, 0);
				return RT_MIX_C | RT_ENDEDIT_P;
			}
		}
		else{
			return RT_DEL | RT_ENDEDIT_A;
		}
	}
	return RT_NULL;
}
//====================================================================================
unsigned int makeNumChar(MakeBoard *mb, unsigned int key){
	switch (key){
	case KEY_SPACE: mb->newChar = CODE_SPACE; return RT_NEW_ENDEDTER | RT_ENDEDIT_A;
	case KEY_DEL: return RT_DEL | RT_ENDEDIT_A;
	case KEY_1: mb->newChar = CODE_NUM_0 + 1; return RT_NEW_ENDEDTER | RT_ENDEDIT_A;
	case KEY_2: mb->newChar = CODE_NUM_0 + 2; return RT_NEW_ENDEDTER | RT_ENDEDIT_A;
	case KEY_3: mb->newChar = CODE_NUM_0 + 3; return RT_NEW_ENDEDTER | RT_ENDEDIT_A;
	case KEY_4: mb->newChar = CODE_NUM_0 + 4; return RT_NEW_ENDEDTER | RT_ENDEDIT_A;
	case KEY_5: mb->newChar = CODE_NUM_0 + 5; return RT_NEW_ENDEDTER | RT_ENDEDIT_A;
	case KEY_6: mb->newChar = CODE_NUM_0 + 6; return RT_NEW_ENDEDTER | RT_ENDEDIT_A;
	case KEY_7: mb->newChar = CODE_NUM_0 + 7; return RT_NEW_ENDEDTER | RT_ENDEDIT_A;
	case KEY_8: mb->newChar = CODE_NUM_0 + 8; return RT_NEW_ENDEDTER | RT_ENDEDIT_A;
	case KEY_9: mb->newChar = CODE_NUM_0 + 9; return RT_NEW_ENDEDTER | RT_ENDEDIT_A;
	case KEY_0: mb->newChar = CODE_NUM_0 + 0; return RT_NEW_ENDEDTER | RT_ENDEDIT_A;
	case KEY_star: mb->newChar = 0x002A; return RT_NEW_ENDEDTER | RT_ENDEDIT_A;
	case KEY_pppp: mb->newChar = 0x0023; return RT_NEW_ENDEDTER | RT_ENDEDIT_A;
	}
	return RT_NULL;
}
//-------------------------------------------------------------------------------- ENG

unsigned int onEng(MakeBoard *mb, unsigned short startCode, int rotation){
	if (startCode <= mb->Char && mb->Char < (startCode + rotation)){
		unsigned short temp = mb->Char - startCode;
		temp = (temp + 1) % rotation;
		mb->Char = startCode + temp;
		return RT_MIX_C;
	}
	else{
		mb->newChar = startCode;
		return RT_NEW | RT_ENDEDIT_P;
	}
	return RT_NULL;
}

unsigned int makeEngChar(MakeBoard *mb, unsigned int key, int isCaps){
	unsigned short code_a = CODE_ENG_a;

	// Delete & Space Bar
	if (key == KEY_SPACE){
		if (mb->Char > 0){
			return RT_ENDEDIT_A;
		}
		mb->newChar = CODE_SPACE;
		return RT_NEW_ENDEDTER | RT_ENDEDIT_A;
	}
	else if (key == KEY_DEL){
		return RT_DEL | RT_ENDEDIT_A;
	}
	else if (key == KEY_CAPS){
		if (isCaps)
			return RT_CHMOD | KEYPAD_ENG;
		else
			return RT_CHMOD | KEYPAD_ENG_CAPS;
	}

	if (isCaps)
		code_a = CODE_ENG_A;
	switch (key){
	case KEY_abc: return onEng(mb, code_a    , 3);
	case KEY_def: return onEng(mb, code_a + 3, 3);
	case KEY_ghi: return onEng(mb, code_a + 6, 3);
	case KEY_jkl: return onEng(mb, code_a + 9, 3);
	case KEY_mno: return onEng(mb, code_a + 12, 3);
	case KEY_pqrs: return onEng(mb, code_a + 15, 4);
	case KEY_tuv: return onEng(mb, code_a + 19, 3);
	case KEY_wxyz: return onEng(mb, code_a + 22, 4);
	case KEY_star: case KEY_pppp:
		return makeNumChar(mb, key);
	}

	return RT_NULL;
}
//====================================================================================

unsigned int makeCharacter(MakeBoard *mb, unsigned int key, int inputKeyType){
	// init
	mb->newChar = 0x00;

	if (key == KEY_CHMOD){
		// Change keypad [kor] <-> [eng]
		// [num] : long touch
		// [eng_caps] : event KEY_CAPS
		// ** temp **
		// [kor] -> [eng] -> [num] -> [kor]
		if ( inputKeyType == KEYPAD_KOR )
			return RT_CHMOD | KEYPAD_ENG;
		else if (inputKeyType == KEYPAD_ENG || inputKeyType == KEYPAD_ENG_CAPS)
			return RT_CHMOD | KEYPAD_NUM;
		else
			return RT_CHMOD | KEYPAD_KOR;
	}


	switch (inputKeyType)
	{
	case KEYPAD_KOR:		return makeKorChar(mb, key);
	case KEYPAD_ENG:		return makeEngChar(mb, key, 0);
	case KEYPAD_ENG_CAPS:	return makeEngChar(mb, key, 1);
	case KEYPAD_NUM:		return makeNumChar(mb, key);
	}


	return RT_NULL;
}
