uniform float stripe_width;
uniform float value_min;
uniform vec4 col_min;
uniform vec4 col_max;

varying float scaled_value;

void main(void)
{
    float weight = fract(scaled_value) / stripe_width;

    vec4 color = mix(col_min, col_max, weight);

    gl_FragColor = color;
}
