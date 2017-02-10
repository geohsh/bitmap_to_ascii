#include <stdio.h>
#include <stdlib.h>

//********************************************************
// this program only works on 24-bits
// MS bitmap (.bmp) file
// output is pure txt, no colour is
// generated lol
//********************************************************


// bitmap_convert converts the bitmap picture in_pic to ASCII arts
//   and output the result into the out_txt file. clr is a the compress
//   ratio
// requries: use my main... (or give me the strings...)
// Note: clr >= 1
void bitmap_convert(char* in_pic, char* out_txt, int clr);

int main(void){
    char input_pic[20] = {'\0'};
    char out_put[20] = {'\0'};
    printf("Please in put the file name you want to convert in the following format\n");
    printf("file_name_of_picture file_name_of_output\n");
    scanf("%s %s", &input_pic, &out_put);
    printf("from scale 1 ~ 4, how clear do you want to output be?\n");
    printf("1 means keep the original width while 4 means width / 4\n");
    int resl = 0;
    scanf("%d",&resl);
    printf("working...\n");

    bitmap_convert(input_pic, out_put, resl);
    printf("done\n");

    printf("Press any key to continue...\n");
    getchar();

    return 0;
}

void bitmap_convert(char* in_pic, char* out_txt, int clr)
{
    FILE * pic = fopen(in_pic, "rb");
    const char grey_scale[16] = "$@8&MV+<|\"=~;`. "; //drakest to lightest

    if (pic == NULL){
        fprintf(stderr, "cannot open the pic");
    }
    unsigned char bitmap_header [54]; // header of bitmap
    fread(bitmap_header, sizeof(unsigned char), 54, pic);

    int width = *(int*)&bitmap_header[18];
    int height = *(int*)&bitmap_header[22];
    int pic_size = *(int*)&bitmap_header[34];

    int true_width = 3 * width; // the size (byte-wise)
    int padding = (4 - (true_width % 4)) % 4;

    unsigned char* pic_line = (unsigned char*) malloc(true_width);

    char to_grey[height / (2 * clr) + 1][width / clr];
    int to_grey_w = 0;
    int to_grey_h = 0;
    for (int i = 0; i < height; ++i){
        fread(pic_line, sizeof(unsigned char), true_width + padding, pic);
        if (i % (2 * clr)) continue; // used to compress the picture
        for (int j = padding; j < true_width; j += (3 * clr)){
            int gr_sele = ((int)(pic_line[j] + pic_line[j+1] + pic_line[j+2])) / 3;
            to_grey[to_grey_h][to_grey_w] = grey_scale[gr_sele / 16];
            to_grey_w++;
        }
       to_grey_h++;
       to_grey_w = 0;
    }

    FILE * output_file = fopen(out_txt, "w");
    for(int i = to_grey_h - 1; i >= 0; i--){
        for (int j = 0 ; j < (width / clr); j++){
            fputc (to_grey[i][j], output_file);
        }
        fputc('\n', output_file);
    }

    free(pic_line);
    fclose(output_file);
    fclose(pic);
}
