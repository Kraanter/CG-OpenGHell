#version 430 core

// Uniform inputs
uniform vec3 mat_ambient;
uniform vec3 mat_diffuse;
uniform vec3 mat_specular;
uniform float mat_power;
uniform sampler2D texsampler;

// Inputs from vertexshader
in VS_OUT
{
    vec3 N;
    vec3 L;
    vec3 V;
} fs_in;

in vec2 UV;

// Output color
out vec4 fragColor;

void main()
{
    // Normalize the incoming N, L and V vectors
    vec3 N = normalize(fs_in.N);
    vec3 L = normalize(fs_in.L);
    vec3 V = normalize(fs_in.V);

    // Calculate R locally
    vec3 R = reflect(-L, N);

    // Compute the diffuse and specular components for each fragment
    // vec3 diffuse = max(dot(N, L), 0.0) * mat_diffuse;
    vec3 diffuse = max(dot(N, L), 0.0) * texture(texsampler, UV).rgb;
    vec3 specular = pow(max(dot(R, V), 0.0), mat_power) * mat_specular;

    // Write final color to the framebuffer
    fragColor = vec4(mat_ambient + diffuse + specular, 1.0);
    // fragColor = vec4(mat_ambient + diffuse, 1.0);
}
