#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <errno.h>
#include <inttypes.h>
#include <pthread.h>
#include <alsa/asoundlib.h>
#include <math.h>

#define ARG "hDp:d:C:LRGS"
#define	BUFFSIZE 1470

#define INIT_REQUEST 0x01
//#define INIT_RESPONCE 0x02
#define OFFSET_RESPONCE 0x03
#define OFFSET_REQUEST 0x04
#define RTT_RESPONCE 0x05
#define RTT_REQUEST 0x06
#define START_SYNC 0xff
#define CLOCK_SYNC_REQUEST 0x10
#define CLOCK_SYNC_RESPONCE 0x11
#define LOST_PACKETS 0x12
#define BROADCAST 0xff

#define PLAY 0xa0
#define STOP 0xa1
#define PAUSE 0xa2
#define VOLUME 0xa4
#define SET 0xa5
#define DATA 0xa9
#define PREPARE 0xa3
#define SKEW 0xa8

#define FREQUENCY 200
#define SAMPLEFREQ 44100
#define CHANNNELS 1
#define BUFFERSIZE SAMPLEFREQ/FREQUENCY

#define YES 1
#define NO 0

struct args{
	uint16_t	port;
	char		ip[15];
	uint8_t		mode;
	uint8_t		channel;
	uint8_t		volume;
}args;

struct Params{
	struct timeval	time;
}params;

struct status{
		uint8_t	playing:1;
		uint8_t	pausing:1;
		uint8_t	stoped:1;
		uint8_t	ready:1;
		uint8_t	prepared:4;
}status;

struct audio_info{
		uint32_t	size;
		uint32_t	rate;
}audio_info;

int32_t				sock;
struct sockaddr_in	serv, source;
int					servLen, sourceLen;
unsigned char		rxbuff[BUFFSIZE],txbuff[BUFFSIZE], rttTxBuff[5], dataConfirm[5], lostPackets[1026];
unsigned char		heartBeatBuff[2];
int8_t				isExit;
int16_t				bytes_r;
pthread_t			thrd_play;
char				*sync_res;
unsigned char		*audio_buff;
unsigned char		*packets_map;	
uint32_t			poss;
unsigned char		playingStatus;
float				timeSkew, sampleCount, samplePeriod;
int8_t				skewDir;
snd_pcm_t			*PlaybackHandle;
uint32_t			current;

void readAttr(int _argc, char ** _argv);
void start(void);
void createClientUdpSocket(int32_t *__sock);
void *thrd_play_func();
void timer(struct timeval *__time);
void openGPIO(char *dir);                                                                                     
//---------------------------------------------------------------------------------------------------------

snd_pcm_uframes_t	WaveSize;

struct wavHead_t{
	int32_t		chunkId;// RIFF: 0x52494646 in big-endian 
	int32_t		chunkSize;
	int32_t		format;//WAVE: 0x57415645 in big-endian
	int32_t		subchunk1Id;//fmt: 0x666d7420 in big-endian
	int32_t		subchunk1Size;
	uint16_t	audioFormat;// if is not 1, than have some compression
	uint16_t	numChannels;// number of Channels
	uint32_t	sampleRate;// Sampling frequency
	uint32_t	byteRate;//A quantity of bytes per second
	uint16_t	blockAlign;// The quantity of byte per sample, including all channels
	uint16_t	bitsPerSample; // The quantity of bits per sample 
	int32_t		subchunk2Id;//data:0x64617461 inв big-endian
	int32_t		subchunk2Size;//Data size
};

struct stereoSample_t{
	char	channelLeft[2];
	char	channelRight[2];
};

struct monoSamples_t{
	char	channel[2];
};

struct wavHead_t	wavHead;

int8_t readWavHead(int32_t __fd,struct wavHead_t *__wavHead);
void getLeftChannel(struct stereoSample_t *__samples, struct monoSamples_t *__mono, int32_t *__bytes);
void setAlsaVolume(float volume);

