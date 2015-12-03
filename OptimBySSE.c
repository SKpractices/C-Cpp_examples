//
//  main.c
//  testing
//
//  Created by Sourabh Kulhare on 11/21/15.
//  Copyright © 2015 Sourabh Kulhare. All rights reserved.
//

#include <stdio.h>
#include <wand/MagickWand.h>// ImageMagick's C framework for image processing.
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <malloc/malloc.h>
#include <immintrin.h>

#define quant(x) (unsigned char)(x*256.0f)
#define minusOne(x) (1.0f - x)

/*image structure.
 gray_Data = Pointer to store buffer of grayscale values.
 height = Height of image.
 width = Width of image.
 */
struct image
{
    unsigned char *gray_Data;
    int height;
    int width;
};


/*function to get image properties.
        return type : struct image.
        name = Image fila name with path.
 */


struct image get_Image(char name[])
{
    
    MagickWandGenesis();
    MagickBooleanType img_Check;
    MagickWand *wand_ip; // wand_ip = instance of image.
    wand_ip = NewMagickWand();
    img_Check =  MagickReadImage(wand_ip, name); // Read image as command line arguments.
    
    if(img_Check==MagickFalse)
    {
        printf("ERROR : - Image not found. ");
    }
    
    int height1,width1;
    
    // get the height and width(type cast ot int as it gives unsigned long as return type).
    width1 =  (int) MagickGetImageWidth(wand_ip);
    height1 = (int) MagickGetImageHeight(wand_ip);
    
    //Allocate space for gray scale values.
    size_t total_gray_pixels =  height1*width1;
    unsigned char * image_Buff = (unsigned char *)malloc(total_gray_pixels*sizeof(unsigned char));
    //perror("memory allocation failure");
    //exit(EXIT_FAILURE);
    
    //Read grayscale values ans store them into memory buffer.
    MagickExportImagePixels(wand_ip,   // Image instance
                            0,         // Start X
                            0,         // Start Y
                            width1,    // End X
                            height1,   // End Y
                            "I",       // Value where "I" = intensity = gray value
                            CharPixel, // Storage type where "int == (0 ~ 255)
                            image_Buff);     // Destination pointer
    
    //Destroy image instance.
    wand_ip = DestroyMagickWand(wand_ip);
    MagickWandTerminus();
    
    struct image image1 = {image_Buff, height1, width1};
    
    return image1;
}



struct image biLinearInterPolateSSE(struct image img1, int h2, int w2)
{
    
    
    struct image img2;
    
    img2.gray_Data = (unsigned char *)malloc(h2*w2*sizeof(unsigned char));
    
    if(w2%4!=0)
    {
        int temp;
        temp = (int)(w2>>2);
        w2 = temp*4;
    }

    unsigned char p1[4],p2[4],p3[4],p4[4];
    float del_h[4],del_h1[4],del_w[4],del_w1[4],f1[4],f2[4],f3[4],f4[4];
    register float height_Frac_Idx,width_Frac_Idx[4];
    register int height_idx,width_idx[4];
    
    __m128 set1,set2,set3,setResults1,setResults2;
    
    
    float* ones = _mm_malloc(sizeof(float)*4, 4);
    for(int i = 0 ; i<4 ; i++)
    {
        ones[i] = 1.0f;
    }
    
    set3 = _mm_loadu_ps(&ones[0]);

    for (int i = 0; i<h2; i++)
    {

        height_Frac_Idx = ((i*img1.height)*(1.0/h2));
        height_idx = (int) height_Frac_Idx;
        
        for(int l = 0; l<4 ; l++)
        {
        del_h[l] = height_Frac_Idx - height_idx;
        del_h1[l] = minusOne(del_h[l]);
        }
        
        for (int j=0;j<w2;j+=4)
        {
            for(int k=0 ; k<4 ;k++)
            {
                
            width_Frac_Idx[k] =  (((j+k)*img1.width)*(1.0/(w2)));
            width_idx[k] =  (int) width_Frac_Idx[k];
                
            p1[k] = img1.gray_Data[((height_idx*img1.width)+width_idx[k])];
            p2[k] = img1.gray_Data[((height_idx+1)*img1.width)+width_idx[k]];
            p3[k] = img1.gray_Data[(height_idx*img1.width)+(width_idx[k]+1)];
            p4[k] = img1.gray_Data[((height_idx+1)*img1.width)+(width_idx[k]+1)];
            del_w[k] = width_Frac_Idx[k]- width_idx[k];
            }
            
            set2 = _mm_loadu_ps(&del_w[0]);
            //Calculate del_w1
            setResults1 = _mm_sub_ps(set3,set2);
            //get del_h1
            set1 = _mm_loadu_ps(&del_h1[0]);
            
            //calculate del_w1*del_h1
            setResults2 = _mm_mul_ps(set1, setResults1);
            _mm_store_ps(&f1[0], setResults2);
            
            //get del_h
            set1 = _mm_loadu_ps(&del_h[0]);
            //calculate del_h1*del_w1
            setResults2 = _mm_mul_ps(set1, setResults1);
            _mm_store_ps(&f2[0], setResults2);
            
            set1 = _mm_loadu_ps(&del_h1[0]);
            
            //calculate del_h1*del_w
            setResults2 = _mm_mul_ps(set2, set1);
            _mm_store_ps(&f3[0], setResults2);
            
            //calculate del_h*del_w
            set1 = _mm_loadu_ps(&del_h[0]);
            setResults2 = _mm_mul_ps(set1, set2);
            _mm_store_ps(&f4[0], setResults2);
        
        for(int k=0;k<4;k++)
        {

            img2.gray_Data[(i*w2)+j+k] = (p1[k]*quant(f1[k])+p2[k]*quant(f2[k])+p3[k]*quant(f3[k])+p4[k]*quant(f4[k]))>>8;
        }
        }
    }
    free(ones);
    img2.height = h2;
    img2.width = w2;
    
    return img2;
}


/* Main function.
 It accepts three agruments from commandline.
 argv[1] = Height of output image.
 argv[2] = Width of output image.
 argv[3] = filename with path.
 
 for example -
 ./main.o 1200 800 /Users/SK_Mac/Downloads/image3.jpg
 */


int main(int argc,  char * argv[])
{
    clock_t time_Log;
    //time_Log = clock();
    
    struct image get_Image(char []);
    
    struct image biLinearInterPolateSSE(struct image img_ran,int h_ran, int w_ran);
    
    //get file name
    char *filename = argv[3];
    //Get image properties.
    struct image image_IP = get_Image(filename);
    
    int height2 = (int) strtol(argv[1], NULL, 0);
    int width2 = (int) strtol(argv[2], NULL, 0);
    
    time_Log = clock();

    struct image img_OP = biLinearInterPolateSSE(image_IP, height2, width2);
    
    time_Log = clock() - time_Log;

    
    //Initialize the wand for output window.
    
    MagickWandGenesis();
    // Wand variables
    char hex[8];
    MagickWand *wand_op;
    wand_op = NewMagickWand();
    PixelIterator *iterator_op;
    unsigned long w2_temp = (unsigned long)img_OP.width;
    PixelWand *p_wand = NULL;
    p_wand = NewPixelWand();
    //Set the initial color, it is necessary in MagicWand to have it.
    PixelSetColor(p_wand,"white");
    //Configure the output image wand.
    MagickNewImage(wand_op,(unsigned long)width2,(unsigned long)height2,p_wand);
    iterator_op=NewPixelIterator(wand_op);
    PixelWand **pixels_op;
    for(int i=0;i<height2;i++)
    {
        // Get the next row of the image as an array of PixelWands
        pixels_op= PixelGetNextIteratorRow(iterator_op,&w2_temp);
        // Set the row of wands to a simple gray scale gradient
        for(int j=0;j<w2_temp;j++)
        {
            /*formating RGB values to hex, as these are gray scale values so assgning R,G,B same values.
             It is the way MagicWand render an image.
             */
            
            unsigned int temp_char = img_OP.gray_Data[(i*w2_temp)+j];
            sprintf(hex,"#%02x%02x%02x",temp_char,temp_char,temp_char);
            PixelSetColor(pixels_op[j],hex);
        }
        // Sync writes the pixels back to the m_wand
        PixelSyncIterator(iterator_op);
    }
    
    //MagickWriteImage(wand_op,"$HOME/Desktop/scaled.jpg");
    //get the processing time.
    //time_Log = clock() - time_Log;
    double time_taken = ((double)time_Log)/CLOCKS_PER_SEC; // in seconds
    printf("took %f seconds to execute \n", time_taken);
    
    //Display the output image.
    MagickDisplayImage(wand_op, ":0");
    free(img_OP.gray_Data);
    
    
    //Destroy the image instance.
    wand_op = DestroyMagickWand(wand_op);
    MagickWandTerminus();
    return 0;
}








