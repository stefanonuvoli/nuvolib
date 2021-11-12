uniform float stripe_alpha;
uniform float stripe_width;
varying float scaled_value;
uniform float value_min;

void main(void)
{
    float weight = fract(scaled_value) / stripe_width;

    vec3 colA = vec3(1.0, 0.25, 0.25); //Red
    vec3 colB = vec3(0.25, 1.0, 0.25); //Green

    vec3 final_col = mix(colA, colB, weight);

    vec4 color = vec4(final_col, stripe_alpha);

    gl_FragColor = color;
}
