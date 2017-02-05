#include <stdio.h>
#include <stdlib.h>


void bit_map_convert(char* in_pic, char* out_txt, int clr)
{
    FILE * pic = fopen(in_pic, "rb");
    const char gs[18] = "$@8&MV+<|\"=~;`. ";

    if (pic == NULL){
        fprintf(stderr, "cannot open the pic");
    }
    unsigned char bitmap_header [54];
    fread(bitmap_header, sizeof(unsigned char), 54, pic);

    int width = (*(int*)&bitmap_header[18]);
    int height = *(int*)&bitmap_header[22];
    int pic_size = *(int*)&bitmap_header[34];
    int true_width = 3 * width;

    unsigned char* orig_pic = (unsigned char*) malloc(pic_size);
    fread(orig_pic, sizeof(unsigned char), pic_size, pic);

    int to_grey[height / 4][width / 2];


    for (int i = 0; i < height; i += 4){
        for (int j = 0; j < width; j += 2){
            to_grey[i / 4][j / 2] =
            (orig_pic[i * true_width + j * 3]
             + orig_pic[i * true_width + j * 3 + 1]
             + orig_pic[i * true_width + j * 3 + 2]) / 3;
        }
    }

    FILE * output_txt = fopen(out_txt, "w");
    for(int i = height / 4 - 1; i >= 0 ; i-=clr){
        for (int j = 0 ; j < width / 2 ; j+=clr){
            fputc ( gs[(to_grey[i][j] / 17)], output_txt);
        }
        fputc('\n', output_txt);
    }

    free(orig_pic);
    fclose(output_txt);
    fclose(pic);
}

int main(void){
    char input_pic[20] = {'\0'};
    char out_put[20] = {'\0'};
    printf("Please in put the file name you want to convert in the following format\n");
    printf("file_name_of_picture file_name_of_output\n");
    scanf("%s %s", &input_pic, &out_put);
    printf("from scale 1 ~ 4, how clear do you want to output be?\n");
    printf("1 will result in a huge picture ouput (provide your original pic is clear enough...)\n");
    int resl = 0;
    scanf("%d",&resl);
    printf("working...\n");

    bit_map_convert(input_pic, out_put, resl);
    printf("done\n");

    printf("Press any key to continue...\n");
    getchar();

    return 0;
}
