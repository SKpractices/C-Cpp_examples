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
            
            
            img2.gray_Data[i*w2+j] = (p1*f1 + p2*f2 + p3*f3 + p4*f4);
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


















