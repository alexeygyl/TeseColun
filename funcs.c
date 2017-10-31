#include "main.h"


void timer(struct timeval *__time){
		struct timeval tp;
		gettimeofday(&tp,NULL);
		while(__time->tv_sec > tp.tv_sec)gettimeofday(&tp,NULL);
		while(__time->tv_usec > tp.tv_usec)gettimeofday(&tp,NULL);
		//printf("Playing in  %d.%d\n",(int)tp.tv_sec,(int)tp.tv_usec);
}





void openGPIO(char *dir){                                                                                     
    int fd_gpio, fd_dir;                                                                                      
    fd_gpio = open("/sys/class/gpio/export",O_WRONLY);                                                        
    if(fd_gpio == -1){                                                                                        
        perror("Export");                                                                                     
        return;                                                                                               
    }                                                                                                         
    write(fd_gpio,"21",2);                                                                                    
    fd_dir = open("/sys/class/gpio/gpio21/direction",O_RDWR);                                                 
    if(!strcmp(dir,"in"))write(fd_dir,"in",2);                                                                
    else if(!strcmp(dir,"out"))write(fd_dir,"out",3);                                                         
    close(fd_dir);                                                                                            
    close(fd_gpio);                                                                                           
}


void setAlsaVolume(float volume){
    long min, max, pass;
    float exp = 2.718, a,k, vol_max = 100.0, vol_min = 2.0;
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    const char *card = "default";
    const char *selem_name = "PCM";
    volume = volume>100?100:volume; 
    volume = volume<2?2:volume;
    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, card);
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);
    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
    pass = (max - min)/100;
    a = pow(exp,((float)max*log(vol_min)-(float)min*log(vol_max))/(float)(max-min));
    k = (float)max/log(vol_max/a);
    //printf("A = %.3f,  K = %.3f\n",a,k);
    //printf("Volume %.3f  VALUE = %.3f", volume, k*log(volume/a));
    snd_mixer_selem_set_playback_volume_all(elem, k*log(volume/a));
    snd_mixer_close(handle);
}