#include "Shaders.h"
#include <string>

static const std::string POSCOLOR_VERT = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 vertexColor;

uniform mat4 viewMat;

void main() {
    gl_Position = viewMat * vec4(aPos, 1.0);
    vertexColor = aColor;
}
)";

static const std::string POSCOLOR_FRAG = R"(#version 330 core

out vec4 FragColor;
in vec4 vertexColor;

void main() {
    FragColor = vertexColor;
}
)";

static const std::string TEXT_VERT = R"(#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
)";

static const std::string TEXT_FRAG = R"(#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0,
                        texture(text, TexCoords).r);
    color = vec4(textColor, 1.0) * sampled;
}
)";


void Shaders::initShaders() {
    posColor = std::make_unique<Shader>(POSCOLOR_VERT, POSCOLOR_FRAG);
    text = std::make_unique<Shader>(TEXT_VERT, TEXT_FRAG);
}