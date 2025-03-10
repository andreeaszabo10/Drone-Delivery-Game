#version 330
in vec3 frag_position;
in float noise_value;
in float altitude_factor;

layout(location = 0) out vec4 out_color;

void main() {
    vec3 mud1 = vec3(0.34f, 0.16f, 0.0f);
    vec3 between1 = vec3(0.25f, 0.4f, 0.1f);
    vec3 grass1 = vec3(0.2f, 0.6f, 0.2f);
    vec3 mud2 = vec3(0.5f, 0.4f, 0.3f);
    vec3 between2 = vec3(0.4f, 0.5f, 0.4f);
    vec3 grass2 = vec3(0.4f, 0.6f, 0.4f);
    vec3 mud = mix(mud1, mud2, altitude_factor);
    vec3 between = mix(between1, between2, altitude_factor);
    vec3 grass = mix(grass1, grass2, altitude_factor);
    
    vec3 final_color;
    if (noise_value < 0.3) {
        final_color = mix(mud, between, noise_value / 0.3);
    } else if (noise_value < 0.7) {
        final_color = mix(between, grass, (noise_value - 0.3) / 0.4);
    } else {
        final_color = grass;
    }
    
    out_color = vec4(final_color, 1.0);
}