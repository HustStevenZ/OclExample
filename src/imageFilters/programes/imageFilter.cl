/*********
*
*Author: Sanqian Zhao
**********/

__kernel void image_filter(image2d_t output) {

  // int id_x=get_global_id(1);
  // int id_y=get_global_id(0);
  int2 coord = (int2)(get_global_id(0),
get_global_id(1));
    int2 coord = (int2)(get_global_id(0),get_global_id(1));
    int4 pixel=(int4)(255,0,255,0);
    write_imagei(output,coord,pixel);
}
