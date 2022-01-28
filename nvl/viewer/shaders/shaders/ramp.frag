uniform float value_min;
uniform float value_max;
uniform vec4 col_min;
uniform vec4 col_mid;
uniform vec4 col_max;

varying float scaled_value;

void main(void)
{
    vec4 col_unknown = vec4(0.0, 0.0, 0.0, 1.0); //Unknown

    vec4 color;

    float value_mid = (value_max - value_min) * 0.5;
    if (scaled_value >= value_min && scaled_value <= value_mid)
        color = mix(col_min, col_mid, float(scaled_value * 2.0));
    else if (scaled_value > value_mid && scaled_value <= value_max)
        color = mix(col_mid, col_max, float((scaled_value - 0.5) * 2.0));
    else
        color = col_unknown;

    gl_FragColor = color;
}
