#version 330
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 pos;

out vec3 frag_position;
out float noise_value;
out float altitude_factor;

float hash(vec2 p) {
    return fract(sin(dot(p, vec2(127.1f, 311.7f))) * 43758.5453123f);
}

float noise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    
    f = f * f * (3.0f - 2.0f * f);
    
    float a = hash(i);
    float b = hash(i + vec2(1.0f, 0.0f));
    float c = hash(i + vec2(0.0f, 1.0f));
    float d = hash(i + vec2(1.0f, 1.0f));
    
    return mix(
        mix(a, b, f.x),
        mix(c, d, f.x),
        f.y
    );
}

void main() {
    vec2 noiseInput = position.xz * 0.2f;
    noise_value = noise(noiseInput);
    
    vec3 modified_position = position;
    modified_position.y = noise_value * 1.5f;
    
    float max_altitude = 20.0f;
    altitude_factor = clamp(pos.y / max_altitude, 0.0f, 1.0f);
    
    gl_Position = projection * view * model * vec4(modified_position, 1.0f);
    frag_position = modified_position;
}