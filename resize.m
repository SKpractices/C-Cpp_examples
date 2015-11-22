%testing image resize by my own 

img_ip = imread('img.jpg');
dim = size(img_ip);
img_ip = img_ip(:,:,1);
h1 = dim(1);
w1 = dim(2);

h2 = 100;
w2 = 200; 

img_op = rand(h2,w2);
scale_h = h1/h2;
scale_w = w1/w2;

for i = 1:h2
    for j = 1:w2
        height_Fr = i*scale_h;
        width_Fr = j*scale_w;
        height_idx = floor(height_Fr)-1;
        width_idx = floor(width_Fr)-1;
        del_h = height_Fr-height_idx;
        del_w = width_Fr-width_idx;
        if(height_idx<=0 || width_idx<=0)
                height_idx=1;
                width_idx=1;
        end
        img_op(i,j) = img_ip(height_idx,width_idx)*(1-del_h)*(1-del_w) + img_ip(height_idx+1,width_idx)*(del_h)*(1-del_w)+img_ip(height_idx,width_idx+1)*(1-del_h)*(del_w)+img_ip(height_idx+1,width_idx+1)*(del_h)*(del_w);
        
    end
end
%imshow(img_ip)
img_op=(img_op-min(img_op(:)))/(max(img_op(:))-min(img_op(:)));

imshow(img_op)
           
              
