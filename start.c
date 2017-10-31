#include "main.h"

void *action_thrd_func();


void start(void){
	struct timeval timepoint,timep2;
	uint32_t	seq, last = 0, i;
	int			err;
	uint8_t		count;
	sampleCount = samplePeriod = 0.0;
	skewDir = 1;
	heartBeatBuff[0] = INIT_REQUEST;
	args.channel = 1;
	heartBeatBuff[1] = args.channel;
	rttTxBuff[0] = RTT_RESPONCE;
	isExit = 0;
	status.ready = YES;
	args.volume = 60;
	createClientUdpSocket(&sock);
	poss = 0;	
	txbuff[0] = OFFSET_RESPONCE;
	while(isExit == 0){
			bytes_r = recvfrom(sock,rxbuff,BUFFSIZE,0,(struct sockaddr *)&source,&sourceLen);
			switch(rxbuff[0]){
				case BROADCAST:
					serv = source;
					servLen = sourceLen;
					sendto(sock, heartBeatBuff, 2, 0, (struct sockaddr*)&serv, servLen);
				break;
				case OFFSET_REQUEST:
					gettimeofday(&timepoint,NULL);
					memcpy(&txbuff[1],&rxbuff[1],4);
					memcpy(&txbuff[5],&timepoint,sizeof(struct timeval));
					sendto(sock, txbuff, sizeof(struct timeval) + 5, 0, (struct sockaddr*)&serv, servLen);
				break;
				case RTT_REQUEST:
					memcpy(&rttTxBuff[1],&rxbuff[1],4);
					sendto(sock, rttTxBuff, 5, 0, (struct sockaddr*)&serv, servLen);
				break;
				case VOLUME:
					args.volume = rxbuff[1];
					setAlsaVolume((float)args.volume);
					//printf("VOLUME to %d\n",args.volume);
				break;
				case PLAY:
						if(playingStatus == PLAY || status.prepared == NO){
							//printf("PLAY -- >> NO\n");
							continue;
						}
						playingStatus = PLAY;
						memcpy(&params.time,&rxbuff[1],8);
						//printf("PLAYING  -->> YES \n");
						pthread_create(&thrd_play,NULL,&thrd_play_func,&params);
				break;
				case STOP:
						if(status.playing == YES){
							//printf("STOPING-1\n");	
							memcpy(&params.time,&rxbuff[1],8);
							timer(&params.time);
							//snd_pcm_drop(PlaybackHandle);
							playingStatus = STOP;
							status.prepared = NO;
							pthread_join(thrd_play, NULL);
						}else {
							//printf("STOPING-2\n");	
							playingStatus = STOP;
							status.prepared = NO;
						}

				break;
				case PAUSE:
						memcpy(&params.time,&rxbuff[1],8);
						if(playingStatus == PLAY){
							//printf("PAUSE ON in %d.%d\n",params.time.tv_sec,params.time.tv_usec);
							timer(&params.time);
							playingStatus = PAUSE;
						}
						else if(playingStatus == PAUSE){
							//printf("PAUSE OFF in %d.%d\n",params.time.tv_sec,params.time.tv_usec);
							timer(&params.time);
							playingStatus = PLAY;	
						}
				break;

				case DATA:
						if(status.prepared == NO){
							//printf("-");
							continue;
						}
						if(audio_buff == NULL)continue;
						//printf("+");
						memcpy(&seq,&rxbuff[1],4);
						//printf("Seq %d\n",seq);
						if(packets_map[seq] == 1)continue;
						memcpy(&audio_buff[seq*1450],&rxbuff[5],bytes_r-5);
						packets_map[seq] = 1;
						
						break;
				case PREPARE:
						if(status.playing == YES){
							playingStatus = STOP;
							pthread_join(thrd_play, NULL);
							//continue;
							//printf("PREPARE -- >> NO READY\n");
						}
						if(status.prepared == NO){
								memcpy(&audio_info.rate,&rxbuff[1],4);
								memcpy(&audio_info.size,&rxbuff[5],4);
								//printf("PREPARE -- >> YES\n");
								audio_buff = (unsigned char *)malloc(audio_info.size);
								packets_map = (unsigned char *)malloc(audio_info.size/1450);
								memset(audio_buff,'\0',audio_info.size);
								memset(packets_map,'\0',audio_info.size/1450);
								//printf("Rate %d\nsize %d\n",audio_info.rate,audio_info.size);
								dataConfirm[0] = rxbuff[0];
								sendto(sock, dataConfirm, 1, 0, (struct sockaddr*)&serv, servLen);
								status.prepared = YES;
								status.ready = NO;
						}
						else {	
							//printf("PREPARE -- >>ALREADY\n");
							sendto(sock, dataConfirm, 1, 0, (struct sockaddr*)&serv, servLen);
						}
						break;
				case LOST_PACKETS:
						if(status.prepared == NO){	
							//printf("LOST_PACKETS -- >> NO\n");
							continue;
						}
						//printf("LOST_PACKETS -- >> YES\n");
						last = 1;
						count = 0;
						lostPackets[0] = rxbuff[0];
						for(i = 0; i < audio_info.size/1450; i++){
								if(count >=255)break;
								if(packets_map[i] == 0){
										memcpy(&lostPackets[count*4+2],&i,4);
										count++;
								}
						}
						lostPackets[1] = count;
						sendto(sock, lostPackets, count*4 + 2, 0, (struct sockaddr*)&serv, servLen);
						break;
				case SKEW:
						memcpy(&timeSkew,&rxbuff[1],4);
						//printf("Skew %.3f\n",timeSkew);
						if(timeSkew<0){                                                                                    
							skewDir = -1;                                                                                     
							timeSkew= timeSkew*(-1.0);                                                              
						}
						else skewDir = 1;
						sampleCount = 44100.0*(timeSkew/1000000.0);                                                      
						if(sampleCount<1.0){                                                                                
							if(modff((float)1.0/sampleCount,&samplePeriod)>(float)0.5)samplePeriod++;                               
							sampleCount = 1.0;                                                                          
						}                                                                                             
						else {                                                                                        
							if(modff(sampleCount,&sampleCount)>(float)0.5)sampleCount++;                                            
							samplePeriod = 1.0;                                                                         
						}                                                                                             
						break;
				case SET:
						if(status.playing == YES){
							memcpy(&params.time,&rxbuff[1],8);
							timer(&params.time);
							current = audio_info.rate*2*(uint32_t)(rxbuff[10]<< 8 | rxbuff[9]);	
							//printf("SET %d\n", (int)(rxbuff[10]<<8 | rxbuff[9]));
						}
						break;
				default:
						printf("default '%p'\n",rxbuff[0]);
				
			}
	}	

}
