#ifndef VAO_H
#define VAO_H

#include "vbo.h"

typedef struct VAO {
	GLuint	handle;
} VAO;

VAO create_vao();
void destroy_vao(VAO *self);
void vao_bind(const VAO *self);
void vao_attr(
	const VAO *self,
	const VBO *vbo,
	GLuint index,
	GLint size,
	GLenum type,
	GLsizei stride,
	size_t offset
);
void vao_disable_attr(const VAO *self, const GLuint *ids, size_t id_count);

#endif // VAO_H
