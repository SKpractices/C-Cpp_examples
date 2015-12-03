//
//  main.c
//  testing
//
//  Created by Sourabh Kulhare on 11/19/15.
//  Copyright © 2015 Sourabh Kulhare. All rights reserved.
//

#include <stdio.h>
#include <wand/MagickWand.h>// ImageMagick's C framework for image processing.
#include <time.h>
#include <stdint.h>



//Macros for fixed point operations:-  
//quant = This multiplies two float values and quanlize them between 1 to 255. 

#define quant(x,y) (unsigned char)(x*y*256.0f)
#define minusOne(x) (1.0f - x)
#define getFractionPart(a) (float)(a-(int)(a))



//image structure.
//     (1) - gray_Data = Pointer to store buffer of grayscale values.
//     (2) - height = Height of image.
//     (3) - width = Width of image.
 
struct image
{
    unsigned char *gray_Data;
    int height;
    int width;
};


//function to get image properties.
//return type : struct image.
//name = path of image with filename.

struct image get_Image(char name[])
{
    
    MagickWandGenesis();
    MagickBooleanType img_Check;

    // wand_ip = instance of image.
    MagickWand *wand_ip; 
    wand_ip = NewMagickWand();

    // Read image as command line arguments.
    img_Check =  MagickReadImage(wand_ip, name); 
    
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
    if(image_Buff==NULL)
    {
        printf("Out of memory.");
        exit(-1);
    }
    
    //Read grayscale values and store them into memory buffer.
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



//  function for bilinear interpolation with SSE-2 Optimization. 
//  Parameters : img1 = Input image for scaling.  
//               h2   = Height for new image. 
//               w2   = Width for new image. 
struct image biLinearInterPolate(struct image img1, int h2, int w2)
{
    
    struct image img2;
    //allocate space for gray data. 
    img2.gray_Data = (unsigned char *)malloc(h2*w2*sizeof(unsigned char));    
    img2.height = h2;
    img2.width = w2;


    //Calculate scale factors for later use. 
    register float heightRatio = img1.height*(1.0/h2);
    register float widthRatio = img1.width*(1.0/w2);

    //Pixel values for neighboring pixels
    unsigned char pixel1,pixel2,pixel3,pixel4;

    //fractions differences between indexes. 
    float del_h,del_h1,del_w,del_w1;
    float height_Frac_Idx,width_Frac_Idx;
    int weight1,weight2,weight3,weight4,height_idx,width_idx;
    

    for (int i = 0; i<h2; i++)
    {
        height_Frac_Idx = (i*heightRatio);
        height_idx = (int) height_Frac_Idx;
        del_h = getFractionPart(height_Frac_Idx);
        del_h1 = minusOne(del_h);

        for (int j=0;j<w2;j++)
        {

            width_Frac_Idx =  (j*widthRatio);
            width_idx =  (int) width_Frac_Idx;
            
            pixel1 = img1.gray_Data[((height_idx*img1.width)+width_idx)];
            pixel2 = img1.gray_Data[((height_idx+1)*img1.width)+width_idx];
            pixel3 = img1.gray_Data[(height_idx*img1.width)+(width_idx+1)];
            pixel4 = img1.gray_Data[((height_idx+1)*img1.width)+(width_idx+1)];
            
            
            del_w = getFractionPart(width_Frac_Idx);
            del_w1 = minusOne(del_w);


            weight1 = quant(del_h1,del_w1);
            weight2 = quant(del_h,del_w1);
            weight3 = quant(del_h1,del_w);
            weight4 = quant(del_h,del_w);

            img2.gray_Data[i*w2+j] = (pixel1*weight1 + pixel2*weight2 + pixel3*weight3 + pixel4*weight4)>>8;
        }
    }
    
    return img2;
}


//Main function.
// It accepts three agruments from commandline.
//      argv[1] = Height of output image.
//      argv[2] = Width of output image.
//      argv[3] = filename with path.
//      for example -
//      ./main.o 1200 800 /Users/SK_Mac/Downloads/image3.jpg
//


int main(int argc,  char * argv[])
{
    clock_t time_Log;
    time_Log = clock();
    
    struct image get_Image(char []);
    
    struct image biLinearInterPolate(struct image img_ran,int h_ran, int w_ran);

    //get file name
    char *filename = argv[3];


    //Get image properties.
    struct image image_IP = get_Image(filename);
    
    int height2 = (int) strtol(argv[1], NULL, 0);
    int width2 = (int) strtol(argv[2], NULL, 0);
    time_Log = clock();

    //Perform scaling operation. 
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








