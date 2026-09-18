#include <regex.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
	    
	    
	int main(void){
			const char * regStr = "<a [^>]\\+\\?href=\"\\([^\"]\\+\\?\\)\">\\([^<]\\+\\?\\)</a>"; //匹配超连接标签的表达式
		regex_t reg; 
		regcomp(&reg,regStr,0);


		//关于数据源,将文件数据映射到进程内存
		int fd = open("url.html",O_RDWR);
		int size = lseek(fd,0,SEEK_END);//返回文件大小
		char * mmap_ptr = NULL;
		mmap_ptr =(char*) mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_PRIVATE,fd,0);
		close(fd);
		printf("%s\n",mmap_ptr);


		int reg_num = 3; //正则数量
		regmatch_t match[reg_num];
		char link[1024]= {0};
		char title[1024] = {0};
		while(regexec(&reg , mmap_ptr , reg_num , match,0) == 0){
			snprintf(link,match[1].rm_eo-match[1].rm_so+1,"%s",mmap_ptr+match[1].rm_so);
			snprintf(title,match[2].rm_eo-match[2].rm_so+1,"%s",mmap_ptr+match[2].rm_so);
			mmap_ptr += match[0].rm_eo;
			printf("匹配结果 ， title = %s   link = %s\n",title,link);

		}
		regfree(&reg);

		return 0;	
	}
