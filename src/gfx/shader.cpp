#include "shader.hpp"
#include "gfx.hpp"

static void _log_err(
	GLuint handle, const char *adverb, const char *path,
	void (*get_log)(GLuint, GLsizei, GLsizei*, GLchar*),
	void (*get_iv)(GLuint, GLenum, GLint*)
) {
	GLint log_len;
	get_iv(handle, GL_INFO_LOG_LENGTH, &log_len);

	char *log_text = (char*) calloc(1, (u64)log_len);
	get_log(handle, log_len, NULL, log_text);

	fprintf(stderr, "erorr %s shader at %s:\n%s\n", adverb, path, log_text);

	free(log_text);
	exit(1);
}

static GLuint _compile(const char *path, GLenum type) {
	FILE *file;
	char *text;
	u64 len;

	file = fopen(path, "r+");

	if(!file) {
		fprintf(stderr, "error loading shader at %s\n", path);
		exit(1);
	}

	fseek(file, 0, SEEK_END);
	len = (u64)ftell(file);
	ASSERT(len > 0, "shader file length should be greater than 0!");

	fseek(file, 0, SEEK_SET);
	text = (char*) calloc(1, len);
	ASSERT(text, "unable to alloc shader code into memory!");

	u64 text_len = fread(text, 1, len, file);
	ASSERT(text_len, "shader text length should be greater than 0!");

	fclose(file);

	GLuint handle = glCreateShader(type);
	glShaderSource(handle, 1, (const GLchar *const *) &text, (const GLint *) &len);
	glCompileShader(handle);
	
	GLint compiled;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &compiled);

	if(!compiled) {
		_log_err(handle, "compiling", path, glGetShaderInfoLog, glGetShaderiv);
	}

	free(text);
	return handle;
}

void Shader::init(const char *vertex, const char *fragment) {
	this->vertex = _compile(vertex, GL_VERTEX_SHADER);
	this->fragment = _compile(fragment, GL_FRAGMENT_SHADER);

	this->handle = glCreateProgram();
	glAttachShader(this->handle, this->vertex);
	glAttachShader(this->handle, this->fragment);
	glLinkProgram(this->handle);

	GLint linked;
	glGetProgramiv(this->handle, GL_LINK_STATUS, &linked);
	
	if(!linked) {
		char buff[512];
		snprintf(buff, 512, "[%s, %s]", vertex, fragment);
		_log_err(this->handle, "linking", buff, glGetProgramInfoLog, glGetProgramiv);
	}
}

void Shader::destroy() {
	glDeleteShader(this->fragment);
	glDeleteShader(this->vertex);
	glDeleteProgram(this->handle);
}

void Shader::bind() {
	glUseProgram(this->handle);
}

void Shader::uniform_f32(const char *name, f32 value) {
	GLint location = glGetUniformLocation(this->handle, name);
	glUniform1f(location, value);
}

void Shader::uniform_mat4(const char *name, glm::mat4 value) {
	GLint location = glGetUniformLocation(this->handle, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat*) &value[0][0]);
}

void Shader::uniform_i32(const char *name, i32 value) {
	GLint location = glGetUniformLocation(this->handle, name);
	glUniform1i(location, value);
}
