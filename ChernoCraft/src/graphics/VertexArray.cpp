#include "VertexArray.h"

VertexArray::VertexArray(GLfloat *vertices, GLuint *indices, GLsizei vcount, GLsizei icount)
{
	this->count = icount;
	vao = compile(vertices, indices, vcount, icount);
}

VertexArray::VertexArray(GLfloat *vertices, GLuint *indices, GLfloat *tcs, GLsizei vcount, GLsizei icount) 
{
	this->count = icount;
	vao = compile(vertices, indices, tcs, vcount, icount);
}

VertexArray::VertexArray(GLfloat *vertices, GLuint *indices, GLfloat *normals, GLfloat *tcs, GLsizei vcount, GLsizei icount)
{
	this->count = icount;
	vao = compile(vertices, indices, normals, tcs, vcount, icount);
}


GLuint VertexArray::compile(GLfloat *vertices, GLuint *indices, GLsizei vcount, GLsizei icount)
{
	return compile(vertices, indices, nullptr, nullptr, vcount, icount);
}

GLuint VertexArray::compile(GLfloat *vertices, GLuint *indices, GLfloat *tcs, GLsizei vcount, GLsizei icount)
{
	return compile(vertices, indices, nullptr, tcs, vcount, icount);
}

GLuint VertexArray::compile(GLfloat *vertices, GLuint *indices, GLfloat *normals, GLfloat *tcs, GLsizei vcount, GLsizei icount)
{
	GLuint vao, vbo, nbo, tbo;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vcount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (normals != nullptr)
	{
		glGenBuffers(1, &nbo);
		glBindBuffer(GL_ARRAY_BUFFER, nbo);
		glBufferData(GL_ARRAY_BUFFER, vcount * sizeof(GLfloat), normals, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	if (tcs != nullptr)
	{
		glGenBuffers(1, &tbo);
		glBindBuffer(GL_ARRAY_BUFFER, tbo);
		glBufferData(GL_ARRAY_BUFFER, vcount * sizeof(GLfloat), tcs, GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return vao;
}

void VertexArray::render()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}