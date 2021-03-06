/*********
*
*Author: Sanqian Zhao
**********/

__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_NEAREST;

__kernel void image_filter( __read_only image2d_t input, __write_only image2d_t output,__global float* filter) {

      int width = get_global_size(0);
      int height = get_global_size(1);
      int2 coord = (int2)(get_global_id(0),get_global_id(1));

      int2 leftupCord = (int2)(coord.x-1,coord.y-1);
      int2 upCord = (int2)(coord.x,coord.y);
      int2 rightupCord = (int2)(coord.x+1,coord.y-1);
      int2 leftCord = (int2)(coord.x-1,coord.y);
      int2 rightCord = (int2)(coord.x+1,coord.y);
      int2 leftDowCord= (int2)(coord.x-1,coord.y+1);
      int2 downCord = (int2)(coord.x,coord.y+1);
      int2 rightDownCord = (int2)(coord.x+1,coord.y+1);

      //BOUNARY
      //Treat column[0]=column1,clumn[width+1]=column[width] etc
      if(coord.x==1)
      {
        leftupCord.x=1;
        leftCord.x=1;
        leftDowCord.x=1;
      }
      if(coord.x==width)
      {
        rightCord.x=width;
        rightDownCord.x=width;
        rightupCord.x=width;
      }
      if(coord.y==1)
      {
        leftupCord.y=1;
        upCord.y=1;
        rightupCord.y=1;
      }
      if(coord.y==height)
      {
        leftDowCord.y=height;
        downCord.y=height;
        rightDownCord.y=height;
      }
      uint4 pixel = read_imageui(input,sampler,coord);
      uint4 pleftup = read_imageui(input,sampler,leftupCord);
      uint4 pup = read_imageui(input,sampler,upCord);
      uint4 prightup = read_imageui(input,sampler,rightupCord);
      uint4 pleft = read_imageui(input,sampler,leftCord);
      uint4 pright = read_imageui(input,sampler,rightCord);
      uint4 pleftdown = read_imageui(input,sampler,leftDowCord);
      uint4 pdown = read_imageui(input,sampler,downCord);
      uint4 prightdown = read_imageui(input,sampler,rightDownCord);


            if(get_image_channel_data_type(input) == CLK_UNORM_INT8)
            {
              uint4 pixel = convert_uint4(read_imagef(input,sampler,coord)*255);
              uint4 pleftup = convert_uint4(read_imagef(input,sampler,leftupCord)*255);
              uint4 pup = convert_uint4(read_imagef(input,sampler,upCord)*255);
              uint4 prightup = convert_uint4(read_imagef(input,sampler,rightupCord)*255);
              uint4 pleft = convert_uint4(read_imagef(input,sampler,leftCord)*255);
              uint4 pright = convert_uint4(read_imagef(input,sampler,rightCord)*255);
              uint4 pleftdown = convert_uint4(read_imagef(input,sampler,leftDowCord)*255);
              uint4 pdown = convert_uint4(read_imagef(input,sampler,downCord)*255);
              uint4 prightdown = convert_uint4(read_imagef(input,sampler,rightDownCord)*255);

            }


       uint4 resultpixelf =convert_uint4((convert_float4(pleftup)*filter[0]+convert_float4(pup)*filter[1]+convert_float4(prightup)*filter[2]+
                           convert_float4(pleft)*filter[3]+convert_float4(pixel)*filter[4]+convert_float4(pright)*filter[5]+
                           convert_float4(pleftdown)*filter[6]+convert_float4(pdown)*filter[7]+convert_float4(prightdown)*filter[8]));

      write_imageui(output,coord,resultpixelf);
    }
