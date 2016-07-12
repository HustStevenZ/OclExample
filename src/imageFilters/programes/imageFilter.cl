/*********
*
*Author: Sanqian Zhao
**********/

__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;

__kernel void image_filter( __read_only image2d_t input, __write_only image2d_t output,__global float* filter) {

      int width = get_global_size(0);
      int height = get_global_size(1);
      int2 coord = (int2)(get_global_id(0),get_global_id(1));
    //  int mody = (int)fmod((float)(coord.y-1+height-1),(float)height);
      int2 leftupCord = (int2)((int)fmod((float)(coord.x-1+width-1),(float)(width)),
                                (int)fmod((float)(coord.y-1+height-1),(float)height));
      int2 upCord = (int2)(coord.x,
                          (int)fmod((float)(coord.y-1+height-1),(float)height));
      int2 rightupCord = (int2)((int)fmod((float)(coord.x+1+width-1),(float)(width)),
                                (int)fmod((float)(coord.y-1+height-1),(float)height));
      int2 leftCord = (int2)((int)fmod((float)(coord.x-1+width-1),(float)(width)),
                                        coord.y);
      int2 rightCord = (int2)((int)fmod((float)(coord.x+1+width-1),(float)width),
                            coord.y);
      int2 leftDowCord= (int2)((int)fmod((float)(coord.x-1+width-1),(float)width),
                               (int)fmod((float)(coord.y+1+height-1),(float)height));
      int2 downCord = (int2)(coord.x,
                             (int)fmod((float)(coord.y+1+height-1),(float)height));
      int2 rightDownCord = (int2)((int)fmod((float)(coord.x+1+width-1),(float)width),
                                  (int)fmod((float)(coord.y+1+height-1),(float)height));


      uint4 pixel = read_imageui(input,sampler,coord);
      uint4 pleftup = read_imageui(input,sampler,leftupCord);
      uint4 pup = read_imageui(input,sampler,upCord);
      uint4 prightup = read_imageui(input,sampler,rightupCord);
      uint4 pleft = read_imageui(input,sampler,leftCord);
      uint4 pright = read_imageui(input,sampler,rightCord);
      uint4 pleftdown = read_imageui(input,sampler,leftDowCord);
      uint4 pdown = read_imageui(input,sampler,downCord);
      uint4 prightdown = read_imageui(input,sampler,rightDownCord);
      // uint4 resultpixel = (uint4)(((float4)pleftup*filter[0]+(float4)pup*filter[1]+(float4)prightup*filter[2]+
      //                     (float4)pleft*filter[3]+(float4)pixel*filter[4]+(float4)pright*filter[5]+
      //                     (float4)pleftdown*filter[6]+(float4)pdown*filter[7]+(float4)prightdown*filter[8])/9.0f);
       uint4 resultpixelf =convert_uint4((convert_float4(pleftup)*filter[0]+convert_float4(pup)*filter[1]+convert_float4(prightup)*filter[2]+
                           convert_float4(pleft)*filter[3]+convert_float4(pixel)*filter[4]+convert_float4(pright)*filter[5]+
                           convert_float4(pleftdown)*filter[6]+convert_float4(pdown)*filter[7]+convert_float4(prightdown)*filter[8])/9.0f);

      write_imageui(output,coord,resultpixelf);
    }
