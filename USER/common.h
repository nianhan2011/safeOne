#ifndef _COMMON_H
#define _COMMON_H
/*
	∞Ê±æøÿ÷∆
	verion: 
					2 4G-3630 ”Ô“Ù£∫JQ8400	
					3 4G-3630 ”Ô“Ù£∫JQ8400	¥¯¿∂—¿
					4 4G-3630 ”Ô“Ù£∫JQ8400	433
*/

#define version 4 
//#define NO_VOICE

#if version==-1
#define BLE
#define DEBUG
//#define JQ8400
#define ME3630


#elif version==0
#define JR6001
#define ME3616

#elif version==1
#define JR6001
#define ME3630


#elif version==2
#define JQ8400
#define ME3630
#define ONLY_REMOTE_CONTROL

#elif version==3
#define BLE
#define JQ8400
#define ME3630
#define ONLY_REMOTE_CONTROL

#elif version==4

#define JQ8400
#define ME3630
#define ONLY_REMOTE_CONTROL

#elif version==5
#define BLE
#define JQ8400
#define ME3630

#endif

#endif
