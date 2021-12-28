uniform float ramp_alpha;
varying float scaled_value;

void main(void)
{
    float weight = scaled_value;

    const vec3 colA = vec3(1.0, 0.25, 0.25); //Red
    const vec3 colB = vec3(0.25, 1.0, 0.25); //Green
    const vec3 colC = vec3(0.25, 0.25, 1.0); //Blue
    const vec3 colU = vec3(0.7, 0.7, 0.7); //Unknown

    vec3 final_col;

    if (weight >= 0.0 && weight <= 0.5)
        final_col = mix(colA, colB, float(weight*2.0));
    else if (weight > 0.5 && weight <= 1.0)
        final_col = mix(colB, colC, float((weight-0.5)*2.0));
    else
        final_col = colU;

    vec4 color = vec4(final_col, ramp_alpha);
    gl_FragColor = color;
}
