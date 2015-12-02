//
//  main.c
//  testing
//
//  Created by Sourabh Kulhare on 11/22/15.
//  Copyright © 2015 Sourabh Kulhare. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <wand/MagickWand.h>// ImageMagick's C framework for image processing.
#include <time.h>
#include <stdint.h>
#include <malloc/malloc.h>
#include <immintrin.h>



// Deifne macros for fixed point conversion

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

// Bilinear interpolation with SSE-2 implementation. 

struct image biLinearInterPolateSSE(struct image img1, int h2, int w2)
{
    
    struct image img2;
    img2.gray_Data = (unsigned char *)malloc(h2*w2*sizeof(unsigned char));

    
    int temp_count = 0;
    float height_Frac_Idx[4];
    float width_Frac_Idx[4];
    int height_idx[4], width_idx[4];
    unsigned char p1[4],p2[4],p3[4],p4[4];
    float del_h[4],del_h1[4],del_w[4],del_w1[4];
    float f1[4],f2[4],f3[4],f4[4];
    int weight1[4], weight2[4],weight3[4],weight4[4];
    
    
    if(w2%4!=0)
    {
        int temp;
        temp = w2/4;
        w2 = temp*4;
        
    }
    
    
    for (int i = 0; i<h2; i++)
    {
        
        // OPERATION -1
        for (int k=0; k<4;k++){
            height_Frac_Idx[k] = ((i*img1.height)/(float)h2);
        }
        
        
        for (int j=0;j<w2;j+=4)
        {
            
            for (int k=0;k<4;k++){
            width_Frac_Idx[k] =  (((j+k)*img1.width)/(float)w2);
            height_idx[k] = (int) height_Frac_Idx[k];
            width_idx[k] =  (int) width_Frac_Idx[k];
            
                // OPERATION : 3.1-3.10
            p1[k] = img1.gray_Data[((height_idx[k]*img1.width)+width_idx[k])];
            p2[k] = img1.gray_Data[((height_idx[k]+1)*img1.width)+width_idx[k]];
            p3[k] = img1.gray_Data[(height_idx[k]*img1.width)+(width_idx[k]+1)];
            p4[k] = img1.gray_Data[((height_idx[k]+1)*img1.width)+(width_idx[k]+1)];
        
            del_h[k] = height_Frac_Idx[k] - height_idx[k];
            del_w[k] = width_Frac_Idx[k]- width_idx[k];
            del_h1[k] = 1.0f - del_h[k];
            del_w1[k] = 1.0f - del_w[k];
                
            }
            
            //unsigned char* iteratPointer = _mm_malloc(sizeof(unsigned char)*4,4);
            
            __m128i weightSet1,weightSet2, sseResults;
            weightSet1 = _mm_loadu_ps(&del_h1[0]);
            weightSet2 = _mm_loadu_ps(&del_w1[0]);
            sseResults = _mm_mul_ps(weightSet1, weightSet2);
            _mm_store_ps(&f1[0],sseResults);
            
            
            weightSet1 = _mm_loadu_ps(&del_h[0]);
            weightSet2 = _mm_loadu_ps(&del_w1[0]);
            sseResults = _mm_mul_ps(weightSet1, weightSet2);
            _mm_store_ps(&f2[0],sseResults);

            
            weightSet1 = _mm_loadu_ps(&del_h1[0]);
            weightSet2 = _mm_loadu_ps(&del_w[0]);
            sseResults = _mm_mul_ps(weightSet1, weightSet2);
            _mm_store_ps(&f3[0],sseResults);

            
            weightSet1 = _mm_loadu_ps(&del_h[0]);
            weightSet2 = _mm_loadu_ps(&del_w[0]);
            sseResults = _mm_mul_ps(weightSet1, weightSet2);
            _mm_store_ps(&f4[0],sseResults);
            
            for (int k=0; k<4 ; k++)
            {
                weight1[k] = f1[k]*256.0f;
                weight2[k] = f2[k]*256.0f;
                weight3[k] = f3[k]*256.0f;
                weight4[k] = f4[k]*256.0f;
                
                
                
                img2.gray_Data[i*w2+(j+k)] = (p1[k]*weight1[k] + p2[k]*weight2[k] + p3[k]*weight3[k] + p4[k]*weight4[k])>>8;

                
            }
            
            //THIS IS THE PART TO USE SSE
            
            
            

        }
        temp_count+=1;
    }
    
    img2.height =h2;
    img2.width = w2;
    return img2;
}




// Bilinear interpolation with normal C
struct image biLinearInterPolate(struct image img1, int h2, int w2)
{
    
    struct image img2;
    img2.gray_Data = (unsigned char *)malloc(h2*w2*sizeof(unsigned char));
    img2.height = h2;
    img2.width = w2;
    
    for (int i = 0; i<h2; i++)
    {
        for (int j=0;j<w2;j++)
        {
            
            float height_Frac_Idx = ((i*img1.height)/(float)h2);
            float width_Frac_Idx =  ((j*img1.width)/(float)w2);
            
            int height_idx = (int) height_Frac_Idx;
            int width_idx =  (int) width_Frac_Idx;
            
            
            unsigned char p1 = img1.gray_Data[((height_idx*img1.width)+width_idx)];
            unsigned char p2 = img1.gray_Data[((height_idx+1)*img1.width)+width_idx];
            unsigned char p3 = img1.gray_Data[(height_idx*img1.width)+(width_idx+1)];
            unsigned char p4 = img1.gray_Data[((height_idx+1)*img1.width)+(width_idx+1)];
            
            
            float del_h = height_Frac_Idx - height_idx;
            float del_w = width_Frac_Idx- width_idx;
            float del_h1 = 1.0f - del_h;
            float del_w1 = 1.0f - del_w;
            
            int f1 = (del_h1)*(del_w1)*256.0f;
            int f2 = (del_h)*(del_w1)*256.0f;
            int f3 = (del_h1)*(del_w)*256.0f;
            int f4 = (del_w)*(del_h)*256.0f;
            
            
            img2.gray_Data[i*w2+j] = (p1*f1 + p2*f2 + p3*f3 + p4*f4)>>8;
        }
    }
    
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
    time_Log = clock();
    
    struct image get_Image(char []);
    
    struct image biLinearInterPolate(struct image img_ran,int h_ran, int w_ran);

    struct image biLinearInterPolateSSE(struct image img1, int h2, int w2);

    
    //get file name
    char *filename = argv[3];
    //Get image properties.
    struct image image_IP = get_Image(filename);
    
    int height2 = (int) strtol(argv[1], NULL, 0);
    int width2 = (int) strtol(argv[2], NULL, 0);
    
    
    struct image img_OP = biLinearInterPolate(image_IP, height2, width2);
    
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
            unsigned char temp_char = img_OP.gray_Data[(i*w2_temp)+j];
            sprintf(hex,"#%02x%02x%02x",temp_char,temp_char,temp_char);
            PixelSetColor(pixels_op[j],hex);
        }
        // Sync writes the pixels back to the m_wand
        PixelSyncIterator(iterator_op);
    }
    
    //MagickWriteImage(wand_op,"$HOME/Desktop/scaled.jpg");
    //get the processing time.
    time_Log = clock() - time_Log;
    double time_taken = ((double)time_Log)/CLOCKS_PER_SEC; // in seconds
    printf("took %f seconds to execute \n", time_taken);
    
    //Display the output image.
    MagickDisplayImage(wand_op, ":0");
    
    //Destroy the image instance.
    wand_op = DestroyMagickWand(wand_op);
    MagickWandTerminus();
    return 0;
}








