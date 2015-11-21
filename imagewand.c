//
//  main.c
//  testing
//
//  Created by Sourabh Kulhare on 11/19/15.
//  Copyright © 2015 Sourabh Kulhare. All rights reserved.
//


#include <stdio.h>
#include <wand/MagickWand.h>// ImageMagick's C framework for image processing.
#include <math.h>



int main(int argc, const char * argv[])
{
    MagickWandGenesis();
    
    
    // Declearation of variables
        // Wand variables
    PixelWand *pwand_ip;
    PixelIterator *iterator_ip,*iterator_op;
    PixelWand **pixels_ip;
    MagickWand * wand_ip, *wand_op;
    MagickBooleanType flag1;
    char hex[128];

    
    
    
    // Normal variables
    unsigned long height1,width1,height2,width2;
    
    
    // Initialiazation of variables.
    wand_ip = NewMagickWand();
    pwand_ip = NewPixelWand();
    
    // Read image and check its existance.
    flag1 =  MagickReadImage(wand_ip, "/Users/SK_Mac/Downloads/image2.jpg");
    
    if(flag1==MagickFalse)
    {
        printf("ERROR : - Image not found. ");
    }
    
    
    //MagickQuantizeImage(wand_ip,256,GRAYColorspace,0,MagickFalse,MagickFalse);
    
    //iterator to interate each pixel of image.

    iterator_ip=NewPixelIterator(wand_ip);
    


    
    // get the height and width
    
    width1 = MagickGetImageWidth(wand_ip);
    height1 = MagickGetImageHeight(wand_ip);
    int img_IP[height1][width1];

    for (int i=0 ; i<height1;i++)
    {
        //Get the (i+1) th row of image
        pixels_ip = PixelGetNextIteratorRow(iterator_ip, &width1);
        
        for(int j=0 ; j<width1 ; j++)
        {
            //Get the each pixel values.
            img_IP[i][j] = PixelGetRed(pixels_ip[j]);
            //gray_val[i][j] = PixelGetAlpha(pixels_ip[j]);
            //gray_val[i][j] = PixelGetOpacity(pixels_ip[j]);
            //wand_ip(GetPixelY(pixels_ip[j]);
        }
    
    }
    
    height2 = 100;
    width2 = 100;
    
    // Bilineat linterpolation. 
    
    int img_op[height2][width2];
    float scale_h = height1/(float)height2;
    float scale_w = width1/(float)width2;
 
    
    for (int i = 0 ; i <height2 ; i++)
    {
        for (int j = 0 ; j< width2 ; j++)
        {
            float height_Frac_Idx = i*scale_h;
            float width_Frac_Idx = i*scale_w;
            int height_idx, width_idx;
            height_idx = height_Frac_Idx;
            width_idx =width_Frac_Idx;
            float del_h = height_Frac_Idx - height_idx;
            float del_w = width_Frac_Idx - width_idx;
            
            img_op[i][j] = img_IP[height_idx][width_idx]*(1 - del_h)*(1-del_w) + img_IP[height_idx+1][width_idx]*(del_h)*(1-del_w) + img_IP[height_idx][width_idx+1]*(1 - del_h)*(del_w) +img_IP[height_idx+1][width_idx+1]*(del_h)*(del_w);
        }
    }
    
    
    
    
    // Initialize the wand for final image based one the given height and width.
    
    //Initialize the wand for output window.
    wand_op = NewMagickWand();
    
    PixelWand *p_wand = NULL;
    p_wand = NewPixelWand();
    
    //Set the initial color, it is necessary in MagicWand to have it.
    PixelSetColor(p_wand,"white");
    
    //Configure the output image wand.
    MagickNewImage(wand_op,height2,width2,p_wand);
    iterator_op=NewPixelIterator(wand_op);
    PixelWand **pixels_op;
    int i1,j1;
    for(i1=0;i1<height2;i1++)
    {
        // Get the next row of the image as an array of PixelWands
        pixels_op= PixelGetNextIteratorRow(iterator_op,&width2);
        // Set the row of wands to a simple gray scale gradient
        for(j1=0;j1<width2;j1++) {
            //PixelSetColorFromWand(wand_op, pixel_op[j1]);
            int temp = img_IP[i1][j1];
            sprintf(hex,"#%02x%02x%02x",temp,temp,temp);
            PixelSetColor(pixels_op[j1],hex);
        }
        // Sync writes the pixels back to the m_wand
        PixelSyncIterator(iterator_op);
    }
    MagickWriteImage(wand_op,"bits_demo.gif");
    
    
    
    


    
    
    //Display the input image.
    MagickDisplayImage(wand_op, ":0");
    
    
    //MagickReadImage(wand_op, "/Users/SK_Mac/Downloads/image1.jpg");
    
    //Display the output image.
    //MagickDisplayImage(wand_ip, ":0");
    
    for(int i= 1 ; i<10 ; i++)
    {
        for(int j = 1; j<10; j++ )
        {
            printf("%d \t", img_op[i][j]);
            
        }
    }
    
    
    wand_ip = DestroyMagickWand(wand_ip);
    MagickWandTerminus();
    return 0;
}


















