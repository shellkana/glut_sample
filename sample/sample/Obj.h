/*
** Alias OBJ 形式データ
*/
#ifndef OBJ_H
#define OBJ_H

#include <GLUT/glut.h>

class Obj {
  /* シェーダオブジェクト */
  GLuint shaderObject;  // シェーダオブジェクト名
  GLint scr, org, dir;  // uniform 変数の場所
  GLint p0, e1, e2;     // attribute 変数の場所
  GLint v0, v1, v2;     // attribute 変数の場所
  /* バッファオブジェクト */
  GLuint bufferObject;  // バッファオブジェクト名
  int faces;            // 三角形の数
public:
  virtual ~Obj() { if (bufferObject > 0) glDeleteBuffers(1, &bufferObject), bufferObject = 0; }
  bool load(const char *name);
  void shader(GLuint program);
  void setup() const;
  void draw(const float *screen, const float *origin, const float *direction) const;
  void cleanup() const;
};

#endif
