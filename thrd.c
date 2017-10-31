#include "main.h"


void *thrd_play_func(void *__params){
		status.playing = YES;
		struct Params			params = *(struct Params *)__params;
		struct timeval			timepoint;
		int						alsaFormat, bytes,channels;
		int						gpio_fd, audio_fd;
		float					countWritei = 1.0;
		uint32_t				sampleRate;
		//snd_pcm_t				*PlaybackHandle=NULL;
		static const char       *device = "plughw:0,0";
		register int			err;
		int						i;
		snd_pcm_sframes_t		frames;
		float					buff[BUFFERSIZE];
		unsigned char			*buffer;
		int32_t					fd;
		snd_pcm_uframes_t		WaveSize;
	    struct wavHead_t		wavHead;
		channels = CHANNNELS;
		snd_pcm_state_t			state;
		if(args.channel == 0){
					alsaFormat = SND_PCM_FORMAT_FLOAT;
					sampleRate = SAMPLEFREQ;
					for(i = 0; i < BUFFERSIZE; i++){
						buff[i] = sin(FREQUENCY * (2* M_PI) * i/ SAMPLEFREQ);
					}

		}
		else {
				alsaFormat = SND_PCM_FORMAT_S16_LE;
				sampleRate = audio_info.rate;
		}
		struct stereoSample_t	samples[sampleRate];
		struct monoSamples_t	mono[sampleRate];	
	
		if ((err = snd_pcm_open(&PlaybackHandle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0){
			printf("Can't open audio %s: %s\n", device, snd_strerror(err));
			return 0;	
		}
		
		if ((err = snd_pcm_set_params(PlaybackHandle, alsaFormat, SND_PCM_ACCESS_RW_INTERLEAVED, channels,  sampleRate, 1, 500000)) < 0){
			printf("Can't set sound parameters: %s\n", snd_strerror(err));
			return 0;
		}
		timer(&params.time);
		current = 0;	
		switch(args.channel){
				case 1:
					while(current <= audio_info.size){
						if(playingStatus == PLAY){
							frames = snd_pcm_writei(PlaybackHandle, &audio_buff[current], 22050);
							if(frames < 0)frames = snd_pcm_recover(PlaybackHandle, frames, 0);	
								current+=44100;
							}
							else if(playingStatus == PAUSE){
								usleep(200000);
							}
						else if(playingStatus == STOP)break;
					}
						/*
						while((bytes = read(STDIN_FILENO, samples, wavHead.byteRate)) > 0 && status.playing == 1){
								getLeftChannel(samples,mono,&bytes);
								WaveSize = bytes / wavHead.blockAlign;
								if(count2 != period){
										snd_pcm_writei(PlaybackHandle, mono, WaveSize);
										count2++;
								}else {
										if(dir<0){
												printf("------%.3f = %.3f\n",count2,period);
												snd_pcm_writei(PlaybackHandle, mono, WaveSize - count);
										}
										else if (dir >0){
												printf("++++++%.3f = %.3f\n",count2,period);
												snd_pcm_writei(PlaybackHandle, mono, WaveSize);
												snd_pcm_writei(PlaybackHandle, mono, count);
										}
										count2 = 1;
								}
						}
						*/
				break;
				case 2:
					while(current <= audio_info.size){
						if(playingStatus == PLAY){
							frames = snd_pcm_writei(PlaybackHandle, &audio_buff[current], 4410);
							if(frames < 0)frames = snd_pcm_recover(PlaybackHandle, frames, 0);	
								current+=4410*2;
							}
							else if(playingStatus == PAUSE){
								usleep(200000);
							}
						else if(playingStatus == STOP)break;
					}

						/*
						while((bytes = read(STDIN_FILENO, samples, wavHead.byteRate)) > 0 && status.playing == 1){
								getRightChannel(samples,mono,&bytes);
								WaveSize = bytes / wavHead.blockAlign;
								if(count2 != period){
										snd_pcm_writei(PlaybackHandle, mono, WaveSize);
										count2++;
								}else {
										if(dir<0){
												printf("------%.3f = %.3f\n",count2,period);
												snd_pcm_writei(PlaybackHandle, mono, WaveSize - count);
										}
										else if (dir >0){
												printf("++++++%.3f = %.3f\n",count2,period);
												snd_pcm_writei(PlaybackHandle, mono, WaveSize);
												snd_pcm_writei(PlaybackHandle, mono, count);
										}
										count2 = 1;
								}

						}*/
				break;
				default:
						while(playingStatus == PLAY){
								if(countWritei < samplePeriod){
										for(i = 0; i < FREQUENCY; i++)snd_pcm_writei(PlaybackHandle, buff, BUFFERSIZE);
										countWritei++;
								}
								else{
										if(skewDir < 0){
												printf("------%.3f = %.3f\n",countWritei,samplePeriod);
												countWritei = 1;
												for(i = 0; i < FREQUENCY-1; i++)snd_pcm_writei(PlaybackHandle, buff, BUFFERSIZE);
												snd_pcm_writei(PlaybackHandle, buff, BUFFERSIZE - sampleCount);
										}
										else if (skewDir > 0){
												printf("++++++%.3f = %.3f\n",countWritei,samplePeriod);
												countWritei = 1;
												for(i = 0; i < FREQUENCY; i++)snd_pcm_writei(PlaybackHandle, buff, BUFFERSIZE);
												snd_pcm_writei(PlaybackHandle, buff, sampleCount);	
										}
								}
						}	
		}
		playingStatus = STOP;
		free(audio_buff);
		audio_buff = NULL;
		free(packets_map);
		snd_pcm_close(PlaybackHandle);
		status.prepared = NO;
		status.ready = YES;		
		status.playing = NO;
}


