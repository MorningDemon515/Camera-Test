#include "Shader.h"

ShaderFromSource::ShaderFromSource(const char* VSS,const char* FSS)
{
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &VSS, NULL);
    glCompileShader(VertexShader);

    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &FSS, NULL);
    glCompileShader(FragmentShader);

    ShaderProgram = glCreateProgram();

    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);
}

ShaderFromSource::~ShaderFromSource()
{
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
}

void ShaderFromSource::Use()
{
    glUseProgram(ShaderProgram);
}

ShaderFromFile::ShaderFromFile(const char* VSSFile, const char* FSSFile)
{
    std::string vsscode;
    std::ifstream vssshaderfile;
    std::stringstream vssshaderstream;

    vssshaderfile.open(VSSFile);
    vssshaderstream << vssshaderfile.rdbuf();
    vssshaderfile.close();
    vsscode = vssshaderstream.str();

    std::string fsscode;
    std::ifstream fssshaderfile;
    std::stringstream fssshaderstream;

    fssshaderfile.open(FSSFile);
    fssshaderstream << fssshaderfile.rdbuf();
    fssshaderfile.close();
    fsscode = fssshaderstream.str();

    const char* vss = vsscode.c_str();
    const char* fss = fsscode.c_str();

    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1,&vss, NULL);
    glCompileShader(VertexShader);

    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1,&fss, NULL);
    glCompileShader(FragmentShader);

    ShaderProgram = glCreateProgram();

    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);
}

ShaderFromFile::~ShaderFromFile()
{
    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
}

void ShaderFromFile::Use()
{
    glUseProgram(ShaderProgram);
}

int ShaderFromFile::GetShaderSourceUniform(const char* uniform)
{
    return glGetUniformLocation(ShaderProgram,uniform);
}

int ShaderFromSource::GetShaderSourceUniform(const char* uniform)
{
    return glGetUniformLocation(ShaderProgram, uniform);
}
