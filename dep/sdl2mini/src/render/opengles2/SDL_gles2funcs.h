SDL_PROC(void, glActiveTexture, (GLenum))
SDL_PROC(void, glAttachShader, (GLuint, GLuint))
SDL_PROC(void, glBindAttribLocation, (GLuint, GLuint, const char *))
SDL_PROC(void, glBindTexture, (GLenum, GLuint))
SDL_PROC(void, glBlendFuncSeparate, (GLenum, GLenum, GLenum, GLenum))
SDL_PROC(void, glClear, (GLbitfield))
SDL_PROC(void, glClearColor, (GLclampf, GLclampf, GLclampf, GLclampf))
SDL_PROC(void, glCompileShader, (GLuint))
SDL_PROC(GLuint, glCreateProgram, (void))
SDL_PROC(GLuint, glCreateShader, (GLenum))
SDL_PROC(void, glDeleteProgram, (GLuint))
SDL_PROC(void, glDeleteShader, (GLuint))
SDL_PROC(void, glDeleteTextures, (GLsizei, const GLuint *))
SDL_PROC(void, glDisable, (GLenum))
SDL_PROC(void, glDisableVertexAttribArray, (GLuint))
SDL_PROC(void, glDrawArrays, (GLenum, GLint, GLsizei))
SDL_PROC(void, glEnable, (GLenum))
SDL_PROC(void, glEnableVertexAttribArray, (GLuint))
SDL_PROC(void, glFinish, (void))
SDL_PROC(void, glGenFramebuffers, (GLsizei, GLuint *))
SDL_PROC(void, glGenTextures, (GLsizei, GLuint *))
SDL_PROC(void, glGetBooleanv, (GLenum, GLboolean *))
SDL_PROC(const GLubyte *, glGetString, (GLenum))
SDL_PROC(GLenum, glGetError, (void))
SDL_PROC(void, glGetIntegerv, (GLenum, GLint *))
SDL_PROC(void, glGetProgramiv, (GLuint, GLenum, GLint *))
SDL_PROC(void, glGetShaderInfoLog, (GLuint, GLsizei, GLsizei *, char *))
SDL_PROC(void, glGetShaderiv, (GLuint, GLenum, GLint *))
SDL_PROC(GLint, glGetUniformLocation, (GLuint, const char *))
SDL_PROC(void, glLinkProgram, (GLuint))
SDL_PROC(void, glPixelStorei, (GLenum, GLint))
SDL_PROC(void, glReadPixels, (GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLvoid*))
SDL_PROC(void, glScissor, (GLint, GLint, GLsizei, GLsizei))
SDL_PROC(void, glShaderBinary, (GLsizei, const GLuint *, GLenum, const void *, GLsizei))
SDL_PROC(void, glShaderSource, (GLuint, GLsizei, const char **, const GLint *))
SDL_PROC(void, glTexImage2D, (GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void *))
SDL_PROC(void, glTexParameteri, (GLenum, GLenum, GLint))
SDL_PROC(void, glTexSubImage2D, (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *))
SDL_PROC(void, glUniform1i, (GLint, GLint))
SDL_PROC(void, glUniform4f, (GLint, GLfloat, GLfloat, GLfloat, GLfloat))
SDL_PROC(void, glUniformMatrix4fv, (GLint, GLsizei, GLboolean, const GLfloat *))
SDL_PROC(void, glUseProgram, (GLuint))
SDL_PROC(void, glVertexAttribPointer, (GLuint, GLint, GLenum, GLboolean, GLsizei, const void *))
SDL_PROC(void, glViewport, (GLint, GLint, GLsizei, GLsizei))
SDL_PROC(void, glBindFramebuffer, (GLenum, GLuint))
SDL_PROC(void, glFramebufferTexture2D, (GLenum, GLenum, GLenum, GLuint, GLint))
SDL_PROC(GLenum, glCheckFramebufferStatus, (GLenum))
SDL_PROC(void, glDeleteFramebuffers, (GLsizei, const GLuint *))
SDL_PROC(void, glDrawElements,(GLenum,GLsizei,GLenum,const GLvoid *))

