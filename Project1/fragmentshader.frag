#version 430 core

// Uniform inputs
uniform vec3 mat_ambient;
uniform vec3 mat_diffuse;
uniform vec3 mat_specular;
uniform float mat_power;
uniform sampler2D texsampler;
uniform bool use_texture_only;
uniform bool use_panorama;
uniform bool use_toon;

// Inputs from vertexshader
in VS_OUT
{
    vec3 N;
    vec3 L;
    vec3 V;
} fs_in;

in vec2 UV;
in vec3 sky_dir;

// Output color
out vec4 fragColor;

void main()
{
    if (use_toon) {
        vec3 N = normalize(fs_in.N);
        vec3 V = normalize(fs_in.V);
        float edge = dot(N, V);
        if (edge < 0.2) {
            fragColor = vec4(0.0, 0.0, 0.0, 1.0);
        } else {
            fragColor = vec4(mat_diffuse, 1.0);
        }
        return;
    }

    if (use_panorama) {
        float u = 0.5 + atan(sky_dir.z, sky_dir.x) / (2.0 * 3.14159265);
        float v = 0.5 - asin(sky_dir.y) / 3.14159265;
        fragColor = vec4(texture(texsampler, vec2(u, v)).rgb, 1.0);
        return;
    }

    if (use_texture_only) {
        fragColor = vec4(texture(texsampler, UV).rgb, 1.0);
        return;
    }

    // Normalize the incoming N, L and V vectors
    vec3 N = normalize(fs_in.N);
    vec3 L = normalize(fs_in.L);
    vec3 V = normalize(fs_in.V);

    // Calculate R locally
    vec3 R = reflect(-L, N);

    // Compute the diffuse and specular components for each fragment
    // vec3 diffuse = max(dot(N, L), 0.0) * mat_diffuse;
    vec3 texColor = texture(texsampler, UV).rgb;
    float nDotL = max(dot(N, L), 0.0);
    vec3 diffuse;
    vec3 specular;
    if (use_toon) {
        float levels = 4.0;
        float quantized = floor(nDotL * levels) / levels;
        diffuse = quantized * texColor;
        float specStep = step(0.95, max(dot(R, V), 0.0));
        specular = specStep * mat_specular;
    } else {
        diffuse = nDotL * texColor;
        specular = pow(max(dot(R, V), 0.0), mat_power) * mat_specular;
    }

    // Write final color to the framebuffer
    fragColor = vec4(mat_ambient + diffuse + specular, 1.0);
    // fragColor = vec4(mat_ambient + diffuse, 1.0);
}
