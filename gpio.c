void exportGPIO(char *dir){                                                                                     
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


