#include "ofxShader.h"

ofxShader::ofxShader() :
	bLoaded(false) {
}

ofxShader::~ofxShader() {
	unload();
}

void ofxShader::setup(string shaderName) {
	setup(shaderName, shaderName);
}

void ofxShader::setup(string vertexName, string fragmentName) {
	vertexName += ".vert";
	fragmentName += ".frag";
	setupInline(loadTextFile(vertexName), loadTextFile(fragmentName));
}

void ofxShader::compileShader(GLuint shader, string source, string type) {
	const char* sptr = source.c_str();
	int ssize = source.size();
	glShaderSource(shader, 1, &sptr, &ssize);
	glCompileShader(shader);
}

bool ofxShader::checkShaderCompileStatus(GLuint shader, string type) {
	GLint status = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(status == GL_TRUE)
		ofLog(OF_LOG_VERBOSE, type + " shader compiled.");
	else if (status == GL_FALSE) {
		ofLog(OF_LOG_ERROR, type + " shader failed to compile.");
		checkShaderInfoLog(shader, type);
		return false;
	}
	return true;
}

bool ofxShader::checkShaderLinkStatus(GLuint shader, string type) {
	GLint status;
	glGetProgramiv(shader, GL_LINK_STATUS, &status);
	if(status == GL_TRUE)
		ofLog(OF_LOG_VERBOSE, type + " shader linked.");
	else if (status == GL_FALSE) {
		ofLog(OF_LOG_ERROR, type + " shader failed to link.");
		checkShaderInfoLog(shader, type);
		return false;
	}
	return true;
}

void ofxShader::checkShaderInfoLog(GLuint shader, string type) {
	GLsizei infoLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
	if (infoLength > 1) {
		GLchar* infoBuffer = new GLchar[infoLength];
		glGetShaderInfoLog(shader, infoLength, &infoLength, infoBuffer);
		ofLog(OF_LOG_ERROR, type + " shader reports:\n" + infoBuffer);
		delete [] infoBuffer;
	}
}

void ofxShader::checkProgramInfoLog(GLuint program) {
	GLsizei infoLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
	if (infoLength > 1) {
		GLchar* infoBuffer = new GLchar[infoLength];
		glGetProgramInfoLog(program, infoLength, &infoLength, infoBuffer);
		string msg = "Shader program reports:\n";
		ofLog(OF_LOG_ERROR, msg + infoBuffer);
		delete [] infoBuffer;
	}
}

void ofxShader::setupInline(string vertexShaderSource, string fragmentShaderSource) {
	unload();
	if (GLEE_ARB_shader_objects) {
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		compileShader(vertexShader, vertexShaderSource, "Vertex");
		bool vertexCompiled = checkShaderCompileStatus(vertexShader, "Vertex");

		compileShader(fragmentShader, fragmentShaderSource, "Fragment");
		bool fragmentCompiled = checkShaderCompileStatus(fragmentShader, "Fragment");

		if(vertexCompiled && fragmentCompiled) {
			program = glCreateProgram();
			glAttachShader(program, vertexShader);
			glAttachShader(program, fragmentShader);
			glLinkProgram(program);

			checkShaderLinkStatus(vertexShader, "Vertex");
			checkShaderLinkStatus(fragmentShader, "Fragment");
			checkProgramInfoLog(program);

			bLoaded = true;
		}
	} else {
		cout << "Sorry, it looks like you can't run 'ARB_shader_objects'." << endl;
		cout << "Please check the capabilites of your graphics card." << endl;
		cout << "http://www.ozone3d.net/gpu_caps_viewer/" << endl;
	}
}

void ofxShader::unload() {
	if(bLoaded) {
		if (vertexShader) {
			glDetachShader(program, vertexShader);
			glDeleteShader(vertexShader);
			vertexShader = 0;
		}
		if (fragmentShader) {
			glDetachShader(program, fragmentShader);
			glDeleteShader(fragmentShader);
			fragmentShader = 0;
		}
		if (program) {
			glDeleteProgram(program);
			program = 0;
		}
	}
	bLoaded = false;
}

void ofxShader::begin() {
	if (bLoaded == true)
		glUseProgram(program);
}

void ofxShader::end() {
	if (bLoaded == true)
		glUseProgram(0);
}

void ofxShader::setTexture(const char* name, ofBaseHasTexture& img, int textureLocation) {
	setTexture(name, img.getTextureReference(), textureLocation);
}

void ofxShader::setTexture(const char* name, ofTexture& tex, int textureLocation) {
	if(bLoaded) {
		ofTextureData texData = tex.getTextureData();
		glActiveTexture(GL_TEXTURE0 + textureLocation);
		glEnable(texData.textureTarget);
		glBindTexture(texData.textureTarget, texData.textureID);
		glDisable(texData.textureTarget);
		setUniform(name, textureLocation);
		glActiveTexture(GL_TEXTURE0);
	}
}

void ofxShader::setUniform(const char* name, int v1) {
	if(bLoaded)
		glUniform1i(getLoc(name), v1);
}

void ofxShader::setUniform(const char* name, int v1, int v2) {
	if(bLoaded)
		glUniform2i(getLoc(name), v1, v2);
}

void ofxShader::setUniform(const char* name, int v1, int v2, int v3) {
	if(bLoaded)
		glUniform3i(getLoc(name), v1, v2, v3);
}

void ofxShader::setUniform(const char* name, int v1, int v2, int v3, int v4) {
	if(bLoaded)
		glUniform4i(getLoc(name), v1, v2, v3, v4);
}

void ofxShader::setUniform(const char* name, float v1) {
	if(bLoaded)
		glUniform1f(getLoc(name), v1);
}

void ofxShader::setUniform(const char* name, float v1, float v2) {
	if(bLoaded)
		glUniform2f(getLoc(name), v1, v2);
}

void ofxShader::setUniform(const char* name, float v1, float v2, float v3) {
	if(bLoaded)
		glUniform3f(getLoc(name), v1, v2, v3);
}

void ofxShader::setUniform(const char* name, float v1, float v2, float v3, float v4) {
	if(bLoaded)
		glUniform4f(getLoc(name), v1, v2, v3, v4);
}

void ofxShader::setUniform1v(const char* name, int* v, int count) {
	if(bLoaded)
		glUniform1iv(getLoc(name), count, v);
}

void ofxShader::setUniform2v(const char* name, int* v, int count) {
	if(bLoaded)
		glUniform2iv(getLoc(name), count, v);
}

void ofxShader::setUniform3v(const char* name, int* v, int count) {
	if(bLoaded)
		glUniform3iv(getLoc(name), count, v);
}

void ofxShader::setUniform4v(const char* name, int* v, int count) {
	if(bLoaded)
		glUniform4iv(getLoc(name), count, v);
}

void ofxShader::setUniform1v(const char* name, float* v, int count) {
	if(bLoaded)
		glUniform1fv(getLoc(name), count, v);
}

void ofxShader::setUniform2v(const char* name, float* v, int count) {
	if(bLoaded)
		glUniform2fv(getLoc(name), count, v);
}

void ofxShader::setUniform3v(const char* name, float* v, int count) {
	if(bLoaded)
		glUniform3fv(getLoc(name), count, v);
}

void ofxShader::setUniform4v(const char* name, float* v, int count) {
	if(bLoaded)
		glUniform4fv(getLoc(name), count, v);
}

void ofxShader::setAttribute(const char* name, short v1) {
	if(bLoaded)
		glVertexAttrib1s(getLoc(name), v1);
}

void ofxShader::setAttribute(const char* name, short v1, short v2) {
	if(bLoaded)
		glVertexAttrib2s(getLoc(name), v1, v2);
}

void ofxShader::setAttribute(const char* name, short v1, short v2, short v3) {
	if(bLoaded)
		glVertexAttrib3s(getLoc(name), v1, v2, v3);
}

void ofxShader::setAttribute(const char* name, short v1, short v2, short v3, short v4) {
	if(bLoaded)
		glVertexAttrib4s(getLoc(name), v1, v2, v3, v4);
}

void ofxShader::setAttribute(const char* name, float v1) {
	if(bLoaded)
		glVertexAttrib1f(getLoc(name), v1);
}

void ofxShader::setAttribute(const char* name, float v1, float v2) {
	if(bLoaded)
		glVertexAttrib2f(getLoc(name), v1, v2);
}

void ofxShader::setAttribute(const char* name, float v1, float v2, float v3) {
	if(bLoaded)
		glVertexAttrib3f(getLoc(name), v1, v2, v3);
}

void ofxShader::setAttribute(const char* name, float v1, float v2, float v3, float v4) {
	if(bLoaded)
		glVertexAttrib4f(getLoc(name), v1, v2, v3, v4);
}

void ofxShader::setAttribute(const char* name, double v1) {
	if(bLoaded)
		glVertexAttrib1d(getLoc(name), v1);
}

void ofxShader::setAttribute(const char* name, double v1, double v2) {
	if(bLoaded)
		glVertexAttrib2d(getLoc(name), v1, v2);
}

void ofxShader::setAttribute(const char* name, double v1, double v2, double v3) {
	if(bLoaded)
		glVertexAttrib3d(getLoc(name), v1, v2, v3);
}

void ofxShader::setAttribute(const char* name, double v1, double v2, double v3, double v4) {
	if(bLoaded)
		glVertexAttrib4d(getLoc(name), v1, v2, v3, v4);
}

void ofxShader::setAttributeLocation(const char* name, int index) {
	glBindAttribLocation(program, index, name);
}

int ofxShader::getAttributeLocation(const char* name) {
	return glGetAttribLocation(program, name);
}

inline GLint ofxShader::getLoc(const char* name) {
	return glGetUniformLocation(program, name);
}

void ofxShader::printActiveUniforms() {
	GLint numUniforms = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);
	cout << numUniforms << " uniforms:" << endl;

	GLint uniformMaxLength = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformMaxLength);

	GLint count = -1;
	GLenum type = 0;
	GLchar* uniformName = new GLchar[uniformMaxLength];
	for(GLint i = 0; i < numUniforms; i++) {
		GLsizei length;
		glGetActiveUniform(program, i, uniformMaxLength, &length, &count, &type, uniformName);
		cout << " [" << i << "] ";
		for(int j = 0; j < length; j++)
			cout << uniformName[j];
		cout << " @ index " << glGetUniformLocation(program, uniformName) << endl;
	}
	delete [] uniformName;
}

void ofxShader::printActiveAttributes() {
	GLint numAttributes = 0;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttributes);
	cout << numAttributes << " attributes:" << endl;

	GLint attributeMaxLength = 0;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attributeMaxLength);

	GLint count = -1;
	GLenum type = 0;
	GLchar* attributeName = new GLchar[attributeMaxLength];
	for(GLint i = 0; i < numAttributes; i++) {
		GLsizei length;
		glGetActiveAttrib(program, i, attributeMaxLength, &length, &count, &type, attributeName);
		cout << " [" << i << "] ";
		for(int j = 0; j < length; j++)
			cout <<attributeName[j];
		cout << " @ index " << glGetAttribLocation(program, attributeName) << endl;
	}
	delete [] attributeName;
}

string ofxShader::loadTextFile(string filename) {
	ifstream file;
	file.open(ofToDataPath(filename).c_str());
	if(file.is_open()) {
		string text;
		while(!file.eof()) {
			string line;
			getline(file, line);
			text += line + '\n';
		}
		file.close();
		stringstream msg;
		msg << "Loaded " << filename << ", " << text.size() << " characters.";
		ofLog(OF_LOG_VERBOSE, msg.str());
		return text;
	} else {
		ofLog(OF_LOG_ERROR, "Could not open " + filename);
	}
}
