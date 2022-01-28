uniform float stripe_alpha;
uniform float stripe_width;
uniform float value_min;
uniform float col_min;
uniform float col_max;

varying float scaled_value;

void main(void)
{
    float weight = fract(scaled_value) / stripe_width;

    vec3 final_col = mix(col_min, col_max, weight);

    vec4 color = vec4(final_col, stripe_alpha);

    gl_FragColor = color;
}
