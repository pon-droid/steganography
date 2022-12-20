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
int merge_two_bits(int cbin[8], int sbin [8]){
    cbin[6] = sbin[0];
    cbin[7] = sbin[1];

    return d_todecimal(cbin);	
}

int unmerge_bits(int bin[8]){
	bin[0] = bin[4];
	bin[1] = bin[5];
	bin[2] = bin[6];
	bin[3] = bin[7];

	bin[4] = 0;
	bin[5] = 0;
	bin[6] = 0;
	bin[7] = 0;

	return d_todecimal(bin);
}

int unmerge_two_bits(int bin[8]){
	bin[0] = bin[6];
	bin[1] = bin[7];
	bin[2] = 0;
	bin[3] = 0;
	bin[4] = 0;
	bin[5] = 0;
	bin[6] = 0;
	bin[7] = 0;

	return d_todecimal(bin);
}

unsigned char hide_in_colour(unsigned char c,unsigned char s){
	int cbin[8];
	c_tobinary(cbin,c);
	int sbin[8];
	c_tobinary(sbin,s);

	return merge_bits(cbin,sbin);
}

unsigned char hide_in_colour_two_bits(unsigned char c,unsigned char s){
	int cbin[8];
	c_tobinary(cbin,c);
	int sbin[8];
	c_tobinary(sbin,s);

	return merge_two_bits(cbin,sbin);
}

unsigned char unhide_in_colour(unsigned char c){
	int bin[8];
	c_tobinary(bin,c);

	return unmerge_bits(bin);
}

unsigned char unhide_in_colour_two_bits(unsigned char c){
	int bin[8];
	c_tobinary(bin,c);

	return unmerge_two_bits(bin);
}

int main(int argc, char *argv []){
	if(argc != 4 && argc != 3){
		printf("Usage to unhide image: steg [image] [output image]\n");
		printf("Usage to hide image: steg [image] [secret_image] [output]\n");
		return -1;
	}

	if(argc >= 4){
	    int CH,CW,CCHAN;
	    int SH, SW, SCHAN;
	    unsigned char *cover = stbi_load(argv[1], &CW, &CH, &CCHAN, 0);
	    unsigned char *secret = stbi_load(argv[2], &SW, &SH, &SCHAN, 0);
		if(CH < SH || CW < SW){
			printf("Image to embed secret must be the same size or bigger\n");
			printf("%s is smaller than %s by %d and %d\n",argv[1],argv[2],SH*SW,CH*CW);
			return -3;
		}
	    unsigned char *cp, *sp;
		//This enables embedding images smaller than the image to embed in
	    int y, x;
	    cp = cover;
	    sp = secret;
	    for( y = 0; y < SH; y++){
	    	for( x = 0; x < SW; x++){
	    		int align = (y * SW + x) * SCHAN;
	    		int calign = (y * CW + x) * CCHAN;
	    		*(cp + calign) = hide_in_colour_two_bits(*(cp + calign),*(sp + align));
	    		*(cp + calign + 1) = hide_in_colour_two_bits(*(cp + calign + 1),*(sp + align + 1));
	    		*(cp + calign + 2) = hide_in_colour_two_bits(*(cp + calign + 2),*(sp + align + 2));
	    		//cp += CCHAN;
	    		//sp += SCHAN;
	    	}
	    }

/*
	    for(cp = cover, sp = secret; sp != secret + SH * SW * SCHAN; cp += CCHAN, sp += SCHAN){

	      *(cp) = hide_in_colour(*cp,*sp);
		  *(cp + 1) = hide_in_colour(*(cp+1),*(sp+1));
		  *(cp + 2) = hide_in_colour(*(cp+2),*(sp+2));
		}
*/
		stbi_write_png(argv[3],CW,CH,CCHAN,cover, CW * CCHAN);

		printf("Embedded %s into %s, output %s\n",argv[2],argv[1],argv[3]);
		
	} else {
		int HH,HW,HCHAN;
		unsigned char *hidden = stbi_load(argv[1], &HW, &HH, &HCHAN, 0);

		unsigned char *hp;

		for(hp = hidden; hp != hidden + HW * HH * HCHAN; hp += HCHAN){
			*(hp) = unhide_in_colour_two_bits(*(hp));
			*(hp + 1) = unhide_in_colour_two_bits(*(hp+1));
			*(hp + 2) = unhide_in_colour_two_bits(*(hp+2));
		}

		stbi_write_png(argv[2],HW,HH,HCHAN,hidden,HW * HCHAN);
	}




	return 0;
}
