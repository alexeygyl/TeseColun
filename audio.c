#include "main.h"


void getLeftChannel(struct stereoSample_t *__samples, struct monoSamples_t *__mono, int32_t *__bytes){
	uint32_t	i;
	for(i = 0; i < *__bytes/wavHead.blockAlign; i++){
		memcpy(&__mono[i].channel,&__samples[i].channelLeft,2);
	}	
}
void getRightChannel(struct stereoSample_t *__samples, struct monoSamples_t *__mono, int32_t *__bytes){
	uint32_t	i;
	for(i = 0; i < *__bytes/wavHead.blockAlign; i++){
		memcpy(&__mono[i].channel,&__samples[i].channelRight,2);
	}	
}


int8_t readWavHead(int32_t __fd,struct wavHead_t *__wavHead){
	int8_t	bytes;
	bytes = read(__fd, __wavHead, 44);
	if(bytes !=44)return -1;
	return 1;
}

