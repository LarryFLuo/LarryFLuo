#include <stdio.h>
#include <stdint.h>

#define ARRAY_LENGTH 976
//#define pr_info(x)  printf x
#define pr_info(x)  
uint64_t swap_uint64( uint64_t val )
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | (val >> 32);
}

int swap16bit(unsigned char * arr, int arr_len)
{
	uint64_t *tmp = (uint64_t*) arr;
	uint64_t a = 0;
	int i, len;
 	
	if (arr_len%8) {
		pr_info((" \nlen must be the multiple of 8 bytes for SPI DMA \n"));
		return -1;
	}
	len = arr_len/8;
	pr_info((" \nlen =%d \n", len));
	for (i = 0; i < len; i++) {
		a = tmp[i];
		pr_info(("a =0x%0lx\n", a));
#if 1
	        a = (((a & 0x00ffULL) << 56)		| ((a & 0xff00000000000000ULL) >> 56) |
		     ((a & 0x00ff00ULL) << 40) 		| ((a & 0x00ff000000000000ULL) >> 40) |
	      	     ((a & 0x00ff0000ULL) << 24) 	| ((a & 0x0000ff0000000000ULL) >> 24) |
	             ((a & 0x00ff000000ULL) << 8) 	| ((a & 0x000000ff00000000ULL) >> 8)
		    );
  		tmp[i] = a;
#else
		tmp[i] = swap_uint64(a);
#endif
		pr_info(("tmp =0x%0lx\n", tmp[i]));
	}
	return 0;
}	
int main(int argc, char *argv[] )
{
	int i;
	int ret = 0;
	unsigned char c_array[ARRAY_LENGTH] = { 
		0x11, 0x22, 0x33, 0x44,
	       	0x55, 0x66, 0x77, 0x88,
		0x99, 0xaa, 0xbb, 0xcc, 
		0xee, 0xff, 0xff, 0xee,
		0xdd, 0xcc, 0, 0,
	};

	for (i=0; i< 32; i++) {
		if (i%8 == 0)
			pr_info(("\n 0x%02x\t", c_array[i]));
		else 
			pr_info(("0x%02x \t", c_array[i]));
	}
	pr_info(("\n"));

	ret = swap16bit(c_array, sizeof(c_array));
	if (!ret) {
		pr_info(("\n"));
		for (i=0; i< 32; i++) {
			if (i%8 == 0)
				pr_info(("\n 0x%02x\t", c_array[i]));
			else 
				pr_info(("0x%02x \t", c_array[i]));
		}
		pr_info(("\n"));
	}
	else {
		pr_info(("Error:%d\n", ret));
	}

}
