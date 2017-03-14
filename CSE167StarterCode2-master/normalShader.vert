#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 skinweights1;
layout (location = 3) in vec3 skinweights2;
layout (location = 4) in vec3 skinweights3;
layout (location = 5) in vec3 skinweights4;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat4 bindings[64];
uniform mat4 worldMatrices[64];

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. You can define as many
// extra outputs as you need.
out vec3 Normal;
out vec3 FragPos;

void main()
{
	vec3 vert = {0.0f,0.0f,0.0f};
	vec3 norm = {0.0f,0.0f,0.0f};

	vert += vec3( skinweights1.z * (worldMatrices[int(skinweights1.y)] * bindings[int(skinweights1.y)] * vec4(position, 1.0f)));
	norm += vec3( skinweights1.z * (worldMatrices[int(skinweights1.y)] * bindings[int(skinweights1.y)] * vec4(normal, 0.0f)));
	vert += vec3( skinweights2.z * (worldMatrices[int(skinweights2.y)] * bindings[int(skinweights2.y)] * vec4(position, 1.0f)));
	norm += vec3( skinweights2.z * (worldMatrices[int(skinweights2.y)] * bindings[int(skinweights2.y)] * vec4(normal, 0.0f)));
	vert += vec3( skinweights3.z * (worldMatrices[int(skinweights3.y)] * bindings[int(skinweights3.y)] * vec4(position, 1.0f)));
	norm += vec3( skinweights3.z * (worldMatrices[int(skinweights3.y)] * bindings[int(skinweights3.y)] * vec4(normal, 0.0f)));
	vert += vec3( skinweights4.z * (worldMatrices[int(skinweights4.y)] * bindings[int(skinweights4.y)] * vec4(position, 1.0f)));
	norm += vec3( skinweights4.z * (worldMatrices[int(skinweights4.y)] * bindings[int(skinweights4.y)] * vec4(normal, 0.0f)));

	norm = normalize(norm);

	gl_Position = projection * view * model * vec4(vert, 1.0f);
	FragPos = vec3(view  * model * vec4(vert, 1.0f));
	Normal = vec3(view * model * vec4(norm, 0.0f));
}
