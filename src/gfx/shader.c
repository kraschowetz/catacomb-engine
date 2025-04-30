#include "shader.h"

static void _log_err(
	GLuint handle, const char *adverb, const char *path,
	void (*get_log)(GLuint, GLsizei, GLsizei*, GLchar*),
	void (*get_iv)(GLuint, GLenum, GLint*)
) {
	GLint log_len;
	get_iv(handle, GL_INFO_LOG_LENGTH, &log_len);

	char *log_text = calloc(1, (u64)log_len);
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
	assert(len > 0);

	fseek(file, 0, SEEK_SET);
	text = calloc(1, len);
	assert(text);

	u64 text_len = fread(text, 1, len, file);
	assert(text_len);

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

 Shader create_shader(const char *vertex, const char *fragment) {
	Shader self;
	self.vertex_shader = _compile(vertex, GL_VERTEX_SHADER);
	self.fragment_shader = _compile(fragment, GL_FRAGMENT_SHADER);

	self.handle = glCreateProgram();
	glAttachShader(self.handle, self.fragment_shader);
	glAttachShader(self.handle, self.vertex_shader);
	glLinkProgram(self.handle);

	GLint linked;
	glGetProgramiv(self.handle, GL_LINK_STATUS, &linked);

	if(!linked) {
		char buf[512];
		snprintf(buf, 512, "[%s, %s]", vertex, fragment);
		_log_err(self.handle, "linking", buf, glGetProgramInfoLog, glGetProgramiv);
	}

	return self;
};

void shader_bind(const Shader *self) {
	glUseProgram(self->handle);
}

void destroy_shader(Shader *self) {
	glDeleteProgram(self->handle);
	glDeleteShader(self->fragment_shader);
	glDeleteShader(self->vertex_shader);
}

void shader_uniform_float(const Shader *self, const char *name, f32 val) {
	GLint location = glGetUniformLocation(self->handle, name);
	glUniform1f(location, val);
}

void shader_uniform_mat4(const Shader *self, const char *name, mat4 m) {
	GLint location = glGetUniformLocation(self->handle, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat*) &m[0][0]);
}

void shader_uniform_mat4s(const Shader *self, const char *name, mat4s m) {
	GLint location = glGetUniformLocation(self->handle, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat*) &m.raw);
}

void shader_uniform_int(const Shader *self, const char *name, i32 val) {
	GLint location = glGetUniformLocation(self->handle, name);
	glUniform1i(location, val);
}
