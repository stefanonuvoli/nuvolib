uniform float value_min;
uniform float value_max;

attribute float value;

varying float scaled_value;

void main(void)
{
   gl_Position = ftransform();

   if (value > value_max)
       scaled_value = value_max;
   else if (value < value_min)
       scaled_value = value_min;
   else
       scaled_value = ((value - value_min)/(value_max - value_min));
}
