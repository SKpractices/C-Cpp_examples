//
//  main.c
//  testing
//
//  Created by Sourabh Kulhare on 11/19/15.
//  Copyright © 2015 Sourabh Kulhare. All rights reserved.
//


#include <stdio.h>
#include <wand/MagickWand.h>  // ImageMagick's C framework for image processing.




int main(int argc, const char * argv[])
{
    MagickWandGenesis();
    
    
    // Declearation of variables
        // Wand variables
    PixelWand *pwand_ip, *pwand_op;
    PixelIterator *iterator_ip,*iterator_op;
    PixelWand **pixels_ip,**pixels_op;
    MagickWand * wand_ip, *wand_op;
    MagickBooleanType flag1;

    
    
        // Normal variables
    int x,y,gray;
    unsigned long height1,width1,height2,width2;
    register int pixel_val;
    
    char hex[128];
    
    // Initialiazation of variables.
    wand_ip = NewMagickWand();
    
    
    // Initialize the wand for final image based one the given height and width.
    wand_op = NewMagickWand();
    MagickSetSize(wand_op,width2,height2);
    
    
    
    pwand_ip = NewPixelWand();
    
    
    // Read image and check its existance.
    flag1 =  MagickReadImage(wand_ip, "/Users/SK_Mac/Downloads/image2.jpg");
    
    //iterator to interate each pixel of image.

    iterator_ip=NewPixelIterator(wand_ip);
    iterator_op=NewPixelIterator(wand_op);

    
    if(flag1==MagickFalse)
    {
        printf("ERROR : - Image not found. ");
    }
    
    
    // get the height and width
    
    width1 = MagickGetImageWidth(wand_ip);
    height1 = MagickGetImageHeight(wand_ip);
    double gray_val[height1][width1];

    for (int i=0 ; i<height1;i++)
    {
        //Get the (i+1) th row of image
        pixels_ip = PixelGetNextIteratorRow(iterator_ip, &width1);
        
        for(int j=0 ; j<width1 ; j++)
        {
            //Get the each pixel values.
            //gray_val[i][j] = PixelGetBlack(pixels_ip[j]);
            //gray_val[i][j] = PixelGetAlpha(pixels_ip[j]);
            gray_val[i][j] = PixelGetBlue(pixels_ip[j]);
        
        }
    
    }
    

    
    //Display the input image.
    MagickDisplayImage(wand_ip, ":0");
    
    
    //MagickReadImage(wand_op, "/Users/SK_Mac/Downloads/image1.jpg");
    
    //Display the output image.
    //MagickDisplayImage(wand_ip, ":0");
    
    for(int i=30 ; i<40 ; i++)
    {
        for(int j =10 ; j<20; j++ )
        {
            printf("%f \t", gray_val[i][j]);
            
        }
    }
    
    
    wand_ip = DestroyMagickWand(wand_ip);
    MagickWandTerminus();
    return 0;
}


















