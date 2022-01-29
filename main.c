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

int unmerge_bits(int bin[8]){
	bin[0] = bin[4];
	bin[1] = bin[5];
	bin[2] = bin[6];
	bin[3] = bin[7];

	bin[4] = 1;
	bin[5] = 1;
	bin[6] = 1;
	bin[7] = 1;

	return d_todecimal(bin);
}

unsigned char hide_in_colour(unsigned char c,unsigned char s){
	int cbin[8];
	c_tobinary(cbin,c);
	int sbin[8];
	c_tobinary(sbin,s);

	return merge_bits(cbin,sbin);
}

unsigned char unhide_in_colour(unsigned char c){
	int bin[8];
	c_tobinary(bin,c);

	return unmerge_bits(bin);
}

int main(int argc, char *argv []){
	if(argc != 4 || argc != 3){
		printf("Usage to unhide image: steg [image] [output image]\n");
		printf("Usage to hide image: steg [image] [secret_image] [output]\n")
		return -1;
	}

	if(argc == 4){
		
	}
	int CH,CW,CCHAN;
	int SH, SW, SCHAN;
	unsigned char *cover = stbi_load(argv[1], &CW, &CH, &CCHAN, 0);
	unsigned char *secret = stbi_load(argv[2], &SW, &SH, &SCHAN, 0);
/*
	if(SH != CH || SW != CW){
		printf("Stop!\n");
		return -2;
	}
*/
    unsigned char *cp, *sp;

	for(cp = cover, sp = secret; cp != cover + CH * CW * CCHAN; cp += CCHAN, sp += SCHAN){
		*(cp) = hide_in_colour(*cp,*sp);
		*(cp + 1) = hide_in_colour(*(cp+1),*(sp+1));
		*(cp + 2) = hide_in_colour(*(cp+2),*(sp+2));	   	
	}


	stbi_write_jpg(argv[3],CW,CH, CCHAN,cover,100);

	int HH,HW,HCHAN;

	unsigned char *hidden = stbi_load(argv[3], &HW, &HH, &HCHAN, 0);

	unsigned char *hp;

	for(hp = hidden; hp != hidden + HW * HH * HCHAN; hp += HCHAN){
		*(hp) = unhide_in_colour(*(hp));
		*(hp + 1) = unhide_in_colour(*(hp+1));
		*(hp + 2) = unhide_in_colour(*(hp+2));
	}

	stbi_write_jpg("unhidden.jpg",HW,HH,HCHAN,hidden,100);


	return 0;
}
