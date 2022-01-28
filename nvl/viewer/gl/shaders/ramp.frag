uniform float ramp_alpha;
uniform float col_min;
uniform float col_mid;
uniform float col_max;

varying float scaled_value;

void main(void)
{
    float weight = scaled_value;

    const vec3 col_unknown = vec3(0.0, 0.0, 0.0); //Unknown

    vec3 final_col;

    if (weight >= 0.0 && weight <= 0.5)
        final_col = mix(col_min, col_mid, float(weight*2.0));
    else if (weight > 0.5 && weight <= 1.0)
        final_col = mix(col_mid, col_max, float((weight-0.5)*2.0));
    else
        final_col = col_unknown;

    vec4 color = vec4(final_col, ramp_alpha);
    gl_FragColor = color;
}
