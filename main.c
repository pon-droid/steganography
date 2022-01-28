#include<stdio.h>
#include<stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./stb/stb_image_write.h"

void c_tobinary(int bin[8], unsigned char x){
	
	int i;
	for( i = 0; i < 8; i++){
		bin[7-i] = (x >> i) & 1;
	}
	
}

int d_todecimal(int bin[8]){
    int total = 0;
    
	int i;
	for( i = 0; i < 8; i++){
		total *= 2;
		total += bin[i];
	}

	return total;
}

int merge_bits(int cbin [8], int sbin [8]){
	cbin[7] = sbin[3];
	cbin[6] = sbin[2];
	cbin[5] = sbin[1];
	cbin[4] = sbin[0];

	return d_todecimal(cbin);
}

unsigned char hide_in_colour(unsigned char c,unsigned char s){
	int cbin[8];
	c_tobinary(cbin,c);
	int sbin[8];
	c_tobinary(sbin,s);

	return merge_bits(cbin,sbin);
}


int main(int argc, char *argv []){
	if(argc != 4){
		printf("INVALID\nUsage: steg [image] [secret_image] [output]\n");
		return -1;
	}
	int CH,CW,CCHAN;
	int SH, SW, SCHAN;
	unsigned char *cover = stbi_load(argv[1], &CW, &CH, &CCHAN, 0);
	unsigned char *secret = stbi_load(argv[2], &SW, &SH, &SCHAN, 0);

	if(SH != CH || SW != CW){
		printf("Stop!\n");
		return -2;
	}

    unsigned char *cp, *sp;

	for(cp = cover, sp = secret; cp != cover + CH * CW * CCHAN; cp += CCHAN, sp += SCHAN){
		*(cp) = hide_in_colour(*cp,*sp);
		*(cp + 1) = hide_in_colour(*(cp+1),*(sp+1));
		*(cp + 2) = hide_in_colour(*(cp+2),*(sp+2));	   	
	}


	stbi_write_jpg(argv[3],CW,CH, CCHAN,cover,100);


	return 0;
}
