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



/*image structure.
    gray_Data = Pointer to store buffer of grayscale values.
    height = Height of image.
    width = Width of image.
*/
struct image{
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
    unsigned char * image_Buff = malloc(total_gray_pixels);
    
    //Read grayscale values ans store them into memory buffer.
    MagickExportImagePixels(wand_ip,   // Image instance
                            0,         // Start X
                            0,         // Start Y
                            width1,    // End X
                            height1,   // End Y
                            "I",       // Value where "I" = intensity = gray value
                            CharPixel, // Storage type where "unsigned char == (0 ~ 255)
                            image_Buff);     // Destination pointer
    
    //Destroy image instance.
    wand_ip = DestroyMagickWand(wand_ip);
    MagickWandTerminus();

    struct image image1 = {image_Buff, height1, width1};
    
    return image1;
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
    
    //get file name
    char *filename = argv[3];
    
    //Get image properties.
    struct image image_IP = get_Image(filename);

    
    int height1 =image_IP.height;
    int width1 = image_IP.width;

    //2D matrix for input image.
    unsigned char img_IP[height1][width1];
    for (int i = 0; i<height1; i++)
    {
        for (int j = 0; j<width1; j++)
        {
            img_IP[i][j] = (int) image_IP.gray_Data[i*(int)width1+j];
        }
    }

    free(image_IP.gray_Data);

    unsigned long height2 = strtol(argv[1], NULL, 0);
    unsigned long width2 = strtol(argv[2], NULL, 0);
    
    
    //unsigned long height2 = 2500;
    //unsigned long width2 = 2000;
    
    
    
    // BILINEAR INTERPOLATION.
    // Proper explanation about the algorithm is avaiable in the documentation.
    
    unsigned char img_OP[height2][width2];
    
    //Get the scaling factors.
    float scale_h = height1/(float)height2;
    float scale_w = width1/(float)width2;
    
    
    for (int i = 0 ; i <height2 ; i++)
    {
        for (int j = 0 ; j< width2 ; j++)
        {
            
            float height_Frac_Idx = i*scale_h;
            float width_Frac_Idx = j*scale_w;
            int height_idx, width_idx;
            height_idx = (int) height_Frac_Idx;
            width_idx =  (int) width_Frac_Idx;
            float del_h = height_Frac_Idx - height_idx;
            float del_w = width_Frac_Idx - width_idx;
       
            img_OP[i][j] = ((img_IP[height_idx][width_idx])     *   (1 - del_h)   *   (1-del_w))    +
                           ((img_IP[height_idx+1][width_idx])   *   (del_h)       *   (1-del_w))    +
                           ((img_IP[height_idx][width_idx+1])   *   (1 - del_h)   *   (del_w))      +
                           ((img_IP[height_idx+1][width_idx+1]) *   (del_h)       *   (del_w));
            
        }
        
        
    }
    
    
    
    //Initialize the wand for output window.
    
    MagickWandGenesis();
    // Wand variables
    char hex[8];
    MagickWand *wand_op;
    wand_op = NewMagickWand();
    PixelIterator *iterator_op;
    PixelWand *p_wand = NULL;
    p_wand = NewPixelWand();
    //Set the initial color, it is necessary in MagicWand to have it.
    PixelSetColor(p_wand,"white");
    //Configure the output image wand.
    MagickNewImage(wand_op,width2,height2,p_wand);
    iterator_op=NewPixelIterator(wand_op);
    PixelWand **pixels_op;
    for(int i=0;i<height2;i++)
    {
        // Get the next row of the image as an array of PixelWands
        pixels_op= PixelGetNextIteratorRow(iterator_op,&width2);
        // Set the row of wands to a simple gray scale gradient
        for(int j=0;j<width2;j++)
        {
            /*formating RGB values to hex, as these are gray scale values so assgning R,G,B same values.
             It is the way MagicWand render an image.
             */
            sprintf(hex,"#%02x%02x%02x",img_OP[i][j],img_OP[i][j],img_OP[i][j]);
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


















