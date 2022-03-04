uniform mat4 mvp_matrix;

attribute vec4 aPosition;
attribute vec2 aTexCoord;
attribute float aAngle;

varying vec2 vTexCoord;

vec4 rotateVertices(vec4 pos) {
    vec4 position = pos;
    position.x = position.x * cos(aAngle);
    position.z = position.x * sin(aAngle);
    return position;
}

void main(void) {
    vTexCoord = aTexCoord;
    vec4 position = aPosition;
    if (position.x > 0.0) {
        position = rotateVertices(position);
    }
    gl_Position = mvp_matrix * position;
}
