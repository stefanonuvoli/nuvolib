uniform float value_min;
uniform float value_max;
uniform float stripe_num;

attribute float vertex_value;

varying float scaled_value;

void main(void)
{
   gl_Position = ftransform();

   if (vertex_value > value_max)
       scaled_value = value_max;
   else if (vertex_value < value_min)
       scaled_value = value_min;
   else
       scaled_value = ((vertex_value - value_min)/(value_max - value_min)) * (stripe_num + 1);
}
