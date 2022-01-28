#include<stdio.h>
#include<stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./stb/stb_image_write.h"


int main(void){
	int width, height, chan;
	unsigned char *img = stbi_load("cover.jpg", &width, &height, &chan, 0);
	printf("P3\n%d %d\n255\n",width,height);
//	unsigned char *new_img = STBI_MALLOC(width * height * chan);

    unsigned char *cond = img + width * height * chan;
    printf("%d\n",*cond);
    int i = 0;
	for(unsigned char *p = img; p != cond; p += chan){
	//	printf("%d %d %d\n",*(img),*(img + 1),*(img + 2));
		*(p) = 255;
		i += 3;
	}
	printf("%d\n",i);

	stbi_write_jpg("new.jpg",width,height, chan,img,100);


	return 0;
}
