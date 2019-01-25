#include "shaders.h"

bool CShader::LoadShader(const std::string &shFile, GLenum a_iType) 
{
	std::string sFile = "data\\shaders\\" + shFile;
    std::ifstream fp(sFile, std::ios::in);
    std::string sCode;

    if (fp.is_open()) {
        std::stringstream sstr;
        sstr << fp.rdbuf();
        sCode = sstr.str();
        fp.close();
    }
    else {
        std::cerr << "Unable to open file: " << sFile << std::endl;
        return false;
    }

	shaderID = glCreateShader(a_iType);

    const char* sCodePtr = sCode.c_str();
	glShaderSource(shaderID, 1, &sCodePtr, nullptr);
	glCompileShader(shaderID);

	int iCompilationStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &iCompilationStatus);

	if (iCompilationStatus == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(static_cast<unsigned long>(maxLength - 1));
        glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

        std::stringstream ss;
        for(GLchar i: errorLog) ss << i;
        std::cerr << ss.str() << std::endl;

        std::string s = ss.str();
        glDeleteShader(shaderID);

        return false;
    }
	type = a_iType;
	loaded = true;

	return true;
}

CShader::CShader()
{
	loaded = false;
}

CShader::CShader(const std::string &shFile, GLenum a_iType)
{
	LoadShader(shFile, a_iType);
}

CShader::~CShader()
{
	DeleteShader();
}

bool CShader::isLoaded()
{
	return loaded;
}

GLuint CShader::GetShaderID()
{
	return shaderID;
}

void CShader::DeleteShader()
{
	if (!isLoaded())return;
	loaded = false;
	glDeleteShader(shaderID);
}

CShaderProgram::CShaderProgram()
{
	linked = false;
}

CShaderProgram::CShaderProgram(CShader* v, CShader* f)
{
	linked = false;
	CreateProgram();
	AddShaderToProgram(v);
	AddShaderToProgram(f);
	LinkProgram();
}

CShaderProgram::~CShaderProgram()
{
	DeleteProgram();
}

void CShaderProgram::CreateProgram()
{
	programID = glCreateProgram();
}

bool CShaderProgram::AddShaderToProgram(CShader *shShader)
{
	if (!shShader->isLoaded())return false;

	glAttachShader(programID, shShader->GetShaderID());

	return true;
}

bool CShaderProgram::LinkProgram()
{
	glLinkProgram(programID);
	int iLinkStatus;
	glGetProgramiv(programID, GL_LINK_STATUS, &iLinkStatus);
	linked = iLinkStatus == GL_TRUE;
	return linked;
}

void CShaderProgram::DeleteProgram()
{
	if (!linked)return;
	linked = false;
	glDeleteProgram(programID);
}

void CShaderProgram::UseProgram()
{
	if (linked)glUseProgram(programID);
}

GLuint CShaderProgram::GetProgramID()
{
	return programID;
}

void CShaderProgram::SetVec3(float* begin, const char* var)
{
	GLint VectorID = glGetUniformLocation(programID, var);
	glUniform3fv(VectorID, 1, begin);
}

void CShaderProgram::SetMat4(float* begin, const char* var)
{
	GLint MatrixID = glGetUniformLocation(programID, var);
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, begin);
}

void CShaderProgram::SetInt(int begin, const char* var)
{
	GLint IntID = glGetUniformLocation(programID, var);
	glUniform1i(IntID, begin);
}

void CShaderProgram::SetFloat(float begin, const char* var)
{
	GLint FloatID = glGetUniformLocation(programID, var);
	glUniform1f(FloatID, begin);
}